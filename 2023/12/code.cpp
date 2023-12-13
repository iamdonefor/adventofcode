#include "all.h"

using namespace std;
using namespace advent;

struct tspring {
    string spring;
    vector<int> comps;
};

using tinput = vector<tspring>;

tinput parse_input(istream&& is) {
    tinput result;

    for (string s; getline(is, s); ) {
        if (s.empty()) { continue; }

        stringstream ss{s};
        string tmp;
        vector<int> tmpv;

        ss >> tmp;

        while (ss && ss.peek() != EOF) {
            int x;
            ss >> x;
            ss.ignore(1);
            tmpv.push_back(x);
        }

        result.push_back({});
        auto& b = result.back();
        b.spring = tmp + "?" + tmp + "?" + tmp + "?" + tmp + "?" + tmp;
        for (int i=0; i<5; ++ i) {
            b.comps.insert(tmpv.begin(), tmpv.end(), b.comps.end());
        }
    }

    return result;
}

tinput parse_input(istream& is) {
    return parse_input(move(is));
}

stringstream test{R"(???.### 1,1,3
.??..??...?##. 1,1,3
?#?#?#?#?#?#?#? 1,3,1,6
????.#...#... 4,1,1
????.######..#####. 1,6,5
?###???????? 3,2,1
)"};

int64_t check(string s, const vector<int>& comps) {
    vector<vector<vector<int64_t>>> dp(s.size(), vector<vector<int64_t>>(comps.size(), vector<int64_t>(s.size(), 0)));

    if (s[0] == '.') {
        dp[0][0][0] = 1;
    } else if (s[0] == '#') {
        dp[0][0][1] = 1;
    } else {
        dp[0][0][0] = 1;
        dp[0][0][1] = 1;
    }

    for (int i=1; i<s.size(); ++i) {
    for (int j=0; j<comps.size(); ++j) {
    // for (int k=1; k<s.size(); ++i) {
        if (s[i] == '.') {
            s[i]
        }
    }
    return 0;
}

int64_t solution1(const tinput& data) {
    int64_t result{0};
    for (const auto& d : data) {
        result += check(d.spring, d.comps);
    }
    return result;
}

int main() {
    const auto& testi = parse_input(test);
    cout << solution1(testi) << endl;

    const auto& cini = parse_input(cin);
    cout << solution1(cini) << endl;
}
