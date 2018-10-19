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
#include <libgen.h>


#include <sys/stat.h> //contains stat

void initFS(char* fileName){}

int main(int argc, char *argv[]) {
    //std::cout << argc << std::endl;
    //for (int i = 0; i < argc; ++i) {
    //    std::cout << argv[i] << std::endl;
    //}

    if(argc==2){
        printf("create empty FS");

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
        imap->write(bd);

        RootBlock* rootblock = new RootBlock();
        rootblock->init(bd);

        char* emptyblock = (char*)malloc(BLOCK_SIZE);

        for(uint32_t i = 0; i < BLOCK_SIZE; i++){
            emptyblock[i] = 0;
        }

        printf("%x\n", NUMBER_OF_USABLE_DATABLOCKS);

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
    }else if(argc>2){
        printf("create Fs and fill with %d files\n", argc -2);
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
        imap->write(bd);

        RootBlock* rootblock = new RootBlock();
        rootblock->init(bd);

        char* emptyblock = (char*)malloc(BLOCK_SIZE);

        for(uint32_t i = 0; i < BLOCK_SIZE; i++){
            emptyblock[i] = 0;
        }

        //fill empty FS with datablocks
        for(uint32_t i = 0; i < NUMBER_OF_USABLE_DATABLOCKS; i++){
            bd->write(FIRST_DATA_BLOCK+i,emptyblock);
        }

        for(int i = 2;i<argc;i++){                                      //write each file to FS
            printf("Nr%i. filename:'%s' basename:'%s'\n",i,argv[i],basename(argv[i]));

            struct stat sb;                             //store metadate of given files
            if (stat(argv[i], &sb) == -1) {
                 return -(EIO);
            }

            if(superblock->getNumberOfFreeInodes()<1){    //check if a inode is free
                printf("not possibel to add more files then 62\n");
                return -(EIO);
            }

            if(superblock->getNumberOfFreeBlocks() <= sb.st_blocks){ //check if enough space (datablocks) ar free
                printf("not enough space in FS\n");
                return -(EIO);
            }

            BlockDevice* inputfile = new BlockDevice();             //open inputfile to read blocks
            inputfile->open(argv[i]);

          

            //write blocks, occupy blocks fill fat
            uint32_t indexFreeDB = superblock->getFirstFreeBlockIndex();
            uint32_t firstDataBlock;
            char* filecontent = (char*) malloc(BLOCK_SIZE);                         //buffer to store one block of a file
            for(int k = 0;k<sb.st_blocks;k++){
                
                indexFreeDB = superblock->getFirstFreeBlockIndex();

                //copy content of file to FS
                inputfile->read(k,filecontent);

                printf("%i write index:%x adress:%x\n",k, indexFreeDB,indexFreeDB+FIRST_DATA_BLOCK*0x200);
                bd->write(FIRST_DATA_BLOCK+indexFreeDB, filecontent);

                dmap->occupyDatablock(indexFreeDB);

                if(k==0){
                    printf("fat start\n");
                    firstDataBlock = indexFreeDB;
                    fat->set(indexFreeDB, superblock->getFirstFreeBlockIndex());
                }else if(k==sb.st_blocks-1){
                    printf("fat end\n");
                    fat->set(indexFreeDB, END_OF_FILE_ENTRY);
                }else{
                    fat->set(indexFreeDB, superblock->getFirstFreeBlockIndex());
                }

                superblock->updateFirstFreeBlockIndex(dmap->getNextFreeDatablock(indexFreeDB));


            }

            inputfile->close();
            delete inputfile;

            free(filecontent);

            uint32_t inodeIndex = superblock->getFirstFreeInodeIndex();
            imap->occupyIMapEntry(inodeIndex);
                       
            //rootblock->updateInode(); //todo

            //get inode index

            superblock->updateFirstFreeInodeIndex(imap->getNextFreeInode(indexFreeDB));


        }

        superblock->writeSuperblock(*bd);
        dmap->writeDMap(*bd);
        fat->writeFat(*bd);
        imap->write(bd);
        rootblock->init(bd);

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
