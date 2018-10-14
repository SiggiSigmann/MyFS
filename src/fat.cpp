#include "fat.h"

Fat::Fat(unsigned int fatSize){
    fat = new unsigned int[fatSize];
    //TODO: free
}