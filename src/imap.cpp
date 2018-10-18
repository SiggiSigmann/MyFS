#include "imap.h"

/*
Allocates Memory for the IMap.
*/
IMapHandler::IMapHandler(){
    char* data = (char*) malloc(BLOCK_SIZE);
    iMapStruct = (IMapStruct*) data;
    for(int i = 0; i<BLOCK_SIZE;i++){
        data[i] = 0;
    }
}

/*
Clear the memory allocated by the IMap.
*/
IMapHandler::~IMapHandler(){
    free(iMapStruct);
}

/*
Initializes the imap with default values (0 => unsued)
*/
void IMapHandler::init(){
    *iMapStruct = IMapStruct();
    for(int i=0; i<NUM_DIR_ENTRIES;i++){
        iMapStruct->imap[i]=0;
    }
}

/*
Writes the IMap to the block device.
*/
void IMapHandler::write(BlockDevice* bd){
    bd->write(I_MAP_FIRST_BLOCK, (char *) iMapStruct);

}

/*
Reads the IMap from the block device into the dmap data structure.
*/
void IMapHandler::read(BlockDevice* bd){
    bd->read(I_MAP_FIRST_BLOCK,(char*) iMapStruct);
}

/*
Set the imap entry at the position index to occupied (true)
*/
void IMapHandler::occupyIMapEntry(uint32_t index){
    iMapStruct->imap[index] = true;
}

/*
Set the imap entry at the position index to free (false)
*/
void IMapHandler::freeIMapEntry(uint32_t index){
    iMapStruct->imap[index] = false;
}

/*
Get the value of the imap at the position index
*/
bool IMapHandler::getIMapEntry(uint32_t index){
    return iMapStruct->imap[index];
}