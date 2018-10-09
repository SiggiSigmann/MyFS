/*
manage the fat of the FS
*/

#ifndef fat
#define fat

#include <cstdint>
#include "constants.h"
#include "blockdevice.h"
//structure contains the fat of the File system
struct FatStruct{
    uint32_t* fatMap;
};

class Fat{
    private:
        FatStruct* fat;
    public:
        Fat();

};

#endif