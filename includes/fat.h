#ifndef fatblock_h
#define fatblock_h

#include <cstdint>
#include "blockdevice.h"
#include "constants.h"

//structure containing the FAT of the File system
struct FatStruct{
    uint32_t* fatMap;
};

/*
This class manages the FAT of a filesystem
*/
class FatHandler{
    private:
        FatStruct *fat;
    public:
        FatHandler();
        ~FatHandler();
        uint32_t get(uint32_t blockIndex);
        void set(uint32_t blockIndex, uint32_t value);
        void writeFat(BlockDevice bd);
        void readFat(BlockDevice bd);
};

#endif