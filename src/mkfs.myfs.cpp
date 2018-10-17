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

        std::cout <<"1" << std::endl;

        Superblock* superblock = new Superblock();
        superblock->writeSuperblock(*bd);

        std::cout <<"2" << std::endl;

        DMap* dmap = new DMap();
        dmap->writeDMap(*bd);

        std::cout <<"3" << std::endl;

        FatHandler* fat = new FatHandler();
        fat->writeFat(*bd);

        std::cout <<"4" << std::endl;

        IMapHandler* imap = new IMapHandler();
        imap->init();
        imap->write(*bd);

        std::cout <<"5" << std::endl;

        RootBlock* rootblock = new RootBlock();
        rootblock->init(bd);
    





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
