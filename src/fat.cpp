#include "fat.h"

FatHandler::FatHandler(){
    fat = new FatStruct();
}

/*
Returns the successor of the given block index.
*/
uint32_t FatHandler::get(uint32_t blockIndex){
    return 0;
}

/*
Sets the successor of the given block index.
*/
void FatHandler::set(uint32_t blockIndex){

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