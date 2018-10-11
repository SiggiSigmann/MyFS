/*
manage the fat of the FS
*/

#ifndef inode
#define inode
#include "myfs.h"
#include "blockdevice.h"
#include <stdlib.h>

struct InodeStruct{
    char* fileName;
    bool used;
    uint32_t firstDataBlock;
    uint32_t fileSize;
    uint32_t atime;
    uint32_t mtime;
    uint32_t ctime;
    uint32_t userID;
    uint32_t groupID;
    uint32_t mode;
    
};

class RootBlock{
    public:  
        void createInode(BlockDevice *bd, uint32_t index, bool used,char* fileName, uint32_t firstDataBlock,uint32_t fileSize, uint32_t atime, uint32_t mtime, uint32_t ctime, uint32_t userID, uint32_t groupID, uint32_t mode);
        InodeStruct* getInode(BlockDevice *bd, uint32_t index);

};

#endif