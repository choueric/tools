#include <iostream>
#include <fstream>
#include <cstring>
#include <thread>
#include <chrono>
#include <vector>
#include <climits>

using namespace std;

int main(int argc, char **argv)
{
    vector<uint32_t> candidates;
    vector<uint32_t> no;
    uint32_t cache = 0;

    for (uint32_t i = 0; i <= 0x1fffff; i++) {
        int count = __builtin_popcount(i);
        if (count <= 10) {
            candidates.push_back(i);
            if (i != (cache+1) && i != 0) {
                printf("%x(%d) -> %x(%d): %d\n",
                        cache, __builtin_popcount(cache),
                        i, __builtin_popcount(i), i - cache);
            }
            cache = i;
        } else
            no.push_back(i);

    }

    cout << "number is: " << candidates.size() << endl;
    cout << "         : " << no.size() << endl;
    cout << " " << candidates.size() + no.size() << endl;
    cout << " " << 0x1fffff << endl;
    return 0;
}
