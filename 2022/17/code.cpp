#include "all.h"

using namespace std;
using namespace advent;

using tinput = vector<int>;
using twind = vector<int>;

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
};

class tfiguregen {
public:
    const tfigure& next() const {
        return figures[current++ % figures.size()];
    }

private:
    const vector<tfigure> figures{
        { '-', 1, 4, { {0, 0}, {0, 1}, {0, 2}, {0, 3} }},
        { '+', 3, 3, { {0, 1}, {1, 0}, {1, 1}, {1, 2}, {2, 2} }},
        { 'L', 3, 3, { {0, 2}, {1, 2}, {2, 0}, {2, 1}, {2, 2} }},
        { '|', 4, 1, { {0, 0}, {1, 0}, {2, 0}, {3, 0}, }},
        { '@', 2, 2, { {0, 0}, {1, 0}, {0, 1}, {1, 1},}},
    };

    mutable int current{0};
};

class tchamber {
public:
    tchamber(twind wind_)
    : wind(move(wind_))
    {}

    void run(int64_t steps) {}

private:
    twind wind;
    tfiguregen gen;
};

tinput parse_input(istream&& is) {
    tinput result;

    for (string s; getline(is, s); ) {
        if (s.empty()) { continue; }
        for (const auto c : s) {
            result.push_back(c == '>' ? nwind::L : nwind::R);
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
    cout << parse_input(test).size() << endl;
}
