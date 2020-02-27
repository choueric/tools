#include <iostream>
#include <regex>

using namespace std;

string getValue(const string& key, const string& line)
{
    regex specialChars { R"([-[\]{}()*+?.,\^$|#\s])" };
    auto reKey = regex_replace(key, specialChars, R"(\$&)");

    regex exp(R"(\b)" + reKey + R"(\s*=\s*(\d+\.\d+\.\d+))");
    smatch match;
    if (regex_search(line, match, exp))
	return match[1];
    return "no";
}

int main(int argc, char **argv)
{
    vector<pair<string, string>> test = {
	{"abc", "abc=1.1.1"},
	{"a.c", "a.c=2.2.2"},
	{"a.c", "acc=2.2.2"},
    };

    for (const auto& p: test) {
	cout << getValue(p.first, p.second) << endl;
    }

    return 0;
}
