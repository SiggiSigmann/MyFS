#ifndef superblock_h
#define superblock_h

#include <cstdint>
#include "constants.h"
#include "blockdevice.h"

/*
The class Superblock provides managing functions for the filesystems superblock.
*/

struct SuperblockStruct {
    /*
    Superblock | I-Map | Inodes | D-Map | FAT | Data Blocks
    */

    //general specs
    uint32_t uSUPERBLOCK_BLOCK_INDEX = SUPERBLOCK_BLOCK_INDEX;
    uint32_t uNUMBER_OF_USABLE_DATABLOCKS = NUMBER_OF_USABLE_DATABLOCKS;
    uint32_t uNUMBER_OF_INODES = NUMBER_OF_INODES;

    //I-Map
    uint32_t uNUMBER_OF_I_MAP_BLOCKS = NUMBER_OF_I_MAP_BLOCKS;
    uint32_t uI_MAP_FIRST_BLOCK = I_MAP_FIRST_BLOCK;
    uint32_t uI_MAP_LAST_BLOCK = I_MAP_LAST_BLOCK;

    //Inodes
    uint32_t uNUMBER_OF_INODE_BLOCKS = NUMBER_OF_INODE_BLOCKS;
    uint32_t uFIRST_INODE_BLOCK = FIRST_INODE_BLOCK;
    uint32_t uLAST_INODE_BLOCK = LAST_INODE_BLOCK;

    //D-Map
    uint32_t uNUMBER_OF_D_MAP_BLOCKS = NUMBER_OF_D_MAP_BLOCKS; //can map (4byte int32_t)BlockNumber -> (1byte bool)used, 512-times per D-Map Block (512Bytes)
    uint32_t uD_MAP_FIRST_BLOCK = D_MAP_FIRST_BLOCK;
    uint32_t uD_MAP_LAST_BLOCK = D_MAP_LAST_BLOCK;

    //FAT
    uint32_t uFAT_SIZE_IN_BYTES = FAT_SIZE_IN_BYTES; //mapping requires sizeof(uint32_t)=4bytes per block: (uint32_t)blockIndex -> (uint32_t)nextBlockIndex
    uint32_t uNUMBER_OF_FAT_BLOCKS = NUMBER_OF_FAT_BLOCKS;
    uint32_t uFAT_FIRST_BLOCK = FAT_FIRST_BLOCK;
    uint32_t uFAT_LAST_BLOCK = FAT_LAST_BLOCK;

    //Data Blocks
    uint32_t uFIRST_DATA_BLOCK = FIRST_DATA_BLOCK;
    uint32_t uLAST_DATA_BLOCK = LAST_DATA_BLOCK;

    //runtime calculated values
    uint32_t uNumber_of_free_inodes;
    uint32_t uFirst_free_inode_index;
    uint32_t uNumber_of_free_blocks;
    uint32_t uFirst_free_block_index;
};


class Superblock{
    public:
        SuperblockStruct *superblockStruct;
        Superblock();
        ~Superblock();
        /*
        void updateNumberOfFreeInodes(uint32_t newNumber);
        void updateFirstFreeInodeIndex(uint32_t newIndex);
        void updateNumberOfFreeBlocks(uint32_t newNumber);
        void updateFirstFreeBlockIndex(uint32_t newIndex);
        */
        void writeSuperblock(BlockDevice bd);
        void readSuperblock(BlockDevice bd);
};

#endif