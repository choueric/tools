#include <iostream>
#include <fstream>
#include <cstring>
#include <thread>
#include <chrono>
#include <vector>
#include <climits>

using namespace std;

#ifdef STORE
vector<uint32_t> okVector;
vector<uint32_t> badVector;

static inline void handleOk(int i) { okVector.push_back(i); }
static inline void handleBad(int i) { badVector.push_back(i); }
static inline int okNum() { return okVector.size(); }
static inline int badNum() { return badVector.size(); }

#else
int okSum = 0;
int badSum = 0;

static inline void handleOk(int i) { okSum++; }
static inline void handleBad(int i) { badSum++; }
static inline int okNum() { return okSum; }
static inline int badNum() { return badSum; }
#endif

int main(int argc, char **argv)
{
    uint32_t cache = 0;

    for (uint32_t i = 0; i <= 0x1fffff; i++) {
        int count = __builtin_popcount(i);
        if (count <= 10) {
            handleOk(i);
            if (i != (cache+1) && i != 0) {
                printf("%x(%d) -> %x(%d): %d\n",
                        cache, __builtin_popcount(cache),
                        i, __builtin_popcount(i), i - cache);
            }
            cache = i;
        } else {
            handleBad(i);
        }

    }

    cout << "OK number is  : " << okNum() << endl;
    cout << "Bad number is : " << badNum() << endl;
    cout << "total is " << okNum() + badNum()  << endl;
    cout << " expect: " << 0x1fffff << endl;
    return 0;
}
