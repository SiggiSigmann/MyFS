//
//  myfs.h
//  myfs
//
//  Created by Oliver Waldhorst on 02.08.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
//

#ifndef myfs_h
#define myfs_h

#include <fuse.h>
#include <cmath>
#include <cstring>

#include "blockdevice.h"
#include "constants.h"
#include "myfs-structs.h"
#include "dmap.h"
#include "fat.h"

class MyFS {
private:
    static MyFS *_instance;
    FILE *logFile;
    
public:
    static MyFS *Instance();
    
    // TODO: Add attributes of your file system here
    
    struct Superblock {
        /*
        Superblock | I-Map | Inodes | D-Map | FAT | Data Blocks
        */

        //general specs
        uint32_t uSUPERBLOCK_BLOCK_INDEX = SUPERBLOCK_BLOCK_INDEX;
        uint32_t uNUMBER_OF_USABLE_DATABLOCKS = NUMBER_OF_USABLE_DATABLOCKS;
        uint32_t uNUMBER_OF_INODES = NUMBER_OF_INODES;

        //I-Map
        uint32_t uNUMBER_OF_I_MAP_BLOCKS = NUMBER_OF_I_MAP_BLOCKS;
        uint32_t uI_MAP_FIRST_BLOCK = I_MAP_FIRST_BLOCK;
        uint32_t uI_MAP_LAST_BLOCK = I_MAP_LAST_BLOCK;

        //Inodes
        uint32_t uNUMBER_OF_INODE_BLOCKS = NUMBER_OF_INODE_BLOCKS;
        uint32_t uFIRST_INODE_BLOCK = FIRST_INODE_BLOCK;
        uint32_t uLAST_INODE_BLOCK = LAST_INODE_BLOCK;

        //D-Map
        uint32_t uNUMBER_OF_D_MAP_BLOCKS = NUMBER_OF_D_MAP_BLOCKS; //can map (4byte int32_t)BlockNumber -> (1byte bool)used, 512-times per D-Map Block (512Bytes)
        uint32_t uD_MAP_FIRST_BLOCK = D_MAP_FIRST_BLOCK;
        uint32_t uD_MAP_LAST_BLOCK = D_MAP_LAST_BLOCK;

        //FAT
        uint32_t uFAT_SIZE_IN_BYTES = FAT_SIZE_IN_BYTES; //mapping requires sizeof(uint32_t)=4bytes per block: (uint32_t)blockIndex -> (uint32_t)nextBlockIndex
        uint32_t uNUMBER_OF_FAT_BLOCKS = NUMBER_OF_FAT_BLOCKS;
        uint32_t uFAT_FIRST_BLOCK = FAT_FIRST_BLOCK;
        uint32_t uFAT_LAST_BLOCK = FAT_LAST_BLOCK;

        //Data Blocks
        uint32_t uFIRST_DATA_BLOCK = FIRST_DATA_BLOCK;
        uint32_t uLAST_DATA_BLOCK = LAST_DATA_BLOCK;

        //runtime calculated values
        uint32_t uNumber_of_free_inodes;
        uint32_t uFirst_free_inode;
        uint32_t uNumber_of_free_blocks;
        uint32_t uFirst_free_block;
    } superblock;

    MyFS();
    ~MyFS();
    
    // --- Methods called by FUSE ---
    // For Documentation see https://libfuse.github.io/doxygen/structfuse__operations.html
    int fuseGetattr(const char *path, struct stat *statbuf);
    int fuseReadlink(const char *path, char *link, size_t size);
    int fuseMknod(const char *path, mode_t mode, dev_t dev);
    int fuseMkdir(const char *path, mode_t mode);
    int fuseUnlink(const char *path);
    int fuseRmdir(const char *path);
    int fuseSymlink(const char *path, const char *link);
    int fuseRename(const char *path, const char *newpath);
    int fuseLink(const char *path, const char *newpath);
    int fuseChmod(const char *path, mode_t mode);
    int fuseChown(const char *path, uid_t uid, gid_t gid);
    int fuseTruncate(const char *path, off_t newSize);
    int fuseUtime(const char *path, struct utimbuf *ubuf);
    int fuseOpen(const char *path, struct fuse_file_info *fileInfo);
    int fuseRead(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo);
    int fuseWrite(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo);
    int fuseStatfs(const char *path, struct statvfs *statInfo);
    int fuseFlush(const char *path, struct fuse_file_info *fileInfo);
    int fuseRelease(const char *path, struct fuse_file_info *fileInfo);
    int fuseFsync(const char *path, int datasync, struct fuse_file_info *fi);
#ifdef __APPLE__
    int fuseSetxattr(const char *path, const char *name, const char *value, size_t size, int flags, uint32_t x);
    int fuseGetxattr(const char *path, const char *name, char *value, size_t size, uint x);
#else
    int fuseSetxattr(const char *path, const char *name, const char *value, size_t size, int flags);
    int fuseGetxattr(const char *path, const char *name, char *value, size_t size);
#endif
    void* fuseInit(struct fuse_conn_info *conn);
    int fuseListxattr(const char *path, char *list, size_t size);
    int fuseRemovexattr(const char *path, const char *name);
    int fuseOpendir(const char *path, struct fuse_file_info *fileInfo);
    int fuseReaddir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fileInfo);
    int fuseReleasedir(const char *path, struct fuse_file_info *fileInfo);
    int fuseFsyncdir(const char *path, int datasync, struct fuse_file_info *fileInfo);
    int fuseTruncate(const char *path, off_t offset, struct fuse_file_info *fileInfo);
    int fuseCreate(const char *, mode_t, struct fuse_file_info *);
    void fuseDestroy();
    
    // TODO: Add methods of your file system here
    void writeSuperblock(BlockDevice bd);
    void readSuperblock(BlockDevice bd);
};
void writeBytes(BlockDevice bd, int firstBlockIndex, char* bytes, int size);
char* readBytes(BlockDevice bd, int firstBlockIndex, int numberOfBytes);
void memset4(uint32_t* ptr, uint32_t value, uint32_t size);

#endif /* myfs_h */
