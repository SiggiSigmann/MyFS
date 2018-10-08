/*
manage the fat of the FS
*/

#ifndef inode
#define inode
#include "myfs.h"
#include "blockdevice.h"
#include <stdlib.h>

struct InodeStruct{
    unsigned int dmapAddress;
    unsigned int fatAdress;
    unsigned int rootAdress;
    unsigned int maxFiles;
    unsigned int blockSize;
    unsigned int amountOfBlocks;
};

class Inode{
    public:        
        InodeStruct* get(BlockDevice *bd, int index);
        void create(BlockDevice *bd, int index, int testNumber);

};

#endif