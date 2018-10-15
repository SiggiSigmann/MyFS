#include "rootblock.h"

#include "blockdevice.h"
#include <string.h>
#include <iostream>

//create a new Inode directly on the block device
void RootBlock::createInode(BlockDevice *bd, uint32_t index, char *fileName, uint32_t firstDataBlock, uint32_t fileSize,
        uint32_t atime, uint32_t mtime, uint32_t ctime, uint32_t userID, uint32_t groupID, uint32_t mode){
    InodeStruct *inode = (InodeStruct *)malloc(BLOCK_SIZE);
    inode->firstDataBlock = firstDataBlock;
    
    strcpy(inode->fileName, fileName);
    
    inode->fileSize = fileSize;
    inode->atime = atime;
    inode->mtime = mtime;
    inode->ctime = ctime;
    inode->userID = userID;
    inode->groupID = groupID;
    inode->mode = mode;
    //write inode to block device
    bd->write(index, (char *)inode);
    //delete the inode in memory
    free(inode);
}

//reads and returns the demanded inode from the blockdevice
InodeStruct* RootBlock::getInode(BlockDevice *bd, uint32_t index){
    //create struct with the size of BLOCK_SIZE
    InodeStruct* currentInode = (InodeStruct *)malloc(BLOCK_SIZE);
    bd->read(index, (char *)currentInode);
    return currentInode;
}

//creates the rootblock including all Inodes and writes them on the block device
void RootBlock::init(BlockDevice*bd){
    //create inodes for rootblock
    for(int i=0;i < NUM_DIR_ENTRIES; i++){
        char* fileName = new char[NAME_LENGTH];
        strcpy(fileName,"empty");
        createInode(bd,I_MAP_FIRST_BLOCK+i, fileName,0,0,0,0,0,0,0,0);
        printf("Created Inode %i \n",I_MAP_FIRST_BLOCK+i);

    }
    printf("Created %i inodes starting from %i\n",NUM_DIR_ENTRIES,I_MAP_FIRST_BLOCK);

}
