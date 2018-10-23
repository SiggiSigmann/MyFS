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
}

MyFS::~MyFS() {
    bd->close();
    delete bd;
    delete superblock;
    delete dmap;
    delete fat;
    delete imap;
    delete rootblock;
}

/**
 * path:    path to the file
 * statbuf: will be filles with metadata from the file in path
 */
int MyFS::fuseGetattr(const char *path, struct stat *statbuf) {
    LOGM();
    
    LOGF("\tPath:%s\n",path);
    if ( strcmp( path, "/" ) == 0 ){
        statbuf->st_uid = getuid();
        statbuf->st_gid = getgid();
         
        statbuf->st_mode = S_IFDIR | 0555;
		statbuf->st_nlink = 2; // Why "two" hardlinks instead of "one"? The answer is here: http://unix.stackexchange.com/a/101536
	
    }else{
        InodeStruct* inode = (InodeStruct *)malloc(BLOCK_SIZE);

        //copy path in name to make it not constant
        char* name = (char*)malloc(NAME_LENGTH); 
        strcpy(name,path+1);

        inode = rootblock->getInodeByName(bd,name);

        if(inode == NULL){
            LOG("HILFE");
            //todo:errorcode
        }

        statbuf->st_dev = 0;
        statbuf->st_ino = 0;
        statbuf->st_mode = inode->mode;
        statbuf->st_nlink = 0;
        statbuf->st_uid = inode->userID;
        statbuf->st_gid = inode->groupID;
        statbuf->st_rdev = 0;
        statbuf->st_size = inode->fileSize*BLOCK_SIZE;
        statbuf->st_blksize = BLOCK_SIZE;
        statbuf->st_blocks = inode->fileSize;
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

int MyFS::fuseMknod(const char *path, mode_t mode, dev_t dev) {
    LOGM();
    
    // TODO: Implement this!
    
    RETURN(0);
}

int MyFS::fuseMkdir(const char *path, mode_t mode) {
    LOGM();
    return 0;
}

int MyFS::fuseUnlink(const char *path) {
    LOGM();
    
    // TODO: Implement this!
    
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
    return 0;
}

int MyFS::fuseUtime(const char *path, struct utimbuf *ubuf) {
    LOGM();
    return 0;
}

int MyFS::fuseOpen(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();
    LOGF("\tNumber of open Files: %d",numberOfOpendFiles);
    
    if(numberOfOpendFiles<NUM_OPEN_FILES){
        numberOfOpendFiles++;
        RETURN(0);
    }

    RETURN(-EMFILE);
    
    
}

int MyFS::fuseRead(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();
    //TODO:think about EMPTY_FAT_ENTRY; 
    //TODO:read only the amount of bytes which are needed.
    
    if ( strcmp( path, "/" ) == 0 ){
        RETURN(-EISDIR);
    }

    //copy path in name to make it not constant
    char* name = (char*)malloc(NAME_LENGTH); 
    strcpy(name,path+1);

    InodeStruct* inode = (InodeStruct *)malloc(BLOCK_SIZE);
    inode = rootblock->getInodeByName(bd,name);

    if(inode == NULL){
        //TODO: errorcode
        //errorhandling read
    }

    //skip blocks
    uint32_t blockOffset = offset/BLOCK_SIZE;
    uint32_t byteOffset = offset - (blockOffset*BLOCK_SIZE);
    LOGF("offset = Blocks:%d Bytes%d", blockOffset ,byteOffset);

    uint32_t currentblock = inode->firstDataBlock;
    LOGF("first index : %x", currentblock);
    for(uint32_t i = 0;i>blockOffset;i++){
        currentblock = fat->get(currentblock);
    }

    char* emptyblock = (char*)malloc(BLOCK_SIZE);
    for(uint32_t i = 0; i < BLOCK_SIZE; i++){
        emptyblock[i] = 0;
    }

    uint32_t bocksToRead = size/BLOCK_SIZE;
    uint32_t lastBytesToRead  = size - (bocksToRead*BLOCK_SIZE);
    LOGF("read = Blocks:%d Bytes%d", bocksToRead ,lastBytesToRead);

    //copy needed blocks to buf
    char* buffer = (char*)malloc(BLOCK_SIZE);
    for(uint32_t i = 0;i<bocksToRead;i++){
        LOGF("fat %x",currentblock);
        if(currentblock == END_OF_FILE_ENTRY){
            //strcpy(buf+(i*BLOCK_SIZE),emptyblock);
            for(uint32_t j = 0; j < BLOCK_SIZE; j++){
                (buf+(i*BLOCK_SIZE))[j] = emptyblock[i];
            }
        }else{
            bd->read(FIRST_DATA_BLOCK+currentblock,buffer);
            //strcpy(buf+(i*BLOCK_SIZE),buffer);
            for(uint32_t j = 0; j < BLOCK_SIZE; j++){
                (buf+(i*BLOCK_SIZE))[j] = buffer[j];
            }
            LOGF("content : %p",buf+(i*BLOCK_SIZE));
            LOGF("content : %s",buf+(i*BLOCK_SIZE));
            currentblock = fat->get(currentblock);
        }
    }
    free(buffer);

    //copy bytes which are to small for a block in buf
    char* bytebuffer = (char*)malloc(BLOCK_SIZE);
    if(currentblock == END_OF_FILE_ENTRY){
        for(uint32_t i = 0; i < BLOCK_SIZE; i++){
            bytebuffer[i] = 0;
        }
    }else{
        bd->read(FIRST_DATA_BLOCK+currentblock,bytebuffer);
    }
    for(uint32_t i = 0;i<lastBytesToRead;i++){
        LOG("writebit");
        (buf+(bocksToRead*BLOCK_SIZE))[i] = bytebuffer[i];
    }

    



    LOGF("%s",buf);

    free(bytebuffer);
    free(emptyblock);
    free(inode);
    free(name);

    LOGF("%s",buf);
    RETURN(size);
}

int MyFS::fuseWrite(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();
    
    // TODO: Implement this!
    
    RETURN(0);
}

int MyFS::fuseStatfs(const char *path, struct statvfs *statInfo) {
    LOGM();
    return 0;
}

int MyFS::fuseFlush(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();
    return 0;
}

int MyFS::fuseRelease(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();
    LOGF("openfiles %d",numberOfOpendFiles);
    if(numberOfOpendFiles>0){
        numberOfOpendFiles--;
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
    
    // TODO: Implement this! //we don't need this
    
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
    if(strcmp( path, "/" ) == 0){
        LOGF("\tPath:%s\n",path);
        filler( buf, ".", NULL, 0 );
        filler( buf, "..", NULL, 0 );
        for(uint32_t i = 0; i<NUM_DIR_ENTRIES;i++){
            if(imap->getIMapEntry(i)){
                filler(buf, rootblock->getFileName(bd, i), NULL, 0 );
            }
        }
    
        RETURN(0);
    }
    
    RETURN(-ENOTDIR);
    
    // <<< My new code
}

int MyFS::fuseReleasedir(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();
    
    // TODO: Implement this! //we don't need this
    
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

int MyFS::fuseCreate(const char *path, mode_t mode, struct fuse_file_info *fileInfo) {
    LOGM();
    
    // TODO: Implement this!
    
    RETURN(0);
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
        LOGF("Container file name: %s", containerName);

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
        
// TODO: Add your own additional methods here!

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
