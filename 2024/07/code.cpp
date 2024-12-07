#include "all.h"

using namespace std;
using namespace advent;

struct tequation {
    int64_t result;
    vector<int64_t> values;
};

using tinput = vector<tequation>;

tinput parse_input(istream&& is) {
    tinput result;

    for (string s; getline(is, s); ) {
        if (s.empty()) { continue; }

        tequation e;
        const auto& splitted = split(s, " ");

        e.result = stol(string{splitted.front()});
        for (int i=1; i<splitted.size(); ++i) {
            e.values.push_back(stol(string{splitted[i]}));
        }

        result.emplace_back(e);
    }

    return result;
}

tinput parse_input(istream& is) {
    return parse_input(move(is));
}

stringstream test{R"(190: 10 19
3267: 81 40 27
83: 17 5
156: 15 6
7290: 6 8 6 15
161011: 16 10 13
192: 17 8 14
21037: 9 7 18 13
292: 11 6 16 20
)"};

int64_t check_eq1(const tequation& d) {
    const auto nops = d.values.size() - 1;

    for (int64_t i=0; i<pow(2, nops); ++i) {
        int64_t check = d.values[0];
    
        for (int64_t j=0; j<nops; ++j) {
            if (i & (1 << j)) {
                check *= d.values[j+1];
            } else {
                check += d.values[j+1];
            }
        }

        if (check == d.result) {
            return d.result;
        }
    }

    return 0;
}

int64_t solution1(const tinput& data) {
    int64_t result{0};

    for (const auto& d : data) {
        result += check_eq1(d);
    }

    return result;
}

int64_t orderof3(int64_t v, int64_t p) {
    const static vector<int64_t> powersof3{1, 3, 9, 27, 81, 243, 729, 2187, 6561, 19683, 59049, 177147};
    assert(p < 11);

    return (v % powersof3[p+1]) / powersof3[p];
}

int64_t concat(int64_t what, int64_t by) {
    const auto result = what * (pow(10, static_cast<int64_t>(log10(by)) + 1)) + by;
    return result;
}

int64_t check_eq2(const tequation& d) {
    const auto nops = d.values.size() - 1;

    for (int64_t i=0; i<pow(3, nops); ++i) {
        int64_t check = d.values[0];
    
        for (int64_t j=0; j<nops; ++j) {
            switch (orderof3(i, j)) {
                case 0:
                    check *= d.values[j+1];
                    break;
                case 1:
                    check += d.values[j+1];
                    break;
                case 2:
                    check = concat(check, d.values[j+1]);
                    break;
                default:
                    throw runtime_error("not pow 3");
            }
        }

        if (check == d.result) {
            return d.result;
        }
    }

    return 0;
}

int64_t solution2(const tinput& data) {
    int64_t result{0};

    for (const auto& d : data) {
        result += check_eq2(d);
    }

    return result;
}

int main() {
    const auto& testi = parse_input(test);
    cout << solution2(testi) << endl;

    const auto& cini = parse_input(cin);
    cout << solution2(cini) << endl;
}
