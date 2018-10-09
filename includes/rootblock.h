/*
Manage the rootblock of the FS
*/

#ifndef rootblock
#define rootblock

#include <cstdint>
#include "constants.h"
#include "blockdevice.h"
struct InodeStruct{
    char fileName[NAME_LENGTH];
    uint32_t fileSize;
    uint32_t userID;
    uint32_t groupID;
    uint32_t mode;
    uint32_t mtime;
    uint32_t ctime;
    uint32_t atime;
    uint32_t data;
};

class RootBlock{
    private:
        Inode *inode;
    public:
        RootBlock();

}


#endif