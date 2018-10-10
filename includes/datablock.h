#ifndef dmapblock_h
#define dmapblock_h

#include "constants.h"
#include "blockdevice.h"

//structure contains the datablocks of the File system
struct DataStruct{
    BlockDevice *blocks;
};

/*
The class DataBlock manage the datablocks of a filesystem
*/
class DataBlockHelper{
    private:
        DataStruct *data;
    public:
        DataBlockHelper();

};

#endif