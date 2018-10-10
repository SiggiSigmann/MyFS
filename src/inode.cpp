#include "inode.h"

#include "blockdevice.h"
#include <string.h>
#include <iostream>

void Inode::create(BlockDevice *bd, uint32_t index, uint32_t testNumber){
    InodeStruct *newInode = (InodeStruct *)malloc(BLOCK_SIZE);
    newInode->testNumber = testNumber;
    bd->write(index, (char *)newInode);

}

InodeStruct* Inode::get(BlockDevice *bd, uint32_t index){
    InodeStruct* currentInode = (InodeStruct *)malloc(BLOCK_SIZE);
    bd->read(index, (char *)currentInode);
    return currentInode;
}
