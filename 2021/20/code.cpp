#include "all.h"
using namespace std;

class EnhancedImage {
public:
    const static int MAPSIZE{512};
    const static int DARK{0};
    const static int LIGHT{MAPSIZE - 1};
    const static char ZERO{'.'};
    const static char ONE{'#'};

    EnhancedImage(const vector<string>& seed, const string& enhancer)
    : Y(seed.size())
    , X(seed.back().size())
    , data(seed)
    , E(enhancer)
    {
        assert(E.size() == MAPSIZE);
        assert(E[DARK] == ZERO || E[LIGHT] == ZERO);
        for (const auto& s : data) {
            LitPointsCount += count(s.begin(), s.end(), '#');
        }
    }

    int Get(int y, int x) const {
        if (y < 0 || y >= Y || x < 0 || x >= X) {
            return Area;
        }
        return data[y][x];
    }

    /* MUTATES */
    void EnhanceImage() {
        LitPointsCount = 0;
        int NY = Y + 2;
        int NX = X + 2;
        vector<string> nd(NY, string(NX, ZERO));

        for (int ny=0; ny < NY; ++ny) {
        for (int nx=0; nx < NX; ++nx) {
            nd[ny][nx] = Enhance(ny - 1, nx - 1);
            if (nd[ny][nx] == ONE) {
                ++LitPointsCount;
            };
        }}

        Y = NY;
        X = NX;
        Area = (Area == ONE ? E[LIGHT] : E[DARK]);

        data = move(nd);
    }

    int Enhance(int y, int x) const {
        uint16_t result{0};

        for (int dy = y-1; dy <= y + 1; ++dy) {
        for (int dx = x-1; dx <= x + 1; ++dx) {
            result <<= 1;
            if (Get(dy, dx) == ONE) {
                result |= 1;
            }
        }}

        assert(result <= MAPSIZE);
        return E[result];
    }

    int GetLitPointsCount() const {
        return LitPointsCount;
    }

    string ToString() const {
        stringstream builder;
        for (const auto& d : data) {
            builder << d;
            builder << "\n";
        }
        return builder.str();
    }

private:
    int Y{0};
    int X{0};
    int LitPointsCount{0};
    vector<string> data;
    string E;
    char Area{ZERO};
};

EnhancedImage parse_input(istream& in) {
    string s;
    string e;

    getline(in, e);
    getline(in, s);

    vector<string> image_data;
    while (getline(in, s)) {
        image_data.push_back(s);
    }

    return EnhancedImage(image_data, e);
}

int enhance(EnhancedImage& i, int rounds) {
    for (int r=0; r<rounds; ++r) {
        i.EnhanceImage();
    }
    return i.GetLitPointsCount();
}

void verify() {
    stringstream in{R"==(..#.#..#####.#.#.#.###.##.....###.##.#..###.####..#####..#....#..#..##..###..######.###...####..#..#####..##..#.#####...##.#.#..#.##..#.#......#.###.######.###.####...#.##.##..#..#..#####.....#.#....###..#.##......#.....#..#..#..##..#...##.######.####.####.#.#...#.......#..#.#.#...####.##.#......#..#...##.#.##..#...##.#.##..###.#......#.#.......#.#.#.####.###.##...#.....####.#..#..#.##.#....##..#.####....##...##..#...#......#.#.......#.......##..####..#...#.#.#...##..#.#..###..#####........#..####......#..#

#..#.
#....
##..#
..#..
..###)=="};

    auto i = parse_input(in);
    cout << i.ToString() << endl;
    enhance(i, 2);
    assert(35 == i.GetLitPointsCount());
    enhance(i, 48);
    assert(3351 == i.GetLitPointsCount());
    cout << i.ToString() << endl;
}


int main() {
    verify();
    auto i = parse_input(cin);
    cout << "part1: " << enhance(i, 2) << endl;
    cout << "part2: " << enhance(i, 48) << endl;
    cout << i.ToString() << endl;
}
