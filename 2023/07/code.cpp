#include "all.h"

using namespace std;
using namespace advent;

enum etype {
    high = 0,
    pair = 1,
    pairs = 2,
    set = 3,
    house = 4,
    four = 5,
    five = 6,
};

unordered_map<char, char> MAP{
    {'2', 1},
    {'3', 2},
    {'4', 3},
    {'5', 4},
    {'6', 5},
    {'7', 6},
    {'8', 7},
    {'9', 8},
    {'T', 9},
    {'J', 0},
    {'Q', 10},
    {'K', 11},
    {'A', 13},
};

etype get_type(const string& s) {
    unordered_map<char, int> counter;
    int jokers = 0;

    for (const auto& c : s) {
        if (c == 'J') {
            ++jokers;
        } else {
            counter[c]++;
        }
    }

    vector<int> vs;
    for (const auto& [k, v] : counter) {
        vs.push_back(v);
    }
    sort(vs.begin(), vs.end(), std::greater<int>());

    if (vs.empty()) { return five; } // 5 jokers

    if (vs[0] == 5) {
        return five;
    }

    if (vs[0] == 4) {
        return jokers == 0 ? four : five;
    }
    if (vs[0] == 3) {
        if (vs.size() > 1 && vs[1] == 2) {
            return house;
        }

        switch(jokers) {
            case 0: return etype::set;
            case 1: return four;
            case 2: return five;
        }

        return etype::set;
    }

    if (vs[0] == 2) {
        if (vs.size() > 1 && vs[1] == 2) {
            switch(jokers) {
                case 0: return pairs;
                case 1: return house;
            }
        }
        switch(jokers) {
            case 0: return etype::pair;
            case 1: return etype::set;
            case 2: return four;
            case 3: return five;
        }
    }

    switch(jokers) {
        case 0: return high;
        case 1: return etype::pair;
        case 2: return etype::set;
        case 3: return four;
        case 4: return five;
    }
}

string convert_hand(const string& hand) {
    string result;
    for (const auto c : hand) {
        result.push_back(MAP.at(c));
    }
    return result;
}

struct thand {
    etype type;
    string value;
};

struct tgame {
    thand hand;
    int64_t bet;
};

bool operator< (const thand& l, const thand& r) {
    if (l.type == r.type) { return l.value < r.value; }
    return l.type < r.type;
}

bool operator< (const tgame& l, const tgame& r) {
    return l.hand < r.hand;
}

using tinput = vector<tgame>;

tinput parse_input(istream&& is) {
    tinput result;

    for (string s; getline(is, s); ) {
        if (s.empty()) { continue; }
        stringstream h(s);

        string raw_hand;
        int64_t bet;

        h >> raw_hand >> bet;
        // cout << raw_hand << endl;

        tgame new_game;
        new_game.bet = bet;
        new_game.hand.type = get_type(raw_hand);
        new_game.hand.value = convert_hand(raw_hand);
        result.push_back(new_game);
    }

    return result;
}

tinput parse_input(istream& is) {
    return parse_input(move(is));
}

stringstream test{R"(32T3K 765
T55J5 684
KK677 28
KTJJT 220
QQQJA 483
)"};

int64_t solution(const tinput& inp) {
    int64_t result = 0;

    std::set<tgame> games(inp.begin(), inp.end());

    int rank = 0;
    for (const auto game : games) {
        result += game.bet * (++rank);
    }

    return result;
}

int main() {
    cout << solution(parse_input(test)) << endl;
    cout << solution(parse_input(cin)) << endl;
}

