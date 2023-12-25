#include "all.h"

using namespace std;
using namespace advent;

using tinput = vector<vector<char>>;

enum {
    EMPTY = '.',
    STOP = '#',
    MOVE = 'O',
};

tinput parse_input(istream&& is) {
    tinput result;

    for (string s; getline(is, s); ) {
        if (s.empty()) { continue; }
        result.push_back({});
        for (const auto c : s) {
            switch (c) {
                case '.':
                    result.back().push_back(EMPTY);
                    break;
                case 'O':
                    result.back().push_back(MOVE);
                    break;
                case '#':
                    result.back().push_back(STOP);
                    break;
                default:
                    throw runtime_error("invalid char in input");
            }
        }
    }

    return result;
}

tinput tilt_north(const tinput& mirrors) {
    const auto X = mirrors.back().size();
    const auto Y = mirrors.size();

    auto result = mirrors;

    for (int x=0; x<X; ++x) {
        int stop = -1;
        for (int y=0; y<Y; ++y) {
            if (result[y][x] == STOP) {
                stop = y;
                continue;
            }
            if (result[y][x] == MOVE) {
                result[y][x] = EMPTY;
                result[stop+1][x] = MOVE;
                ++stop;
                continue;
            }
        }
    }
    return result;
}

tinput tilt_south(const tinput& mirrors) {
    const auto X = mirrors.back().size();
    const auto Y = mirrors.size();

    auto result = mirrors;

    for (int x=0; x<X; ++x) {
        int stop = Y;
        for (int y=Y-1; y>=0; --y) {
            if (result[y][x] == STOP) {
                stop = y;
                continue;
            }
            if (result[y][x] == MOVE) {
                result[y][x] = EMPTY;
                result[stop-1][x] = MOVE;
                --stop;
                continue;
            }
        }
    }
    return result;
}

tinput tilt_west(const tinput& mirrors) {
    const auto X = mirrors.back().size();
    const auto Y = mirrors.size();

    auto result = mirrors;

    for (int y=0; y<Y; ++y) {
        int stop = -1;
        for (int x=0; x<X; ++x) {
            if (result[y][x] == STOP) {
                stop = x;
                continue;
            }
            if (result[y][x] == MOVE) {
                result[y][x] = EMPTY;
                result[y][stop+1] = MOVE;
                ++stop;
                continue;
            }
        }
    }
    return result;
}

tinput tilt_east(const tinput& mirrors) {
    const auto X = mirrors.back().size();
    const auto Y = mirrors.size();

    auto result = mirrors;

    for (int y=Y-1; y>=0; --y) {
        int stop = X;
        for (int x=X-1; x>=0; --x) {
            if (result[y][x] == STOP) {
                stop = x;
                continue;
            }
            if (result[y][x] == MOVE) {
                result[y][x] = EMPTY;
                result[y][stop-1] = MOVE;
                --stop;
                continue;
            }
        }
    }
    return result;
}

tinput cycle(const tinput& mirrors) {
    return tilt_east(
        tilt_south(
        tilt_west(
        tilt_north(
            mirrors
        ))));
}

int64_t weight(const tinput& data) {
    int64_t result{0};

    for (int x=0; x<data.front().size(); ++x) {
    for (int y=0; y<data.size(); ++y) {
        if (data[y][x] == MOVE) {
            result += (data.size() - y);
        }
    }}

    return result;
}

tinput parse_input(istream& is) {
    return parse_input(move(is));
}

stringstream test{R"(O....#....
O.OO#....#
.....##...
OO.#O....O
.O.....O#.
O.#..O.#.#
..O..#O..O
.......O..
#....###..
#OO..#....
)"};

int64_t solution1(const tinput& data) {
    return(weight(tilt_north(data)));
}

uint64_t hashsh(const tinput& i) {
    string s;
    for (const auto v : i) {
        string ss{v.begin(), v.end()};
        s += ss;
    }

    hash<string> hasher;
    return hasher(s);
}

int64_t solution2(const tinput& data) {
    auto x = data;
    vector<int64_t> first_seen;
    vector<int64_t> weights;

    int cycle_start;
    int cycle_length;

    for (int i=0;;++i) {
        const auto h = hashsh(x);
        if (auto it = find(first_seen.begin(), first_seen.end(), h); it != first_seen.end()) {
            cycle_start = (it - first_seen.begin());
            cycle_length = i - cycle_start;
            break;
        }
        first_seen.push_back(h);
        weights.push_back(weight(x));
        x = cycle(x);
    }

    
    int64_t cycles = 1000000000;
    const auto pos_at_finish = (cycles - cycle_start) % cycle_length;

    cout << cycle_start << " " << cycle_length << " " << pos_at_finish << endl;
    return weights[pos_at_finish + cycle_start];

    return 0;
}

int main() {
    const auto& testi = parse_input(test);
    cout << solution1(testi) << endl;

    const auto& cini = parse_input(cin);
    cout << solution1(cini) << endl;

    cout << solution2(testi) << endl;
    cout << solution2(cini) << endl;
}
