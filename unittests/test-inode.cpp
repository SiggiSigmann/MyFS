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
        uint32_t testNumber = 1234;
        inodeHelper->create(bd,index,testNumber);
        
        InodeStruct* currentInode = (InodeStruct *)malloc(BLOCK_SIZE);
        currentInode = inodeHelper->get(bd,index);
        REQUIRE(testNumber==currentInode->testNumber);
        delete currentInode;
    }
    
    bd->close();
    remove(BD_PATH);
    
}
