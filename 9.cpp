#include "all.h"
using namespace std;

// note to self: do no engeneeir architecture before you've seen the specification
class TMap { // completeley useless for part 2
public:
    TMap(vector<vector<int>> heights)
    : Map(move(heights))
    , Y(Map.size())
    , X(Map.back().size())
    {}

    bool isLowPoint(int y, int x) const {
        auto this_height = at(y, x);
        for (const auto& n : neis(y, x)) {
            if (this_height >= at(n)) {
                return false;
            }
        }
        return true;
    }

    vector<pair<int, int>> neis(int y, int x) const {
        vector<int> helper{0, -1, 0, 1, 0};
        vector<pair<int, int>> result;
        for (int i=0; i + 1 < helper.size(); ++i) {
            auto dy = y + helper[i];
            auto dx = x + helper[i+1];

            if (dy >= Y || dy < 0 || dx >= X || dx < 0) {
                continue;
            }

            result.push_back({dy, dx});
        }

        return result;
    }

    int at(int y, int x) const { return Map[y][x]; }
    int at(const pair<int, int>& v) const { return Map[v.first][v.second]; }

    void foreach(function<void(int, int)> f) {
        for (int y=0; y<Y; ++y) {
        for (int x=0; x<X; ++x) {
            f(y, x);
        }}
    }

private:
    vector<vector<int>> Map;
    int Y;
    int X;
};

vector<vector<int>> parse_input(istream& in) {
    vector<vector<int>> result;

    string next;
    while (getline(in, next)) {
        result.push_back({});
        for (const auto c : next) {
            result.back().push_back(c - '0');
        }
    }

    return result;
}

int part1(const vector<vector<int>>& heights) {
    TMap m(heights);

    int result = 0;
    m.foreach(
        [&](int y, int x) mutable {
            if (m.isLowPoint(y, x)) {
                result += m.at(y, x) + 1;
            }
        }
    );
    return result;
}

int part2(vector<vector<int>> h) {
    int Y  = h.size();
    int X  = h.back().size();
    vector<vector<int>> seen(Y, vector<int>(X, 0));
    vector<int> basin_sizes;
    const vector<int> helper{0, -1, 0, 1, 0};


    for (int y=0; y < Y; ++y) {
    for (int x=0; x < X; ++x) {
        if (h[y][x] == 9 || seen[y][x]) {
            continue;
        }

        // ok, we've got a new basin
        int basin_size = 0;
        { // bfs
            deque<pair<int, int>> q;
            q.push_back({y, x});
            seen[y][x] = 1;

            while (!q.empty()) {
                basin_size++;
                auto [cy,cx] = q.front();
                q.pop_front();

                for (int i=0; i + 1 < helper.size(); ++i) {
                    auto dy = cy + helper[i];
                    auto dx = cx + helper[i+1];

                    if (dy >= Y || dy < 0 || dx >= X || dx < 0) {
                        continue;
                    }

                    if (h[dy][dx] != 9 && seen[dy][dx] != 1) {
                        seen[dy][dx] = 1;
                        q.push_back({dy, dx});
                    }
                }
            }
        }

        basin_sizes.push_back(basin_size);
    }}

    const int top = 3;
    partial_sort(basin_sizes.begin(), basin_sizes.begin() + top, basin_sizes.end(), greater<int>());

    int result = 1;
    for (int i=0; i<top; ++i) {
        result *= basin_sizes[i];
    }
    return result;
}


bool verify() {
    stringstream input_stream{R"--(2199943210
3987894921
9856789892
8767896789
9899965678)--"};

    auto input = parse_input(input_stream);
    return 15 == part1(input) && 1134 == part2(input);
}


int main() {
    assert(verify());

    auto input = parse_input(cin);
    cout << "part1: " << part1(input) << endl;
    cout << "part2: " << part2(input) << endl;
}
