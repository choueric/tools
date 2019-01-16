#include <iostream>
#include <fstream>
#include <cstring>
#include <thread>
#include <chrono>

using namespace std;

int main(int argc, char **argv)
{
    bool create = true;
    unique_ptr<test> p;
    if (create) {
        test t("one");
        p.reset(new test("unique"));
    }

    test tt("two");

    this_thread::sleep_for(chrono::seconds(5));
	return 0;
}
