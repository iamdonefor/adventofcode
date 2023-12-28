#include "all.h"

using namespace std;
using namespace advent;

using tinput = vector<vector<char>>;
using tlab = tinput;

struct tray {
    tcoords current;
    tcoords direction;
};

tinput parse_input(istream&& is) {
    tinput result;

    for (string s; getline(is, s); ) {
        if (s.empty()) { continue; }
        result.push_back({s.begin(), s.end()});
    }

    return result;
}

tinput parse_input(istream& is) {
    return parse_input(move(is));
}

stringstream test{R"(.|...\....
|.-.\.....
.....|-...
........|.
..........
.........\
..../.\\..
.-.-/..|..
.|....-|.\
..//.|....
)"};

const tcoords EAST{0, 1}; 
const tcoords WEST{0, -1}; 
const tcoords SOUTH{1, 0}; 
const tcoords NORTH{-1, 0};

bool operator< (const tray& l, const tray& r) {
    if (l.current == r.current) {
        return l.direction < r.direction;
    }
    return l.current < r.current;
} 

int64_t solution1(const tinput& data, const tray initital = {{0, -1}, {0, 1}}) {
    int X = data.front().size();
    int Y = data.size();
    
    vector<tray> rays;
    set<tray> seen_rays;
    set<tcoords> energized;

    rays.push_back(initital);

    for (;!rays.empty();) {
        auto ray = rays.back();
        rays.pop_back();
        if (seen_rays.count(ray) > 0) {
            continue;
        }

        for (bool ray_runs = true; ray_runs;) {
            const auto next = ray.current + ray.direction;
            if (next[0] < 0 || next[0] >= Y || next[1] < 0 || next[1] >= X) {
                break;
            }
            ray.current = next;
            energized.insert(next);

            if (seen_rays.count(ray) > 0) { break; }
            seen_rays.insert(ray);

            switch (data[next[0]][next[1]]) {
                case '.':
                    break;
                case '\\':
                    if (ray.direction == EAST) { ray.direction = SOUTH; }
                    else if (ray.direction == WEST) { ray.direction = NORTH; }
                    else if (ray.direction == NORTH) { ray.direction = WEST; }
                    else if (ray.direction == SOUTH) { ray.direction = EAST; }
                    break;
                case '/':
                    if (ray.direction == EAST) { ray.direction = NORTH; }
                    else if (ray.direction == WEST) { ray.direction = SOUTH; }
                    else if (ray.direction == NORTH) { ray.direction = EAST; }
                    else if (ray.direction == SOUTH) { ray.direction = WEST; }
                    break;
                case '-':
                    if (ray.direction == SOUTH || ray.direction == NORTH) {
                        ray_runs = false;
                        rays.push_back({ ray.current, EAST });
                        rays.push_back({ ray.current, WEST });
                    }
                    break;
                case '|':
                    if (ray.direction == EAST || ray.direction == WEST) {
                        ray_runs = false;
                        rays.push_back({ ray.current, SOUTH });
                        rays.push_back({ ray.current, NORTH });
                    }
                    break;
                default:
                    throw runtime_error("unknown char in input");
            }
        }
    }

    return energized.size();
}

int64_t solution2(const tinput& data) {
    int64_t result = 0;

    int X = data.front().size();
    int Y = data.size();

    for (int x=0; x<X; ++x) {
        result = max(result, solution1(data, {{-1, x}, SOUTH}));
        result = max(result, solution1(data, {{Y, x}, NORTH}));
    }

    for (int y=0; y<Y; ++y) {
        result = max(result, solution1(data, {{y, -1}, EAST}));
        result = max(result, solution1(data, {{y, X}, WEST}));
    }

    return result;
}

int main() {
    const auto& testi = parse_input(test);
    cout << solution1(testi) << endl;
    cout << solution2(testi) << endl;

    const auto& cini = parse_input(cin);
    cout << solution1(cini) << endl;
    cout << solution2(cini) << endl;
}
