#include "inode.h"

#include "blockdevice.h"
#include <string.h>
#include <iostream>

void Inode::create(BlockDevice *bd, uint32_t index, char* fileName, uint32_t firstDataBlock, uint32_t fileSize, uint32_t atime, uint32_t mtime, uint32_t ctime, uint32_t userID, uint32_t groupID, uint32_t mode){
    InodeStruct *newInode = (InodeStruct *)malloc(BLOCK_SIZE);
    newInode->fileName = fileName;
    newInode->uses = false;
    newInode->firstDataBlock = firstDataBlock
    newInode->fileSize = fileSize;
    newInode->atime = atime;
    newInode->mtime = mtime;
    newInode->ctime = ctime;
    newInode->userID = userID;
    newInode->groupID = groupID;
    newInode->mode = mode;


    bd->write(index, (char *)newInode);

}

InodeStruct* Inode::get(BlockDevice *bd, uint32_t index){
    //create struct the with the size of Block_Size
    InodeStruct* currentInode = (InodeStruct *)malloc(BLOCK_SIZE);
    bd->read(index, (char *)currentInode);
    return currentInode;
}
