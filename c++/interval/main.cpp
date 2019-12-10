#include <iostream>
#include <thread>
#include <chrono>
#include <ratio>

using namespace std;
using namespace std::chrono;

int main(int argc, char **argv)
{
    auto t1 = duration_cast<milliseconds>(steady_clock::now().time_since_epoch());

    this_thread::sleep_for(milliseconds{200});

    auto t2 = duration_cast<milliseconds>(steady_clock::now().time_since_epoch());
    auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

    cout << "Interval is " << int_ms.count() << "ms" << endl;

	return 0;
}
