#include "rootblock.h"

#include "blockdevice.h"
#include <string.h>
#include <iostream>

//create new Inode
void RootBlock::createInode(BlockDevice *bd, uint32_t index, bool used, char* fileName, uint32_t firstDataBlock, uint32_t fileSize, uint32_t atime, uint32_t mtime, uint32_t ctime, uint32_t userID, uint32_t groupID, uint32_t mode){
    InodeStruct *newInode = (InodeStruct *)malloc(BLOCK_SIZE);
    newInode->used = false;
    newInode->firstDataBlock = firstDataBlock;
    newInode->fileName = fileName;
    newInode->fileSize = fileSize;
    newInode->atime = atime;
    newInode->mtime = mtime;
    newInode->ctime = ctime;
    newInode->userID = userID;
    newInode->groupID = groupID;
    newInode->mode = mode;
    bd->write(index, (char *)newInode);
    free(newInode);

}
//gets inode from blockdevice
InodeStruct* RootBlock::getInode(BlockDevice *bd, uint32_t index){
    //create struct the with the size of Block_Size
    InodeStruct* currentInode = (InodeStruct *)malloc(BLOCK_SIZE);
    bd->read(index, (char *)currentInode);
    return currentInode;
}
//create rootblock, creates NUM_DIR_ENTRIES Inodes starting from I_MAP_FIRST_BLOCKS
void RootBlock::create(BlockDevice*bd){
    //create inodes for rootblock
    for(int i;i <= NUM_DIR_ENTRIES; i++){
        
        char fileName[FILENAME_MAX] = "empty";
        createInode(bd,I_MAP_FIRST_BLOCK+i,false, fileName,0,0,0,0,0,0,0,0);
        printf("Create inode at %i \n",I_MAP_FIRST_BLOCK+i);

    }
    printf("Created %i inodes starting from %i \n",NUM_DIR_ENTRIES,I_MAP_FIRST_BLOCK);

}
