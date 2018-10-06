/*
manage the fat of the FS
*/

#ifndef fat
#define fat

class Fat{
    private:
        unsigned int* fat;
    public:
        Fat(unsigned int fatSize);

}

#endif