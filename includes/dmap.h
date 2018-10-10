#ifndef dmapblock
#define dmapblock

#include <cstdint>
#include "constants.h"

//structure contains the dmp of the File system
struct DMapStruct{
    bool datablockMap[NUMBER_OF_USABLE_DATABLOCKS];
};

/*
The class DMap manage the dmap of a filesystem
*/
class DMapHelper{
    private:
        DMapStruct *dMap;             //defines if datablock is used (0=free, 1=used)
    public:
        DMapHelper();

};

#endif