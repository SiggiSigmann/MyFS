#include "fat.h"

/*
Initializes the FAT.
*/
FatHandler::FatHandler(){
    fat = new Fat();
    fat->fatMap = new uint32_t[FAT_SIZE_IN_BYTES];
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

}

/*
Reads the FAT from the block device into the FAT data structure.
*/
void FatHandler::readFat(BlockDevice bd){

}