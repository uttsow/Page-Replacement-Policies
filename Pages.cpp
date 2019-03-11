#include "Pages.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <deque>
#include <iterator>
#include <fstream>
#include <iomanip>


using namespace std;


Pages::Pages(){

}

Pages::~Pages(){

}



/*
    No Localitiy Specifications

Each reference is to a random page within the set of acccsed Pages
100 unique page, 10,000 access;
*/
vector<int> Pages::noLocality(int page, int pageAccess){
    //using the random seed technique from prog1generator
    srand(time(0));
    vector<int> retVal;
    int randNum = -1;
    for(int i = 0; i<pageAccess; i++){
        randNum = (rand() % page +1);
        retVal.push_back(randNum);
    }

    // for(auto i: retVal){
    //     cout << ' ' << i;
    // }

    return retVal;
}

/*
    80-20 workload Specifications
80% of the references are made to 20% of the pages (the “hot” pages); the remaining 20% of the references are made to the remaining 80% of the pages (the “cold” pages)
*/

vector<int> Pages::_8020(int page, int pageAccess){
    vector<int> retVal;
    srand(time(0));

    int hotPage = page*(0.2);
    int coldPage = page - hotPage;

    int eightyRef = pageAccess * (0.8);
    int twentyRef = pageAccess - eightyRef;

    for(int i = 0; i < eightyRef; i++){
        int randNum = (rand() % hotPage +1);
        retVal.push_back(randNum);
    }

    for(int i =0; i< twentyRef; i++){
        int randNum = (rand() % coldPage +1);
        retVal.push_back(randNum);
    }

    //shuffle the array to make it truly random
    random_shuffle(retVal.begin(), retVal.end());

    return retVal;


}

/*
    Looping workload Specifications
Refer to 50 pages in seq starting at 0 then going up to 49,
then loop, repeating those access for a total of 10,000 access
for 50 unique Pages

*/

vector<int> Pages::looping(int page, int pageAccess){
    vector<int> retVal;

    (void)page;

    //Pages 1- 50 Generation
    for(int i =0; i<pageAccess; i++){
        int loopPage = ((i % 50) + 1);
        retVal.push_back(loopPage);
    }

    // for(auto i: retVal){
    //     cout << ' '<< i;
    // }

    return retVal;

}


/*
    Optimal Specifications
Replace pages that will be accessed furthest in the future;
*/

int Pages::repOPT(vector<int> work, int cacheSize){

    int hitRate = 0;
    vector<int> optimal (cacheSize);
    vector<int> dist;
    int delIndex = -1;

    //Inital warm up
    for(int i = 0; i < cacheSize; i++){
        optimal[i] = -1;
        dist.push_back(-1);
    }

    for(unsigned int i = 0; i < work.size(); i++){
        // checks if its in memory
        if(find(optimal.begin(), optimal.end(), work[i]) != optimal.end()){
            hitRate++;
        }else{
            //Not found in cache, look furthest for replacement
            bool found = false;
            //Keeps track of distance. looking for furthest
            for(int j = 0; j < cacheSize; j++){
                for(unsigned int temp = i+1; temp < work.size(); temp++){
                    dist[j] = 0;
                    if(optimal[j] == work[temp]){
                        dist[j] = temp - i;
                        break;
                    }
                }
            }


            //checks to see if cache is full or not
            vector<int>::iterator it = find(optimal.begin(), optimal.end(), -1);
            if(it == optimal.end()){
                //cache full, so find the 0 distance;
                for(int j = 0; j < cacheSize; j++){
                    if(dist[j] == 0){
                        delIndex = j;
                        found = true;
                        break;
                    }
                }
            }else{
                //cache not full, so find the index of -1;
                delIndex = distance(optimal.begin(), it);
                found = true;


            }

            //if no 0 distance and cache full, find largest distance;
            if(found == false){
                //find largest distance;
                int largest = dist[0];
                for(int i =1; i < cacheSize; i++){
                    if(dist[i] > largest){
                        largest = dist[i];
                        delIndex = i;
                        found = true;
                    }
                }
            }

            optimal[delIndex] = work[i];



        }
    }
    // cout << "Cache size Optimal " << optimal.size() << '\n';

    // cout << "OPTIMAl HIT RATE: " << hitRate << '\n';
    return hitRate;
}

/*
    LRU Specifications
Uses histroy to replace pages that have been accessed least recently
*/

int Pages::repLRU(vector<int> work, int cacheSize){
    //create a vector of lru structures based on the frame size
    vector<Lru> lru (cacheSize);

    int hitRate = 0;

    //inital warm up. set everything to -1.
    for(int i =0; i < cacheSize; i++){
        lru[i].refString = -1;
    }

    for(unsigned int i =0; i < work.size(); i++){
        bool found = false;
        for(int j= 0; j < cacheSize; j++){
            //Found in memory!
            if(lru[j].refString == work[i]){
                hitRate++;
                lru[j].counter += 1;
                found = true;
                break;
            }
        }

        //Not found in memory. need page replacement;
        if(found == false){
            //find min counter;
            int min = 0;
            for(int j = 1; j < cacheSize; j++){
                if(lru[min].counter > lru[j].counter){
                    min = j;    //get the index of the smaller counter
                }
            }

            //do the replacement. set counter to 0;
            lru[min].refString = work[i];
            lru[min].counter += 1;


        }
    }

    // cout << "Cache size LRU " << lru.size() << '\n';

    // cout << "LRU HIT RATE: " << hitRate << '\n';
    return hitRate;

}


/*
    FIFO Specifications
First in first out. Will be using a deque;
*/
int Pages::repFIFO(vector<int> work, int cacheSize){
    deque<int> fifo;
    int hitRate = 0;

    //inital warm up;
    for(int i = 0; i < cacheSize; i++){
        fifo.push_back(-1);
    }


    for(unsigned int i = 0; i < work.size(); i++){
        //checking to see if page is in cache;
        if(find(fifo.begin(), fifo.end(), work[i]) != fifo.end()){
            hitRate++;

        }else{
            fifo.pop_front();
            fifo.push_back(work[i]);
        }

    }

    // cout << "Cache size FIFO " << fifo.size() << '\n';

    // cout << "FIFO HIT RATE: " << hitRate << '\n';
    return hitRate;

}

/*
    Random Replacement Specifications
Random simply picks a random page to replace under memory pressure.
*/

int Pages::repRAND(vector<int> work, int cacheSize){
    srand(time(0));

    //fixed size cache array
    vector<int> random (cacheSize);

    int hitRate = 0;


    //initally warm up
    for(int i= 0; i< cacheSize; i++){
        random[i] = -1;
    }

    //algorithm for Random Replacement
    for(unsigned int i = 0; i< work.size(); i++){
        //if found
        if(find(random.begin(), random.end(), work[i]) !=random.end()){
            hitRate++;
        }else{
            //Missed, not in cache
            int randomReplacementIndex = (rand() % cacheSize);
            // cout << "Replacement index: " << randomReplacementIndex << '\n';
            random.erase(random.begin() + randomReplacementIndex);
            random.push_back(work[i]);
        }
    }

    // cout << "RAAND HIT RATE: " << hitRate << endl;
    return hitRate;

}

/*
    Second Chance Algorithim
Same as LRU but use a reference bit. When a page is reference, set it to 1. if the page is already set to 1, set it back to 0 and continue. Replace in the next iteration.
*/
int Pages::repCLOCK(vector<int> work, int cacheSize){
    vector<Lru> lru (cacheSize);
    int hitRate = 0;
    int tracker = 0;
    //Inital warm up
    for(int i = 0; i < cacheSize; i++){
        lru[i].refString = -1;
        lru[i].referenceBit = 0;
    }

    for(unsigned int i =0; i < work.size(); i++){
        bool found = false;
        for(int j= 0; j < cacheSize; j++){
            //Found in memory!
            if(lru[j].refString == work[i]){
                hitRate++;
                lru[j].referenceBit = 1;
                found = true;
                break;
            }
        }

        //Not found in memory. need page replacement;
        if(found == false){

            for(int j = tracker; j < cacheSize; j++){
                //if refbit is 1, set to 0
                if(lru[j].referenceBit == 1){
                    lru[j].referenceBit = 0;
                }else if(lru[j].referenceBit ==0){
                    //found victim;
                    lru[j].refString = work[i];
                    lru[j].referenceBit = 1;
                    break;
                }
            }

            //updates tracker for positon, then loops back when its bigger than the cache.
            tracker++;
            if(tracker >= cacheSize){
                tracker = 0;
            }

        }
    }
    // cout << "CLOCK HIT RATE: " << hitRate;
    return hitRate;

}

//save as csv so you can use it for gnuplot
void Pages::display(string name, vector<int> opt, vector<int> lru, vector<int> fifo, vector<int> rand, vector<int> clock, int pageAccess){
    ofstream outputFile;
    string pathName = name + "_data.csv";
    outputFile.open(pathName);

    //Top header
    outputFile << "#cache, OPT, LRU, FIFO, RAND, CLOCK" << endl;
    outputFile << "0, 0, 0, 0, 0, 0" << endl;

    //cache goes by increments of 5;
    for(int i = 0; i < 20 ; i++){
        float optVal = ((float)opt[i] / (float)pageAccess);
        float lruVal = ((float)lru[i] / (float)pageAccess);
        float fifoVal = ((float)fifo[i] / (float)pageAccess);
        float randVal = ((float)rand[i] / (float)pageAccess);
        float clockVal = ((float)clock[i] / (float)pageAccess);

        //cout<< "FIFO VAL" << fifoVal << endl;
        //writing to FILES. Percentage values
        outputFile<< ((i+1) *5) << ", " << fixed << setprecision(13) << (optVal * 100) << ", " << fixed << setprecision(13) << (lruVal * 100) << ", " << fixed << setprecision(13) << (fifoVal * 100) << ", " << fixed << setprecision(13) << (randVal * 100) << ", " << fixed << setprecision(13) << (clockVal * 100) << endl;
    }
}
