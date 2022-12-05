#include "all.h"

struct tmove {
    int from;
    int to;
    int q;
};

using tinput = vector<tmove>;
using tstate = vector<deque<char>>;

tinput parseInput(istream& is) {
    tinput result;

    for (string s; getline(is, s); ) {
        int q, from, to;
        if (sscanf(s.data(), "move %d from %d to %d", &q, &from, &to) == 3) {
            result.push_back({from-1, to-1, q});
        }
    }

    return result;
}

string solve(tstate state, const tinput& moves, bool retainOrder = false) {
    for (const auto& m : moves) {
        assert(m.from < state.size());
        assert(m.to < state.size());
        assert(state[m.from].size() >= m.q);

        deque<char> supp;
        for (int i=0; i<m.q; ++i) {
            if (retainOrder) {
                supp.push_front(state[m.from].back());
            } else {
                supp.push_back(state[m.from].back());
            }
            state[m.from].pop_back();
        }
        state[m.to].insert(state[m.to].end(), supp.begin(), supp.end());
    }

    string result;
    for (const auto& s : state) {
        result.push_back(s.back());
    }
    return result;
}

tstate testState{
    {'Z', 'N'},
    {'M', 'C', 'D'},
    {'P'},
};

tstate inputState{
    {'P', 'F', 'M', 'Q', 'W', 'G', 'R', 'T'},
    {'H', 'F', 'R'},
    {'P', 'Z', 'R', 'V', 'G', 'H', 'S', 'D'},
    {'Q', 'H', 'P', 'B', 'F', 'W', 'G'},
    {'P', 'S', 'M', 'J', 'H'},
    {'M', 'Z', 'T', 'H', 'S', 'R', 'P', 'L'},
    {'P', 'T', 'H', 'N', 'M', 'L'},
    {'F', 'D', 'Q', 'R'},
    {'D', 'S', 'C', 'N', 'L', 'P', 'H'},
};

stringstream test{R"(
move 1 from 2 to 1
move 3 from 1 to 3
move 2 from 2 to 1
move 1 from 1 to 2
)"};

int main() {
    cout << solve(testState, parseInput(test), true) << endl;
    cout << solve(inputState, parseInput(cin), true) << endl;
}
