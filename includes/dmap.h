#ifndef dmapblock
#define dmapblock

#include <cstdint>
#include "constants.h"

//structure contains the datablock map of the File system, the index corresponds to the data block index.
//True  = block is free
//False = block is occupied

/*
The class DMap manages the dmap of a filesystem
*/
class DMap{
    struct DMapStruct{
        bool* dataBlockMap;
    };
    private:
        DMapStruct *dMapStruct;             //defines if datablock is used (0=free, 1=used)
    public:
        DMap();
        void occupyDatablock(uint32_t dataBlockIndex);
        void freeDatablock(uint32_t dataBlockIndex);
};

#endif