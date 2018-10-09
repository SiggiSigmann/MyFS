#include "dmap.h"

DMap::DMap(){
    dMapStruct->dataBlockMap = new bool[NUMBER_USABLE_DATABLOCKS];
}

DMap::~DMap(){
    delete[] dMapStruct->dataBlockMap;
}