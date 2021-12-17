#include "all.h"
using namespace std;

struct Drone {
    Drone(int vx, int vy) : Vx(vx), Vy(vy) {}
    pair<int, int> tick() {
        X += Vx;
        Y += Vy;

        if (Vx > 0) { --Vx; }
        if (Vx < 0) { ++Vx; }
        --Vy;

        return at();
    }

    pair<int, int> at() const { return make_pair(X, Y); }
    pair<int, int> speed() const { return make_pair(Vx, Vy); }

    int X{0};
    int Y{0};
    int Vx{0};
    int Vy{0};
};

struct Area {
    Area(pair<int, int> xs, pair<int, int> ys) {
        xmin = xs.first;
        xmax = xs.second;
        ymin = ys.first;
        ymax = ys.second;
    }

    bool contains(pair<int, int> c) const { return contains_x(c) && contains_y(c); }
    bool contains_x(pair<int, int> c) const {return c.first >= xmin && c.first <= xmax; }
    bool contains_y(pair<int, int> c) const { return c.second >= ymin && c.second <= ymax; }

    int xmin, xmax, ymin, ymax;
};

const int LOW{-10000};

optional<int> launch(const Area& a, pair<int, int> iv) {
    Drone d{iv.first, iv.second};
    int max_elevation = LOW;

    for (;;) {
        auto curr = d.tick();
        max_elevation = max(max_elevation, curr.second);

        if (a.contains(curr)) {
            return max_elevation;
        }

        auto [vx, vy] = d.speed();
        if (   (curr.second < a.ymin && vy < 0) // miss y
            || (!a.contains_x(curr) && vx == 0) // miss x
        ) {
            return nullopt;
        }
    }
}

int main() {
    // Area target({20, 30}, {-10, -5});
    Area target({135, 155}, {-102, -78});
    set<pair<int, int>> velocities;

    // we will have that speed at level 0
    // so guaranteed to overflew the target
    int maxy = abs(target.ymin) + 1;
    // actually, you need to solve quadratic equation
    // to get the minx value, but sqrt is lower then that
    int minx = int(sqrt(target.xmin));
    // one tick is enough to hit
    int miny = target.ymin;
    int maxx = target.xmax;

    int max_elevation = LOW;
    for (int x = minx; x <= maxx; ++x) {
    for (int y = miny; y <= maxy; ++y) {
        auto thistry = launch(target, {x, y});

        if (thistry) {
            max_elevation = max(max_elevation, *thistry);
            velocities.insert({x, y});
        }
    }}

    cout << "max_elevation: " << max_elevation << endl;
    cout << "total different speeds: " << velocities.size() << endl;
}
