#ifndef superblock_h
#define superblock_h

#include <cstdint>
#include "constants.h"
#include "blockdevice.h"

/*
The class Superblock provides managing functions for the filesystems superblock.
*/
class Superblock{
    public:
        SuperblockStruct *superblockStruct;
        Superblock();
        ~Superblock();
        void updateNumberOfFreeInodes(uint32_t newNumber);
        void updateFirstFreeInodeIndex(uint32_t newIndex);
        void updateNumberOfFreeBlocks(uint32_t newNumber);
        void updateFirstFreeBlockIndex(uint32_t newIndex);
        void writeSuperblock(BlockDevice bd);
        void readSuperblock(BlockDevice bd);
};

#endif