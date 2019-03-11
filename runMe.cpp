#include "Pages.hpp"
#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;
int main(int argc, char const *argv[]) {
    //supress warning
    (void)argc;
    (void)argv;

    Pages p;

    //Acutal program Program runner

    //Default values
    int pages = 100;
    int pageAccess = 10000;

    //First get the worloads from the 3 types
    vector<int> wrkNoLoc = p.noLocality(pages, pageAccess);
    vector<int> wrk8020 = p._8020(pages, pageAccess);
    vector<int> wrkLoop = p.looping(pages, pageAccess);

    //Vector to store hitRate of each replacement policy for each workload

    //1. No localitiy
    vector<int> optNoLoc;
    vector<int> lruNoLoc;
    vector<int> fifoNoLoc;
    vector<int> randNoLoc;
    vector<int> clockNoLoc;

    //2. 80-20
    vector<int> opt8020;
    vector<int> lru8020;
    vector<int> fifo8020;
    vector<int> rand8020;
    vector<int> clock8020;

    //3. looping
    vector<int> optLoop;
    vector<int> lruLoop;
    vector<int> fifoLoop;
    vector<int> randLoop;
    vector<int> clockLoop;

    //do cache size incrementations and store values in vector
;
    unsigned int cacheSize;
    for(cacheSize = 5; cacheSize <= 100; cacheSize+=5){

        //1. No locality
        optNoLoc.push_back(p.repOPT(wrkNoLoc, cacheSize));
        lruNoLoc.push_back(p.repLRU(wrkNoLoc, cacheSize));
        fifoNoLoc.push_back(p.repFIFO(wrkNoLoc, cacheSize));
        randNoLoc.push_back(p.repRAND(wrkNoLoc, cacheSize));
        clockNoLoc.push_back(p.repCLOCK(wrkNoLoc, cacheSize));

        //2. 8020
        opt8020.push_back(p.repOPT(wrk8020, cacheSize));
        lru8020.push_back(p.repLRU(wrk8020, cacheSize));
        fifo8020.push_back(p.repFIFO(wrk8020, cacheSize));
        rand8020.push_back(p.repRAND(wrk8020, cacheSize));
        clock8020.push_back(p.repCLOCK(wrk8020, cacheSize));

        //3. Looping
        optLoop.push_back(p.repOPT(wrkLoop, cacheSize));
        lruLoop.push_back(p.repLRU(wrkLoop, cacheSize));
        fifoLoop.push_back(p.repFIFO(wrkLoop, cacheSize));
        randLoop.push_back(p.repRAND(wrkLoop, cacheSize));
        clockLoop.push_back(p.repCLOCK(wrkLoop, cacheSize));

    }


    //call display function
    p.display("noLocality", optNoLoc, lruNoLoc, fifoNoLoc, randNoLoc, clockNoLoc, pageAccess);
    p.display("8020", opt8020, lru8020, fifo8020, rand8020, clock8020, pageAccess);
    p.display("looping", optLoop, lruLoop, fifoLoop, randLoop, clockLoop, pageAccess);


    return 0;
}
