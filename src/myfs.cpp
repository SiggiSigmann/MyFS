//
//  myfs.cpp
//  myfs
//
//  Created by Oliver Waldhorst on 02.08.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
//

// For documentation of FUSE methods see https://libfuse.github.io/doxygen/structfuse__operations.html

#undef DEBUG

// TODO: Comment this to reduce debug messages
#define DEBUG
#define DEBUG_METHODS
#define DEBUG_RETURN_VALUES

#include "macros.h"

#include "myfs.h"
#include "myfs-info.h"

MyFS* MyFS::_instance = NULL;

MyFS* MyFS::Instance() {
    if(_instance == NULL) {
        _instance = new MyFS();
    }
    return _instance;
}

MyFS::MyFS() {
    this->logFile= stderr;
    bd = new BlockDevice();
    superblock = new Superblock();
    dmap = new DMap();
    fat = new FatHandler();
    imap = new IMapHandler();
    rootblock = new RootBlock();
    blockBuffer = new FsBuffer();

    LOGF("Update Buffer: %d",EMPTY_FAT_ENTRY);
    blockBuffer->buffer = (char*)malloc(BLOCK_SIZE);
    blockBuffer->blockindex = EMPTY_FAT_ENTRY;
}

MyFS::~MyFS() {
    bd->close();
    delete bd;
    delete superblock;
    delete dmap;
    delete fat;
    delete imap;
    delete rootblock;
    free(blockBuffer->buffer);
    delete blockBuffer;
}

/**
 * path:    path to the file
 * statbuf: will be filles with metadata from the file in path
 */
int MyFS::fuseGetattr(const char *path, struct stat *statbuf) {
    LOGM();
    LOGF("Name: %s",path);
    
    if ( strcmp( path, "/" ) == 0 ){
        LOG("Is dir");
        statbuf->st_uid = getuid();
        statbuf->st_gid = getgid();
         
        statbuf->st_mode = S_IFDIR | 0555;
		statbuf->st_nlink = 2; // Why "two" hardlinks instead of "one"? The answer is here: http://unix.stackexchange.com/a/101536
	
    }else{
        LOG("Is file");
        InodeStruct* inode = (InodeStruct *)malloc(BLOCK_SIZE);

        //copy path to name with the offste 1 to skip the / at the end of the name
        char* name = (char*)malloc(NAME_LENGTH); 
        strcpy(name,path+1);

        inode = rootblock->getInodeByName(bd,name);

        if(inode == NULL){
            RETURN(-ENOENT); /*No such file or directory*/
        }

        statbuf->st_dev = 0;
        statbuf->st_ino = rootblock->checkFilenameOccupied(bd,name);
        statbuf->st_mode = inode->mode;
        statbuf->st_nlink = 1;      //static value
        statbuf->st_uid = inode->userID;
        statbuf->st_gid = inode->groupID;
        statbuf->st_rdev = 0;
        statbuf->st_size = inode->fileSizeBytes;
        statbuf->st_blksize = BLOCK_SIZE;
        statbuf->st_blocks = inode->fileSizeBlocks;
        statbuf->st_atime = inode->atime;
        statbuf->st_mtime = inode->mtime;
        statbuf->st_ctime = inode->ctime;        
        free(inode);
        free(name);
    }
   
    RETURN(0);
}

int MyFS::fuseReadlink(const char *path, char *link, size_t size) {
    LOGM();
    return 0;
}

/**
 * path	: path with the filename which sould be created
 * mode : excess mode of the file
 * fileInfo : ?
 */
int MyFS::fuseMknod(const char *path, mode_t mode, dev_t dev) {
    LOGM();
    LOGF("Name: %s",path);
    
    //check if path is dir
    if( strcmp( path, "/" ) == 0 ){
        LOG("Is Path");
        RETURN(-EISDIR); /* Is a directory */
    }
    LOG("Is File");

    //check if a inode is free
    if(superblock->getNumberOfFreeInodes()<1){
        RETURN(-ENOSPC);
    }
    LOGF("Number of free inodes: %d",superblock->getNumberOfFreeInodes());

    //check if filename allready exists
    char* name = (char*)malloc(NAME_LENGTH); 
    strcpy(name,path+1);
    if(rootblock->checkFilenameOccupied(bd,name)!=(uint32_t)-1){
       RETURN(EEXIST); 
    }

    //get inode index and calculate inode superblock values
    uint32_t inodeIndex = superblock->getFirstFreeInodeIndex();
    LOGF("InodeIndex: %d",inodeIndex);
    imap->occupyIMapEntry(inodeIndex);
    superblock->updateFirstFreeInodeIndex(imap->getNextFreeInode(inodeIndex));
    LOGF("new free InodeIndex: %d",superblock->getFirstFreeInodeIndex());
    superblock->updateNumberOfFreeInodes(superblock->getNumberOfFreeInodes()-1);
    LOGF("new Number of free inodes: %d",superblock->getNumberOfFreeInodes());
    
    rootblock->updateInode(bd, inodeIndex, name, END_OF_FILE_ENTRY, 0, 0,time(NULL),time(NULL),getuid(),getgid(),mode);

    //write
    superblock->writeSuperblock(bd);
    imap->write(bd);

    free(name);
    RETURN(0);
}

int MyFS::fuseMkdir(const char *path, mode_t mode) {
    LOGM();
    return 0;
}

/**
 * path: file to delete
 */
int MyFS::fuseUnlink(const char *path) {
    LOGM();
    LOGF("\nName: %s",path);
    
    //check if path is dir
    if ( strcmp( path, "/" ) == 0 ){
        RETURN(-EISDIR); /* Is a directory */
    }

    //get inodeindex
    char* name = (char*)malloc(NAME_LENGTH); 
    strcpy(name,path+1);
    LOGF("name: %s",name);
    uint32_t inodeindex = rootblock->checkFilenameOccupied(bd,name);
    LOGF("inodeindex: %d",inodeindex);
    if(inodeindex == (uint32_t)-1){
        RETURN(-ENOENT); /* No such file or directory */
    }

    //get inode
    InodeStruct* inode = (InodeStruct *)malloc(BLOCK_SIZE);
    inode = rootblock->getInodeByName(bd,name);
    uint32_t currentBlock = inode->firstDataBlock;

    //define inode as free
    if(!imap->getIMapEntry(inodeindex)){
        LOG("is already free");
        RETURN(-ENOENT);
    }
    imap->freeIMapEntry(inodeindex);
    superblock->updateNumberOfFreeInodes(superblock->getNumberOfFreeInodes()+1);

    //free datablcoks
    while(currentBlock!=END_OF_FILE_ENTRY){
        LOGF("free index: %d",currentBlock);
        dmap->freeDatablock(currentBlock);
        currentBlock = fat->get(currentBlock);
    }

    //write to FS
    superblock->writeSuperblock(bd);
    dmap->writeDMap(bd);
    imap->write(bd);

    free(name);
    free(inode);
    
    RETURN(0);
}

int MyFS::fuseRmdir(const char *path) {
    LOGM();
    return 0;
}

int MyFS::fuseSymlink(const char *path, const char *link) {
    LOGM();
    return 0;
}

int MyFS::fuseRename(const char *path, const char *newpath) {
    LOGM();
    return 0;
}

int MyFS::fuseLink(const char *path, const char *newpath) {
    LOGM();
    return 0;
}

int MyFS::fuseChmod(const char *path, mode_t mode) {
    LOGM();
    return 0;
}

int MyFS::fuseChown(const char *path, uid_t uid, gid_t gid) {
    LOGM();
    return 0;
}

int MyFS::fuseTruncate(const char *path, off_t newSize) {
    LOGM();
    LOGF("Name: %s",path);
    LOGF("newSize: %d",newSize);

    //TODO: catch size 0

    //get new sizes
    uint32_t newSizeInBlocks = newSize/BLOCK_SIZE;
    uint32_t bytesOfLastBlocks = newSize%BLOCK_SIZE;
    if(bytesOfLastBlocks){
        newSizeInBlocks++;
    }
    LOGF("New size Bytes: %d (Blocks: %d | Bytes: %d )",newSize, newSizeInBlocks, bytesOfLastBlocks);

    //get inode
    char* name = (char*)malloc(NAME_LENGTH);
    strcpy(name,path+1);
    uint32_t inodenumber = rootblock->checkFilenameOccupied(bd,name);
    InodeStruct* inode = (InodeStruct *)malloc(BLOCK_SIZE);
    inode = rootblock->getInodeByName(bd, name);
    uint32_t firstBlockEntry = inode->firstDataBlock;
    uint32_t currentblock = firstBlockEntry;
    uint32_t originalSizeInBlocks = inode->fileSizeBlocks;
    LOGF("Orginal block size: %d",originalSizeInBlocks);
    LOGF("Startblock: %d", currentblock);

    //check new size
    if(newSizeInBlocks > originalSizeInBlocks){
        RETURN(-EINVAL); /* Invalid argument */
    }

    //skip blocks
    if(newSizeInBlocks){
        for(uint32_t i = 0;i<newSizeInBlocks-1;i++){
            currentblock = fat->get(currentblock);
            LOGF("Fat entry: %d",currentblock);
            /*if(currentblock == END_OF_FILE_ENTRY){
                RERURN(-EIO);
            }*/
        }

            //rewrite eof
        uint32_t firstBlockToDele = fat->get(currentblock);
        fat->set(currentblock, END_OF_FILE_ENTRY);
        LOGF("update fat: %d->%d", currentblock,END_OF_FILE_ENTRY);

    }else{
        firstBlockEntry = END_OF_FILE_ENTRY;
        LOGF("new start block: %d", firstBlockEntry);
    }



    //overrite last Block
    char* buffer = (char*)malloc(BLOCK_SIZE);
    if(bytesOfLastBlocks){
        LOG("delete bytes");
        bd->read(FIRST_DATA_BLOCK+currentblock,buffer);
        LOGF("from %d to %d",bytesOfLastBlocks,BLOCK_SIZE);
        for(uint32_t i = bytesOfLastBlocks;i<BLOCK_SIZE;i++){
            buffer[i]=0;
        }
        bd->write(FIRST_DATA_BLOCK+currentblock,buffer);
    }

    //TODO UPDATE BUFFER

    //delete blocks
    for(uint32_t i = newSizeInBlocks; i < originalSizeInBlocks; i++){
        LOGF("free block: %d",currentblock);
        dmap->freeDatablock(currentblock);
        currentblock = fat->get(currentblock);
        superblock->updateNumberOfFreeBlocks(superblock->getNumberOfFreeBlocks()+1);
        if(currentblock==END_OF_FILE_ENTRY){
            LOGF("reached end of file (%d)",i);
            break;
        }
    }

  
    LOGF("Startblock: %d", firstBlockEntry);
    rootblock->updateInode(bd, rootblock->checkFilenameOccupied(bd,name), inode->fileName, firstBlockEntry, newSize, newSizeInBlocks,time(0),time(0),getuid(),getgid(),inode->mode);
    
    fat->writeFat(bd);
    dmap->writeDMap(bd);

    free(buffer);
    free(inode);
    free(name);

    return 0;
}

int MyFS::fuseUtime(const char *path, struct utimbuf *ubuf) {
    LOGM();
    return 0;
}

/**
 * path	:  file to open
 * fileInfo: ?
 */
int MyFS::fuseOpen(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();
    LOGF("Name: %s",path);
    LOGF("Number of opened files: %d",numberOfOpendFiles);

    fileInfo->fh = -1;

    //update accestime
    if(numberOfOpendFiles<NUM_OPEN_FILES){
        numberOfOpendFiles++;
        LOGF("New number of opened files: %d",numberOfOpendFiles);
        
        char* name = (char*)malloc(NAME_LENGTH);
        //copy path to name with the offste 1 to skip the / at the end of the name
        strcpy(name,path+1);
        fileInfo->fh = rootblock->checkFilenameOccupied(bd,name);
        LOGF("\tfh (%s): %d",name,fileInfo->fh);
        free(name);

        //get inode
        RETURN(0);
    }

    RETURN(-ENFILE);    /* File table overflow */
}

/**
 * path: file to read out data
 * buf:  will contain content out of file path
 * size: number of bytes ehich sould be read
 * offset: bytes with should be skipped
 * fileInfo: ?
 */
int MyFS::fuseRead(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();
    LOGF("Name: %s",path);
    LOGF("\tset fh: %d",fileInfo->fh);
    //TODO:filehandler put iniode insidee

    //check if path is dir
    if ( strcmp( path, "/" ) == 0 ){
        LOG("Is dir");
        RETURN(-EISDIR); /* Is a directory */
    }
    LOG("Is file");

    /*//get inode by name
    char* name = (char*)malloc(NAME_LENGTH);
    //copy path to name with the offste 1 to skip the / at the end of the name
    strcpy(name,path+1);
    InodeStruct* inode = (InodeStruct *)malloc(BLOCK_SIZE);
    inode = rootblock->getInodeByName(bd,name);
    if(inode == NULL){
        RETURN(-ENOENT); /* No such file or directory 
    }*/

    //get inode from fileInfo->fh
    if(fileInfo->fh==(uint64_t)-1){
       RETURN(-ENOENT); /* No such file or directory */
    }
    InodeStruct* inode = (InodeStruct *)malloc(BLOCK_SIZE);
    inode = rootblock->getInode(bd,fileInfo->fh);

    //get first datablock
    uint32_t currentblock = inode->firstDataBlock;
    LOGF("Startbock: %d", currentblock);

    //skip blocks
    uint32_t blockOffset = offset/BLOCK_SIZE;
    uint32_t byteOffset = offset - (blockOffset*BLOCK_SIZE);
    LOGF("offset: total=%d (Blocks=%d Bytes=%d)",offset,blockOffset,byteOffset);
    for(uint32_t i = 0;i<blockOffset;i++){
        if(currentblock == END_OF_FILE_ENTRY){
            break;
        }
        LOGF("used fat entry: %d->%d", currentblock, fat->get(currentblock));
        //get index of next data block
        currentblock = fat->get(currentblock);      
    }
    LOGF("Jump to block: %d", currentblock);

    if(currentblock == END_OF_FILE_ENTRY){
        RETURN(0); //No data in file 
    }

    //read blocks
    uint32_t readedBytes = 0;   //count read in bytes
    uint32_t blocksToRead;
    uint32_t lastBytesToRead;
    if(byteOffset){
        blocksToRead = (size+byteOffset)/BLOCK_SIZE;
        lastBytesToRead  = size-((blocksToRead*BLOCK_SIZE)-byteOffset);
    }else{
        blocksToRead = size/BLOCK_SIZE;
        lastBytesToRead  = size - (blocksToRead*BLOCK_SIZE);
    }
    LOGF("read: total=%d (Blocks=%d Bytes=%d)",size,blocksToRead,lastBytesToRead);
    
    char* buffer = (char*)malloc(BLOCK_SIZE);   //read buffer
    for(uint32_t i = 0;i<blocksToRead;i++){
        if(currentblock == EMPTY_FAT_ENTRY){
            RETURN(-EIO);
        }
        if(currentblock == END_OF_FILE_ENTRY){
            LOG("reach end of file");
            break;  //if no more data to read
        }
        LOGF("Virtuell adress: %d", currentblock);

        //read block in buffer
        if(blockBuffer->blockindex == currentblock){
            LOG("\tRead form Buffer");
            memcpy(buffer,blockBuffer->buffer, BLOCK_SIZE);
        }else{
            LOG("\tRead from Block");
            if(bd->read(FIRST_DATA_BLOCK+currentblock,buffer)){
                LOG("problems to read");
                RETURN(-EIO)
            }
        }
        if(currentblock == EMPTY_FAT_ENTRY){
            RETURN(-EIO)
        }

        //write bytes
        if(byteOffset){
            LOG("Read with byte offset");

            LOGF("Update Buffer: %d",currentblock);
            memcpy(blockBuffer->buffer,buffer, BLOCK_SIZE);
            blockBuffer->blockindex = currentblock;

            LOGF("start to read in Block at: %d",byteOffset);
            for(uint32_t j = byteOffset; j < BLOCK_SIZE; j++){
                (buf+(i*BLOCK_SIZE))[j] = buffer[j];
            }
            readedBytes += byteOffset;
            byteOffset = 0;
        }else{
            LOG("Read full block");
            memcpy(buf+(i*BLOCK_SIZE),buffer, BLOCK_SIZE);
            readedBytes += BLOCK_SIZE;

            LOGF("Update Buffer: %d",currentblock);
            memcpy(blockBuffer->buffer,buffer, BLOCK_SIZE);
            blockBuffer->blockindex = currentblock;
        }

        //get index of next datablock
        currentblock = fat->get(currentblock);
    }
    
    //copy bytes which are to small for a block in buf
    if(lastBytesToRead){
        LOGF("read last bytes: %d", lastBytesToRead);
        LOGF("Virtuell adress: %d", currentblock);
        LOGF("Block adress %x",FIRST_DATA_BLOCK+currentblock);

         if(currentblock == EMPTY_FAT_ENTRY){
            RETURN(-EIO);
        }
        if(currentblock == END_OF_FILE_ENTRY){
            RETURN(-EIO);  //if no more data to read
        }

        if(bd->read(FIRST_DATA_BLOCK+currentblock,buffer)){
            RETURN(-EIO)
        }

        LOGF("Update Buffer: %d",currentblock);
        memcpy(blockBuffer->buffer,buffer, BLOCK_SIZE);
        blockBuffer->blockindex = currentblock;

        for(uint32_t i = 0;i<lastBytesToRead;i++){
            readedBytes++;
            (buf+(blocksToRead*BLOCK_SIZE))[i] = buffer[i];
        }
    }

    free(buffer);
    free(inode);

    RETURN(readedBytes);
}

int MyFS::fuseWrite(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();
    LOGF("Name: %s",path);
    LOGF("Free blocks: %d",superblock->getNumberOfFreeBlocks());

    //TODO: • EBADF – Datei nicht zum Schreiben geöffnet
    
    //check if path is dir
    if ( strcmp( path, "/" ) == 0 ){
        RETURN(-EISDIR); /* Is a directory */
    }

    //calculate bloks and bytes to skipp
    uint32_t blockOffset = offset/BLOCK_SIZE;
    uint32_t byteOffset = offset - (blockOffset*BLOCK_SIZE);
    LOGF("offset: total=%d (Blocks=%d Bytes=%d)",offset,blockOffset,byteOffset);

    //calculates blocks and bytes to write
    uint32_t firstBytesToWrite = BLOCK_SIZE - byteOffset;
    if(firstBytesToWrite>size){
        firstBytesToWrite = size;
    }
    if(!byteOffset){
        firstBytesToWrite=0;
    }
    uint32_t blocksToWrite = (size-firstBytesToWrite)/BLOCK_SIZE;
    if(firstBytesToWrite+(blocksToWrite*BLOCK_SIZE)>size){
        blocksToWrite--;
    }
    uint32_t lastBytesToWrite  = size-((blocksToWrite*BLOCK_SIZE)+firstBytesToWrite);
    uint32_t writtenBytes = 0;   //count write in bytes
    LOGF("written bytes: %d", writtenBytes);
    LOGF("to write: total=%d (BytesStart=%d Blocks=%d BytesEND=%d)",size,firstBytesToWrite,blocksToWrite,lastBytesToWrite);

    //check space on disk
    if(superblock->getNumberOfFreeBlocks()<blocksToWrite+1){
        RETURN(-ENOSPC);    /* No space left on device */
    }

    //get inode by name
    /*char* name = (char*)malloc(NAME_LENGTH);
    strcpy(name,path+1);    //copy path to name with the offste 1 to skip the / at the end of the name
    InodeStruct* inode = (InodeStruct *)malloc(BLOCK_SIZE);
    inode = rootblock->getInodeByName(bd,name);
    if(inode == NULL){
        RETURN(-ENOENT); /* No such file or directory 
    }*/

    //get inode from fileInfo->fh
    if(fileInfo->fh==(uint64_t)-1){
       RETURN(-ENOENT); /* No such file or directory */
    }
    InodeStruct* inode = (InodeStruct *)malloc(BLOCK_SIZE);
    inode = rootblock->getInode(bd,fileInfo->fh);

    //get startblock
    uint32_t currentblock = inode->firstDataBlock; //get first datablock
    uint32_t firstBlockIndex=currentblock;
    if(firstBlockIndex==END_OF_FILE_ENTRY){
        LOG("\Get new block because file is empty");
        firstBlockIndex = superblock->getFirstFreeBlockIndex();
        dmap->occupyDatablock(firstBlockIndex);
        superblock->updateFirstFreeBlockIndex(dmap->getNextFreeDatablock(firstBlockIndex));
        superblock->updateNumberOfFreeBlocks(superblock->getNumberOfFreeBlocks()-1);
        currentblock =firstBlockIndex;
    }

    //skip blocks
    if(blockOffset){
        uint32_t jumpTo = currentblock;
        for(uint32_t i = 0;i<blockOffset;i++){
            /*if(currentblock == END_OF_FILE_ENTRY){
                LOG("error");
                RETURN(-EINVAL); /* Invalid argument 
            }*/

            //get index of next data block
            jumpTo = fat->get(jumpTo);
            if(jumpTo == END_OF_FILE_ENTRY){
                LOGF("Break Jump at rounf %d from %d because end of file",i, blockOffset);
                LOGF("add FAT: %d->%d",currentblock,superblock->getFirstFreeBlockIndex());
                fat->set(currentblock, superblock->getFirstFreeBlockIndex());
                currentblock=superblock->getFirstFreeBlockIndex();
                break;
            }else{
               currentblock =  jumpTo;
            }
            LOGF("Fat next: %d",jumpTo);
        }

    }

    LOGF("Startblock: %d",firstBlockIndex);

    //write bytes in first data block which are not a complete block
    char* buffer = (char*)malloc(BLOCK_SIZE);   //read buffer
    if(firstBytesToWrite){
        LOGF("Block: %d byteOffset: %d",currentblock, firstBytesToWrite);
        bd->read(FIRST_DATA_BLOCK+currentblock, buffer);
        for(uint32_t i = 0;i<firstBytesToWrite;i++){
            buffer[i+byteOffset] = buf[i+writtenBytes];
        }
        bd->write(FIRST_DATA_BLOCK+currentblock, buffer);

        LOGF("Update Buffer: %d",currentblock);
        memcpy(blockBuffer->buffer,buffer, BLOCK_SIZE);
        blockBuffer->blockindex = currentblock;

        writtenBytes += firstBytesToWrite;
        LOGF("written bytes: %d", writtenBytes);
    }

    //write compleat blocks
    if(blocksToWrite){
        for(uint32_t i = 0;i<blocksToWrite;i++){
            LOGF("write full Block: %d", i);
            LOGF("dmap[%d]: %d",currentblock,dmap->get(currentblock));
            if(currentblock==END_OF_FILE_ENTRY||currentblock==EMPTY_FAT_ENTRY||!dmap->get(currentblock)){
                LOG("get new block");
                currentblock = superblock->getFirstFreeBlockIndex();
                //modify dmap
                dmap->occupyDatablock(currentblock);
                superblock->updateFirstFreeBlockIndex(dmap->getNextFreeDatablock(currentblock));
                superblock->updateNumberOfFreeBlocks(superblock->getNumberOfFreeBlocks()-1);
            
                LOGF("write to : %d", currentblock);
                //copy content of file to FS
                for(uint32_t j = 0; j<BLOCK_SIZE;j++){
                    buffer[j] = buf[j+writtenBytes];
                }
                bd->write(FIRST_DATA_BLOCK+currentblock, buffer);

                LOGF("Update Buffer: %d",currentblock);
                memcpy(blockBuffer->buffer,buffer, BLOCK_SIZE);
                blockBuffer->blockindex = currentblock;

                //modify fat
                if(i==blocksToWrite-1 && lastBytesToWrite == 0){
                    LOGF("add FAT: %d->%d",currentblock,END_OF_FILE_ENTRY);
                    fat->set(currentblock, END_OF_FILE_ENTRY);
                    currentblock = fat->get(currentblock);
                }else{
                    LOGF("add FAT: %d->%d",currentblock,superblock->getFirstFreeBlockIndex());
                    fat->set(currentblock, superblock->getFirstFreeBlockIndex());
                    currentblock = fat->get(currentblock);
                }
            }else{
                currentblock = fat->get(currentblock);
                //fehler wenn block überschreiebn wirt neue datei uns os
                LOG("overwrite old block");
                LOGF("write to : %d", currentblock);
                //copy content of file to FS
                for(uint32_t j = 0; j<BLOCK_SIZE;j++){
                    buffer[j] = buf[j+writtenBytes];
                }
                bd->write(FIRST_DATA_BLOCK+currentblock, buffer);
                
                LOGF("Update Buffer: %d",currentblock);
                memcpy(blockBuffer->buffer,buffer, BLOCK_SIZE);
                blockBuffer->blockindex = currentblock;
            }
            writtenBytes += BLOCK_SIZE;
            LOGF("written bytes: %d", writtenBytes);
        }
        if(currentblock==END_OF_FILE_ENTRY||currentblock==EMPTY_FAT_ENTRY||!dmap->get(currentblock)){
            currentblock = fat->get(currentblock);
        }
        
        
    }

    //write last bytes which dont fit in a block
    if(lastBytesToWrite){
        LOGF("Last Blocks to write: %d",lastBytesToWrite);
        LOGF("write to: %d", currentblock);
        if(currentblock==END_OF_FILE_ENTRY || currentblock==EMPTY_FAT_ENTRY || !dmap->get(currentblock)){
            LOG("get new block");
            currentblock = superblock->getFirstFreeBlockIndex();
              //modify dmap
            dmap->occupyDatablock(currentblock);
            superblock->updateFirstFreeBlockIndex(dmap->getNextFreeDatablock(currentblock));
            superblock->updateNumberOfFreeBlocks(superblock->getNumberOfFreeBlocks()-1);
        
            LOGF("write to : %d", currentblock);
        }
        for(uint32_t i = 0;i<BLOCK_SIZE;i++){
            buffer[i]=0;
        }
        for(uint32_t i = 0;i<lastBytesToWrite;i++){
            buffer[i]=buf[i+writtenBytes];
        }
        writtenBytes += lastBytesToWrite;
        LOGF("written bytes: %d", writtenBytes);
        bd->write(FIRST_DATA_BLOCK+currentblock, buffer);

        LOGF("Update Buffer: %d",currentblock);
        memcpy(blockBuffer->buffer,buffer, BLOCK_SIZE);
        blockBuffer->blockindex = currentblock;

        LOGF("add FAT: %d->%d",currentblock,END_OF_FILE_ENTRY);
        fat->set(currentblock, END_OF_FILE_ENTRY);
    }

    LOGF("old file size: %d",inode->fileSizeBytes);
    LOGF("written bytes: %d", writtenBytes);

    //calc block size
    uint32_t newBlockSize = (inode->fileSizeBytes+writtenBytes)/BLOCK_SIZE;
    if((inode->fileSizeBytes+writtenBytes)%BLOCK_SIZE){
        newBlockSize++;
    }

    rootblock->updateInode(bd, fileInfo->fh, inode->fileName, firstBlockIndex, inode->fileSizeBytes+writtenBytes,newBlockSize,time(0),time(0),inode->userID,inode->groupID,inode->mode);
    
    LOGF("new file size: %d",inode->fileSizeBytes+writtenBytes);
    //write to fs
    superblock->writeSuperblock(bd);
    dmap->writeDMap(bd);
    fat->writeFat(bd);

    LOGF("Free blocks: %d",superblock->getNumberOfFreeBlocks());
    
    free(buffer);
    free(inode);

    RETURN(writtenBytes);
}

int MyFS::fuseStatfs(const char *path, struct statvfs *statInfo) {
    LOGM();
    return 0;
}

int MyFS::fuseFlush(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();
    return 0;
}

/**
 * path: path to file which should be Release
 * fileInfo: ?
 */
int MyFS::fuseRelease(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();
    LOGF("Name: %s",path);

    LOGF("Number of opend files: %d", numberOfOpendFiles);
    if(numberOfOpendFiles>0){
        numberOfOpendFiles--;
        LOGF("new number of opend files: %d", numberOfOpendFiles);
    }
    
    RETURN(0);
}

int MyFS::fuseFsync(const char *path, int datasync, struct fuse_file_info *fi) {
    LOGM();
    return 0;
}

int MyFS::fuseListxattr(const char *path, char *list, size_t size) {
    LOGM();
    RETURN(0);
}

int MyFS::fuseRemovexattr(const char *path, const char *name) {
    LOGM();
    RETURN(0);
}

int MyFS::fuseOpendir(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();
    
    //we don't need this 
    
    RETURN(0);
}

/**
 * path:    path where the information should be filles in buf
 * buf:     solution buffer
 * filler:  methid to fill buf
 * offset:  ?
 * fileInfo:?
 */
int MyFS::fuseReaddir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();
    LOGF("Name: %s",path);

    if(strcmp( path, "/" ) == 0){   //check if path is a directory
        filler( buf, ".", NULL, 0 );
        filler( buf, "..", NULL, 0 );
        for(uint32_t i = 0; i<NUM_DIR_ENTRIES;i++){
            if(imap->getIMapEntry(i)){
                char* name = rootblock->getFileName(bd, i);
                filler(buf, name, NULL, 0 );    //fill buf with the name of the contained files
                                LOGF("Inode(%d): %s",i, name);
                delete[] name;
            }
        }
    
        RETURN(0);
    }
    
    RETURN(-ENOTDIR);
}

int MyFS::fuseReleasedir(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();
    
    //we don't need this
    
    RETURN(0);
}

int MyFS::fuseFsyncdir(const char *path, int datasync, struct fuse_file_info *fileInfo) {
    LOGM();
    RETURN(0);
}

int MyFS::fuseTruncate(const char *path, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();
    RETURN(0);
}

/**
 * path	: path with the filename which sould be created
 * mode : excess mode of the file
 * fileInfo : ?
 */
int MyFS::fuseCreate(const char *path, mode_t mode, struct fuse_file_info *fileInfo) {
    LOGM();
    
    RETURN(-ENOSYS); /* Function not implemented --> instead mknod and open will be called */
}

void MyFS::fuseDestroy() {
    LOGM();
}


//init Fuse File System and load bin file.
void* MyFS::fuseInit(struct fuse_conn_info *conn) {
    // Open logfile
    this->logFile= fopen(((MyFsInfo *) fuse_get_context()->private_data)->logFile, "w+");
    if(this->logFile == NULL) {
        fprintf(stderr, "ERROR: Cannot open logfile %s\n", ((MyFsInfo *) fuse_get_context()->private_data)->logFile);
    } else {
        //    this->logFile= ((MyFsInfo *) fuse_get_context()->private_data)->logFile;
        
        // turn of logfile buffering
        setvbuf(this->logFile, NULL, _IOLBF, 0);
        
        LOG("Starting logging...\n");
        LOGM();
        
        // you can get the containfer file name here:
        char* containerName = (((MyFsInfo *) fuse_get_context()->private_data))->contFile;
        LOGF("Containerfile: %s", containerName);

        bd->open(containerName);
        superblock->readSuperblock(bd);
        dmap->readDMap(bd);
        fat->readFat(bd);
        imap->read(bd);
        
    }
    
    RETURN(0);
}

#ifdef __APPLE__
int MyFS::fuseSetxattr(const char *path, const char *name, const char *value, size_t size, int flags, uint32_t x) {
#else
int MyFS::fuseSetxattr(const char *path, const char *name, const char *value, size_t size, int flags) {
#endif
    LOGM();
    RETURN(0);
}
    
#ifdef __APPLE__
int MyFS::fuseGetxattr(const char *path, const char *name, char *value, size_t size, uint x) {
#else
int MyFS::fuseGetxattr(const char *path, const char *name, char *value, size_t size) {
#endif
    LOGM();
    RETURN(0);
}

/*
Writes 'size' bytes to the block device, starting at index 'firstBlockIndex'
*/
void writeBytes(BlockDevice* bd, int firstBlockIndex, char* bytes, int size) {
    // number of perfect blocks
    int numberOfBlocks = size / BD_BLOCK_SIZE;

    int remainder = size % BD_BLOCK_SIZE;

    // write all perfect blocks
    for(int i = 0; i < numberOfBlocks; i++) {
        bd->write(i+firstBlockIndex, bytes + i * BD_BLOCK_SIZE);
    }

    // write remainding bytes (as one block), if necessary
    if (remainder > 0){
        //bd.write only accepts buffers of size BD_BLOCK_SIZE, so we need to temporarily create one
        char* remainderBlockBytes = new char[BD_BLOCK_SIZE];
        //copy the remainder amount of bytes to the new block buffer, offset: all recently wrote perfect blocks
        memcpy(remainderBlockBytes, bytes + (numberOfBlocks * BD_BLOCK_SIZE), remainder);
        //set the boilerplate values to zero
        memset(remainderBlockBytes+remainder, 0, BD_BLOCK_SIZE-remainder);
        bd->write(numberOfBlocks, remainderBlockBytes);
        //cleanup the temporary buffer
        delete [] remainderBlockBytes;
    }
}

/*
Reads 'numberOfBytes' bytes from the block device, starting at index 'firstBlockIndex'
*/
char* readBytes(BlockDevice* bd, int firstBlockIndex, int numberOfBytes) {
    char* readBuffer = new char[numberOfBytes];
    // number of perfect blocks
    int numberOfBlocks = numberOfBytes / BD_BLOCK_SIZE;
    
    int remainder = numberOfBytes % BD_BLOCK_SIZE;

    // read all perfect blocks
    for(int i = 0; i < numberOfBlocks; i++) {
        bd->read(i+firstBlockIndex, readBuffer + i * BD_BLOCK_SIZE);
    }

    // read remainding bytes (as one block), if necessary
    if (remainder > 0){
        //bd.read only accepts buffers of size BD_BLOCK_SIZE, so we need to temporarily create one
        char* remainderBlockBytes = new char[BD_BLOCK_SIZE];
        //read the entire block containing the remainding bytes (obviously the block after the last perfect block)
        bd->read(numberOfBlocks, remainderBlockBytes);
        //append all remainding bytes after the perfect blocks
        memcpy(readBuffer + (numberOfBlocks*BD_BLOCK_SIZE), remainderBlockBytes, remainder);
        //cleanup the temporary buffer
        delete [] remainderBlockBytes;
    }
    return readBuffer;
}

/*
Extension for the original memset function (only applies on 1byte arrays):
Fills the first 'size' values of a specified array with 'value' starting from the given pointer.
*/
void memset4(uint32_t* ptr, uint32_t value, uint32_t size){
    for(uint32_t i = 0; i < size; i++){
        ptr[i] = value;
    }
}
