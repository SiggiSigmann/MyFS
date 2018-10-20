#ifndef rootblock_h
#define rootblock_h
#include "blockdevice.h"
#include <stdlib.h>
#include "constants.h"

struct InodeStruct{
    char fileName[NAME_LENGTH];
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
        void init(BlockDevice*bd);
        void updateInode(BlockDevice *bd, uint32_t blockIndex, char *fileName, uint32_t firstDataBlock, uint32_t fileSize,
            uint32_t atime, uint32_t mtime, uint32_t ctime, uint32_t userID, uint32_t groupID, uint32_t mode);
        InodeStruct* getInode(BlockDevice *bd, uint32_t relativeIndex);
        uint32_t checkFilenameOccupied(BlockDevice *bd,char *fileName);
        InodeStruct* getInodeByName(BlockDevice *bd, char *fileName);
        char* getFileName(BlockDevice *bd, uint32_t relativeIndex);
    private:
        void createInode(BlockDevice *bd, uint32_t blockIndex, char *fileName, uint32_t firstDataBlock, uint32_t fileSize,
            uint32_t atime, uint32_t mtime, uint32_t ctime, uint32_t userID, uint32_t groupID, uint32_t mode);

};

#endif
