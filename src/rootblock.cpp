#include "rootblock.h"
#include "blockdevice.h"
#include <string.h>
#include <iostream>

//creates the rootblock including all Inodes and writes them on the block device
void RootBlock::init(BlockDevice*bd){
    //create inodes for rootblock
    for(int i=0;i < NUMBER_OF_INODE_BLOCKS; i++){
        char* fileName = new char[NAME_LENGTH];
        strcpy(fileName,EMPTY_INODE_STRING);
        createInode(bd,FIRST_INODE_BLOCK+i, fileName,EMPTY_INODE_VALUE,EMPTY_INODE_VALUE,EMPTY_INODE_VALUE,EMPTY_INODE_VALUE,EMPTY_INODE_VALUE,EMPTY_INODE_VALUE,EMPTY_INODE_VALUE,EMPTY_INODE_VALUE,EMPTY_INODE_VALUE);
        // printf("Created Inode %i \n",FIRST_INODE_BLOCK+i);
        delete[] fileName;
    }
    //printf("Created %i inodes starting from %i\n",NUM_DIR_ENTRIES,FIRST_INODE_BLOCK);
}

//create a new Inode directly on the block device
void RootBlock::createInode(BlockDevice *bd, uint32_t blockIndex, char *fileName, uint32_t firstDataBlock, uint32_t fileSizeBytes, uint32_t fileSizeBlocks,
        uint32_t atime, uint32_t mtime, uint32_t ctime, uint32_t userID, uint32_t groupID, uint32_t mode){
    InodeStruct *inode = (InodeStruct *)malloc(BLOCK_SIZE);
    
    //copy everything into InodeStruct
    strcpy(inode->fileName, fileName);
    inode->firstDataBlock = firstDataBlock;
    inode->fileSizeBytes = fileSizeBytes;
    inode->fileSizeBlocks = fileSizeBlocks;
    inode->atime = atime;
    inode->mtime = mtime;
    inode->ctime = ctime;
    inode->userID = userID;
    inode->groupID = groupID;
    inode->mode = mode;
    //write inode to block device
    bd->write(blockIndex, (char *)inode);
    //delete the inode in memory
    free(inode);
}

//reads and returns the demanded inode from the blockdevice
InodeStruct* RootBlock::getInode(BlockDevice *bd, uint32_t relativeIndex){
    //create struct with the size of BLOCK_SIZE
    InodeStruct* currentInode = (InodeStruct *)malloc(BLOCK_SIZE);
    bd->read(FIRST_INODE_BLOCK+relativeIndex, (char *)currentInode);
    return currentInode;
}

/**
 * expects fileName, returns inode with filename, returns Null if filename doenst expects
 */
InodeStruct* RootBlock::getInodeByName(BlockDevice *bd, char *fileName){
    uint32_t relativeIndex = checkFilenameOccupied(bd,fileName);
    if(relativeIndex==(uint32_t)-1) return NULL;

    InodeStruct* inode = (InodeStruct *)malloc(BLOCK_SIZE);
    inode = getInode(bd,relativeIndex);
    return inode;
}

/**
 * Expects Blockdevice and relative inode index and returns
 * inode filename
 */
char* RootBlock::getFileName(BlockDevice *bd, uint32_t relativeIndex){
    InodeStruct *inode = (InodeStruct *)malloc(BLOCK_SIZE);
    bd->read(FIRST_INODE_BLOCK + relativeIndex, (char*)inode);
    char* name = new char[NAME_LENGTH];
    strcpy(name,inode->fileName);
    free(inode);
    return name;
}

/**
 * Updates inote, expexts relativeIndex and all inode params
 */
void RootBlock::updateInode(BlockDevice *bd, uint32_t relativeIndex, char *fileName, uint32_t firstDataBlock, uint32_t fileSizeBytes, uint32_t fileSizeBlocks,
    uint32_t atime, uint32_t mtime, uint32_t ctime, uint32_t userID, uint32_t groupID, uint32_t mode){
    char* chareinode = (char *)malloc(BLOCK_SIZE);
    for(int i =0; i<BLOCK_SIZE; i++){
        chareinode[i]=0;
    }
    InodeStruct* inode = (InodeStruct *)chareinode;
    printf("==> %s\n",fileName);
    strcpy(inode->fileName, fileName);
    inode->firstDataBlock = firstDataBlock;
    inode->fileSizeBytes = fileSizeBytes;
    inode->fileSizeBlocks = fileSizeBlocks;
    inode->atime = atime;
    inode->mtime = mtime;
    inode->ctime = ctime;
    inode->userID = userID;
    inode->groupID = groupID;
    inode->mode = mode;
    bd->write(FIRST_INODE_BLOCK + relativeIndex, (char*)inode);
    free(inode);
}

/**
 * Checks if fileName already Exists, Returns relative Index of 
 * the found Inode, returns -1 if not found
 */
uint32_t RootBlock::checkFilenameOccupied(BlockDevice *bd, char *fileName){
    InodeStruct *inode = (InodeStruct *)malloc(BLOCK_SIZE);
    for (uint32_t i = 0; i < NUMBER_OF_INODES; i++) {
            inode = getInode(bd, i); 
            if (strcmp(inode->fileName ,fileName) == 0){
                free(inode);
                return i;
            }
    }
    return -1;
}
