#include <string>
#include <vector>

class options {
public:
    static constexpr const char *TEST_INFO = "info";
    static constexpr const char *TEST_READ_VERSION = "read-version";
    static constexpr const char *TEST_LIST_BAD = "list-bad";
    static constexpr const char *TEST_WRITE_PKG = "write-pkg";
    static constexpr const char *TEST_ERASE = "erase";
    static constexpr const char *TEST_ERASE_NOSKIP_BB = "erase-noskip-bb";
    static constexpr const char *TEST_READ_BLOCK_OOB = "read-block-oob";
    static constexpr const char *TEST_READ_OOB = "read-oob";
    static constexpr const char *TEST_MARK_BAD = "mark-bad";
    static constexpr const char *TEST_WRITE = "write";

    options();
    void parse(int argc, char **argv);
    void usage();
    void dump();

    std::string mDevice;
    std::string mTestName;
    std::vector<std::pair<std::string, std::string>> mTestCases;
    std::string mPkgPath;
    int mBlock;
    int mPageNum;
    int mOobOffset;
    int mOobLen;
    uint8_t mValue;
    bool mWriteOob;
    bool mJffs2Cleanmarker;
};
