//
//  test-inode.cpp
//  testing
//


#include "catch.hpp"
#include <iostream>

#include <stdio.h>
#include <string.h>

#include "helper.hpp"
#include "inode.h"

#include "blockdevice.h"
#include "myfs.h"

#define BD_PATH "/tmp/inode.bin"

TEST_CASE( "Test Inode helper", "[inode]" ) {
    BlockDevice *bd = new BlockDevice(BLOCK_SIZE);

    remove(BD_PATH);
    bd->create(BD_PATH);
    
    SECTION("write / read To inode") {
        Inode *inodeHelper = new Inode();

        uint32_t index = 10;
        //write inode with data to block 10
        char fileName[FILENAME_MAX] = "test";
        inodeHelper->create(bd,index,fileName,100,100,100,100,100,100,100);
        
        //create new inode struct to store data, size = blockSize
        InodeStruct* currentInode = (InodeStruct *)malloc(BLOCK_SIZE);
        //read inode struct from block 10
        currentInode = inodeHelper->get(bd,index);

        REQUIRE(strcmp(fileName,currentInode->fileName) == 0);
        REQUIRE(100==currentInode->fileSize);
        REQUIRE(100==currentInode->mode);

        delete inodeHelper;
        delete currentInode;
    }
    
    bd->close();
    remove(BD_PATH);
    
}
