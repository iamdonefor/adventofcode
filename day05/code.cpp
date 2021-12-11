#include "../all.h"

using namespace std;

class TMap {
public:
    static const int X{1000};
    static const int Y{1000};

    TMap()
    : Map(Y, vector<int>(Y, 0))
    , Overlaps(0)
    {}

    void AddLine(int x1, int y1, int x2, int y2) {
        int dx = (x1 == x2) ? 0 : (x1 > x2 ? -1 : 1);
        int dy = (y1 == y2) ? 0 : (y1 > y2 ? -1 : 1);

        for (int x = x1, y = y1; ; x += dx, y += dy) {
            AddPoint(y, x);
            if ((dx && x == x2) || (dy && y == y2)) {
                break;
            }
        }
    }

    void AddPoint(int y, int x) {
        Map[y][x]++;
        if (Map[y][x] == 2) {
            ++Overlaps;
        }
    }

    int TotalDencities() {
        return Overlaps;
    }

private:
    vector<vector<int>> Map;
    int Overlaps;
};

void processInput(const string& line, TMap& map) {
    int x1, x2, y1, y2;
    if (sscanf(line.data(), "%d,%d -> %d,%d", &x1, &y1, &x2, &y2) != 4) {
        return;
    };

    // if (x1 == x2 || y1 == y2) // part1
    map.AddLine(x1, y1, x2, y2);
}

int main() {
    string s;
    TMap map;

    while(getline(cin, s)) {
        processInput(s, map);
    }

    cout << map.TotalDencities() << endl;
}
