#include "all.h"

using tcoords = array<int, 2>;
using tinput = vector<pair<tcoords, int>>;

tcoords operator+ (const tcoords& l, const tcoords& r) {
    return {l[0] + r[0], l[1] + r[1]};
};

const static tcoords UP{1, 0};
const static tcoords DOWN{-1, 0};
const static tcoords LEFT{0, -1};
const static tcoords RIGHT{0, 1};
const static double SHOULDMOVE{1.5};

const static map<char, tcoords> moves = {
    {'U', UP},
    {'D', DOWN},
    {'L', LEFT},
    {'R', RIGHT},
};

double dist(const tcoords& p1, const tcoords& p2) {
    return sqrt(pow(p1[0] - p2[0], 2) + pow(p1[1] - p2[1], 2));
}

tinput parseInput(istream& is) {
    tinput result;

    for (string s; getline(is, s); ) {
        if (s.empty()) { continue; }

        char c; int d;
        if (sscanf(s.data(), "%c %d", &c, &d) == 2) {
            result.emplace_back(moves.at(c), d);
        } else {
            cout << "err: " << s << endl;
        }
    }

    cout << result.size() << " moves" << endl;
    return result;
}

bool update(const tcoords& head, tcoords& tail) {
    if (dist(head, tail) < SHOULDMOVE) {
        return false;
    }

    auto sign = []<typename T>(const T& v) {
        if (v > 0) return T{1};
        if (v < 0) return T{-1};
        return T{0};
    };

    tail[0] += sign(head[0] - tail[0]);
    tail[1] += sign(head[1] - tail[1]);
    return true;
}

int solve(const tinput& in, int knots) {
    assert(knots > 1);
    vector<tcoords> heads(knots, {0, 0});
    set<tcoords> seen;

    for (const auto& [where, steps] : in) {
        for (int i=0; i<steps; ++i) {
            heads.front() = heads.front() + where;
            for (auto knot = heads.begin(); next(knot) != heads.end(); knot = next(knot)) {
                if (!update(*knot, *next(knot))) {
                    break; // small optimization if head didnt move
                };
            }

            // cout << heads.back()[0] << ", " <<  heads.back()[1] << endl;
            seen.insert(heads.back());
        }
    }

    return seen.size();
}

stringstream test{R"(
R 4
U 4
L 3
D 1
R 4
D 1
L 5
R 2
)"};

stringstream test2{R"(
R 5
U 8
L 8
D 3
R 17
D 10
L 25
U 20
)"};

int main() {
    cout << solve(parseInput(test), 2) << endl;
    cout << solve(parseInput(test2), 10) << endl;

    const auto& in = parseInput(cin);
    cout << solve(in, 2) << endl;
    cout << solve(in, 10) << endl;
}
