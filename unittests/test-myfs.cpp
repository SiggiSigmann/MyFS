//
//  test-myfs.cpp
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
#include "myfs.h"

// TODO: Implement your tests here!
#define BD_PATH "/tmp/bd.bin"
#define NUM_TESTBYTES 13337

TEST_CASE( "Write/Read bytes", "[blockdevice]" ) {
    
    remove(BD_PATH);
    
    BlockDevice bd;
    bd.create(BD_PATH);
    
    SECTION("writing 13337 bytes") {
        char* writeBuffer = new char[NUM_TESTBYTES];
        gen_random(writeBuffer, NUM_TESTBYTES);
        
        // write all bytes
        writeBytes(bd, 0, writeBuffer, NUM_TESTBYTES);

        // read all bytes
        char* readBuffer = readBytes(bd, 0, NUM_TESTBYTES);

        REQUIRE(memcmp(writeBuffer, readBuffer, NUM_TESTBYTES) == 0);
    }

    bd.close();
    remove(BD_PATH);
    
}

TEST_CASE( "Write/Read Superblock", "[blockdevice]" ) {
    
    remove(BD_PATH);
    
    BlockDevice bd;
    bd.create(BD_PATH);
    MyFS* myfs = new MyFS();
    
    SECTION("writing Superblock") {
        //TODO fix: test fails
        //copy Superblock
        MyFS::Superblock originalSuperblock = myfs->superblock;
        originalSuperblock.number_of_free_inodes = 21;
        myfs->superblock.number_of_free_inodes = 45;
        //write Superblock
        myfs->writeSuperblock(bd);
        //modify Superblock
        myfs->superblock.number_of_free_inodes = 1337;
        //read Superblock
        myfs->readSuperblock(bd);
        REQUIRE(originalSuperblock.number_of_free_inodes == 21);
        REQUIRE(myfs->superblock.number_of_free_inodes == 45);
    }

    bd.close();
    remove(BD_PATH);
    
}
