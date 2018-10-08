#include "inode.h"

#include "blockdevice.h"
#include <string.h>
#include <iostream>

void Inode::create(BlockDevice *bd, int index){
    InodeStruct *newInode = (InodeStruct *)malloc(BLOCK_SIZE);
    bd->write(index, (char *)newInode);

}

InodeStruct* getInode((BlockDevice *bd, int index){
    InodeStruct* currentInode = (InodeStruct *)malloc(BLOCK_SIZE);
    bd->read(index, (char *)currentInode);
    return currentInode;
}
