// XXX solved by p&p, didn't finish the code


#include "all.h"
using namespace std;

const int CS{7}; // corridor size

enum egenus {
    A=0, // amber
    B=1, // bronze
    C=2, // copper
    D=3, // desert
    N=8, // nobody
};

using tcorridor = vector<egenus>;

struct troom {
    troom() : troom(A, {}) {}
    troom(egenus type) : troom(type, {}) {}
    troom(egenus whos, const vector<egenus>& who)
    : type_(whos)
    , who_(who) {}

    bool ready() const {
        if (ready_) {
            return ready_;
        }
        if (all_of(who_.begin(), who_.end(),
            [this](const auto w) { return w == type_; })) {
            ready_ = true;
        }
        return ready_;
    }

    bool mutable ready_{false};
    egenus type_;
    vector<egenus> who_;

    bool reachable(const tcorridor& corridor, int from) const {
        if (from == type_ + 1 || from == type_ + 2) {
            return true;
        }
        for (int i=from + 1; i <= type_ + 1; ++i) {
            if (corridor[i] != N) {
                return false;
            }
        }
        for (int i=from-1; i >= type_ + 2; --i) {
            if (corridor[i] != N) {
                return false;
            }
        }
        return true;
    }

    void push(egenus w) {
        who_.push_back(w);
    }

    egenus pop() { auto b = top(); who_.pop_back(); return b; }
    egenus top() const { return who_.back(); }
    bool empty() const { return who_.empty(); }
};

using tstate = pair<vector<egenus>, array<troom, 4>>;
// actualy the map is just a straight line
// 0 1 - extra space to the left
// 2 3 4 - space between rooms
// 5 6 - extra space to the right

// room[i] can be entered from position j
// if there are no obstacles from j to either
// i + 1 or i + 2

// luky we, because of stubborness any amphipode can
// move only twice

vector<tstate> find_moves(tstate& state) {
    const auto& [corridor, rooms] = state;
    vector<tstate> result;

    // first try to move to room
    for (int i=0; i<CS; ++i) {
        auto sp = corridor[i];
        if (sp == N) {
            continue;
        }

        if (rooms[sp].reachable(corridor, i) && rooms[sp].ready()) {
            result.emplace_back(corridor, rooms);
            result.back().first[i] = N;
            result.back().second[sp].push(sp);
        }
    }

    for (int i=0; i<rooms.size(); ++i) {
        
    }

    return result;
}

int main() {
    tstate init{vector<egenus>(7, N), {
        troom(A, {D, D, D, C}),
        troom(B, {A, B, C, A}),
        troom(C, {B, A, B, C}),
        troom(D, {B, C, A, D}),
    }};
}
