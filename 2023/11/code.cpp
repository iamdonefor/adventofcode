#include "all.h"

using namespace std;
using namespace advent;

using tinput = vector<vector<int>>;

tinput parse_input(istream&& is) {
    tinput result;
    int glxn{0};

    for (string s; getline(is, s); ) {
        if (s.empty()) { continue; }
        result.push_back({});

        for (const auto c : s) {
            if (c == '.') {
                result.back().push_back(0);
            } else {
                result.back().push_back(++glxn);
            }
        }
    }

    return result;
}

tinput parse_input(istream& is) {
    return parse_input(move(is));
}

stringstream test{R"(...#......
.......#..
#.........
..........
......#...
.#........
.........#
..........
.......#..
#...#.....
)"};

using advent::tcoords;
int64_t mhd(const tcoords& l, const tcoords& r) {
    return
        abs(l[0] - r[0]) + abs(l[1] - r[1]);
}

int64_t solution1(const tinput& data, int64_t expand_by) {
    vector<tcoords> galaxies;
    set<int> expandingy;
    set<int> expandingx;

    for (int64_t y=0; y<data.size(); ++y) {
        bool expme{true};
        for (int64_t x=0; x<data.front().size(); ++x) {
            if (data[y][x] != 0) {
                expme = false;
                break;
            }
        }
        if (expme) {
            expandingy.insert(y);
        }
    }

    for (int64_t x=0; x<data.front().size(); ++x) {
        bool expme{true};
        for (int64_t y=0; y<data.size(); ++y) {
            if (data[y][x] != 0) {
                expme = false;
                break;
            }
        }
        if (expme) {
            expandingx.insert(x);
        }
    }

    int64_t ay{0};
    for (int64_t y=0; y<data.size(); ++y) {
        int64_t ax{0};
        if (expandingy.contains(y)) {
            ay += expand_by;
        }
        for (int64_t x=0; x<data.front().size(); ++x) {
            if (expandingx.contains(x)) {
                ax += expand_by;
            }

            if (data[y][x] != 0) {
                galaxies.push_back({y + ay, x + ax});
            }
        }
    }

    int64_t result{0};
    for (int i=0; i+1<galaxies.size(); ++i) {
    for (int j=i+1; j<galaxies.size(); ++j) {
        result += mhd(galaxies[i], galaxies[j]);
    }}

    return result;
}

int main() {
    const auto& testi = parse_input(test);
    const auto& cini = parse_input(cin);

    cout << solution1(testi, 99) << endl;
    cout << solution1(cini, 999999) << endl;
}
