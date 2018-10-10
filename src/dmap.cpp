#include "dmap.h"

/*
Initialize the DMap.
*/
DMap::DMap(){
    dMapStruct->dataBlockMap = new bool[NUMBER_USABLE_DATABLOCKS];
}

/*
Clear the memory allocated by the DMap.
*/
DMap::~DMap(){
    delete[] dMapStruct->dataBlockMap;
}

/*
Marks the specified datablock as occupied.
*/
void DMap::occupyDatablock(uint32_t dataBlockIndex){
    dMapStruct->dataBlockMap[dataBlockIndex] = true;
}

/*
Marks the specified datablock as free.
*/
void DMap::freeDatablock(uint32_t dataBlockIndex){
    dMapStruct->dataBlockMap[dataBlockIndex] = false;
}

/*
Return a boolean indicating whether the specified data block is occupied or free.
*/
bool DMap::get(uint32_t dataBlockIndex){
    return dMapStruct->dataBlockMap[dataBlockIndex];
}

/*
Writes the DMap to the block device.
*/
void DMap::writeDMap(BlockDevice bd){

}

/*
Reads the DMap from the block device into the dmap data structure.
*/
void DMap::readDMap(BlockDevice bd){

}