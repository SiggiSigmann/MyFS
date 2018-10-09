/*
Manage the superblock of the FS
*/

#ifndef superblock
#define superblock

#include <cstdint>
#include "constants.h"
#include "blockdevice.h"

//define which metedata the fs conatains
struct SuperStruct {
    /*
    Superblock |D-Map | FAT | Root | Data Blocks
    */

    //Superblock
    uint32_t SUPERBLOCK_BLOCK_INDEX = 0;

    //important: NUMBER_OF_USABLE_DATABLOCKS needs to be a multiple of BD_BLOCK_SIZE, to become perfectly mappable
    uint32_t NUMBER_OF_USABLE_DATABLOCKS = 1 << 18; //2^18 = 262144 blocks => 128 MiB in total
    uint32_t NUMBER_OF_INODES = NUM_DIR_ENTRIES; //needs to be a multiple of BD_BLOCK_SIZE, to become perfectly mappable

    //D-Map
    uint32_t NUMBER_OF_D_MAP_BLOCKS = NUMBER_OF_USABLE_DATABLOCKS/BD_BLOCK_SIZE; //can map (4byte int32_t)BlockNumber -> (1byte bool)used, 512-times per D-Map Block (512Bytes)
    uint32_t D_MAP_FIRST_BLOCK = SUPERBLOCK_BLOCK_INDEX + 1;
    uint32_t D_MAP_LAST_BLOCK = D_MAP_FIRST_BLOCK + NUMBER_OF_D_MAP_BLOCKS;

    //FAT
    uint32_t FAT_SIZE_IN_BYTES = NUMBER_OF_USABLE_DATABLOCKS * sizeof(uint32_t); //mapping requires sizeof(uint32_t)=4bytes per block: (uint32_t)blockIndex -> (uint32_t)nextBlockIndex
    uint32_t NUMBER_OF_FAT_BLOCKS = FAT_SIZE_IN_BYTES/BD_BLOCK_SIZE;
    uint32_t FAT_FIRST_BLOCK = D_MAP_LAST_BLOCK + 1;
    uint32_t FAT_LAST_BLOCK = FAT_FIRST_BLOCK + NUMBER_OF_FAT_BLOCKS;

        //Inodes
    uint32_t NUMBER_OF_INODE_BLOCKS = NUMBER_OF_INODES; //one block for each inode
    uint32_t FIRST_INODE_BLOCK = FAT_LAST_BLOCK + 1;
    uint32_t LAST_INODE_BLOCK = LAST_INODE_BLOCK + NUMBER_OF_INODE_BLOCKS;

    //Data Blocks
    uint32_t FIRST_DATA_BLOCK = LAST_INODE_BLOCK + 1;
    uint32_t LAST_DATA_BLOCK = FIRST_DATA_BLOCK + NUMBER_OF_USABLE_DATABLOCKS;

    //runtime calculated values
    uint32_t number_of_free_blocks;
    uint32_t first_free_block;
};

/*
The class SuperBlock manage the superblock of a filesystem
*/
class SuperBlockHelper{
    private:
        SuperStruct *superBlock;
    public:
        SuperBlockHelper();

};


#endif