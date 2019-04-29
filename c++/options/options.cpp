#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include <algorithm>

#include "options.h"

using namespace std;

options::options()
{
	mDevice = "/dev/mtd0";
    mTestName = "info";
    mPkgPath = "./test.pkg";
    mBlock = 0;
    mPageNum = 0;
    mOobOffset = 0;
    mOobLen = 2; // bad block marker size
    mValue = 0xff;
    mWriteOob = false;
    mJffs2Cleanmarker = false;
    mTestCases = {
        {TEST_INFO, "show the MTD sysfs summary info"},
        {TEST_READ_VERSION, "read pkg versions from each MTD partition"},
        {TEST_LIST_BAD, "scan and list bad block on MTD partition. [-d]"},
        {TEST_WRITE_PKG, "erase and write pkg to MTD partition. [-d]\n"
            "    -p to specify pkg path."},
        {TEST_ERASE, "erase the whole MTD partition. [-d]\n"
            "    -j write JFFS2 cleanmarker"},
        {TEST_ERASE_NOSKIP_BB, "erase the whole MTD partition without skiping bad block. [-d]\n"
            "    NOTE: use `echo 1 > /sys/kernel/debug/nanderasebb` to allow erasing bad blocks"},
        {TEST_READ_BLOCK_OOB, "read all OOBs of one block. [-d]\n"
            "    -b to specify block number."},
        {TEST_READ_OOB, "read OOB specifid by page num and offset. [-d]\n"
            "    -o to specify offset in the pattern page_number,offset,len."},
        {TEST_MARK_BAD, "mark block bad. [-d]\n"
            "    -b to specify block number."},
        {TEST_WRITE, "write a page with specified value. Do not skip bad blocks. [-d]\n"
            "    -i to specify page number.\n"
            "    -v to specify value to write in hex format (eg. 0x55).\n"
            "    -s to write value to spare ares (ie. oob) as well."},
    };
}

void options::usage()
{
	cout << "Usage: mtd_test " << endl;
	cout << "  -t: Test case. Default is " << mTestName << endl;
	cout << "  -d: MTD device. Default is " << mDevice << endl;
    cout << "  -p: pkg path for write-pkg test. Default is " << mPkgPath << endl;
	cout << "  -b: block number. Default is " << mBlock << endl;
	cout << "  -o: offset for oob read. Default is " << mPageNum << ","
         << mOobOffset << "," << mOobLen << endl;
	cout << "  -i: page number. Default is " << mPageNum << endl;
	cout << "  -v: write value. Default is " << to_string(mValue) << endl;
	cout << "  -s: write value to OOB. Default is " << boolalpha << mWriteOob << endl;
	cout << "  -j: write JFFS2 cleanmarker when erasing partition. Default is "
         << boolalpha << mJffs2Cleanmarker << endl;

	cout << endl << "Test cases are : " << endl;
    for (auto &p: mTestCases)
        cout << "  - " << p.first << " : " << p.second << endl;
}

void options::parse(int argc, char **argv)
{
	int opt;
    bool find = false;
    vector<string>::iterator pos;
    string str;

	while ( (opt = getopt(argc, argv, "d:t:p:b:o:i:v:sjh")) != -1) {
        switch (opt) {
            case 's': mWriteOob = true; break;
            case 'j': mJffs2Cleanmarker = true; break;
            case 'd': mDevice = optarg; break;
            case 'p': mPkgPath = optarg; break;
            case 'b': mBlock = stoi(optarg); break;
            case 'i': mPageNum = stoi(optarg); break;
            case 'v': mValue = static_cast<uint8_t>(stoi(optarg, nullptr, 16)); break;
            case 't':
                  for (auto &p: mTestCases) {
                      if (p.first == optarg) {
                          mTestName = p.first;
                          find = true;
                          break;
                      }
                  }
                  if (!find) {
                      cout << "invalid test case name: " << optarg << endl;
                      usage();
                      exit(1);
                  }
                  break;
            case 'o':
                  {
                  str = optarg;
                  auto pos1 = str.find(',');
                  auto pos2 = str.find(',', pos1+1);
                  if (pos1 == string::npos || pos2 == string::npos) {
                      cout << "invalid -o format" << endl;
                      usage();
                      exit(1);
                  }
                  mPageNum = stoi(str.substr(0, pos1));;
                  mOobOffset = stoi(str.substr(pos1+1, pos2 - pos1));
                  mOobLen = stoi(str.substr(pos2+1, str.length() - pos2));
                  }
                  break;
            case 'h':
            default:
                      usage();
                      exit(0);
        }
	}
}

void options::dump()
{
	cout << "=========================================================" << endl;
	cout << "  device    : " << mDevice << endl;
	cout << "  test case : " << mTestName << endl;
	cout << "=========================================================" << endl;
}
