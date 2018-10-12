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

TEST_CASE( "Test Inode helper", "[inode]" ) {
    BlockDevice *bd = new BlockDevice(BLOCK_SIZE);

    remove(BD_PATH);
    bd->create(BD_PATH);
    
    SECTION("write / read To inode") {
        RootBlock *rootb = new RootBlock();

        uint32_t index = 1000;
        //write inode with data to block 10
        char fileName[FILENAME_MAX] = "test";
        rootb->createInode(bd,index,true,fileName,1,100,100,100,100,100,100,100);
        
        //create new inode struct to store data, size = blockSize
        InodeStruct* testInode = (InodeStruct *)malloc(BLOCK_SIZE);
        //read inode struct from block 10
        testInode = rootb->getInode(bd,index);

        REQUIRE(strcmp(fileName,testInode->fileName) == 0);
        REQUIRE(100==testInode->fileSize);
        REQUIRE(100==testInode->mode);
        //create rootblock with 64 inodes
        rootb->create(bd);
        //get first inode
        InodeStruct* testInode2 = (InodeStruct *)malloc(BLOCK_SIZE);
        testInode = rootb->getInode(bd,I_MAP_FIRST_BLOCK);
        //test first inode in rootblock
        REQUIRE(false==testInode->used);
        REQUIRE(strcmp(testInode->fileName,"empty") == 0);
        print
        

        delete rootb;
        free(testInode);
        free(testInode2);


    }
    
    bd->close();
    remove(BD_PATH);
    
}
