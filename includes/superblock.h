/*
Manage the superblock of the FS
*/

#ifndef superblock
#define superblock

struct SuperStruct{
    uint dmapAddress;
    uint fatAdress;
    uint rootAdress;
    uint maxFiles;
    uint blockSize;
    uint amountOfBlocks;
}

class SuperBlock{
    private:
        SuperStruct *superBlock;
    public:
        SuperBlock();

}


#endif