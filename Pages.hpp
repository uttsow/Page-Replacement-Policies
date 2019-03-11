#ifndef PAGES_H
#define PAGES_H

#include <vector>
#include <string>


using namespace std;

class Pages{

private:

    //LRU Policy
    typedef struct{
        int counter = 0;
        int refString = 0;
        bool referenceBit;  //used for Second Chance
    }Lru;



public:
    Pages();
    ~Pages();

    //For generating workloads. Returns vectors of pages
    vector<int> noLocality(int page, int pageAccess);
    vector<int> _8020(int page, int pageAccess);
    vector<int> looping(int page, int pageAccess);


    //Page Replacement policies (5 Total). Returns hit count
    int repOPT(vector<int> work, int cacheSize);
    int repLRU(vector<int> work, int cacheSize);
    int repFIFO(vector<int> work, int cacheSize);
    int repRAND(vector<int> work, int cacheSize);
    int repCLOCK(vector<int> work, int cacheSize);


    //Display

    void display(string name, vector<int> opt, vector<int> lru, vector<int> fifo, vector<int> rand, vector<int> clock, int pageAccess);



};

#endif
