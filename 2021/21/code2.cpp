#include "all.h"
using namespace std;

const map<int, int64_t> splits{
    {3, 1ULL}, {4, 3ULL}, {5, 6ULL}, {6, 7ULL}, {7, 6ULL}, {8, 3ULL}, {9, 1ULL},
};

int pos(int p, int d) {
    p += (d % 10);
    if (p > 10) p-=10;
    return p;
}

struct tstate {
    vector<uint64_t> p1;
    vector<uint64_t> p2;
    tstate()
    : p1(11, 0)
    , p2(11, 0)
    {}
};

const static int SCORE{21};
using tgame = vector<vector<tstate>>;

string to_string(const tgame& g) {
    stringstream builder;
    for (int y=0; y<SCORE; ++y) {
    for (int x=0; x<SCORE; ++x) {
        auto& s = g[y][x];
        for (int i=1; i<11; ++i) {
            if (s.p1[i] > 0) { builder << y << " / " << x << " p1 state: " << i << " " << s.p1[i] << endl; }
            // if (s.p2[i] > 0) { builder << y << " / " << x << " p2 state: " << i << " " << s.p2[i] << endl; }
        }
    }}
    return builder.str();
}

uint64_t total_uvs(const tgame& g, bool first) {
    uint64_t result{0};

    for (int y=0; y<SCORE; ++y) {
    for (int x=0; x<SCORE; ++x) {
        const auto& p = (first ? g[y][x].p1 : g[y][x].p2);

        for (int i=1; i<11; ++i) {
            result += p[i];
        }
    }}

    return result;
}

uint64_t first_player_wins{0};
uint64_t second_player_wins{0};

tgame make_move(const tgame& g, bool first) {
    tgame ng(SCORE, vector<tstate>(SCORE, tstate()));

    uint64_t tu{0};
    uint64_t to{0};

    for (int y=0; y<SCORE; ++y) {
    for (int x=0; x<SCORE; ++x) {
        const auto& s = g[y][x];
        const auto& active = (first ? s.p1 : s.p2);
        const auto& other = (first ? s.p2 : s.p1);

        auto& active_score = (first ? y : x);
        auto& other_score = (first ? x : y);

        uint64_t n_other = accumulate(other.begin(), other.end(), 0ULL);

        if (n_other == 0) { continue; }

        to += n_other;

        bool once = true;
        for (int i=1; i<11; ++i) {
            if (active[i] == 0) { continue; }
            // we've got state, now split the universes
            // y - first player points
            // x - second player points
            // i - position
            // active[i] - total universies up to this position
            // n_other - previos player turn multiplier

            for (const auto& [dice, multiplier] : splits) {
                int new_pos = pos(i, dice);
                assert(new_pos > 0 && new_pos < 11);

                int new_score = active_score + new_pos;
                uint64_t new_universies = n_other * multiplier;

                tu += new_universies;

                if (new_score >= 21) {
                    if (first) {
                        first_player_wins += new_universies;
                    } else {
                        second_player_wins += new_universies;
                    }
                } else {
                    if (first) {
                        ng[new_score][other_score].p1[new_pos] += new_universies;
                        ng[new_score][other_score].p2 = other;
                    } else {
                        ng[other_score][new_score].p2[new_pos] += new_universies;
                        ng[other_score][new_score].p1 = other;
                    }
                }
            }
        }
    }}


    cout << "tu: " << tu << ", to: " << to << ", " << "1: " << first_player_wins << ", 2: " << second_player_wins << " " << (first ? "*" : "") <<  endl;
    // assert(tu == to * 27);

    return ng;
}

int m() {
    tgame g(SCORE, vector<tstate>(SCORE, tstate{}));

    g[0][0].p1[4] = 1;
    g[0][0].p2[8] = 1;

    for (int i=0; i<2; ++i) {
        g = make_move(g, true);
        cout << total_uvs(g, true) << " " << total_uvs(g, false) << endl;

        g = make_move(g, false);
        cout << total_uvs(g, true) << " " << total_uvs(g, false) << endl;
    }

    // g = make_move(g, false);
    // cout << total_uvs(g, false) << endl;
    // g = make_move(g, true);
    // cout << total_uvs(g, true) << endl;

    //     g = make_move(g, false);
    //     g = make_move(g, true);
    // }
}

// taken from reddit
using State = std::tuple<std::array<int, 2>, std::array<int, 2>>; // pos1, pos2, s1, s2
void part2()
{
    int start1 = 8, start2 = 5;
    // int r = std::scanf("Player 1 starting position: %d\nPlayer 2 starting position: %d", &start1, &start2);

    std::map<State, uint64_t> uCount;
    uCount[{{start1 - 1, start2 - 1}, {0, 0}}] = 1;

    uint64_t wins[2] = {0, 0};
    for (int pt = 0; !uCount.empty(); pt = (pt + 1) % 2)
    {
        cout << "XXX: " << uCount.size() << " " << wins[0] << " " << wins[1] << endl;

        std::map<State, uint64_t> nextCount;
        for (auto const &[state, universes] : uCount) {
            for (const auto& [dice, mult] : splits) {
                auto [pos, sco] = state;
                pos[pt] += dice;
                pos[pt] = (pos[pt] % 10) + 1;
                sco[pt] += pos[pt];
                if (sco[pt] >= 21)
                    wins[pt] += universes * mult;
                else
                    nextCount[{pos, sco}] += universes * mult;
            }
        }
        uCount = nextCount;
    }
    std::cout << std::max(wins[0], wins[1]) << std::endl;
}

int main() {
    part2();
}
