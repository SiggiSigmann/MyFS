/*
Writes the Superblock to the BlockDevice.
*/
void MyFS::writeSuperblock(BlockDevice bd) {
    char* buffer = new char[BD_BLOCK_SIZE];
    //Serialization of the Superblock
    uint32_t* ptrBuffer = (uint32_t*) buffer;

    //general specs
    *ptrBuffer++ = MyFS::superblock.SUPERBLOCK_BLOCK_INDEX;
    *ptrBuffer++ = MyFS::superblock.NUMBER_OF_USABLE_DATABLOCKS;
    *ptrBuffer++ = MyFS::superblock.NUMBER_OF_INODES;

    //I-Map
    *ptrBuffer++ = MyFS::superblock.NUMBER_OF_I_MAP_BLOCKS;
    *ptrBuffer++ = MyFS::superblock.I_MAP_FIRST_BLOCK;
    *ptrBuffer++ = MyFS::superblock.I_MAP_LAST_BLOCK;

    //Inodes
    *ptrBuffer++ = MyFS::superblock.NUMBER_OF_INODE_BLOCKS;
    *ptrBuffer++ = MyFS::superblock.FIRST_INODE_BLOCK;
    *ptrBuffer++ = MyFS::superblock.LAST_INODE_BLOCK;

    //D-Map
    *ptrBuffer++ = MyFS::superblock.NUMBER_OF_D_MAP_BLOCKS;
    *ptrBuffer++ = MyFS::superblock.D_MAP_FIRST_BLOCK;
    *ptrBuffer++ = MyFS::superblock.D_MAP_LAST_BLOCK;

    //FAT
    *ptrBuffer++ = MyFS::superblock.FAT_SIZE_IN_BYTES;
    *ptrBuffer++ = MyFS::superblock.NUMBER_OF_FAT_BLOCKS;
    *ptrBuffer++ = MyFS::superblock.FAT_FIRST_BLOCK;
    *ptrBuffer++ = MyFS::superblock.FAT_LAST_BLOCK;

    //Data Blocks
    *ptrBuffer++ = MyFS::superblock.FIRST_DATA_BLOCK;
    *ptrBuffer++ = MyFS::superblock.LAST_DATA_BLOCK;

    //runtime calculated values
    *ptrBuffer++ = MyFS::superblock.number_of_free_inodes;
    *ptrBuffer++ = MyFS::superblock.first_free_inode;
    *ptrBuffer++ = MyFS::superblock.number_of_free_blocks;
    *ptrBuffer++ = MyFS::superblock.first_free_block;

    //Write the Superblock
    writeBytes(bd, MyFS::superblock.SUPERBLOCK_BLOCK_INDEX, buffer, BD_BLOCK_SIZE);
    delete [] buffer;
}

/*
Reads the Superblock from the BlockDevice into the Superblock struct.
*/
void MyFS::readSuperblock(BlockDevice bd) {
    char* buffer = readBytes(bd, MyFS::superblock.SUPERBLOCK_BLOCK_INDEX, BD_BLOCK_SIZE);
    uint32_t* ptrBuffer = (uint32_t*) buffer;
    
    //general specs
    MyFS::superblock.SUPERBLOCK_BLOCK_INDEX = *ptrBuffer++;
    MyFS::superblock.NUMBER_OF_USABLE_DATABLOCKS = *ptrBuffer++;
    MyFS::superblock.NUMBER_OF_INODES = *ptrBuffer++;

    //I-Map
    MyFS::superblock.NUMBER_OF_I_MAP_BLOCKS = *ptrBuffer++;
    MyFS::superblock.I_MAP_FIRST_BLOCK = *ptrBuffer++;
    MyFS::superblock.I_MAP_LAST_BLOCK = *ptrBuffer++;

    //Inodes
    MyFS::superblock.NUMBER_OF_INODE_BLOCKS = *ptrBuffer++;
    MyFS::superblock.FIRST_INODE_BLOCK = *ptrBuffer++;
    MyFS::superblock.LAST_INODE_BLOCK = *ptrBuffer++;

    //D-Map
    MyFS::superblock.NUMBER_OF_D_MAP_BLOCKS = *ptrBuffer++;
    MyFS::superblock.D_MAP_FIRST_BLOCK = *ptrBuffer++;
    MyFS::superblock.D_MAP_LAST_BLOCK = *ptrBuffer++;

    //FAT
    MyFS::superblock.FAT_SIZE_IN_BYTES = *ptrBuffer++;
    MyFS::superblock.NUMBER_OF_FAT_BLOCKS = *ptrBuffer++;
    MyFS::superblock.FAT_FIRST_BLOCK = *ptrBuffer++;
    MyFS::superblock.FAT_LAST_BLOCK = *ptrBuffer++;

    //Data Blocks
    MyFS::superblock.FIRST_DATA_BLOCK = *ptrBuffer++;
    MyFS::superblock.LAST_DATA_BLOCK = *ptrBuffer++;

    //runtime calculated values
    MyFS::superblock.number_of_free_inodes = *ptrBuffer++;
    MyFS::superblock.first_free_inode = *ptrBuffer++;
    MyFS::superblock.number_of_free_blocks = *ptrBuffer++;
    MyFS::superblock.first_free_block = *ptrBuffer++;

    delete [] buffer;
}
