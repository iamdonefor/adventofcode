#include "all.h"
using namespace std;

using TPoint = array<int64_t, 2>;
using TLine = pair<TPoint, TPoint>;
using TPath = vector<TLine>;

stringstream test{R"==(R8,U5,L5,D3
U7,R6,D4,L4
)=="};

TPath parse_input(istream& in) {
    TPath lines;

    for (string s; getline(in, s);) {
        stringstream ss{s};
        TLine line;
        TPoint prev{0, 0};
        while(ss) {
            char direction;
            int length;
            TPoint end;
            ss >> direction >> length;
            switch (direction) {
                case 'U':
                    end = {prev[0], prev[1] + length};
                    break;
                case 'D':
                    end = {prev[0], prev[1] - length};
                    break;
                case 'R':
                    end = {prev[0] + length, prev[1]};
                    break;
                case 'L':
                    end = {prev[0] - length, prev[1]};
                    break;
            }
            line.push_back({prev, end});
            prev = end;
            ss >> direction; // skip ,
        }
        lines.push_back(line);
    }

    return lines;
}

int manhatten(const TPoint p, const TPoint other = TPoint{0, 0}) {
    return abs(p[0] - other[0]) + abs(p[1] - other[1]);
}

optional<TPoint> intersection(const TLine& l1, ) {

}

int nearest_intersection(const TPath& l1, const TPath& l2) {

}

int main() {
    auto input = parse_input(test);

}
