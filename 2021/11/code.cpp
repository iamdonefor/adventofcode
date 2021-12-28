#include "all.h"
using namespace std;

using TInput = vector<vector<int>>;

vector<pair<int, int>> neis(const TInput& g, const pair<int, int>& p) {
    vector<pair<int, int>> result;
    vector<int> helper{-1, 1, -1, -1, 0, 1, 1, 0, -1};

    for (int i=0; i+1<helper.size(); ++i) {
        auto dy = p.first + helper[i];
        auto dx = p.second + helper[i+1];

        if (dy < 0 || dy >= g.size() || dx < 0 || dx >= g.back().size()) {
            continue;
        }

        result.push_back({dy, dx});
    }

    return result;
}

enum class EMode {
    STEPS,
    SYNCRO
};

int simulate(TInput g, int steps_to_simulate, EMode mode = EMode::STEPS) {
    int Y = g.size();
    int X = g.back().size();
    const int BOOM{9};
    int total_flashes{0};

    for (int step = 0; step < steps_to_simulate || mode == EMode::SYNCRO; ++step) {
        deque<pair<int, int>> ready_to_flash;
        set<pair<int, int>> flushed;

        for (int y=0; y < Y; ++y) {
        for (int x=0; x < X; ++x) {
            if (++g[y][x] > BOOM) {
                ready_to_flash.push_back({y, x});
                flushed.insert({y, x});
                ++total_flashes;
            }
        }}

        while (!ready_to_flash.empty()) {
            int current = ready_to_flash.size();
            for (int i=0; i<current; ++i) {
                auto flasher = ready_to_flash.front();
                ready_to_flash.pop_front();

                for (const auto& nei : neis(g, flasher)) {
                    if (flushed.count(nei)) {
                        continue;
                    }
                    if (++g[nei.first][nei.second] > BOOM) {
                        ready_to_flash.push_back(nei);
                        flushed.insert(nei);
                        ++total_flashes;
                    }
                }
            }
        }

        if (flushed.size() == X*Y && mode == EMode::SYNCRO) { // big boom balaboom
            return step + 1;
        }
        for (const auto& flasher : flushed) {
            g[flasher.first][flasher.second] = 0;
        }
        
#ifdef NOTDEFINED
        {
            const string bright("\033[1;33m");
            const string reset("\033[0m");

            cout << endl << "after step: " << step << endl;
            for (int y=0; y < Y; ++y) {
            for (int x=0; x < X; ++x) {
                cout << (g[y][x] == 0 ? bright : "") << g[y][x]
                 << (g[y][x] == 0 ? reset : "");
            }
            cout << endl;
            }
        }
#endif
    }

    return total_flashes;
}

TInput parse_input(istream& in) {
    TInput result;

    string s;
    while(getline(in, s)) {
        result.emplace_back();
        transform(s.begin(), s.end(), back_inserter(result.back()),
            [](auto c) { return c - '0'; }
        );
    }

    return result;
}

bool verify() {
    stringstream input_stream{R"===(5483143223
2745854711
5264556173
6141336146
6357385478
4167524645
2176841721
6882881134
4846848554
5283751526)==="};

    auto input = parse_input(input_stream);
    return 1656 == simulate(input, 100) && 195 == simulate(input, 0, EMode::SYNCRO);
}

int main() {
    assert(verify());

    auto input = parse_input(cin);
    cout << "part1: " << simulate(input, 100) << endl;
    cout << "part2: " << simulate(input, 100, EMode::SYNCRO) << endl;
}
