//
//  test-blockdevice.cpp
//  testing
//
//  Created by Oliver Waldhorst on 15.12.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
//

#include "catch.hpp"

#include <stdio.h>
#include <string.h>

#include "helper.hpp"

#include "blockdevice.h"

#define BD_PATH "/tmp/bd.bin"
#define NUM_TESTBLOCKS 1024

TEST_CASE( "Write/Read blocks", "[blockdevice]" ) {
    
    remove(BD_PATH);
    
    BlockDevice bd;
    bd.create(BD_PATH);
    
    SECTION("writing single block") {
        char* readBuffer= new char[BD_BLOCK_SIZE];
        memset(readBuffer, 0, BD_BLOCK_SIZE);
        
        char* writeBuffer= new char[BD_BLOCK_SIZE];
        gen_random(writeBuffer, BD_BLOCK_SIZE);
        
        bd.write(0, writeBuffer);
        bd.read(0, readBuffer);
        
        REQUIRE(memcmp(writeBuffer, readBuffer, BD_BLOCK_SIZE) == 0);

        delete [] readBuffer;
        delete [] writeBuffer;
    }
    
    SECTION("writing multiple blocks") {
        char* readBuffer = new char[BD_BLOCK_SIZE * NUM_TESTBLOCKS];
        memset(readBuffer, 0, BD_BLOCK_SIZE * NUM_TESTBLOCKS);
        
        char* writeBuffer = new char[BD_BLOCK_SIZE * NUM_TESTBLOCKS];
        gen_random(writeBuffer, BD_BLOCK_SIZE * NUM_TESTBLOCKS);
        
        // write all blocks
        for(int i = 0; i < NUM_TESTBLOCKS; i++) {
            bd.write(i, writeBuffer + i*BD_BLOCK_SIZE);
        }
        
        // read all blocks
        for(int i = 0; i < NUM_TESTBLOCKS; i++) {
            bd.read(i, readBuffer + i*BD_BLOCK_SIZE);
        }

        REQUIRE(memcmp(writeBuffer, readBuffer, BD_BLOCK_SIZE * NUM_TESTBLOCKS) == 0);
        
        delete [] readBuffer;
        delete [] writeBuffer;
    }
    
    bd.close();
    remove(BD_PATH);
    
}
