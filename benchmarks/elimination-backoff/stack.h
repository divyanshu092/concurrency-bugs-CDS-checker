struct Cell {
    Cell *pnext;
    void *pdata;
};

struct ThreadInfo {
    unsigned int id;
    char op;
    Cell cell;
    int spin;
};

struct Simple_Stack {
    Cell *ptop;
};

