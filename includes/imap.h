#ifndef imap_h
#define imap_h

#include <cstdint>
#include <stdlib.h>
#include "constants.h"
#include "blockdevice.h"

/*
The class DMap provides managing functions for the filesystems dmap.
*/
struct IMapStruct{
     bool imap[NUM_DIR_ENTRIES];
};

class IMapHandler{
    private:
        IMapStruct* iMapStruct;
    public:
        IMapHandler();
        ~IMapHandler();
        void init();
        void write(BlockDevice bd);
        void read(BlockDevice bd);
        void occupyIMapEntry(uint32_t index);
        void freeIMapEntry(uint32_t index);
        bool getIMapEntry(uint32_t index);
};

#endif