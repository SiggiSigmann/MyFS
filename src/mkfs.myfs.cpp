//
//  mk.myfs.cpp
//  myfs
//
//  Created by Oliveriltus Waldhorst on 07.09.17.


#include "myfs.h"
#include "blockdevice.h"
#include "macros.h"
#include <iostream>
#include "constants.h"
#include "dmap.h"
#include "fat.h"
#include "imap.h"
#include "rootblock.h"
#include "superblock.h"

void initFS(char* fileName){                            //create and write emty FS to file 
    BlockDevice* bd = new BlockDevice(BLOCK_SIZE);      //create BlockDevice to write the data into a file
    bd->create(fileName);

    Superblock* superblock = new Superblock();          //create default Superblock and write to file
    superblock->writeSuperblock(*bd);

    DMap* dmap = new DMap();                            //create default DMap and write to file
    dmap->writeDMap(*bd);

    FatHandler* fat = new FatHandler();                 //create default FatHandler and write to file
    fat->writeFat(*bd);

    IMapHandler* imap = new IMapHandler();              //create default IMapHandler and write to file
    imap->init();
    imap->write(bd);

    RootBlock* rootblock = new RootBlock();             //create default RootBlock and write to file
    rootblock->init(bd);

    char* emptyblock = (char*)malloc(BLOCK_SIZE);       //create empty blockDevice chararray

    for(uint32_t i = 0; i < BLOCK_SIZE; i++){
        emptyblock[i] = 0;
    }

    printf("%x\n", NUMBER_OF_USABLE_DATABLOCKS);

    for(uint32_t i = 0; i < NUMBER_OF_USABLE_DATABLOCKS; i++){
        bd->write(FIRST_DATA_BLOCK+i,emptyblock);       //write empty datablocks to file
    }

    free(emptyblock);

    delete bd;
    delete superblock;
    delete dmap;
    delete fat;
    delete imap;
    delete rootblock;

}

int main(int argc, char *argv[]) {
    //std::cout << argc << std::endl;
    //for (int i = 0; i < argc; ++i) {
    //    std::cout << argv[i] << std::endl;
    //}

    if(argc > 1){                                           //check if enough arguments ar given
       initFS(argv[1]);
       return 0;
    }
    
    return -1;
}
