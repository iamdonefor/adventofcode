#include "all.h"

using namespace std;
using namespace advent;

using tinput = vector<tcoords3>;

tinput parse_input(istream&& is) {
    tinput result;

    for (string s; getline(is, s); ) {
        if (s.empty()) { continue; }
        result.push_back({});
        auto&x = result.back();
        if (sscanf(s.data()
            , "%ld,%ld,%ld"
            , &x[0]
            , &x[1]
            , &x[2]) == 3) {
        } else {
             throw runtime_error("invalid input");
        }
    }

    return result;
}

tinput parse_input(istream& is) {
    return parse_input(move(is));
}

stringstream test{R"(
2,2,2
1,2,2
3,2,2
2,1,2
2,3,2
2,2,1
2,2,3
2,2,4
2,2,6
1,2,5
3,2,5
2,1,5
2,3,5
)"};

const static vector<int> nei{-1, 0, 0, 1, 0, 0, -1, 0};

auto adj(const tinput& in) {
    set<tcoords3> cubes(in.begin(), in.end());
    map<tcoords3, int64_t> result;

    for (const auto& cube : in) {
        for (auto i = 2; i < nei.size(); ++i) {
            auto dx = cube[0] + nei[i-2];
            auto dy = cube[1] + nei[i-1];
            auto dz = cube[2] + nei[i];

            tcoords3 n{dx, dy, dz};
            result[n]++;
        }
    }

    return result;
}

int64_t total_sides(const tinput& in) {
    set<tcoords3> cubes(in.begin(), in.end());
    const auto& a = adj(in);
    return accumulate(a.begin(), a.end(), 0ll, [&cubes](const auto prev, const auto& n) {
        return prev + (cubes.count(n.first) > 0 ? 0 : n.second);
    });
}

int main() {
    cout << total_sides(parse_input(test)) << endl;
    cout << total_sides(parse_input(fstream("input"))) << endl;
}
