#include "all.h"
using namespace std;

const static int MAPSIZE{512};
const static char ZRO{'.'};
const static char ONE{'#'};
const static int DARK{0};
const static int LIGHT{511};

class EnhancedImage;

class Enhancer {
public:
    Enhancer(const string& init)
    : charmap(init)
    {
        assert(charmap.size() == MAPSIZE);
        assert(charmap[DARK] == ZRO || charmap[LIGHT] == ZRO);
    }

    int Enhance(const EnhancedImage& img, pair<int, int> c) const;
    int Area(char c) {
        if (c == ONE) {
            return charmap[LIGHT];
        }
        if (c == ZRO) {
            return charmap[DARK];
        }
        assert(false);
    }
private:
    string charmap;
};

class EnhancedImage {
public:
    EnhancedImage(const vector<string>& init, const Enhancer& enhancer)
    : Y(init.size())
    , X(init.back().size())
    , data(init)
    , E(enhancer)
    , Area(ZRO)
    {
    }

    int Get(int y, int x) const {
        if (y < 0 || y >= Y || x < 0 || x >= X) {
            return Area;
        }
        return data[y][x];
    }

    void Enhance() {
        int NY = Y + 2;
        int NX = X + 2;
        vector<string> nd(NY, string(NX, ZRO));

        for (int ny=0; ny < NY; ++ny) {
        for (int nx=0; nx < NX; ++nx) {
            nd[ny][nx] = E.Enhance(*this, {ny - 1, nx - 1});
        }}

        Y = NY;
        X = NX;
        Area = E.Area(Area);
        data = move(nd);
    }

    int get_lit_points() const {
        int result{0};
        for (const auto& s : data) {
        for (const auto c : s) {
            if (c == ONE) {
                ++result;
            }
        }}
        return result;
    }

    string to_string() const {
        stringstream builder;
        for (const auto& d : data) {
            builder << d;
            builder << "\n";
        }
        return builder.str();
    }

private:
    int Y;
    int X;
    vector<string> data;
    Enhancer E;
    char Area;
};

int Enhancer::Enhance(const EnhancedImage& img, pair<int, int> c) const {
    const auto [y, x] = c;
    uint16_t result{0};

    for (int dy = y-1; dy <= y + 1; ++dy) {
    for (int dx = x-1; dx <= x + 1; ++dx) {
        result <<= 1;
        if (img.Get(dy, dx) == ONE) {
            result |= 1;
        }
    }}

    assert(result <= MAPSIZE);
    return charmap[result];
}

EnhancedImage parse_input(istream& in) {
    string s;

    getline(in, s);
    Enhancer e(s);

    getline(in, s);

    vector<string> image_data;
    while (getline(in, s)) {
        image_data.push_back(s);
    }

    return EnhancedImage(image_data, e);
}

int enhance(EnhancedImage& i, int rounds) {
    for (int r=0; r<rounds; ++r) {
        i.Enhance();
    }
    return i.get_lit_points();
}

void verify() {
    stringstream in{R"==(..#.#..#####.#.#.#.###.##.....###.##.#..###.####..#####..#....#..#..##..###..######.###...####..#..#####..##..#.#####...##.#.#..#.##..#.#......#.###.######.###.####...#.##.##..#..#..#####.....#.#....###..#.##......#.....#..#..#..##..#...##.######.####.####.#.#...#.......#..#.#.#...####.##.#......#..#...##.#.##..#...##.#.##..###.#......#.#.......#.#.#.####.###.##...#.....####.#..#..#.##.#....##..#.####....##...##..#...#......#.#.......#.......##..####..#...#.#.#...##..#.#..###..#####........#..####......#..#

#..#.
#....
##..#
..#..
..###)=="};

    auto i = parse_input(in);
    enhance(i, 2);
    assert(35 == i.get_lit_points());
    enhance(i, 48);
    assert(3351 == i.get_lit_points());
}


int main() {
    verify();
    auto i = parse_input(cin);
    cout << "part1: " << enhance(i, 2) << endl;
    cout << "part2: " << enhance(i, 48) << endl;
}
