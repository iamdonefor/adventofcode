#include "all.h"
#include <cstring> // strcmp oh my
#include "test.h"
using namespace std;

struct tcube {
    array<int64_t, 2> x{0, 0};
    array<int64_t, 2> y{0, 0};
    array<int64_t, 2> z{0, 0};
    int sign{1};

    tcube() = default;
    tcube(int x0, int x1, int y0, int y1, int z0, int z1) {
        x[0] = x0; x[1] = x1;
        y[0] = y0; y[1] = y1;
        z[0] = z0; z[1] = z1;
    }

    int64_t size() const {
        return (x[1] - x[0] + 1) * (y[1] - y[0] + 1) * (z[1] - z[0] + 1) * sign;
    }
};

vector<pair<int, tcube>> parse_input(istream& in) {
    int x1, x2, y1, y2, z1, z2;
    char onoff[4];

    vector<pair<int, tcube>> result;

    for (string s; getline(in, s);) {
        if (sscanf(s.data(), "%s x=%d..%d,y=%d..%d,z=%d..%d",
            onoff, &x1, &x2, &y1, &y2, &z1, &z2) == 7) {
                result.push_back(make_pair(
                    (strcmp(onoff, "on") ? 0 : 1),
                    tcube{x1, x2, y1, y2, z1, z2}
                ));
            }
    }

    return result;
}

// long and naive
int part1(vector<pair<int, tcube>> data) {
    set<array<int, 3>> ons;
    const int THRESH{50};

    for (const auto [on, cube] : data) {
        for (int x = cube.x[0]; x <= cube.x[1] && x <= THRESH; ++x) {
            if (x < -50) { x = -50; continue; }
        for (int y = cube.y[0]; y <= cube.y[1] && y <= THRESH; ++y) {
            if (y < -50) { y = -50; continue; }
        for (int z = cube.z[0]; z <= cube.z[1] && z <= THRESH; ++z) {
            if (z < -50) { z = -50; continue; }

            if (on) {
                ons.insert({x, y, z});
            } else {
                ons.erase({x, y, z});
            }
        }}}
    }

    return ons.size();
}


optional<tcube> intersect(const tcube& l, const tcube& r) {
    if (l.x[0] > r.x[1] || l.x[1] < r.x[0]
     || l.y[0] > r.y[1] || l.y[1] < r.y[0]
     || l.z[0] > r.z[1] || l.z[1] < r.z[0]
    ) {
        return nullopt;
    }

    tcube cube;
    cube.x[0] = max(l.x[0], r.x[0]);
    cube.x[1] = min(l.x[1], r.x[1]);
    cube.y[0] = max(l.y[0], r.y[0]);
    cube.y[1] = min(l.y[1], r.y[1]);
    cube.z[0] = max(l.z[0], r.z[0]);
    cube.z[1] = min(l.z[1], r.z[1]);

    return cube;
}

// used the nice idea from reddit:
// https://www.reddit.com/r/adventofcode/comments/rlxhmg/2021_day_22_solutions/hpizza8/
// instead of splitting the cube to 6 prizmas on intersection just count the sum of subcubes
// this algorithm can grow exponentially, but lick for me it does not
uint64_t part2(vector<pair<int, tcube>> data) {
    vector<tcube> cubes; // 47475 max size, run time ~0.08s

    for (const auto& [on, cube] : data) {
        vector<tcube> cc;
        for (const auto& prev : cubes) {
            auto common = intersect(prev, cube);
            if (common) { // we have an intersection, so lets nullify all previous cubes
                common->sign = -prev.sign;
                cc.push_back(*common);
            }
        }
        if (on) {
            cubes.push_back(cube);
        }
        cubes.insert(cubes.end(), make_move_iterator(cc.begin()), make_move_iterator(cc.end()));
    }

    return accumulate(cubes.begin(), cubes.end(), 0LL,
        [](int64_t sum, const tcube& cube) { return sum += cube.size(); }
    );
}

void verify() {
    stringstream test{NTest::part2};
    assert(2758514936282235 == part2(parse_input(test)));
}

int main() {
    verify();
    auto data = parse_input(cin);
    cout << part1(data) << endl;
    cout << part2(data) << endl;
}
