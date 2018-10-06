/*
Manage the rootblock of the FS
*/

#ifndef rootblock
#define rootblock

struct Inode{
    char *fileName;
    unsigned int fileSize;
    unsigned int userID;
    unsigned int groupID;
    unsigned int mode;
    unsigned int mtime;
    unsigned int ctime;
    unsigned int atime;
    unsigned int data;
};

class RootBlock{
    private:
        Inode *inode;
    public:
        RootBlock();

}


#endif