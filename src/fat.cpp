#include "fat.h"
#include "myfs.h"

/*
Initializes the FAT.
*/
FatHandler::FatHandler(){
    fat = new Fat();
    fat->fatMap = new uint32_t[FAT_SIZE_IN_BYTES];
    memset4(fat->fatMap, EMPTY_FAT_ENTRY, FAT_SIZE_IN_BYTES);
}

/*
Clears up the FAT's allocated memory.
*/
FatHandler::~FatHandler(){
    delete [] fat->fatMap;
    delete fat;
}

/*
Returns the successor of the given block index.
*/
uint32_t FatHandler::get(uint32_t blockIndex){
    return fat->fatMap[blockIndex];
}

/*
Sets the successor of the given block index.
*/
void FatHandler::set(uint32_t blockIndex, uint32_t value){
    fat->fatMap[blockIndex] = value;
}

/*
Writes the FAT to the block device.
*/
void FatHandler::writeFat(BlockDevice bd){
    writeBytes(bd, FAT_FIRST_BLOCK, (char* )fat->fatMap, FAT_SIZE_IN_BYTES);
}

/*
Reads the FAT from the block device into the FAT data structure.
*/
void FatHandler::readFat(BlockDevice bd){
    fat->fatMap = (uint32_t* ) readBytes(bd, FAT_FIRST_BLOCK, FAT_SIZE_IN_BYTES);
}

/*
Deletes all FAT entries associated with the given block index until an entry tagged as END_OF_FILE_ENTRY was cleared. 
*/
void FatHandler::deleteAll(uint32_t firstBlockIndex){
    uint32_t currentIndex = firstBlockIndex;
    //clear all entries except the last one
    while(fat->fatMap[currentIndex] != END_OF_FILE_ENTRY){
        //get the index value for the next iteration
        uint32_t nextIndex = fat->fatMap[currentIndex];
        //clear the current index
        fat->fatMap[currentIndex] = EMPTY_FAT_ENTRY;
        //update the currentIndex for the next iteration
        currentIndex = nextIndex;
    }
    //clear the last entry
    fat->fatMap[currentIndex] = EMPTY_FAT_ENTRY;
}