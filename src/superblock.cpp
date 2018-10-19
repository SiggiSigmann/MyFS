#include "superblock.h"
#include "myfs.h"

/*
Initialize the Superblock.
*/
Superblock::Superblock(){
    superblockStruct = new SuperblockStruct();
    superblockStruct->uNumber_of_free_inodes = NUMBER_OF_INODES;
    superblockStruct->uFirst_free_inode_index = 0;
    superblockStruct->uNumber_of_free_blocks = NUMBER_OF_USABLE_DATABLOCKS;
    superblockStruct->uFirst_free_block_index = 0;
}

/*
Clear the memory allocated by the Superblock.
*/
Superblock::~Superblock(){
    delete superblockStruct;
}

/*
Writes the Superblock to the BlockDevice.
*/
void Superblock::writeSuperblock(BlockDevice bd){
    char* buffer = new char[BD_BLOCK_SIZE];
    for(int i = 0; i<BLOCK_SIZE;i++){
        buffer[i] = 0;
    }
    //Serialization of the Superblock
    uint32_t* ptrBuffer = (uint32_t*) buffer;

    //general specs
    *ptrBuffer++ = superblockStruct->uSUPERBLOCK_BLOCK_INDEX;
    *ptrBuffer++ = superblockStruct->uNUMBER_OF_USABLE_DATABLOCKS;
    *ptrBuffer++ = superblockStruct->uNUMBER_OF_INODES;
    printf("Superblock %x - %x\n",superblockStruct->uSUPERBLOCK_BLOCK_INDEX*512,superblockStruct->uSUPERBLOCK_BLOCK_INDEX*512);

    //I-Map
    *ptrBuffer++ = superblockStruct->uNUMBER_OF_I_MAP_BLOCKS;
    *ptrBuffer++ = superblockStruct->uI_MAP_FIRST_BLOCK;
    *ptrBuffer++ = superblockStruct->uI_MAP_LAST_BLOCK;
    printf("Imap %x - %x\n",superblockStruct->uI_MAP_FIRST_BLOCK*512,superblockStruct->uI_MAP_LAST_BLOCK*512);


    //Inodes
    *ptrBuffer++ = superblockStruct->uNUMBER_OF_INODE_BLOCKS;
    *ptrBuffer++ = superblockStruct->uFIRST_INODE_BLOCK;
    *ptrBuffer++ = superblockStruct->uLAST_INODE_BLOCK;
    printf("Inodes %x - %x\n",superblockStruct->uFIRST_INODE_BLOCK*512,superblockStruct->uLAST_INODE_BLOCK*512);

    // //D-Map
    *ptrBuffer++ = superblockStruct->uNUMBER_OF_D_MAP_BLOCKS;
    *ptrBuffer++ = superblockStruct->uD_MAP_FIRST_BLOCK;
    *ptrBuffer++ = superblockStruct->uD_MAP_LAST_BLOCK;
    printf("D-Map %x - %x\n",superblockStruct->uD_MAP_FIRST_BLOCK*512,superblockStruct->uD_MAP_LAST_BLOCK*512);

    // //FAT
    *ptrBuffer++ = superblockStruct->uFAT_SIZE_IN_BYTES;
    *ptrBuffer++ = superblockStruct->uNUMBER_OF_FAT_BLOCKS;
    *ptrBuffer++ = superblockStruct->uFAT_FIRST_BLOCK;
    *ptrBuffer++ = superblockStruct->uFAT_LAST_BLOCK;
    printf("FAT %x - %x\n",superblockStruct->uFAT_FIRST_BLOCK*512,superblockStruct->uFAT_LAST_BLOCK*512);

    // //Data Blocks
    *ptrBuffer++ = superblockStruct->uFIRST_DATA_BLOCK;
    *ptrBuffer++ = superblockStruct->uLAST_DATA_BLOCK;
    printf("Data Blocks %x - %x\n",superblockStruct->uFIRST_DATA_BLOCK*512,superblockStruct->uLAST_DATA_BLOCK*512);

    //runtime calculated values
    *ptrBuffer++ = superblockStruct->uNumber_of_free_inodes;
    *ptrBuffer++ = superblockStruct->uFirst_free_inode_index;
    *ptrBuffer++ = superblockStruct->uNumber_of_free_blocks;
    *ptrBuffer++ = superblockStruct->uFirst_free_block_index;

    //Write the Superblock
    bd.write(superblockStruct->uSUPERBLOCK_BLOCK_INDEX, buffer);    
    //writeBytes(bd, superblockStruct->uSUPERBLOCK_BLOCK_INDEX, buffer, BD_BLOCK_SIZE);
    delete [] buffer;
}
/*
Reads the Superblock from the block device into the Superblock data structure.
*/
void Superblock::readSuperblock(BlockDevice bd){
    char* buffer = readBytes(bd, superblockStruct->uSUPERBLOCK_BLOCK_INDEX, BD_BLOCK_SIZE);
    uint32_t* ptrBuffer = (uint32_t*) buffer;
    
    //general specs
    superblockStruct->uSUPERBLOCK_BLOCK_INDEX = *ptrBuffer++;
    superblockStruct->uNUMBER_OF_USABLE_DATABLOCKS = *ptrBuffer++;
    superblockStruct->uNUMBER_OF_INODES = *ptrBuffer++;

    //I-Map
    superblockStruct->uNUMBER_OF_I_MAP_BLOCKS = *ptrBuffer++;
    superblockStruct->uI_MAP_FIRST_BLOCK = *ptrBuffer++;
    superblockStruct->uI_MAP_LAST_BLOCK = *ptrBuffer++;

    //Inodes
    superblockStruct->uNUMBER_OF_INODE_BLOCKS = *ptrBuffer++;
    superblockStruct->uFIRST_INODE_BLOCK = *ptrBuffer++;
    superblockStruct->uLAST_INODE_BLOCK = *ptrBuffer++;

    //D-Map
    superblockStruct->uNUMBER_OF_D_MAP_BLOCKS = *ptrBuffer++;
    superblockStruct->uD_MAP_FIRST_BLOCK = *ptrBuffer++;
    superblockStruct->uD_MAP_LAST_BLOCK = *ptrBuffer++;

    //FAT
    superblockStruct->uFAT_SIZE_IN_BYTES = *ptrBuffer++;
    superblockStruct->uNUMBER_OF_FAT_BLOCKS = *ptrBuffer++;
    superblockStruct->uFAT_FIRST_BLOCK = *ptrBuffer++;
    superblockStruct->uFAT_LAST_BLOCK = *ptrBuffer++;

    //Data Blocks
    superblockStruct->uFIRST_DATA_BLOCK = *ptrBuffer++;
    superblockStruct->uLAST_DATA_BLOCK = *ptrBuffer++;

    //runtime calculated values
    superblockStruct->uNumber_of_free_inodes = *ptrBuffer++;
    superblockStruct->uFirst_free_inode_index = *ptrBuffer++;
    superblockStruct->uNumber_of_free_blocks = *ptrBuffer++;
    superblockStruct->uFirst_free_block_index = *ptrBuffer++;
    //
    delete [] buffer;    
}

void Superblock::updateNumberOfFreeInodes(uint32_t newNumber){
    superblockStruct->uNumber_of_free_inodes = newNumber;
}

uint32_t Superblock::getNumberOfFreeInodes(){
    return superblockStruct->uNumber_of_free_inodes;
}

void Superblock::updateFirstFreeInodeIndex(uint32_t newIndex){
    superblockStruct->uFirst_free_inode_index = newIndex;
}

uint32_t Superblock::getNumberOfFreeBlocks(){
    return  superblockStruct->uNumber_of_free_blocks;
}

void Superblock::updateNumberOfFreeBlocks(uint32_t newNumber){
    superblockStruct->uNumber_of_free_blocks = newNumber;
}

uint32_t Superblock::getFirstFreeBlockIndex(){
    return   superblockStruct->uFirst_free_block_index;
}

void Superblock::updateFirstFreeBlockIndex(uint32_t newIndex){
    superblockStruct->uFirst_free_block_index = newIndex;
}

uint32_t Superblock::getFirstFreeInodeIndex(){
    return superblockStruct->uFirst_free_inode_index;
}
