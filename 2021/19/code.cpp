#include "all.h"
using namespace std;

#include "transform.h"
#include "scaner.h"

vector<Coords> allSigns={
    {1, 1, 1}, {1, 1, -1}, {1, -1, 1}, {1, -1, -1},
    {-1, 1, 1}, {-1, 1, -1}, {-1, -1, 1}, {-1, -1, -1},
};
vector<Coords> allAxis={
    {0, 1, 2}, {0, 2, 1}, {1, 0, 2},
    {1, 2, 0}, {2, 0, 1}, {2, 1, 0},
};

int check_transform(const Transform& tr, const Scanner& l, const Scanner& r) {
    int count{0};
    for (int i=0; i<r.size(); ++i) {
        if (auto it = find(l.begin(), l.end(), tr(r[i])); it != l.end()) {
            ++count;
        }
    }
    return count;
}

optional<Transform> intersect(const Transform& tr, const Scanner& l, const Scanner& r) {
    for (int li=0; li<l.size(); ++li) {
    for (int ri=0; ri<r.size(); ++ri) {
        // cout << "trying to match " << l[li] << " on " << r[ri] << endl;

        auto corrected_transform = tr.set_corrections(l[li] - tr(r[ri]));
        if (check_transform(corrected_transform, l, r) > 11) {
            return corrected_transform;
        }
    }}

    return nullopt;
}

optional<Transform> find_transformation(const Scanner& l, const Scanner& r) {
    for (const auto& sign : allSigns) {
    for (const auto& axis : allAxis) {
        Transform basic_transform(axis, sign);
        if (auto corrected = intersect(basic_transform, l, r); corrected) {
            return corrected;
        }
    }}

    return nullopt;
}

vector<Scanner> parse_input(istream& in) {
    // return scanners;

    vector<Scanner> result;
    for (string s; getline(in, s);) {
        if (s.find("scanner") != string::npos) {
            result.push_back({});
        }

        int x,y,z;
        if (sscanf(s.data(), "%d,%d,%d", &x, &y, &z) == 3) {
            result.back().push_back({x,y,z});
        }
    }
    return result;
}

int64_t manhattan(const Coords& l, const Coords& r) {
    int64_t result{0};
    for (int i=0; i<DIM; ++i) {
        result += abs(l[i] - r[i]);
    }
    return result;
}

int main() {
    auto input = parse_input(cin);

    vector<int> seen(input.size(), 0);
    vector<int> next(input.size(), -1);
    vector<Transform> tonext(input.size());

    deque<int> q;
    q.push_back(0);
    seen[0] = 1;

    while (!q.empty()) {
        int curr = q.size();
        for (int i=0; i<curr; ++i) {
            auto scanner_id = q.front();
            q.pop_front();

            for (int j=0; j<input.size(); ++j) {
                if (seen[j]) continue;
                auto t = find_transformation(input[scanner_id], input[j]);
                if (t) {
                    cout << scanner_id << " <- " << j << endl;
                    seen[j] = 1;
                    next[j] = scanner_id;
                    tonext[j] = *t;
                    q.push_back(j);
                }
            }
        }
    }

    set<Coords> part1;
    set<Coords> part2;
    for (int scanner_id=0; scanner_id<input.size(); ++scanner_id) {
        for (auto c : input[scanner_id]) {
            for (int curr = scanner_id; next[curr] != -1; curr = next[curr]) {
                c = tonext[curr](c);
            }

            part1.insert(c);

            auto scanner_c = tonext[scanner_id].corrections_;
            for (int curr = next[scanner_id]; curr >=0 && next[curr] != -1; curr = next[curr]) {
                scanner_c = tonext[curr](scanner_c);
            }

            part2.insert(scanner_c);
        }
    }

    cout << part1.size() << endl;

    int64_t max_manhatten{-1};
    for (const auto& l : part2) {
    for (const auto& r : part2) {
        max_manhatten = max(max_manhatten, manhattan(l, r));
    }}

    cout << max_manhatten << endl;
}
