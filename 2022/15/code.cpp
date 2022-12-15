#include "all.h"

using namespace std;
using namespace advent;

using trange = array<int64_t, 2>;
using tcoords = array<int64_t, 2>;

class tscaner {
public:
    static int64_t mhd(const tcoords& l, const tcoords& r) {
        return abs(l[0] - r[0]) + abs(l[1] - r[1]);
    }

    tscaner(tcoords self, tcoords beacon)
    : self_(self)
    , beacon_(beacon)
    , range_(mhd(self, beacon))
    {}

    optional<trange> get_at_distance(int64_t y) const {
        const auto width = range_ - (self_[0] - y);

        if (width < 0) { return nullopt; }

        return trange{self_[1] - width, self_[1] + width};
    }

    friend ostream& operator<< (ostream& os, const tscaner& s) {
        os << "[" << s.self_[0] << ", " << s.self_[1] << "](" << s.range_ << ")";
        return os;
    }
private:
    tcoords self_;
    tcoords beacon_;
    int64_t range_;
};

using tinput = vector<tscaner>;
using tscanners = tinput;

tinput parse_input(istream&& is) {
    tinput result;

    for (string s; getline(is, s); ) {
        if (s.empty()) { continue; }

        tcoords self, beacon;
        if (sscanf(
                s.data()
                , "Sensor at x=%ld, y=%ld: closest beacon is at x=%ld, y=%ld"
                , &self[1]
                , &self[0]
                , &beacon[1]
                , &beacon[0]
            ) == 4) {
            result.emplace_back(self, beacon);
        } else {
            throw runtime_error("invalid data");
        }
    }

    return result;
}

tinput parse_input(istream& is) {
    return parse_input(move(is));
}

vector<trange> find_all_invalid(const tscanners& scanners, int64_t y) {
    vector<trange> ranges;

    for (const auto& scanner : scanners) {
        if (const auto range = scanner.get_at_distance(y)) {
            cout << scanner << " " << (*range)[0] << ", " << (*range)[1] << endl;
            ranges.push_back(*range);
        }
    }

    return ranges;
}

stringstream test{R"(Sensor at x=2, y=18: closest beacon is at x=-2, y=15
Sensor at x=9, y=16: closest beacon is at x=10, y=16
Sensor at x=13, y=2: closest beacon is at x=15, y=3
Sensor at x=12, y=14: closest beacon is at x=10, y=16
Sensor at x=10, y=20: closest beacon is at x=10, y=16
Sensor at x=14, y=17: closest beacon is at x=10, y=16
Sensor at x=8, y=7: closest beacon is at x=2, y=10
Sensor at x=2, y=0: closest beacon is at x=2, y=10
Sensor at x=0, y=11: closest beacon is at x=2, y=10
Sensor at x=20, y=14: closest beacon is at x=25, y=17
Sensor at x=17, y=20: closest beacon is at x=21, y=22
Sensor at x=16, y=7: closest beacon is at x=15, y=3
Sensor at x=14, y=3: closest beacon is at x=15, y=3
Sensor at x=20, y=1: closest beacon is at x=15, y=3
)"};

int main() {
    const auto ss = parse_input(test);
    const auto r = find_all_invalid(ss, 10);
    cout << parse_input(test).size() << endl;
    // cout << parse_input(fstream("input")).size() << endl;
}
