#include "fat.h"

Fat::Fat(unsigned int fatSize){
    this->fat = new unsigned int[fatSize];
    //TODO: free
}