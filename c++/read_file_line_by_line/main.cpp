#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

using namespace std;

class aa {
    public:
    aa() { cout << "aa" << endl;};
};


struct AssetInfo
{
	int assetType;             ///< type
    aa a;
    int ttt;
	string assetVersion;                   ///< version string
	vector<uint8_t> signature;             ///< signature
	uint32_t obfuscationAlgo = 0;          ///< obfuscation

	explicit AssetInfo(int type) : assetType(type) {};
};

void test(const AssetInfo& info)
{
    cout << "> type: " << info.assetType << endl;
    cout << "> size: " << info.signature.size() << endl;
}

int main(int argc, char **argv)
{
    AssetInfo info(1);
    info.signature.resize(10);
    info.assetType = 9;
    cout << "size: " << info.signature.size() << endl;
    int v = 12;
    test(v);

	return 0;
}
