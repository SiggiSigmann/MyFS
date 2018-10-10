#ifndef dmapblock
#define dmapblock

#include <cstdint>
#include "constants.h"

/*
The class DMap provides managing functions for the filesystems dmap.
*/
class DMap{
    struct DMapStruct{
        //structure contains the datablock map of the File system, the index corresponds to the data block index.
        //True  = block is free
        //False = block is occupied
        bool* dataBlockMap;
    };
    private:
        DMapStruct *dMapStruct;
    public:
        DMap();
        ~DMap();
        bool get(uint32_t dataBlockIndex);
        void occupyDatablock(uint32_t dataBlockIndex);
        void freeDatablock(uint32_t dataBlockIndex);
        void writeDMap();
        void readDMap();
};

#endif