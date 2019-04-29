#include <iostream>
#include <fstream>
#include <cstring>
#include <thread>
#include <chrono>
#include <vector>
#include <climits>

#include "options.h"

using namespace std;

class Test {
public:
    Test(int argc, char **argv)
    {
        mOpts.parse(argc, argv);
        mOpts.dump();
    }

    void execute();
    void test_read_version() { cout << __func__ << endl;};
    void test_info() { cout << __func__ << endl;};
    void test_list_bad() { cout << __func__ << endl;};
    void test_write_pkg() { cout << __func__ << endl;};
    void test_erase() { cout << __func__ << endl;};
    void test_erase_noskip_bb() { cout << __func__ << endl;};
    void test_read_block_oob() { cout << __func__ << endl;};
    void test_read_oob() { cout << __func__ << endl;};
    void test_mark_bad() { cout << __func__ << endl;};
    void test_write() { cout << __func__ << endl;};

private:
    options mOpts;
};

void Test::execute()
{
    if (mOpts.mTestName == options::TEST_INFO)
        test_info();
    else if (mOpts.mTestName == options::TEST_READ_VERSION)
        test_read_version();
    else if (mOpts.mTestName == options::TEST_LIST_BAD)
        test_list_bad();
    else if (mOpts.mTestName == options::TEST_WRITE_PKG)
        test_write_pkg();
    else if (mOpts.mTestName == options::TEST_ERASE)
        test_erase();
    else if (mOpts.mTestName == options::TEST_ERASE_NOSKIP_BB)
        test_erase_noskip_bb();
    else if (mOpts.mTestName == options::TEST_READ_BLOCK_OOB)
        test_read_block_oob();
    else if (mOpts.mTestName == options::TEST_READ_OOB)
        test_read_oob();
    else if (mOpts.mTestName == options::TEST_MARK_BAD)
        test_mark_bad();
    else if (mOpts.mTestName == options::TEST_WRITE)
        test_write();
    else
        cout << "unsupported test case: " << mOpts.mTestName << endl;
}

int main(int argc, char **argv)
{
    Test test(argc, argv);
    test.execute();
	return 0;
}
