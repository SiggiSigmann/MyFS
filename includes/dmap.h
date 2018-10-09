/*
manage the dmap of the FS
*/

#ifndef dmapblock
#define dmapblock

#include <cstdint>
#include "constants.h"
#include "blockdevice.h"
//structure contains the dmp of the File system
struct DMapStruct{
    bool* datablockMap;
};

class DMap{
    private:
        DMapStruct *dMap;             //defines if datablock is used (0=free, 1=used)
    public:
        DMap();

}

#endif