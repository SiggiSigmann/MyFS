#ifndef dmapblock
#define dmapblock

#include <cstdint>
#include "constants.h"
#include "blockdevice.h"

//structure contains the dmp of the File system
struct DMapStruct{
    bool* datablockMap;
};

/*
The class DMap manage the dmap of a filesystem
*/
class DMap{
    private:
        DMapStruct *dMap;             //defines if datablock is used (0=free, 1=used)
    public:
        DMap();

};

#endif