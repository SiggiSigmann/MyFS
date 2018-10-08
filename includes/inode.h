/*
manage the fat of the FS
*/

#ifndef inode
#define inode
#include "myfs.h"
#include "blockdevice.h"
#include <stdlib.h>

struct InodeStruct{
    unsigned int testNumber;
};

class Inode{
    public:        
        InodeStruct* get(BlockDevice *bd, int index);
        void create(BlockDevice *bd, int index, int testNumber);

};

#endif