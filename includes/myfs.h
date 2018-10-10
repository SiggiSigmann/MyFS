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
#include "myfs-structs.h"

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
        uint32_t uSUPERBLOCK_BLOCK_INDEX = 0;
        //important: uNUMBER_OF_USABLE_DATABLOCKS needs to be a multiple of BD_BLOCK_SIZE, to become perfectly mappable
        uint32_t uNUMBER_OF_USABLE_DATABLOCKS = 1 << 18; //2^18 = 262144 blocks => 128 MiB in total
        uint32_t uNUMBER_OF_INODES = 512; //needs to be a multiple of BD_BLOCK_SIZE, to become perfectly mappable

        //I-Map
        uint32_t uNUMBER_OF_I_MAP_BLOCKS = uNUMBER_OF_INODES/BD_BLOCK_SIZE; //can map (4byte int32_t)InodeNumber -> (1byte bool)used, 512-times per I-Map Block (512Bytes)
        uint32_t uI_MAP_FIRST_BLOCK = uSUPERBLOCK_BLOCK_INDEX + 1;
        uint32_t uI_MAP_LAST_BLOCK = uI_MAP_FIRST_BLOCK + uNUMBER_OF_I_MAP_BLOCKS;

        //Inodes
        uint32_t uNUMBER_OF_INODE_BLOCKS = uNUMBER_OF_INODES; //one block for each inode
        uint32_t uFIRST_INODE_BLOCK = uI_MAP_LAST_BLOCK + 1;
        uint32_t uLAST_INODE_BLOCK = uLAST_INODE_BLOCK + uNUMBER_OF_INODE_BLOCKS;

        //D-Map
        uint32_t uNUMBER_OF_D_MAP_BLOCKS = uNUMBER_OF_USABLE_DATABLOCKS/BD_BLOCK_SIZE; //can map (4byte int32_t)BlockNumber -> (1byte bool)used, 512-times per D-Map Block (512Bytes)
        uint32_t uD_MAP_FIRST_BLOCK = uLAST_INODE_BLOCK + 1;
        uint32_t uD_MAP_LAST_BLOCK = uD_MAP_FIRST_BLOCK + uNUMBER_OF_D_MAP_BLOCKS;

        //FAT
        uint32_t uFAT_SIZE_IN_BYTES = uNUMBER_OF_USABLE_DATABLOCKS * sizeof(uint32_t); //mapping requires sizeof(uint32_t)=4bytes per block: (uint32_t)blockIndex -> (uint32_t)nextBlockIndex
        uint32_t uNUMBER_OF_FAT_BLOCKS = uFAT_SIZE_IN_BYTES/BD_BLOCK_SIZE;
        uint32_t uFAT_FIRST_BLOCK = uD_MAP_LAST_BLOCK + 1;
        uint32_t uFAT_LAST_BLOCK = uFAT_FIRST_BLOCK + uNUMBER_OF_FAT_BLOCKS;

        //Data Blocks
        uint32_t uFIRST_DATA_BLOCK = uFAT_LAST_BLOCK + 1;
        uint32_t uLAST_DATA_BLOCK = uFIRST_DATA_BLOCK + uNUMBER_OF_USABLE_DATABLOCKS;

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

#endif /* myfs_h */
