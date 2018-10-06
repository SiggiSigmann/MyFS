/*
manage the dmap of the FS
*/

#ifndef dmapblock
#define dmapblock

class DMap{
    private:
        bool *dmap;             //defines if datablock is used (0=free, 1=used)
    public:
        DMap(unsigned int dmapSize);

}

#endif