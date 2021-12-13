#include "all.h"
using namespace std;

using TInput = pair<vector<pair<int, int>>, vector<pair<int, int>>>;

set<pair<int, int>> fold(vector<pair<int, int>> dots, pair<int, int> fold) {
    set<pair<int, int>> folded;

    for (const auto& d : dots) {
        if (fold.first == 0) { // fold over x
            if (d.second > fold.second) {
                folded.insert({d.first, 2 * fold.second - d.second});
            } else {
                folded.insert(d);
            }
        } else { // over y
            if (d.first > fold.first) {
                folded.insert({2 * fold.first - d.first, d.second});
            } else {
                folded.insert(d);
            }
        }
    }

    return folded;
}

TInput parse_input(istream& in) {
    string s;
    vector<pair<int, int>> dots;
    vector<pair<int, int>> folds;

    while (getline(in, s)) {
        int x, y;
        if (sscanf(s.data(), "%d,%d", &x, &y) == 2) {
            dots.push_back({y, x});
            continue;
        }
        if (sscanf(s.data(), "fold along x=%d", &x) == 1) {
            folds.push_back({0, x});
            continue;
        }
        if (sscanf(s.data(), "fold along y=%d", &y) == 1) {
            folds.push_back({y, 0});
            continue;
        }
    }

    return {dots, folds};
}

bool verify() {
    stringstream input_stream{R"===(6,10
0,14
9,10
0,3
10,4
4,11
6,0
6,12
4,1
0,13
10,12
3,4
3,0
8,4
1,10
2,14
8,10
9,0

fold along y=7
fold along x=5)==="};

    auto [paper, folds] = parse_input(input_stream);
    return 17 == fold(paper, folds.front()).size();
}

int main() {
    assert(verify());

    auto [paper, folds] = parse_input(cin);
    cout << "part1: " << fold(paper, folds.front()).size();
}
