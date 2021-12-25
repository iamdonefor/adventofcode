#include "all.h"
using namespace std;
#include "test_data.h"

enum {
    NONE = 0,
    EAST = 1,
    SOUTH = 2,
};

using tgrid = vector<vector<int>>;

tgrid from_stream(istream& in) {
    tgrid result;
    for (string s; getline(in, s);) {
        result.push_back({});
        for (const auto c : s) {
            if (c == '>') {
                result.back().push_back(EAST);
            }
            if (c == 'v') {
                result.back().push_back(SOUTH);
            }
            if (c == '.') {
                result.back().push_back(NONE);
            }
        }
    }

    return result;
}

int part1(tgrid g) {
    int Y = g.size();
    int X = g.front().size();

    int turns{1};

    while (true) {
        int moved{0};
        tgrid ng(Y, vector<int>(X, NONE));

        // east
        for (int y=0; y<Y; ++y) {
        for (int x=0; x<X; ++x) {
            if (g[y][x] == EAST) {
                if (g[y][(x + 1) % X] == NONE) {
                    ng[y][(x + 1) % X] = EAST;
                    moved++;
                } else {
                    ng[y][x] = EAST;
                }
            }
        }}

        // south
        for (int y=0; y<Y; ++y) {
        for (int x=0; x<X; ++x) {
            if (g[y][x] == SOUTH) {
                if (ng[(y + 1) % Y][x] == EAST || g[(y + 1) % Y][x] == SOUTH) {
                    ng[y][x] = SOUTH;
                } else {
                    ng[(y + 1) % Y][x] = SOUTH;
                    moved++;
                }
            }
        }}

        if (moved == 0) {
            // uncomment to see final postion. nothing special though.
            // for (int y=0; y<Y; ++y) {
            // for (int x=0; x<X; ++x) {
            //     if (ng[y][x] == NONE) cout << ".";
            //     if (ng[y][x] == EAST) cout << ">";
            //     if (ng[y][x] == SOUTH) cout << "v";
            // }
            //     cout <<endl;
            // }


            return turns;
        }

        ++turns;
        g = move(ng);
    }
}


int main() {
    stringstream ss{ntd::test1};
    cout << "part1: " << part1(from_stream(cin)) << endl;
}
