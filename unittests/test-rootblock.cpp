//
//  test-inode.cpp
//  testing
//


#include "catch.hpp"
#include <iostream>

#include <stdio.h>
#include <string.h>

#include "helper.hpp"
#include "rootblock.h"

#include "blockdevice.h"
#include "myfs.h"

#define BD_PATH "/tmp/block.bin"

TEST_CASE( "update inode + read/write to block device", "[Rootblock]" ) {
    BlockDevice *bd = new BlockDevice(BLOCK_SIZE);

    remove(BD_PATH);
    bd->create(BD_PATH);
    
    SECTION("write / read To inode") {
        RootBlock *rootb = new RootBlock();

        uint32_t index = 1000;
        //write inode with data to block 10
        //char fileName[NAME_LENGTH] = "empty";
        char* fileName = new char[NAME_LENGTH];
        strcpy(fileName,EMPTY_INODE_STRING);
        rootb->createInode(bd,index,fileName,1,100,100,100,100,100,100,100);
        //create new inode struct to store data, size = blockSize
        InodeStruct* inode = (InodeStruct *)malloc(BLOCK_SIZE);
        //read inode struct from block 10
        inode = rootb->getInode(bd,index);
        REQUIRE(strcmp(fileName,inode->fileName) == 0);
        REQUIRE(100==inode->fileSize);
        REQUIRE(100==inode->mode);
        free(inode);


        //create rootblock with 64 inodes
        rootb->init(bd);
        //get first inode
        InodeStruct* firstInode = (InodeStruct *)malloc(BLOCK_SIZE);
        firstInode = rootb->getInode(bd,FIRST_INODE_BLOCK);
        
        //test first inode in rootblock
        REQUIRE(EMPTY_INODE_VALUE ==firstInode->firstDataBlock);
        REQUIRE(strcmp(firstInode->fileName,fileName) == 0);
        free(firstInode);

        //get last inode
        InodeStruct* lastInode = (InodeStruct *)malloc(BLOCK_SIZE);
        //needs to be changed to LAST_INODE_BLOCK
        lastInode = rootb->getInode(bd,FIRST_INODE_BLOCK+NUM_DIR_ENTRIES-1);
        //test last inode in rootblock
        REQUIRE(EMPTY_INODE_VALUE ==lastInode->firstDataBlock);
        REQUIRE(strcmp(lastInode->fileName,fileName) == 0);
        free(lastInode);


        delete rootb;


    }
    
    bd->close();
    remove(BD_PATH);
    
}
