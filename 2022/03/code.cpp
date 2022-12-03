#include "all.h"

using TInput = vector<string>;
using TPriority = uint64_t;

inline TPriority char2pr(char c) {
    if (c <= 'Z') {
        return c - 'A' + 27;
    } else {
        return c - 'a' + 1;
    }
}

TPriority getCommonPriority(const string& s) {
    const auto getDoubleChar = [](const string& s) {
        set<char> seen;

        for (int i=0; i<s.size()/2; ++i) {
            seen.insert(s[i]);
        }

        for (int i=s.size()/2; i<s.size(); ++i) {
            if (seen.count(s[i]) > 0) {
                return s[i];
            }
        }

        throw runtime_error("invalid data");
    };

    const auto c = getDoubleChar(s);
    return char2pr(c);
}

TPriority computeRucksack(const TInput& rucksack) {
    return accumulate(rucksack.begin(), rucksack.end(), 0, [](TPriority prev, const auto& s) {
        return prev + getCommonPriority(s);
    });
}

TPriority getCommonOf3(const TInput& rs, int idx) {
    set<char> s1{rs[idx].begin(), rs[idx].end()};
    set<char> s2;
    for (const auto c : rs[idx+1]) {
        if (s1.count(c) > 0) {
            s2.insert(c);
        }
    }
    set<char> s3;
    for (const auto c : rs[idx+2]) {
        if (s2.count(c) > 0) {
            s3.insert(c);
        }
    }

    assert(s3.size() == 1);
    return char2pr(*s3.begin());
}

TPriority computeRucksack2(const TInput& rucksack) {
    TPriority result{0};

    for (int i=0; i<rucksack.size(); i+=3) {
        result += getCommonOf3(rucksack, i);
    }

    return result;
}

TInput parseInput(istream& is) {
    TInput result;

    for (string s; getline(is, s); ) {
        if (s.empty()) { continue; }

        result.emplace_back(move(s));
    }

    return result;
}

stringstream test{R"(
vJrwpWtwJgWrhcsFMMfFFhFp
jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL
PmmdzqPrVvPwwTWBwg
wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn
ttgJtRGJQctTZtZT
CrZsJsPPZsGzwwsLwLmpwMDw
)"};

int main() {
    // cout << computeRucksack(parseInput(test)) << endl;
    // cout << computeRucksack(parseInput(cin)) << endl;

    cout << computeRucksack2(parseInput(test)) << endl;
    cout << computeRucksack2(parseInput(cin)) << endl;
}
