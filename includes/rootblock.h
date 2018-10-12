/*
manage the fat of the FS
*/

#ifndef rootblock_h
#define rootblock_h
#include "myfs.h"
#include "blockdevice.h"
#include <stdlib.h>
#include "constants.h"

struct InodeStruct{
    char* fileName;
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
        void createInode(BlockDevice *bd, uint32_t index,char* fileName, uint32_t firstDataBlock,uint32_t fileSize, uint32_t atime, uint32_t mtime, uint32_t ctime, uint32_t userID, uint32_t groupID, uint32_t mode);
        InodeStruct* getInode(BlockDevice *bd, uint32_t index);
        void create(BlockDevice*bd);

};

#endif