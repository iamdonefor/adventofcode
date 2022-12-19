#include "all.h"

using namespace std;
using namespace advent;

using tinput = vector<int>;
using twind = vector<int>;

const static int WDTH{7};

namespace nwind {
    enum {
        L = -1,
        R = 1,
    };
};

struct tfigure {
    char name;
    int height;
    int width;
    vector<tcoords> shape;
    array<int, 4> bs; // bit shape, botom - top
};

class tfiguregen {
public:
    const tfigure& next() const {
        return figures[current++ % figures.size()];
    }

private:
    const vector<tfigure> figures{
        { '-', 1, 4, { {0, 0}, {0, 1}, {0, 2}, {0, 3} }, { 30, } },
        { '+', 3, 3, { {0, 1}, {1, 0}, {1, 1}, {1, 2}, {2, 2} }, { 8, 28, 8, } },
        { 'L', 3, 3, { {0, 2}, {1, 2}, {2, 0}, {2, 1}, {2, 2} }, { 28, 4, 4, } },
        { '|', 4, 1, { {0, 0}, {1, 0}, {2, 0}, {3, 0}, }, { 16, 16, 16, 16, } },
        { '@', 2, 2, { {0, 0}, {1, 0}, {0, 1}, {1, 1}, }, { 24, 24, } },
    };

    mutable int current{0};
};

class tchamber {
public:
    tchamber(const twind& wind_)
    : wind(wind_)
    , wind_ptr(wind.begin())
    {
        state.reserve(300000*bsize);
        state.push_back(-1);
    }

    int64_t run(int64_t steps) {
        for (int i=0; i + 1 < steps; ++i) {
            fall();
        }
        return fall();
    }

    int64_t top() const { return top_; }
    int64_t max_fall() const { return max_fall_; }

    friend ostream& operator<<(ostream& os, const tchamber& c) {
        for (auto it = c.state.rbegin(); it != c.state.rend(); ++it) {
            for (int i=0; i<WDTH; ++i) {
                cout << (((1 << (WDTH - 1 - i)) & *it) ? '#' : '.');
            }
            cout << endl;
        }
        return os;
    }
private:
    int64_t fall() {
        const auto& fig = gen.next();

        // prepare state
        auto rows_to_add = (top_ - c_ + 4 + fig.height) - static_cast<int64_t>(state.size());
        for (int i=0; i<rows_to_add; ++i) {
            state.push_back(0);
        }

        // cout << "top: " << top() << " " << ", rows_to_add: " << rows_to_add << endl;

        // fall the figure
        auto y = top_ + 4 - c_;
        auto bs = fig.bs;

        const auto can_fall = [&](auto& bs) {
            for (int i=0; i<fig.height; ++i) {
                if ((state[y + i - 1] & bs[i]) != 0) {
                    return false;
                }
            }
            return true;
        };

        auto apply_wind = [&](auto& bs) mutable {
            for (int i=0; i<fig.height; ++i) {
                if (*wind_ptr == nwind::L) {
                    if (bs[i] & (1 << (WDTH - 1))) {
                        return;
                    }
                    if (state[y + i] & (bs[i] << 1)) {
                        return;
                    }
                }
                if (*wind_ptr == nwind::R) {
                    if (bs[i] & 1) {
                        return;
                    }
                    if (state[y + i] & (bs[i] >> 1)) {
                        return;
                    }
                }
            }

            // cout << "applied: " << *wind_ptr << endl;
            for (int i=0; i<fig.height; ++i) {
                bs[i] = (*wind_ptr == nwind::L) ? (bs[i] << 1) : (bs[i] >> 1);
            }
        };

        for(;; --y) {
            // apply the wind
            apply_wind(bs);
            wind_ptr = next(wind_ptr) == wind.end() ? wind.begin() : next(wind_ptr);

            // try to fall
            if (!can_fall(bs)) {
                break;
            }

            // if (top_ - c_ > 2*bsize) {
            //     copy(state.begin() + bsize, state.end(), state.begin());
            //     fill(state.begin() + top_ - c_ - bsize + 1, state.end(), 0);
            //     c_ += bsize;
            // }
        }

        max_fall_ = max(max_fall_, top_ + 4 - y);

        top_ = max(top_, y + c_ + fig.height - 1);
        for (int i=0; i<fig.height; ++i) {
            state[y + i] |= bs[i];
        }

        return top_;
    }

    const twind& wind;
    twind::const_iterator wind_ptr;
    tfiguregen gen;
    vector<int> state;
    int64_t top_{0};
    int64_t max_fall_{0};
    int64_t c_{0}; // correction
    const static size_t bsize{100};
};

pair<int, int> search_for_cycle(const auto& wind) {
    cout << wind.size() << endl;

    for (int seed = 0; seed < 1000; ++seed) {
        cout << seed << endl;
    for (int cycle = 1000; cycle < 2000; ++cycle) {
        tchamber chamber(wind);
        const auto seed_top = chamber.run(seed);
        const auto top1 = chamber.run(cycle);
        const auto top2 = chamber.run(cycle);
        const auto top3 = chamber.run(cycle);
        const auto top10 = chamber.run(cycle * 7);
        if ( ((top1 - seed_top) * 2 + seed_top == top2)
            && ((top1 - seed_top) * 3 + seed_top == top3)
            && (top10 == (top1 - seed_top) * 10 + seed_top)) {
            cout << "cycle: " << seed << " + X * " << cycle << endl;

            const auto top100 = chamber.run(cycle * 90);
            if (top100 == 100 * (top1 - seed_top) + seed_top) {
                cout << "cycle: " << seed << " + X * " << cycle << endl;
                return {seed, cycle};
            }
        }
    }}
}

tinput parse_input(istream&& is) {
    tinput result;

    for (string s; getline(is, s); ) {
        if (s.empty()) { continue; }
        for (const auto c : s) {
            result.push_back(c == '>' ? nwind::R : nwind::L);
        }
    }

    return result;
}

tinput parse_input(istream& is) {
    return parse_input(move(is));
}

stringstream test{R"(
>>><<><>><<<>><>>><<<>>><<<><<<>><>><<>>
)"};

int main() {
    int64_t Q{1000000000000};
    {
        const auto wind = parse_input(test);
        tchamber chamber(wind);
        // cout << chamber.run(2022) << endl; // part1
        // const auto [s, v] = search_for_cycle(wind); // 15, 1015
        int64_t s = 15;
        int64_t v = 1015;
        cout << s << ", " << v << endl;

        auto seed_height = chamber.run(s);
        auto next_height = chamber.run(v);
        auto cycle_height = chamber.run(v) - next_height;
        auto leftover_height = chamber.run((Q - s) % v) - 2 * cycle_height + seed_height;
        cout << seed_height << " " << cycle_height << " " << leftover_height << endl;

        int64_t total_height = seed_height + ((Q - s) / v) * cycle_height + leftover_height;
        cout << total_height << endl;
    }
    {
        const auto wind = parse_input(fstream("input"));
        tchamber chamber(wind);
        // const auto s = chamber.run(201);
        // const auto c = chamber.run(1740) - s;
        int64_t s = 201;
        int64_t v = 1740;
        cout << s << ", " << v << endl;

        auto seed_height = chamber.run(s);
        auto next_height = chamber.run(v);
        auto cycle_height = chamber.run(v) - next_height;
        auto leftover_height = chamber.run((Q - s) % v) - 2 * cycle_height + seed_height;
        cout << seed_height << " " << cycle_height << " " << leftover_height << endl;

        int64_t total_height = seed_height + ((Q - s) / v) * cycle_height + leftover_height;
        cout << total_height << endl;
        // search_for_cycle(wind);
    }
}
