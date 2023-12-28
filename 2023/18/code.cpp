#include "all.h"

using namespace std;
using namespace advent;

struct tdig {
    int direction;
    int distance;
    string color;
};

using tinput = vector<tdig>;

tinput parse_input(istream&& is) {
    tinput result;

    for (string s; getline(is, s); ) {
        if (s.empty()) { continue; }

        char c; int d; char b[7];
        if (sscanf(s.data(), "%c %d (%7c)", &c, &d, b) == 3) {
            result.push_back({
                c, d, string(b, 7)
            });
        } else {
             throw runtime_error("invalid input");
        }
    }

    return result;
}

tinput parse_input(istream& is) {
    return parse_input(move(is));
}

stringstream test{R"(R 6 (#70c710)
D 5 (#0dc571)
L 2 (#5713f0)
D 2 (#d2c081)
R 2 (#59c680)
D 2 (#411b91)
L 5 (#8ceee2)
U 2 (#caa173)
L 1 (#1b58a2)
U 2 (#caa171)
R 2 (#7807d2)
U 3 (#a77fa3)
L 2 (#015232)
U 2 (#7a21e3)
)"};

void fill_internals(vector<vector<char>>& d, tcoords start) {
    deque<tcoords> q{start};
    int Y = d.size();
    int X = d.front().size();


    while(!q.empty()) {
        const auto n = q.front();
        q.pop_front();

        const static vector<int> neis{1, 0, -1, 0, 1};
        for (int i=0; i<5; ++i) {
            auto dy = n[0] + neis[i];
            auto dx = n[1] + neis[i+1];

            if (dx >= X  || dy >= Y || dx < 0 || dy < 0) {
                continue;
            }

            if (d[dy][dx] == '.') {
                d[dy][dx] = '#';
                q.push_back({dy, dx});
            }
        }
    }
}

int64_t solution1(const tinput& data) {
    int minx = 0;
    int maxx = 0;
    int miny = 0;
    int maxy = 0;

    int x = 0;
    int y = 0;

    for (const auto& d : data) {
        if (d.direction == 'R') { x += d.distance; }
        if (d.direction == 'L') { x -= d.distance; }
        if (d.direction == 'D') { y += d.distance; }
        if (d.direction == 'U') { y -= d.distance; }
    
        maxx = max(maxx, x);
        maxy = max(maxy, y);
        minx = min(minx, x);
        miny = min(miny, y);
    }

    // cout << maxx << " " << minx << " " << maxy << " " << miny << endl;

    vector<vector<char>> digmap(maxy - miny + 1, vector<char>(maxx - minx + 1, '.'));
    x = -minx; y = -miny;
    for (const auto& d : data) {
        if (d.direction == 'R') { for (int xx=0; xx<d.distance; ++xx) { digmap[y][x+xx] = '#'; }; x += d.distance; }
        if (d.direction == 'L') { for (int xx=0; xx<d.distance; ++xx) { digmap[y][x-xx] = '#'; }; x -= d.distance; }
        if (d.direction == 'D') { for (int yy=0; yy<d.distance; ++yy) { digmap[y+yy][x] = '#'; }; y += d.distance; }
        if (d.direction == 'U') { for (int yy=0; yy<d.distance; ++yy) { digmap[y-yy][x] = '#'; }; y -= d.distance; }
    }

    fill_internals(digmap, {-miny+1, -minx+1});

    int64_t result = 0;
    for (const auto& v : digmap) {
    for (const auto& c : v) {
        if (c == '#') { ++result; }
    }}

    return result;
}

int main() {
    const auto& testi = parse_input(test);
    cout << solution1(testi) << endl;

    const auto& cini = parse_input(cin);
    cout << solution1(cini) << endl;
}
