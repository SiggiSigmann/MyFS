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
    uint32_t fileSize;
    uint32_t atime;
    uint32_t mtime;
    uint32_t ctime;
    uint32_t userID;
    uint32_t groupID;
    uint32_t mode;
};

class Inode{
    public:  
        void create(BlockDevice *bd, uint32_t index, char* fileName,uint32_t fileSize, uint32_t atime, uint32_t mtime, uint32_t ctime, uint32_t userID, uint32_t groupID, uint32_t mode);
        InodeStruct* get(BlockDevice *bd, uint32_t index);

};

#endif