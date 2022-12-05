#include "all.h"

using tassignment = pair<pair<int, int>, pair<int, int>>;
using tinput = vector<tassignment>;

tinput parseInput(istream& is) {
    tinput result;

    for (string s; getline(is, s); ) {
        int lb, le, rb, re;
        if (sscanf(s.data(), "%d-%d,%d-%d", &lb, &le, &rb, &re) == 4) {
            result.push_back(make_pair(make_pair(lb, le), make_pair(rb, re)));
        }
    }

    return result;
}

uint64_t countOverlaped(const tinput& ass) {
    uint64_t result{0};

    const auto overlaps1 = [](const tassignment& a) {
        const auto [lb,le] = a.first;
        const auto [rb,re] = a.second;

        if (lb >= rb && le <= re) {
            return true;
        }

        if (lb <= rb && le >= re) {
            return true;
        }

        return false;
    };

    const auto overlaps2 = [](const tassignment& a) {
        auto [lb,le] = a.first;
        auto [rb,re] = a.second;

        if (lb > rb) {
            swap(lb, rb);
            swap(le, re);
        }

        if (le < rb) {
            return 0;
        }

        return (min(le, re) - rb + 1);
    };

    for (const auto& as : ass) {
        // result += overlaps1(as) ? 1 : 0; // part 1
        result += overlaps2(as) ? 1 : 0; // part2
    }

    return result;
}

stringstream test{R"(
2-4,6-8
2-3,4-5
5-7,7-9
2-8,3-7
6-6,4-6
2-6,4-8
)"};

int main() {
    cout << countOverlaped(parseInput(test)) << endl;
    cout << countOverlaped(parseInput(cin)) << endl;
}
