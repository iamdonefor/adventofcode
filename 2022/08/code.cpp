#include "all.h"

using tinput = vector<vector<int>>;
using tforest = tinput;

tinput parseInput(istream& is) {
    tinput result;

    for (string s; getline(is, s); ) {
        if (s.empty()) { continue; }

        result.push_back({});
        for (const auto c : s) {
            result.back().push_back(c - '0');
        }
        // if (sscanf(s.data(), "%d,%d", &x, &y) == 2)
    }

    return result;
}

tforest calcVisibility(const tinput& in) {
    const int32_t Y = in.size();
    const int32_t X = in.back().size();

    cout << "total trees: " << X * Y << endl;

    tforest vis(Y, vector<int>(X, 0));

    for (int y=0; y<Y; ++y) {
        for (int x=0, max_height=-1; x<X; ++x) {
            if (in[y][x] > max_height) {
                vis[y][x] = 1;
            }
            max_height = max(in[y][x], max_height);
        }
        for (int x=X-1, max_height=-1; x>=0; --x) {
            if (in[y][x] > max_height) {
                vis[y][x] = 1;
            }
            max_height = max(in[y][x], max_height);
        }
    }

    for (int x=0; x<X; ++x) {
        for (int y=0, max_height=-1; y<Y; ++y) {
            if (in[y][x] > max_height) {
                vis[y][x] = 1;
            }
            max_height = max(in[y][x], max_height);
        }
        for (int y=Y-1, max_height=-1; y>=0; --y) {
            if (in[y][x] > max_height) {
                vis[y][x] = 1;
            }
            max_height = max(in[y][x], max_height);
        }
    }

    return vis;
}

uint64_t calcVisible(const tforest& vis) {
    uint64_t result{0};
    for (const auto& v : vis) {
        result += accumulate(v.begin(), v.end(), 0);
    }
    return result;
}

struct tvision {
    tforest left;
    tforest right;
    tforest up;
    tforest down;
};

tvision calcVision(const tforest& f) {
    const int32_t Y = f.size();
    const int32_t X = f.back().size();

    tvision vision{
        tforest(Y, vector<int>(X, 0)),
        tforest(Y, vector<int>(X, 0)),
        tforest(Y, vector<int>(X, 0)),
        tforest(Y, vector<int>(X, 0)),
    };

    for (int y=1; y<Y; ++y) {
        vector<int> sup(10, 0);
        for (int x=1; x<X; ++x) {
            const auto h = f[y][x];
            const auto upto = *max_element(sup.begin() + h, sup.end());

            vision.left[y][x] = x - upto;
            sup[h] = x;
        }

        fill(sup.begin(), sup.end(), X-1);
        for (int x=X-2; x>=0; --x) {
            const auto h = f[y][x];
            const auto upto = *min_element(sup.begin() + h, sup.end());

            vision.right[y][x] = upto - x;
            sup[h] = x;
        }
    }

    for (int x=1; x<X; ++x) {
        vector<int> sup(10, 0);
        for (int y=1; y<Y; ++y) {
            const auto h = f[y][x];
            const auto upto = *max_element(sup.begin() + h, sup.end());

            vision.up[y][x] = y - upto;
            sup[h] = y;
        }

        fill(sup.begin(), sup.end(), Y-1);
        for (int y=Y-2; y>=0; --y) {
            const auto h = f[y][x];
            const auto upto = *min_element(sup.begin() + h, sup.end());

            vision.down[y][x] = upto - y;
            sup[h] = y;
        }
    }

    return vision;
}

int findMaxVision(const tvision& vision) {
    const int32_t Y = vision.left.size();
    const int32_t X = vision.left.back().size();

    int max_vision = 1;
    for (int y=0; y<Y; ++y) {
    for (int x=0; x<X; ++x) {
        max_vision = max(max_vision,
            1
            * vision.left[y][x]
            * vision.up[y][x]
            * vision.right[y][x]
            * vision.down[y][x]
        );
    }
 }

    return max_vision;
}

stringstream test{R"(
30373
25512
65332
33549
35390
)"};

int main() {
    // part1
    // cout << calcVisible(calcVisibility(parseInput(test))) << endl;
    // cout << calcVisible(calcVisibility(parseInput(cin))) << endl;

    // part2
    cout << findMaxVision(calcVision(parseInput(test))) << endl;
    cout << findMaxVision(calcVision(parseInput(cin))) << endl;
}
