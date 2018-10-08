#include "dmap.h"

DMap::DMap(unsigned int dmapSize){
    dmap = new bool[dmapSize];
    for(int i=0; i<dmapSize; i++){
        dmap[i] = 0;                            //define all datablocks as free
    }
    //TODO: free
}