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
#include <sys/stat.h>
#include <libgen.h>

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

        //
        for(int i = 2;i<argc;i++){
        
            struct stat sb;                             //store metadate of given files
            if (stat(argv[i], &sb) == -1) {
                 return -(EIO);
            }
            char* basenameOfFile = basename(argv[i]);

            //calculate needed blocks
            int neededBlocks = sb.st_size / BLOCK_SIZE;
            if(sb.st_size % BLOCK_SIZE != 0){
                neededBlocks++;
            }

            //check if a inode is free
            if(superblock->getNumberOfFreeInodes()<1){    //check if a inode is free
                printf("not possibel to add more files then 62\n");
                return -(EIO);
            }

            //check if enough blocks are free
            if(superblock->getNumberOfFreeBlocks() <= neededBlocks){ //check if enough space (datablocks) ar free
                printf("not enough space in FS\n");
                return -(EIO);
            }

            //check if the filename is free
            if(rootblock->checkFilenameOccupied(bd, basenameOfFile)!=-1){
                printf("name already in use\n");
                //return -(EEXIST);
                break;
            }

            //open file to read
            BlockDevice* inputfile = new BlockDevice();             //open inputfile to read blocks
            inputfile->open(argv[i]);

            //write blocks, occupy blocks fill fat
            uint32_t firstDataBlock;
            uint32_t indexFreeDB;
            char* filecontent = (char*) malloc(BLOCK_SIZE);                         //buffer to store one block of a file
            for(int k = 0;k<neededBlocks;k++){
                //empty buffer
                for(int j =0; j<BLOCK_SIZE; j++){
                    filecontent[j]=0;
                }
                indexFreeDB = superblock->getFirstFreeBlockIndex();

                //copy content of file to FS
                inputfile->read(k,filecontent);
                bd->write(FIRST_DATA_BLOCK+indexFreeDB, filecontent);

                //modify dmap
                dmap->occupyDatablock(indexFreeDB);
                superblock->updateFirstFreeBlockIndex(dmap->getNextFreeDatablock(indexFreeDB));

                //modify fat
                if(k==0){
                    printf("fat start %d\n",indexFreeDB);
                    firstDataBlock = indexFreeDB;
                }
                if(k==neededBlocks-1){
                    printf("fat end %d\n",indexFreeDB);
                    fat->set(indexFreeDB, END_OF_FILE_ENTRY);
                }else{
                    fat->set(indexFreeDB, superblock->getFirstFreeBlockIndex());
                }
            }

            //calculate free blocks
            superblock->updateNumberOfFreeBlocks(superblock->getNumberOfFreeBlocks()-neededBlocks);

            inputfile->close();
            delete inputfile;
            free(filecontent);

            //modify imap
            uint32_t inodeIndex = superblock->getFirstFreeInodeIndex();
            imap->occupyIMapEntry(inodeIndex);
            superblock->updateFirstFreeInodeIndex(imap->getNextFreeInode(inodeIndex));
            superblock->updateNumberOfFreeInodes(superblock->getNumberOfFreeInodes()-1);

            //write inode           
            rootblock->updateInode(bd, inodeIndex, basenameOfFile, firstDataBlock, neededBlocks,sb.st_atime,sb.st_mtime,sb.st_ctime,sb.st_uid,sb.st_gid,sb.S_IFREG);
        }

        //write modifed FS blocks to FS
        superblock->writeSuperblock(*bd);
        dmap->writeDMap(*bd);
        fat->writeFat(*bd);
        imap->write(bd);

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
