#include "all.h"

using namespace std;
using namespace advent;

struct trobot {
    tcoords coords;
    tcoords speed;
};

int64_t mod(int64_t what, int64_t by) {
    return (by + what % by) % by;
}

using tinput = vector<trobot>;

tinput parse_input(istream&& is) {
    tinput result;

    for (string s; getline(is, s); ) {
        if (s.empty()) { continue; }

        int64_t x, y, z, zz;
        if (sscanf(s.data(), "p=%ld,%ld v=%ld,%ld", &x, &y, &z, &zz) == 4) {
        } else {
             throw runtime_error("invalid input");
        }

        result.push_back({.coords = {y, x}, .speed = {zz, z}});
    }

    return result;
}

tinput parse_input(istream& is) {
    return parse_input(move(is));
}

stringstream test{R"(p=0,4 v=3,-3
p=6,3 v=-1,-3
p=10,3 v=-1,2
p=2,0 v=2,-1
p=0,0 v=1,3
p=3,0 v=-2,-2
p=7,6 v=-1,-3
p=3,0 v=-1,-2
p=9,3 v=2,3
p=7,3 v=-1,2
p=2,4 v=2,-3
p=9,5 v=-3,-3
)"};


void print_grid(const tinput& robots, tcoords grid_size) {
    vector<vector<int>> where(grid_size[0], vector<int>(grid_size[1], 0)); cout << endl;

    for (const auto& r : robots) {
        ++where[r.coords[0]][r.coords[1]];
    }

    for (const auto& y : where) {
    for (const auto& x : y) {
        if (x) cout << (x % 10); else cout << '.';
    }
    cout << endl;
    }
    cout << endl;
}

int64_t solution1(const tinput& data, tcoords grid_size) {
    const int64_t secs = 100;
    auto robots = data;


    // trobot test{
    //     .coords = {4, 2},
    //     .speed = {-3, 2}
    // };

    // cout << "!! " << mod(test.speed[0] * 5 + test.coords[0], grid_size[0]) << ", "
    //     << mod(test.speed[1] * 5 + test.coords[1], grid_size[1]) << endl << endl;;


    // for (int i=0; i<6; ++i) {
    //     cout << test.coords[0] << ", " << test.coords[1] << endl;
    //     test.coords[0] = mod(test.coords[0] + test.speed[0], grid_size[0]);
    //     test.coords[1] = mod(test.coords[1] + test.speed[1], grid_size[1]);
    // }

    // return 0;

    for (auto& r : robots) {
        r.coords[0] = mod(r.speed[0] * secs + r.coords[0], grid_size[0]);
        r.coords[1] = mod(r.speed[1] * secs + r.coords[1], grid_size[1]);
    }

    int64_t q1{0}, q2{0}, q3{0}, q4{0};
    const auto midy = grid_size[0] / 2;
    const auto midx = grid_size[1] / 2;

    for (const auto& r : robots) {
        if (r.coords[0] < midy && r.coords[1] < midx) ++q1;
        if (r.coords[0] < midy && r.coords[1] > midx) ++q2;
        if (r.coords[0] > midy && r.coords[1] < midx) ++q3;
        if (r.coords[0] > midy && r.coords[1] > midx) ++q4;
    }

    return q1 * q2 * q3 * q4;
}

int main() {
    const auto& testi = parse_input(test);
    cout << solution1(testi, {7, 11}) << endl;

    const auto& cini = parse_input(cin);
    cout << solution1(cini, {103, 101}) << endl;
}
