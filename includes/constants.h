#ifndef constants_h
#define constants_h

//Requirements:
#define NAME_LENGTH 255 
#define BLOCK_SIZE 512
#define NUM_DIR_ENTRIES 64          //maximum amount of files
#define NUM_OPEN_FILES 64           //maximum amount of open files

//Own constants:
#define NUMBER_OF_USABLE_DATABLOCKS 1<<18 //2^18 = 262144 blocks => 128 MiB in total

#endif