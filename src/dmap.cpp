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
