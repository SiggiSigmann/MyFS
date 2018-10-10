#ifndef fatblock
#define fatblock

#include <cstdint>
#include "constants.h"

//structure contains the fat of the File system
struct FatStruct{
    uint32_t fatMap[NUMBER_OF_USABLE_DATABLOCKS];
};

/*
The class Fat manage the fat of a filesystem
*/
class FatHelper{
    private:
        FatStruct *fat;
    public:
        FatHelper();

};

#endif