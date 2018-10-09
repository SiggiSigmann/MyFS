#ifndef dmapblock
#define dmapblock

#include <cstdint>
#include "constants.h"
#include "blockdevice.h"

//structure contains the datablocks of the File system
struct DataStruct{
    BlockDevice *blocks;
};

/*
The class DataBlock manage the datablocks of a filesystem
*/
class DataBlock{
    private:
        DataStruct *data;
    public:
        DataBlock();

};

#endif