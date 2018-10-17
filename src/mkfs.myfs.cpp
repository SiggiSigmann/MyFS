//
//  mk.myfs.cpp
//  myfs
//
//  Created by Oliveriltus Waldhorst on 07.09.17.


#include "myfs.h"
#include "blockdevice.h"
#include "macros.h"
#include <iostream>
#include "constants.h"



int main(int argc, char *argv[]) {
    //std::cout << argc << std::endl;
    //for (int i = 0; i < argc; ++i) {
    //    std::cout << argv[i] << std::endl;
    //}

    if(argc > 1){
        BlockDevice* bd = new BlockDevice(BLOCK_SIZE);
        bd->create(argv[1]);







        return 0;
    }
    
    return -1;
}
