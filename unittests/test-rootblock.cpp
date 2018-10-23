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
    
    SECTION("Inode Lifecycle") {

        char* fileName = new char[NAME_LENGTH];
        strcpy(fileName,EMPTY_INODE_STRING);
        // Init RootBlock
        RootBlock *rootb = new RootBlock();
        //create rootblock with 64 inodes
        rootb->init(bd);

        // ─────────────────────────────────────────────────────────────────
        // Test first Inode

        // get first inode
        InodeStruct* firstInode = (InodeStruct *)malloc(BLOCK_SIZE);
        firstInode = rootb->getInode(bd,FIRST_INODE_BLOCK);
        
        //test first inode in rootblock
        REQUIRE(EMPTY_INODE_VALUE ==firstInode->firstDataBlock);
        REQUIRE(strcmp(firstInode->fileName,fileName) == 0);
        free(firstInode);

        // ─────────────────────────────────────────────────────────────────
        // Test last Inode

        //get last inode
        InodeStruct* lastInode = (InodeStruct *)malloc(BLOCK_SIZE);
        /**
         * getInode takes relative Values so NUMBER_OF_INODE_BLOCKS-1
         * because Inodes start at 0
         */
        lastInode = rootb->getInode(bd,NUMBER_OF_INODE_BLOCKS-1);
        // test last inode in rootblock
        REQUIRE(EMPTY_INODE_VALUE ==lastInode->firstDataBlock);
        REQUIRE(strcmp(lastInode->fileName,fileName) == 0);
        free(lastInode);

        // ─────────────────────────────────────────────────────────────────
        // Update Inode 33
        char* testName = new char[NAME_LENGTH];
        strcpy(testName,"newName");
        rootb->updateInode(bd,32,testName,1,1,1,1,1,1,1,1,1);

        // ─────────────────────────────────────────────────────────────────
        // test checkFilename exists
        uint32_t checkedIndex = rootb->checkFilenameOccupied(bd,testName);
        REQUIRE(checkedIndex == 32);
        //check filename that does not exists 
        char* nameNotFound = new char[NAME_LENGTH];
        strcpy(nameNotFound,"doesntExists");
        uint32_t checkedIndex2 = rootb->checkFilenameOccupied(bd,nameNotFound);
        REQUIRE(checkedIndex2 == -1);

        // ─────────────────────────────────────────────────────────────────
        // test getFileName get
        char* inodeName;
        inodeName = rootb->getFileName(bd,32);
        REQUIRE(strcmp(inodeName,testName) == 0);
        char* inodeName2;
        inodeName2 = rootb->getFileName(bd,1);
        REQUIRE(strcmp(inodeName2,fileName) == 0);
        
        // ─────────────────────────────────────────────────────────────────
        //test getInodeByFileName
        InodeStruct* inode = (InodeStruct *)malloc(BLOCK_SIZE);
        inode = rootb->getInodeByName(bd,inodeName);
        REQUIRE(strcmp(inodeName,inode->fileName) == 0);
        free(inode);
        //null
        InodeStruct* inode2 = (InodeStruct *)malloc(BLOCK_SIZE);
        inode2 = rootb->getInodeByName(bd,nameNotFound);
        REQUIRE(inode2 == NULL);
        free(inode2);

        delete[] inodeName;
        delete[] inodeName2;
        delete[] nameNotFound;

        delete[] testName;
        delete[] fileName;
        delete rootb;


    }
    
    bd->close();
    remove(BD_PATH);
    
}
