#include "all.h"
using namespace std;

class DeterministicDice {
public:
    int roll() const {
        return (rolls_++ % 100) + 1;
    }
    int64_t rolls() const {
        return rolls_;
    }

private:
    mutable int64_t rolls_{0} ;
};

class Player {
public:
    const static int WIN_THRESHOLD{1000};

    Player(int pos) : pos_(pos) {}

    bool won() const { return score_ >= WIN_THRESHOLD; }
    bool turn(int roll) {
        roll = roll % 10;
        pos_ = pos_ + roll;
        if (pos_ > 10) {
            pos_ -= 10;
        }
        score_ += pos_;
        return won();
    }
    int score() const { return score_; }
    int pos() const { return pos_; }

private:
    int64_t score_{0};
    int pos_;
};

int game(int p1pos, int p2pos) {
    Player p1{p1pos}, p2{p2pos};
    DeterministicDice dice;

    auto triple = [&dice]() {
        return dice.roll() + dice.roll() + dice.roll();
    };
    //
    // cout << endl;
    // auto t = triple();
    // cout << t << endl;
    // p1.turn(t);
    // cout << p1.pos() << " " << p1.score() << endl;
    // return 0;

    while (true) {
        if (p1.turn(triple())) {
            return p2.score() * dice.rolls();
        }

        if (p2.turn(triple())) {
            return p1.score() * dice.rolls();
        }

        cout
        << p1.pos() << "|" << p1.score()
        << ", "
        << p2.pos() << "|" << p2.score()
        << endl;
    }
}

int main() {
    cout << "part1: " << game(8,5) << endl;
}
