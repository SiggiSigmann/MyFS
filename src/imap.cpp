#include "imap.h"

/*
Initialize the IMap.
*/
IMapHandler::IMapHandler(){
    iMapStruct = (IMapStruct*) malloc(BLOCK_SIZE);
}

/*
Clear the memory allocated by the IMap.
*/
IMapHandler::~IMapHandler(){
    free(iMapStruct);
}

/*
Fill the imap with default values (0 => unsued)
*/
void IMapHandler::create(){
    *iMapStruct = IMapStruct();
    for(int i=0; i<NUM_DIR_ENTRIES;i++){
        iMapStruct->imap[i]=0;
    }
}

/*
Writes the IMap to the block device.
*/
void IMapHandler::write(BlockDevice bd){
    bd.write(I_MAP_FIRST_BLOCK, (char *) iMapStruct);

}

/*
Reads the IMap from the block device into the dmap data structure.
*/
void IMapHandler::read(BlockDevice bd){
    bd.read(I_MAP_FIRST_BLOCK,(char*) iMapStruct);
}

/*
Set the imap entry at the place index to value
*/
void IMapHandler::setIMapEntry(uint32_t index, bool value){
    iMapStruct->imap[index] = value;
}

/*
Get the value of the imap at the place index
*/
bool IMapHandler::getIMapEntry(uint32_t index){
    return iMapStruct->imap[index];
}