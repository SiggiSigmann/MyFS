#include "dmap.h"

DMap::DMap(){
    dMapStruct->dataBlockMap = new bool[NUMBER_USABLE_DATABLOCKS];
}

DMap::~DMap(){
    delete[] dMapStruct->dataBlockMap;
}

void DMap::occupyDatablock(uint32_t dataBlockIndex){
    dMapStruct->dataBlockMap[dataBlockIndex] = true;
}

void DMap::freeDatablock(uint32_t dataBlockIndex){
    dMapStruct->dataBlockMap[dataBlockIndex] = false;
}