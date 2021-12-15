#include "all.h"
using namespace std;

int solve(const vector<vector<int>>& g) {
    int X = g.front().size();
    int Y = g.size();
    const int max_weight = numeric_limits<int>::max();
    const vector<int> helper{0, -1, 0, 1, 0};

    vector<vector<int>> w(Y, vector<int>(X, max_weight));

    w[0][0] = 0;
    deque<pair<int, int>> q;
    q.push_back({0, 0});

    while (!q.empty()) {
        int curr = q.size();

        for (int i=0; i<curr; ++i) {
            auto [y, x] = q.front();
            q.pop_front();

            for (int i=1; i < helper.size(); ++i) {
                auto dy = helper[i] + y;
                auto dx = helper[i-1] + x;
                if (dy < 0 || dy >= Y || dx < 0 || dx >= X) { continue; }
                if (w[dy][dx] <= w[y][x] + g[y][x]) {
                    continue;
                }

                w[dy][dx] = w[y][x] + g[y][x];
                q.push_back({dy, dx});
            }
        }
    }

    // algoritm does not count the last cell and counts the first one
    return w[Y-1][X-1] + g[Y-1][X-1] - g[0][0];
}

vector<vector<int>> x5(const vector<vector<int>>& g) {
    int X = g.front().size();
    int Y = g.size();

    vector<vector<int>> result(Y*5, vector<int>(X*5));

    for (int yi = 0; yi < 5; ++yi) {
    for (int xi = 0; xi < 5; ++xi) {
        auto mult = yi + xi;
        for (int y = 0; y < Y; ++y) {
        for (int x = 0; x < Y; ++x) {
            auto value = g[y][x] + mult;
            result[Y*yi + y][X*xi + x] = value > 9 ? value - 9 : value;
        }}
    }}

    return result;
}

vector<vector<int>> parse_input(istream& in) {
    vector<vector<int>> result;

    for (string s; getline(in, s);) {
        result.emplace_back();
        for (const auto c : s) {
            result.back().push_back(c - '0');
        }
    }

    return result;
}

bool verify() {
    stringstream input_stream{R"===(1163751742
1381373672
2136511328
3694931569
7463417111
1319128137
1359912421
3125421639
1293138521
2311944581)==="};

    auto graph = parse_input(input_stream);
    auto result1 = solve(graph);

    auto graph2 = x5(graph);
    auto result2 = solve(graph2);

    return 40 == result1 && 315 == result2;
}

int main() {
    assert(verify());

    auto g = parse_input(cin);
    cout << "part1: " << solve(g) << endl;

    auto g2 = x5(g);
    cout << "part2: " << solve(g2) << endl;
}
