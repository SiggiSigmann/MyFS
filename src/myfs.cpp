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
}

MyFS::~MyFS() {
    
}

int MyFS::fuseGetattr(const char *path, struct stat *statbuf) {
    LOGM();
    
    // TODO: Implement this!
    
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
    
    // TODO: Implement this!
    
    RETURN(0);
}

int MyFS::fuseRead(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();
    
    // TODO: Implement this!

    RETURN(0);
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
    
    // TODO: Implement this!
    
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
    
    // TODO: Implement this!
    
    RETURN(0);
}

int MyFS::fuseReaddir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();
    
    // TODO: Implement this!
    
    RETURN(0);
    
    // <<< My new code
}

int MyFS::fuseReleasedir(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();
    
    // TODO: Implement this!
    
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
        LOGF("Container file name: %s", ((MyFsInfo *) fuse_get_context()->private_data)->contFile);
        
        // TODO: Implement your initialization methods here!
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
void writeBytes(BlockDevice bd, int firstBlockIndex, char* bytes, int size) {
    // number of perfect blocks
    int numberOfBlocks = size / BD_BLOCK_SIZE;

    int remainder = size % BD_BLOCK_SIZE;

    // write all perfect blocks
    for(int i = 0; i < numberOfBlocks; i++) {
        bd.write(i, bytes + i * BD_BLOCK_SIZE);
    }

    // write remainding bytes (as one block), if necessary
    if (remainder > 0){
        //bd.write only accepts buffers of size BD_BLOCK_SIZE, so we need to temporarily create one
        char* remainderBlockBytes = new char[BD_BLOCK_SIZE];
        //copy the remainder amount of bytes to the new block buffer, offset: all recently wrote perfect blocks
        memcpy(remainderBlockBytes, bytes + (numberOfBlocks * BD_BLOCK_SIZE), remainder);
        //set the boilerplate values to zero
        memset(remainderBlockBytes+remainder, 0, BD_BLOCK_SIZE-remainder);
        bd.write(numberOfBlocks, remainderBlockBytes);
        //cleanup the temporary buffer
        delete [] remainderBlockBytes;
    }
}

/*
Reads 'numberOfBytes' bytes from the block device, starting at index 'firstBlockIndex'
*/
char* readBytes(BlockDevice bd, int firstBlockIndex, int numberOfBytes) {
    char* readBuffer = new char[numberOfBytes];
    // number of perfect blocks
    int numberOfBlocks = numberOfBytes / BD_BLOCK_SIZE;
    
    int remainder = numberOfBytes % BD_BLOCK_SIZE;

    // read all perfect blocks
    for(int i = 0; i < numberOfBlocks; i++) {
        bd.read(i, readBuffer + i * BD_BLOCK_SIZE);
    }

    // read remainding bytes (as one block), if necessary
    if (remainder > 0){
        //bd.read only accepts buffers of size BD_BLOCK_SIZE, so we need to temporarily create one
        char* remainderBlockBytes = new char[BD_BLOCK_SIZE];
        //read the entire block containing the remainding bytes (obviously the block after the last perfect block)
        bd.read(numberOfBlocks, remainderBlockBytes);
        //append all remainding bytes after the perfect blocks
        memcpy(readBuffer + (numberOfBlocks*BD_BLOCK_SIZE), remainderBlockBytes, remainder);
        //cleanup the temporary buffer
        delete [] remainderBlockBytes;
    }
    return readBuffer;
}
/*
Writes the Superblock to the BlockDevice.
*/
void MyFS::writeSuperblock(BlockDevice bd) {
    char* buffer = new char[BD_BLOCK_SIZE];
    //Serialization of the Superblock
    uint32_t* ptrBuffer = (uint32_t*) buffer;

    //general specs
    *ptrBuffer++ = MyFS::superblock.SUPERBLOCK_BLOCK_INDEX;
    *ptrBuffer++ = MyFS::superblock.NUMBER_OF_USABLE_DATABLOCKS;
    *ptrBuffer++ = MyFS::superblock.NUMBER_OF_INODES;

    //I-Map
    *ptrBuffer++ = MyFS::superblock.NUMBER_OF_I_MAP_BLOCKS;
    *ptrBuffer++ = MyFS::superblock.I_MAP_FIRST_BLOCK;
    *ptrBuffer++ = MyFS::superblock.I_MAP_LAST_BLOCK;

    //Inodes
    *ptrBuffer++ = MyFS::superblock.NUMBER_OF_INODE_BLOCKS;
    *ptrBuffer++ = MyFS::superblock.FIRST_INODE_BLOCK;
    *ptrBuffer++ = MyFS::superblock.LAST_INODE_BLOCK;

    //D-Map
    *ptrBuffer++ = MyFS::superblock.NUMBER_OF_D_MAP_BLOCKS;
    *ptrBuffer++ = MyFS::superblock.D_MAP_FIRST_BLOCK;
    *ptrBuffer++ = MyFS::superblock.D_MAP_LAST_BLOCK;

    //FAT
    *ptrBuffer++ = MyFS::superblock.FAT_SIZE_IN_BYTES;
    *ptrBuffer++ = MyFS::superblock.NUMBER_OF_FAT_BLOCKS;
    *ptrBuffer++ = MyFS::superblock.FAT_FIRST_BLOCK;
    *ptrBuffer++ = MyFS::superblock.FAT_LAST_BLOCK;

    //Data Blocks
    *ptrBuffer++ = MyFS::superblock.FIRST_DATA_BLOCK;
    *ptrBuffer++ = MyFS::superblock.LAST_DATA_BLOCK;

    //runtime calculated values
    *ptrBuffer++ = MyFS::superblock.number_of_free_inodes;
    *ptrBuffer++ = MyFS::superblock.first_free_inode;
    *ptrBuffer++ = MyFS::superblock.number_of_free_blocks;
    *ptrBuffer++ = MyFS::superblock.first_free_block;

    //Write the Superblock
    writeBytes(bd, MyFS::superblock.SUPERBLOCK_BLOCK_INDEX, buffer, BD_BLOCK_SIZE);
    delete [] buffer;
}

/*
Reads the Superblock from the BlockDevice into the Superblock struct.
*/
void MyFS::readSuperblock(BlockDevice bd) {
    char* buffer = readBytes(bd, MyFS::superblock.SUPERBLOCK_BLOCK_INDEX, BD_BLOCK_SIZE);
    uint32_t* ptrBuffer = (uint32_t*) buffer;
    
    //general specs
    MyFS::superblock.SUPERBLOCK_BLOCK_INDEX = *ptrBuffer++;
    MyFS::superblock.NUMBER_OF_USABLE_DATABLOCKS = *ptrBuffer++;
    MyFS::superblock.NUMBER_OF_INODES = *ptrBuffer++;

    //I-Map
    MyFS::superblock.NUMBER_OF_I_MAP_BLOCKS = *ptrBuffer++;
    MyFS::superblock.I_MAP_FIRST_BLOCK = *ptrBuffer++;
    MyFS::superblock.I_MAP_LAST_BLOCK = *ptrBuffer++;

    //Inodes
    MyFS::superblock.NUMBER_OF_INODE_BLOCKS = *ptrBuffer++;
    MyFS::superblock.FIRST_INODE_BLOCK = *ptrBuffer++;
    MyFS::superblock.LAST_INODE_BLOCK = *ptrBuffer++;

    //D-Map
    MyFS::superblock.NUMBER_OF_D_MAP_BLOCKS = *ptrBuffer++;
    MyFS::superblock.D_MAP_FIRST_BLOCK = *ptrBuffer++;
    MyFS::superblock.D_MAP_LAST_BLOCK = *ptrBuffer++;

    //FAT
    MyFS::superblock.FAT_SIZE_IN_BYTES = *ptrBuffer++;
    MyFS::superblock.NUMBER_OF_FAT_BLOCKS = *ptrBuffer++;
    MyFS::superblock.FAT_FIRST_BLOCK = *ptrBuffer++;
    MyFS::superblock.FAT_LAST_BLOCK = *ptrBuffer++;

    //Data Blocks
    MyFS::superblock.FIRST_DATA_BLOCK = *ptrBuffer++;
    MyFS::superblock.LAST_DATA_BLOCK = *ptrBuffer++;

    //runtime calculated values
    MyFS::superblock.number_of_free_inodes = *ptrBuffer++;
    MyFS::superblock.first_free_inode = *ptrBuffer++;
    MyFS::superblock.number_of_free_blocks = *ptrBuffer++;
    MyFS::superblock.first_free_block = *ptrBuffer++;

    delete [] buffer;
}
