#include "all.h"

using namespace std;
using namespace advent;

using tinput = vector<string>;

tinput parse_input(istream&& is) {
    tinput result;

    for (string s; getline(is, s); ) {
        if (s.empty()) { continue; }
        result.push_back(s);
    }

    return result;
}

tinput parse_input(istream& is) {
    return parse_input(move(is));
}



stringstream test{R"(
two1nine
eightwothree
abcone2threexyz
xtwone3four
4nineeightseven2
zoneight234
7pqrstsixteen
)"};



int parse_s1(string s) {
    s.erase(
        remove_if(s.begin(), s.end(), [](auto x) { return !isdigit(x); })
        , s.end()
    );
    return
        (s.front() - '0') * 10 + (s.back() - '0');
}

const unordered_map<string, int> values{
    {"one", 1},
    {"two", 2},
    {"three", 3},
    {"four", 4},
    {"five", 5},
    {"six", 6},
    {"seven", 7},
    {"eight", 8},
    {"nine", 9},
};

int parse_s2(string s) {
    const auto ff = [](const string& s) {
        for (int i=0; i<s.size(); ++i) {
            if (isdigit(s[i])) { return s[i] - '0'; }
            for (const auto& [v, n] : values) {
                // cout << v << " " << s.substr(i, v.size()) << endl;
                if (s.substr(i, v.size()) == v) {
                    return n;
                }
            }
        }
        return 0;
    };

    const auto fl = [](const string& s) {
        for (int i=s.size() -1; i >= 0; --i) {
            if (isdigit(s[i])) { return s[i] - '0'; }
            for (const auto& [v, n] : values) {
                if (s.substr(i, v.size()) == v) {
                    return n;
                }
            }
        }
        return 0;
    };


    cout << s << " " << ff(s) << " " << fl(s) << endl;

    return ff(s) * 10 + fl(s);
}

int main() {
    int result = 0;
    for (const auto& s : parse_input(cin)) {
        result += parse_s2(s);
    }
    cout << result << endl;
}
