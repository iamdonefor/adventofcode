#include "../all.h"
#include <sstream>
using namespace std;

class TBoard {
public:
    friend istream& operator>>(istream& is, TBoard& b) {
        int v;
        for (int y = 0; y < Y; ++y) {
        for (int x = 0; x < X; ++x) {
            is >> v;
            b.Set(v, y, x);
        }}
        return is;
    }

    friend ostream& operator<<(ostream& os, const TBoard& b) {
        for (int y = 0; y < Y; ++y) {
        for (int x = 0; x < X; ++x) {
            int v = b.Get(y, x);
            os << v;
            if (b.Seen(v)) {
                os << "*";
            }
            os << " ";
        }
            os << endl;
        }
        return os;
    }


    TBoard()
    : Board(Y, vector<int>(X, false))
    {}

    void Set(int value, int y, int x) {
        Values[value] = make_pair(y, x);
        Board[y][x] = value;
        Score_ += value;
    }

    int Get(int y, int x) const {
        return Board[y][x];
    }

    bool Seen(int value) const {
        return Seen_.count(value) > 0;
    }

    bool Solved() const {
        return Solved_;
    }

    void Add(int value) {
        auto it = Values.find(value);
        if (it == Values.end()) {
            return;
        }

        Score_ -= value;
        Seen_.insert(value);
        Solved_ = CheckSolved(it->second.first, it->second.second);
    }

    int Score() {
        return Score_;
    }

private:
    const static int X{5};
    const static int Y{5};

    int Score_{0};
    bool Solved_{false};
    vector<vector<int>> Board;
    unordered_set<int> Seen_;
    unordered_map<int, pair<int, int>> Values;

    bool CheckSolved(int yy, int xx) {
        bool solved = true;
        for (int y=0; y<Y; ++y) {
            if (Seen_.count(Board[y][xx]) == 0) {
                solved = false;
            }
        }
        if (solved == true) {
            return true;
        }
        solved = true;
        for (int x=0; x<X; ++x) {
            if (Seen_.count(Board[yy][x]) == 0) {
                solved = false;
            }
        }
        return solved;
    }
};

int part1(const vector<int>& dices, vector<TBoard> boards) {
    for (const auto dice: dices) {
        cout << "dice: " << dice << endl;
        for (auto& b : boards) {
            b.Add(dice);
            if (b.Solved()) {
                cout << "solved: " << dice << " " << b.Score() << " " << dice * b.Score() << endl;
                return dice * b.Score();
            }
        }
    }
}

int part2(const vector<int>& dices, vector<TBoard> boards) {
    int last_solved;
    for (const auto dice : dices) {
        for (int i=0; i<boards.size(); ++i) {
            if (boards[i].Solved()) {
                continue;
            }
            boards[i].Add(dice);
            if (boards[i].Solved()) {
                last_solved = i;
                cout << "baord " << i << " solved with score " << dice * boards[i].Score() << endl;
            }
        }
    }

    return last_solved;
}

int main() {
    string sdices;
    getline(cin, sdices);
    stringstream ss{sdices};
    vector<int> dices;
    while (ss) {
        int v;
        char c;
        ss >> v;
        dices.push_back(v);
        ss >> c;
    }

    vector<TBoard> boards;
    while (getline(cin, sdices)) {
        boards.push_back({});
        cin >> boards.back();
    }

    boards.resize(boards.size() - 1);
    cout << boards.size() << endl;

    part1(dices, boards);
    part2(dices, boards);
}
