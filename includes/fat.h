#ifndef fatblock_h
#define fatblock_h

#include <cstdint>
#include "constants.h"

//structure contains the fat of the File system
struct FatStruct{
    uint32_t* fatMap;
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