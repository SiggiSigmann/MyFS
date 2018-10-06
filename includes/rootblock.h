/*
Manage the rootblock of the FS
*/

#ifndef rootblock
#define rootblock

struct Inode{
    char *fileName;
    uint fileSize;
    uint userID;
    uint groupID;
    uint mode;
    uint mtime;
    uint ctime;
    uint atime;
    uint data;
}

class RootBlock{
    private:
        Inode *inode;
    public:
        RootBlock();

}


#endif