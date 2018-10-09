#ifndef fatblock
#define fatblock

#include <cstdint>
#include "constants.h"
#include "blockdevice.h"

//structure contains the fat of the File system
struct FatStruct{
    uint32_t* fatMap;
};

/*
The class Fat manage the fat of a filesystem
*/
class Fat{
    private:
        FatStruct *fat;
    public:
        Fat();

};

#endif