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



int main(int argc, char *argv[]) {
    //std::cout << argc << std::endl;
    //for (int i = 0; i < argc; ++i) {
    //    std::cout << argv[i] << std::endl;
    //}

    if(argc > 1){
        BlockDevice* bd = new BlockDevice(BLOCK_SIZE);
        bd->create(argv[1]);

        Superblock* superblock = new Superblock();
        superblock->writeSuperblock(*bd);

        DMap* dmap = new DMap();
        dmap->writeDMap(*bd);

        FatHandler* fat = new FatHandler();
        fat->writeFat(*bd);

        IMapHandler* imap = new IMapHandler();
        imap->init();
        imap->write(*bd);

        RootBlock* rootblock = new RootBlock();
        rootblock->init(bd);

        char* emptyblock = (char*)malloc(BLOCK_SIZE);

        for(uint32_t i = 0; i < BLOCK_SIZE; i++){
            emptyblock[i] = 0;
        }
        for(uint32_t i = 0; i < NUMBER_OF_USABLE_DATABLOCKS; i++){
            bd->write(FIRST_DATA_BLOCK+i,emptyblock);
        }

        free(emptyblock);

        delete bd;
        delete superblock;
        delete dmap;
        delete fat;
        delete imap;
        delete rootblock;

        return 0;
    }
    
    return -1;
}
