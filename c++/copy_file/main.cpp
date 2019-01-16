#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

using namespace std;

#include <thread>
#include <chrono>
#define sleep(sec) this_thread::sleep_for(chrono::seconds((sec)))

#define interval 5

// this does not use too much memory
bool readFromFile(const string& fileName, vector<uint8_t>& payload)
{
    if (fileName.size()) {
        ifstream file(fileName);
        if (file.good()) {
            file.seekg(0, file.end);
            size_t fileSize = static_cast<size_t >(file.tellg());
            payload.resize(fileSize);
            file.seekg(0, file.beg);
            file.read(reinterpret_cast<char *>(payload.data()), payload.size());
            return true;
        }
    }
    return false;
}

bool copy(string const &srcFileName, string const &dstFileName)
{
    if (srcFileName.empty()) {
        return false;
    }

    ifstream src(srcFileName, ios::binary);

    src.seekg(0, ios::end);
    size_t fileSize = static_cast<size_t >(src.tellg());
    cout << "File size: " << fileSize << endl;

    sleep(interval);
    src.seekg(0, ios::beg);
    ofstream dest(dstFileName, ios::binary);
    dest << src.rdbuf();
    cout << "after copy" << endl;
    sleep(interval);

    dest.flush();
    dest.close();

    cout << "after close" << endl;
    sleep(interval);

    return true;
}

int main(int argc, char **argv)
{
    const string inFile = "/home/haishanz/Downloads/pkgs/g7_platform_dev_su_01.06.0000.14.jbz";
    copy(inFile, "dest.jbz");

    vector<uint8_t> buffer;
    readFromFile(inFile, buffer);
    cout << "afer read file" << endl;
    sleep(interval);

    buffer.clear();
    buffer.shrink_to_fit();
    cout << "afer shrink" << endl;
    sleep(interval);

	return 0;
}
