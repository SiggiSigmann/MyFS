/*
Manage the superblock of the FS
*/

#ifndef superblock
#define superblock

struct SuperStruct{
    unsigned int dmapAddress;
    unsigned int fatAdress;
    unsigned int rootAdress;
    unsigned int maxFiles;
    unsigned int blockSize;
    unsigned int amountOfBlocks;
};

class SuperBlock{
    private:
        SuperStruct *superBlock;
    public:
        SuperBlock();

}


#endif