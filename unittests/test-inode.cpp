//
//  test-inode.cpp
//  testing
//


#include "catch.hpp"

#include <stdio.h>
#include <string.h>

#include "helper.hpp"

#include "blockdevice.h"
#include "inode.h"
#include "myfs.h"

#define BD_PATH "/tmp/bd.bin"

TEST_CASE( "Test Inode helper", "[inode]" ) {
    
    remove(BD_PATH);
    
    BlockDevice bd;
    bd.create(BD_PATH);
    
    SECTION("writing To inode") {
        
    }
    
    bd.close();
    remove(BD_PATH);
    
}
