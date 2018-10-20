#include "dmap.h"
#include "myfs.h"

/*
Initialize the DMap.
*/
DMap::DMap(){
    dMapStruct = new DMapStruct();
    dMapStruct->dataBlockMap = new bool[NUMBER_OF_USABLE_DATABLOCKS];
    for(int i =0; i<NUMBER_OF_USABLE_DATABLOCKS; i++){
        dMapStruct->dataBlockMap[i]=0;
    }
}

/*
Clear the memory allocated by the DMap.
*/
DMap::~DMap(){
    delete[] dMapStruct->dataBlockMap;
    delete dMapStruct;
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
void DMap::writeDMap(BlockDevice* bd){
    writeBytes(bd, D_MAP_FIRST_BLOCK, (char *) dMapStruct->dataBlockMap, BLOCK_SIZE * NUMBER_OF_D_MAP_BLOCKS);

}

/*
Reads the DMap from the block device into the dmap data structure.
*/
void DMap::readDMap(BlockDevice* bd){
    dMapStruct->dataBlockMap = (bool *) readBytes(bd, D_MAP_FIRST_BLOCK, BLOCK_SIZE * NUMBER_OF_D_MAP_BLOCKS);
}

uint32_t DMap::getNextFreeDatablock(uint32_t lastindex){
    for(uint32_t i = lastindex; i<NUMBER_OF_USABLE_DATABLOCKS;i++){
        if(dMapStruct->dataBlockMap[i] == 0){
            return i;
        }
    }
    for(uint32_t i = 0; i<lastindex;i++){
        if(dMapStruct->dataBlockMap[i] == 0){
            return i;
        }
    }
    return -1;
}
