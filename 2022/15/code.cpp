#include "all.h"

using namespace std;
using namespace advent;

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
        const auto width = range_ - abs(y - self_[0]);

        if (width < 0) { return nullopt; }

        return trange(self_[1] - width, self_[1] + width);
    }

    tcoords beacon() const { return beacon_; }

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
using tranges = vector<trange>;

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

tranges find_all_invalid(const tscanners& scanners, int64_t y) {
    tranges ranges;

    for (const auto& scanner : scanners) {
        if (const auto range = scanner.get_at_distance(y)) {
            // cout << scanner << " " << (*range)[0] << ", " << (*range)[1] << endl;
            ranges.push_back(*range);
        }
    }

    return ranges;
}

tranges collapse_ranges(tranges ranges) {
    sort(ranges.begin(), ranges.end(), [](const auto& l, const auto& r) {
        if (l[0] < r[0]) { return true; }
        if (l[0] > r[0]) { return false; }
        return l.size() > r.size();
    });

    auto lit = ranges.begin();
    for (auto cit = next(ranges.begin()) ; cit != ranges.end(); ++cit) {
        if (lit->intersects(*cit) || lit->is_adj(*cit)) {
            *lit = lit->combine(*cit);
        } else {
            *(++lit) = *cit;
        }
    }
    ranges.erase(next(lit), ranges.end());
    return ranges;
}

int64_t find_beacons_in_ranges(const tscanners& scanners, const tranges& ranges, int64_t y) {
    set<tcoords> beacons;
    for (const auto& scanner : scanners) {
        beacons.insert(scanner.beacon());
    }
    return accumulate(beacons.begin(), beacons.end(), 0ll,
        [y, &ranges](const auto prev, const auto& beacon) {
            if (beacon[0] != y) {
                return prev;
            }

            for (const auto& r : ranges) {
                if (r.contains(beacon[1])) {
                    return prev + 1;
                }
            }

            return prev;
        }
    );
}

int64_t solve(const tscanners& scanners, int64_t y) {
    auto ranges = find_all_invalid(scanners, y);

    if (ranges.size() == 1) {
        return ranges.front().size();
    }

    ranges = collapse_ranges(move(ranges));
    const auto correction = find_beacons_in_ranges(scanners, ranges, y);

    return -correction + accumulate(ranges.begin(), ranges.end(), 0ll,
        [](auto prev, const auto& cur) {
            return prev + cur.size();
    });
}

int64_t solve2(const tscanners& scanners, int64_t from, int64_t to) {
    for (int64_t i=from; i <= to; ++i) {
        const auto r = collapse_ranges(find_all_invalid(scanners, i));
        const auto it = find_if(r.begin(), r.end(), [to](const auto& rr) {
            return rr.contains(trange(0, to));
        });

        if (it == r.end()) {
            assert(r.size() == 2);
            assert(r[0][1] + 2 == r[1][0]);
            return (r[0][1] + 1) * 4000000 + i;
        }
    }
    return -1;
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
    const auto test_input = parse_input(test);
    const auto prod_input = parse_input(fstream("input"));

    cout << solve(test_input, 10) << endl;
    cout << solve(prod_input, 2000000) << endl;

    cout << solve2(test_input, 0, 20) << endl;
    cout << solve2(prod_input, 0, 4000000) << endl;
}
