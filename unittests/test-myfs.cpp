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
        originalSuperblock.uNumber_of_free_inodes = 21;

        //fill superblock struct with custom values
        myfs->superblock.uSUPERBLOCK_BLOCK_INDEX = 2;
        myfs->superblock.uNUMBER_OF_USABLE_DATABLOCKS = 3;
        myfs->superblock.uNUMBER_OF_INODES = 4;
        myfs->superblock.uNUMBER_OF_I_MAP_BLOCKS = 5;
        myfs->superblock.uI_MAP_FIRST_BLOCK = 6;
        myfs->superblock.uI_MAP_LAST_BLOCK = 7;
        myfs->superblock.uNUMBER_OF_INODE_BLOCKS = 8;
        myfs->superblock.uFIRST_INODE_BLOCK = 9;
        myfs->superblock.uLAST_INODE_BLOCK = 10;
        myfs->superblock.uNUMBER_OF_D_MAP_BLOCKS = 11;
        myfs->superblock.uD_MAP_FIRST_BLOCK = 12;
        myfs->superblock.uD_MAP_LAST_BLOCK = 13;
        myfs->superblock.uFAT_SIZE_IN_BYTES = 14;
        myfs->superblock.uNUMBER_OF_FAT_BLOCKS = 15;
        myfs->superblock.uFAT_FIRST_BLOCK = 16;
        myfs->superblock.uFAT_LAST_BLOCK = 17;
        myfs->superblock.uFIRST_DATA_BLOCK = 18;
        myfs->superblock.uLAST_DATA_BLOCK = 19;
        myfs->superblock.uNumber_of_free_inodes = 20;
        myfs->superblock.uFirst_free_inode = 21;
        myfs->superblock.uNumber_of_free_blocks = 22;
        myfs->superblock.uFirst_free_block = 23;
        //write Superblock
        myfs->writeSuperblock(bd);

        //modify Superblock
        myfs->superblock.uNumber_of_free_inodes = 1337;

        //read Superblock
        myfs->readSuperblock(bd);

        //validate that the deep copy is still unchanged
        REQUIRE(originalSuperblock.uNumber_of_free_inodes == 21);
        
        //validate the superblock custom values
        REQUIRE(myfs->superblock.uSUPERBLOCK_BLOCK_INDEX == 2);
        REQUIRE(myfs->superblock.uNUMBER_OF_USABLE_DATABLOCKS == 3);
        REQUIRE(myfs->superblock.uNUMBER_OF_INODES == 4);
        REQUIRE(myfs->superblock.uNUMBER_OF_I_MAP_BLOCKS == 5);
        REQUIRE(myfs->superblock.uI_MAP_FIRST_BLOCK == 6);
        REQUIRE(myfs->superblock.uI_MAP_LAST_BLOCK == 7);
        REQUIRE(myfs->superblock.uNUMBER_OF_INODE_BLOCKS == 8);
        REQUIRE(myfs->superblock.uFIRST_INODE_BLOCK == 9);
        REQUIRE(myfs->superblock.uLAST_INODE_BLOCK == 10);
        REQUIRE(myfs->superblock.uNUMBER_OF_D_MAP_BLOCKS == 11);
        REQUIRE(myfs->superblock.uD_MAP_FIRST_BLOCK == 12);
        REQUIRE(myfs->superblock.uD_MAP_LAST_BLOCK == 13);
        REQUIRE(myfs->superblock.uFAT_SIZE_IN_BYTES == 14);
        REQUIRE(myfs->superblock.uNUMBER_OF_FAT_BLOCKS == 15);
        REQUIRE(myfs->superblock.uFAT_FIRST_BLOCK == 16);
        REQUIRE(myfs->superblock.uFAT_LAST_BLOCK == 17);
        REQUIRE(myfs->superblock.uFIRST_DATA_BLOCK == 18);
        REQUIRE(myfs->superblock.uLAST_DATA_BLOCK == 19);
        REQUIRE(myfs->superblock.uNumber_of_free_inodes == 20);
        REQUIRE(myfs->superblock.uFirst_free_inode == 21);
        REQUIRE(myfs->superblock.uNumber_of_free_blocks == 22);
        REQUIRE(myfs->superblock.uFirst_free_block == 23);
    }

    bd.close();
    remove(BD_PATH);
}

TEST_CASE( "Occupy/Free DMap", "[dmap]" ) {
    
    remove(BD_PATH);
    
    BlockDevice bd;
    bd.create(BD_PATH);
    MyFS* myfs = new MyFS();
    
    SECTION("occupy and free DMap") {
        DMap* map = new DMap();
        uint32_t testIndex = 45;
        map->freeDatablock(testIndex);
        REQUIRE(map->get(testIndex) == false);
        map->occupyDatablock(testIndex);
        REQUIRE(map->get(testIndex) == true);
        map->freeDatablock(testIndex);
        REQUIRE(map->get(testIndex) == false);
    }

    bd.close();
    remove(BD_PATH);
}

TEST_CASE( "read/write DMap", "[dmap]" ) {
    
    remove(BD_PATH);
    
    BlockDevice bd;
    bd.create(BD_PATH);
    MyFS* myfs = new MyFS();
    
    SECTION("read and write DMap") {
        DMap* map = new DMap();
        //choose 3 indices to test
        uint32_t testIndex1 = 12;
        uint32_t testIndex2 = 23;
        uint32_t testIndex3 = 34;
        //initialize them as free
        map->freeDatablock(testIndex1);
        map->freeDatablock(testIndex2);
        map->freeDatablock(testIndex3);
        //validate that
        REQUIRE(map->get(testIndex1) == false);
        REQUIRE(map->get(testIndex2) == false);
        REQUIRE(map->get(testIndex3) == false);
        //claim them as occupied
        map->occupyDatablock(testIndex1);
        map->occupyDatablock(testIndex2);
        map->occupyDatablock(testIndex3);
        //validate that
        REQUIRE(map->get(testIndex1) == true);
        REQUIRE(map->get(testIndex2) == true);
        REQUIRE(map->get(testIndex3) == true);
        //write them to the block device
        map->writeDMap(bd);
        //manipulate the test indices randomly
        map->freeDatablock(testIndex1);
        map->freeDatablock(testIndex3);
        REQUIRE(map->get(testIndex1) == false);
        REQUIRE(map->get(testIndex3) == false);
        //read from the block device and hereby override the manipulated test indices
        map->readDMap(bd);
        //validate the test indices to be restored correctly by the block device
        REQUIRE(map->get(testIndex1) == true);
        REQUIRE(map->get(testIndex2) == true);
        REQUIRE(map->get(testIndex3) == true);
    }

    bd.close();
    remove(BD_PATH);
}

TEST_CASE( "Test FAT", "[fat]" ) {
    
    remove(BD_PATH);
    
    BlockDevice bd;
    bd.create(BD_PATH);
    MyFS* myfs = new MyFS();
    
    SECTION("test updating FAT") {
        FatHandler* fat = new FatHandler();
        REQUIRE(fat->get(12) == EMPTY_FAT_ENTRY);
        REQUIRE(fat->get(23) == EMPTY_FAT_ENTRY);
        REQUIRE(fat->get(34) == EMPTY_FAT_ENTRY);
        fat->set(12,23);
        fat->set(23,34);
        fat->set(34,END_OF_FILE_ENTRY);
        REQUIRE(fat->get(12) == 23);
        REQUIRE(fat->get(23) == 34);
        REQUIRE(fat->get(34) == END_OF_FILE_ENTRY);
        fat->set(12,EMPTY_FAT_ENTRY);
        fat->set(23,EMPTY_FAT_ENTRY);
        fat->set(34,EMPTY_FAT_ENTRY);
        REQUIRE(fat->get(12) == EMPTY_FAT_ENTRY);
        REQUIRE(fat->get(23) == EMPTY_FAT_ENTRY);
        REQUIRE(fat->get(34) == EMPTY_FAT_ENTRY);
    }

    SECTION("test deleting multiple FAT entries") {
        FatHandler* fat = new FatHandler();
        REQUIRE(fat->get(12) == EMPTY_FAT_ENTRY);
        REQUIRE(fat->get(23) == EMPTY_FAT_ENTRY);
        REQUIRE(fat->get(34) == EMPTY_FAT_ENTRY);
        fat->set(12,23);
        fat->set(23,34);
        fat->set(34,END_OF_FILE_ENTRY);
        REQUIRE(fat->get(12) == 23);
        REQUIRE(fat->get(23) == 34);
        REQUIRE(fat->get(34) == END_OF_FILE_ENTRY);
        fat->deleteAll(12);
        REQUIRE(fat->get(12) == EMPTY_FAT_ENTRY);
        REQUIRE(fat->get(23) == EMPTY_FAT_ENTRY);
        REQUIRE(fat->get(34) == EMPTY_FAT_ENTRY);
    }

    SECTION("read/write FAT"){
        FatHandler* fat = new FatHandler();
        REQUIRE(fat->get(12) == EMPTY_FAT_ENTRY);
        REQUIRE(fat->get(23) == EMPTY_FAT_ENTRY);
        REQUIRE(fat->get(34) == EMPTY_FAT_ENTRY);
        fat->set(12,23);
        fat->set(23,34);
        fat->set(34,END_OF_FILE_ENTRY);
        REQUIRE(fat->get(12) == 23);
        REQUIRE(fat->get(23) == 34);
        REQUIRE(fat->get(34) == END_OF_FILE_ENTRY);
        fat->writeFat(bd);
        fat->deleteAll(12);
        REQUIRE(fat->get(12) == EMPTY_FAT_ENTRY);
        REQUIRE(fat->get(23) == EMPTY_FAT_ENTRY);
        REQUIRE(fat->get(34) == EMPTY_FAT_ENTRY);
        fat->readFat(bd);
        REQUIRE(fat->get(12) == 23);
        REQUIRE(fat->get(23) == 34);
        REQUIRE(fat->get(34) == END_OF_FILE_ENTRY);
    }

    bd.close();
    remove(BD_PATH);
}