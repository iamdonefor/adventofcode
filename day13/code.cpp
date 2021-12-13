#include "all.h"
using namespace std;

using TInput = pair<set<pair<int, int>>, vector<pair<int, int>>>;

set<pair<int, int>> fold(const set<pair<int, int>>& dots, pair<int, int> fold) {
    set<pair<int, int>> folded;

    for (const auto& d : dots) {
        assert(d.first >= 0);
        assert(d.second >= 0);

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

set<pair<int, int>> fold(const set<pair<int, int>>& dots, const vector<pair<int, int>>& folds) {
    set<pair<int, int>> result{dots};
    for (const auto& f : folds) {
        result = fold(result, f);
    }
    return result;
}

void print_page(ostream& os, const set<pair<int, int>>& dots) {
    auto [miney, maxey] = minmax_element(dots.begin(), dots.end(), [](const auto& l, const auto& r) { return l.first < r.first; });
    auto [minex, maxex] = minmax_element(dots.begin(), dots.end(), [](const auto& l, const auto& r) { return l.second < r.second; });

    auto maxy = (*maxey).first;
    auto maxx = (*maxex).second;

    vector<vector<int>> m(maxy + 1, vector<int>(maxx + 1, 0));

    for (const auto& d : dots) {
        m[d.first][d.second] = 1;
    }

    for (int y = 0; y <= maxy; ++y) {
    for (int x = 0; x <= maxx; ++x) {
        os << (m[y][x] == 1 ? '#' : ' ');
    }
    os << endl;
    }
};

TInput parse_input(istream& in) {
    string s;
    set<pair<int, int>> dots;
    vector<pair<int, int>> folds;

    while (getline(in, s)) {
        int x, y;
        if (sscanf(s.data(), "%d,%d", &x, &y) == 2) {
            dots.insert({y, x});
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
    // return 17 == fold(paper, folds.front()).size();
    auto page = fold(paper, folds);
    print_page(cout, page);
    return true;
}

int main() {
    assert(verify());

    auto [paper, folds] = parse_input(cin);
    cout << "part1: " << fold(paper, folds.front()).size() << endl;
    cout << "part2: " << endl;
    auto result = fold(paper, folds);
    print_page(cout, result);
}
