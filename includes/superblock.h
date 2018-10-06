/*
Manage the superblock of the FS
*/

#ifndef superblock
#define superblock

struct SuperStruct{
    unsigned intdmapAddress;
    unsigned intfatAdress;
    unsigned introotAdress;
    unsigned intmaxFiles;
    unsigned intblockSize;
    unsigned intamountOfBlocks;
};

class SuperBlock{
    private:
        SuperStruct *superBlock;
    public:
        SuperBlock();

}


#endif