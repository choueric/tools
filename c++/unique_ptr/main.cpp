#include <iostream>
#include <fstream>
#include <cstring>
#include <thread>
#include <chrono>

using namespace std;

class test {
public:
    string mName;
    test(string name): mName(name) {
        cout << "+++ " << mName << endl;
    }
    ~test() {
        cout << "--- " << mName << endl;
    }
};

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
