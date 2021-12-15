#include "all.h"
#include <iomanip>
#include <chrono>
using namespace std;

void print_graph(const vector<vector<int>>& g, const string& prefix = {}) {
    if (!prefix.empty()) {
        cout << prefix << endl;
    }

    int X = g.front().size();
    int Y = g.size();

    for (int y = 0; y < Y; ++y) {
    for (int x = 0; x < X; ++x) {
        cout << setw(2) << g[y][x] << " ";
    }
        cout << endl;
    }

}

const vector<pair<int, int>>& adj(const vector<vector<int>>& g, pair<int, int> v) {
    const vector<int> helper{0, -1, 0, 1, 0};
    static vector<pair<int, int>> result;

    result.resize(4);
    int new_size = 0;

    for (int i=1; i < helper.size(); ++i) {
        auto dy = helper[i] + v.first;
        auto dx = helper[i-1] + v.second;
        if (dy < 0 || dy >= g.size() || dx < 0 || dx >= g.back().size()) { continue; }

        result[new_size++] = {dy, dx};
    }

    result.resize(new_size);
    return result;
}

int dejkstra(const vector<vector<int>>& g) {
    int X = g.front().size();
    int Y = g.size();

    vector<vector<int>> w(Y, vector<int>(X, -1));

    using T = pair<int, pair<int, int>>;
    priority_queue<T, vector<T>, greater<T>> q;

    w[0][0] = 0;
    for (const auto& n : adj(g, {0, 0})) {
        q.push({g[n.first][n.second], n});
    }

    while (!q.empty()) {
        auto [weight, curr] = q.top();
        q.pop();

        if (curr.first == Y-1 && curr.second == X-1) {
            return weight;
        }
        if (w[curr.first][curr.second] != -1) {
            continue;
        }
        w[curr.first][curr.second] = weight;

        for (const auto& n : adj(g, curr)) {
            if (w[n.first][n.second] != -1) {
                continue;
            }

            q.push({weight + g[n.first][n.second], {n.first, n.second}});
        }
    }

    return 0;
}

int optimized_bfs(const vector<vector<int>>& g) {
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

            for (const auto [dy, dx] : adj(g, {y, x})) {
                if (w[dy][dx] <= w[y][x] + g[y][x]) {
                    continue;
                }

                w[dy][dx] = w[y][x] + g[y][x];
                q.push_back({dy, dx});
            }
        }
    }

    // algorithm does not count the last cell and counts the first one
    return w[Y-1][X-1] + g[Y-1][X-1] - g[0][0];
}

vector<vector<int>> x5(const vector<vector<int>>& g, int M=5) {
    int X = g.front().size();
    int Y = g.size();

    vector<vector<int>> result(Y*M, vector<int>(X*M));

    for (int yi = 0; yi < M; ++yi) {
    for (int xi = 0; xi < M; ++xi) {
        auto mult = yi + xi;
        for (int y = 0; y < Y; ++y) {
        for (int x = 0; x < X; ++x) {
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

auto solve = dejkstra;

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

    auto start = chrono::steady_clock::now();
    auto result2 = solve(graph2);
    auto end = chrono::steady_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "test elapsed time: " << elapsed.count() << endl;

    return 40 == result1 && 315 == result2;
}

int main() {
    assert(verify());

    auto g = parse_input(cin);
    cout << "part1: " << solve(g) << endl;

    auto g2 = x5(g, 5);
    vector<function<int(const vector<vector<int>>&)>> solutions{dejkstra, optimized_bfs};

    for (const auto& solution : solutions) {
        auto start = chrono::steady_clock::now();
        cout << "part2: " << solution(g2) << endl;
        auto end = chrono::steady_clock::now();
        chrono::duration<double> elapsed = end - start;
        cout << "input elapsed time: " << elapsed.count() << endl;
    }

}
