/*
Manage the rootblock of the FS
*/

#ifndef rootblock_h
#define rootblock_h

#include "constants.h"
#include <cstdint>

//define what values a inode must contain
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

/*
The class RootBlock manage the indoes of a filesystem
*/
class RootBlockHelper{
    private:
        InodeStruct *inode;
    public:
        RootBlockHelper();

};


#endif