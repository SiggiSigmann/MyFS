/*
manage the fat of the FS
*/

#ifndef inode
#define inode
#include "myfs.h"
#include "blockdevice.h"
#include <stdlib.h>

struct InodeStruct{
    uint32_t testNumber;
};

class Inode{
    public:        
        InodeStruct* get(BlockDevice *bd, uint32_t index);
        void create(BlockDevice *bd, uint32_t index, uint32_t testNumber);

};

#endif