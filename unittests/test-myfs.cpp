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

TEST_CASE( "Write/Read Superblock", "[superblock]" ) {
    
    remove(BD_PATH);
    
    BlockDevice bd;
    bd.create(BD_PATH);
    MyFS* myfs = new MyFS();
    
    SECTION("writing Superblock") {
        //copy Superblock (deep copy)
        MyFS::Superblock originalSuperblock = myfs->superblock;
        originalSuperblock.number_of_free_inodes = 21;

        //fill superblock struct with custom values
        myfs->superblock.SUPERBLOCK_BLOCK_INDEX = 2;
        myfs->superblock.NUMBER_OF_USABLE_DATABLOCKS = 3;
        myfs->superblock.NUMBER_OF_INODES = 4;
        myfs->superblock.NUMBER_OF_I_MAP_BLOCKS = 5;
        myfs->superblock.I_MAP_FIRST_BLOCK = 6;
        myfs->superblock.I_MAP_LAST_BLOCK = 7;
        myfs->superblock.NUMBER_OF_INODE_BLOCKS = 8;
        myfs->superblock.FIRST_INODE_BLOCK = 9;
        myfs->superblock.LAST_INODE_BLOCK = 10;
        myfs->superblock.NUMBER_OF_D_MAP_BLOCKS = 11;
        myfs->superblock.D_MAP_FIRST_BLOCK = 12;
        myfs->superblock.D_MAP_LAST_BLOCK = 13;
        myfs->superblock.FAT_SIZE_IN_BYTES = 14;
        myfs->superblock.NUMBER_OF_FAT_BLOCKS = 15;
        myfs->superblock.FAT_FIRST_BLOCK = 16;
        myfs->superblock.FAT_LAST_BLOCK = 17;
        myfs->superblock.FIRST_DATA_BLOCK = 18;
        myfs->superblock.LAST_DATA_BLOCK = 19;
        myfs->superblock.number_of_free_inodes = 20;
        myfs->superblock.first_free_inode = 21;
        myfs->superblock.number_of_free_blocks = 22;
        myfs->superblock.first_free_block = 23;
        //write Superblock
        myfs->writeSuperblock(bd);

        //modify Superblock
        myfs->superblock.number_of_free_inodes = 1337;

        //read Superblock
        myfs->readSuperblock(bd);

        //validate that the deep copy is still unchanged
        REQUIRE(originalSuperblock.number_of_free_inodes == 21);
        
        //validate the superblock custom values
        REQUIRE(myfs->superblock.SUPERBLOCK_BLOCK_INDEX == 2);
        REQUIRE(myfs->superblock.NUMBER_OF_USABLE_DATABLOCKS == 3);
        REQUIRE(myfs->superblock.NUMBER_OF_INODES == 4);
        REQUIRE(myfs->superblock.NUMBER_OF_I_MAP_BLOCKS == 5);
        REQUIRE(myfs->superblock.I_MAP_FIRST_BLOCK == 6);
        REQUIRE(myfs->superblock.I_MAP_LAST_BLOCK == 7);
        REQUIRE(myfs->superblock.NUMBER_OF_INODE_BLOCKS == 8);
        REQUIRE(myfs->superblock.FIRST_INODE_BLOCK == 9);
        REQUIRE(myfs->superblock.LAST_INODE_BLOCK == 10);
        REQUIRE(myfs->superblock.NUMBER_OF_D_MAP_BLOCKS == 11);
        REQUIRE(myfs->superblock.D_MAP_FIRST_BLOCK == 12);
        REQUIRE(myfs->superblock.D_MAP_LAST_BLOCK == 13);
        REQUIRE(myfs->superblock.FAT_SIZE_IN_BYTES == 14);
        REQUIRE(myfs->superblock.NUMBER_OF_FAT_BLOCKS == 15);
        REQUIRE(myfs->superblock.FAT_FIRST_BLOCK == 16);
        REQUIRE(myfs->superblock.FAT_LAST_BLOCK == 17);
        REQUIRE(myfs->superblock.FIRST_DATA_BLOCK == 18);
        REQUIRE(myfs->superblock.LAST_DATA_BLOCK == 19);
        REQUIRE(myfs->superblock.number_of_free_inodes == 20);
        REQUIRE(myfs->superblock.first_free_inode == 21);
        REQUIRE(myfs->superblock.number_of_free_blocks == 22);
        REQUIRE(myfs->superblock.first_free_block == 23);
    }

    bd.close();
    remove(BD_PATH);
}
