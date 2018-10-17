#ifndef constants_h
#define constants_h
#include <cstdint>

//Requirements:
#define NAME_LENGTH (255) 
#define BLOCK_SIZE (512)
#define NUM_DIR_ENTRIES (64) //maximum amount of files
#define NUM_OPEN_FILES (64) //maximum amount of open files
#define BD_BLOCK_SIZE (512)

//TODO add your own constants here

//general specs
#define SUPERBLOCK_BLOCK_INDEX (1)
#define NUMBER_OF_USABLE_DATABLOCKS (1 << 18)//2^18 = 262144 blocks => 128 MiB in total
#define NUMBER_OF_INODES (NUM_DIR_ENTRIES)

//I-Map
#if ((NUMBER_OF_INODES%BD_BLOCK_SIZE) != 0)
#define NUMBER_OF_I_MAP_BLOCKS ((NUMBER_OF_INODES/BD_BLOCK_SIZE)+1) //one more block to store non-perfect-mappable inodes (inode numbers that differ multiples of BD_BLOCK_SIZE)
#else
#define NUMBER_OF_I_MAP_BLOCKS (NUMBER_OF_INODES/BD_BLOCK_SIZE) //can map (4byte int32_t)InodeNumber -> (1byte bool)used, 512-times per I-Map Block (512Bytes)
#endif
#define I_MAP_FIRST_BLOCK (SUPERBLOCK_BLOCK_INDEX + 1)
#define I_MAP_LAST_BLOCK (I_MAP_FIRST_BLOCK + NUMBER_OF_I_MAP_BLOCKS - 1)

//Inodes
#define NUMBER_OF_INODE_BLOCKS (NUMBER_OF_INODES) //one block for each inode
#define FIRST_INODE_BLOCK (I_MAP_LAST_BLOCK + 1)
#define LAST_INODE_BLOCK (FIRST_INODE_BLOCK + NUMBER_OF_INODE_BLOCKS - 1)
#define EMPTY_INODE_VALUE -1
#define EMPTY_INODE_STRING "empty"

//D-Map
#if ((NUMBER_OF_USABLE_DATABLOCKS%BD_BLOCK_SIZE) != 0)
#define NUMBER_OF_D_MAP_BLOCKS ((NUMBER_OF_USABLE_DATABLOCKS/BD_BLOCK_SIZE)+1) //one more block to store non-perfect-mappable values of NUMBER_OF_USABLE_DATABLOCKS
#else
#define NUMBER_OF_D_MAP_BLOCKS (NUMBER_OF_USABLE_DATABLOCKS/BD_BLOCK_SIZE)
#endif
#define D_MAP_FIRST_BLOCK (LAST_INODE_BLOCK + 1)
#define D_MAP_LAST_BLOCK (D_MAP_FIRST_BLOCK + NUMBER_OF_D_MAP_BLOCKS - 1)

//FAT
#define FAT_SIZE_IN_BYTES (NUMBER_OF_USABLE_DATABLOCKS * 4)
#if ((FAT_SIZE_IN_BYTES%BD_BLOCK_SIZE) != 0)
#define NUMBER_OF_FAT_BLOCKS ((FAT_SIZE_IN_BYTES/BD_BLOCK_SIZE)+1) //one more block to store non-perfect-mappable values of NUMBER_OF_USABLE_DATABLOCKS
#else
#define NUMBER_OF_FAT_BLOCKS (FAT_SIZE_IN_BYTES/BD_BLOCK_SIZE)
#endif
#define FAT_FIRST_BLOCK (D_MAP_LAST_BLOCK + 1)
#define FAT_LAST_BLOCK (FAT_FIRST_BLOCK + NUMBER_OF_FAT_BLOCKS - 1)

//Data Blocks
#define FIRST_DATA_BLOCK (FAT_LAST_BLOCK + 1)
#define LAST_DATA_BLOCK (FIRST_DATA_BLOCK + NUMBER_OF_USABLE_DATABLOCKS - 1)

//indicates the last block of a file in the FAT
#define END_OF_FILE_ENTRY (NUMBER_OF_USABLE_DATABLOCKS + 1)
#define EMPTY_FAT_ENTRY (END_OF_FILE_ENTRY + 1)

#endif
