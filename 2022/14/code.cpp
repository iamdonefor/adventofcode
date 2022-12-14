#include "all.h"

using namespace std;
using namespace advent;

enum {
    AIR = 0,
    STONE = 1,
    SAND = 2,
};

using tcoords = array<int, 2>;
struct tcave {
    vector<vector<int>> c;
    int Y;
    int X;
    int Ymin;
    int Xmin;

    void set(const tcoords& co, int v) {
        c[co[0]][co[1]] = v;
    }

    friend ostream& operator<< (ostream& os, const tcave& c) {
        return c.print(os, {0, c.X  +10}, {c.Y + 2, c.Xmin - 5});
    }

    ostream& print(ostream& os, tcoords tr, tcoords bl) const {
        for (int y = tr[0]; y <= bl[0]; ++y) {
            for (int x = bl[1]; x <= tr[1]; ++x) {
                os << (c[y][x] == AIR ? ' '
                :  c[y][x] == STONE ? '$'
                :  c[y][x] == SAND ? ':'
                : '?');
            }
            cout << endl;
        }
        return os;
    }
};

using tinput = tcave;

tinput parse_input(istream& is) {
    vector<vector<tcoords>> lines;

    const auto stol = [](string_view s) {
        int n{0};
        for (const auto c : s) {
            assert(isdigit(c));
            n = n * 10 + c - '0';
        }
        return n;
    };

    int X = 0;
    int Y = 0;
    int Xmin = numeric_limits<int>::max();
    int Ymin = numeric_limits<int>::max();

    for (string s; getline(is, s); ) {
        if (s.empty()) { continue; }

        lines.push_back({});
        // 432,82 -> 450,82 -> 450,81
        for (const auto cs : split(s, " -> ")) {
            const auto xy = split(cs, ",");

            tcoords c{
                stol(xy[1]),
                stol(xy[0]),
            };

            Y = max(Y, c[0]);
            Ymin = min(Ymin, c[0]);
            X = max(X, c[1]);
            Xmin = min(Xmin, c[1]);

            lines.back().push_back(c);
        }
    }

    vector<vector<int>> c(Y + 1 + 2, vector<int>(2* (X+1), AIR)); // lets double x to have some space, sand pyramid cant be wider then 2 * Y. Add two to add floor
    for (const auto& line : lines) {
        for (int i=1; i<line.size(); ++i) {
            auto st = line[i-1];
            const auto stop = line[i];

            for (;;) {
                c[st[0]][st[1]] = STONE;
                if (st == stop) { break; }

                st[0] += (st[0] == stop[0]) ? 0 : sign(stop[0] - st[0]);
                st[1] += (st[1] == stop[1]) ? 0 : sign(stop[1] - st[1]);
            }
        }
    }

    return {c, Y, X, Ymin, Xmin};
}

int simulate_sand(tcave& cave, bool create_floor = false) {
    if (create_floor) {
        for (int x = 0; x < cave.X*2; ++x) {
            cave.set({cave.Y + 2, x}, STONE);
        }
    }

    auto& c = cave.c;
    const auto fall = [&](tcoords sand) {
        for (;;) {
            if ((!create_floor) && (sand[0] > cave.Y || sand[1] < cave.Xmin || sand[1] > cave.X)) {
                return optional<std::array<int, 2>>(nullopt);
            }

            if (c[sand[0] + 1][sand[1]] == AIR) {
                ++sand[0];
                continue;
            }

            if (c[sand[0] + 1][sand[1] - 1] == AIR) {
                ++sand[0];
                --sand[1];
                continue;
            }

            if (c[sand[0] + 1][sand[1] + 1] == AIR) {
                ++sand[0];
                ++sand[1];
                continue;
            }

            return optional(sand);
        }
    };

    int i=0;
    const tcoords entry{0, 500};
    while (auto sand = fall(entry)) {
        cave.set(*sand, SAND);
        ++i;
        if (sand == entry) {
            break;
        }
    }

    return i;
}

stringstream test{R"(
498,4 -> 498,6 -> 496,6
503,4 -> 502,4 -> 502,9 -> 494,9
)"};

int main() {
    // {
    //     auto cave = parse_input(test);
    //     cout << cave.Ymin << " -- " << cave.Y << " " << cave.Xmin << " -- " << cave.X << endl;
    //     cout << simulate_sand(cave, true) << endl;
    //     cout << cave << endl;
    // }

    {
        auto cave = parse_input(cin);
        cout << cave.Ymin << " -- " << cave.Y << " " << cave.Xmin << " -- " << cave.X << endl;
        cout << simulate_sand(cave, true) << endl;
        cave.print(cout, {0, 800}, {167, 300});
    }
}
