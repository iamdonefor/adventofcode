#include "all.h"
using namespace std;

const int DECKSIZE{10007};

enum class eaction {
    CUT,
    DEAL,
    STACK,
};

using tdeck = vector<int>;
using taction = pair<eaction, int>;
using tactions = vector<taction>;

void stack(tdeck& deck) {
    reverse(deck.begin(), deck.end());
}

void cut(tdeck& deck, int param) {
    if (param < 0) {
        param = deck.size() + param;
    }

    tdeck temp(deck.begin(), deck.begin() + param);

    auto end = move(deck.begin() + param, deck.end(), deck.begin());
    move(temp.begin(), temp.end(), end);
}

void deal(tdeck& deck, int param) {
    tdeck new_deck(deck.size());

    int pos = 0;
    for (int i=0; i<deck.size(); ++i) {
        new_deck[pos] = deck[i];
        pos = (pos + param) % deck.size();
    }

    deck = move(new_deck);
}

tactions parse (istream& in) {
    tactions result;
    int param;

    for (string s; getline(in, s);) {
        if (s == "deal into new stack") {
            result.push_back({eaction::STACK, 0});
        } else if (sscanf(s.data(), "deal with increment %d", &param) == 1) {
            result.push_back({eaction::DEAL, param});
        } else if (sscanf(s.data(), "cut %d", &param) == 1) {
            result.push_back({eaction::CUT, param});
        } else {
            assert(false);
        }
    }

    return result;
}

tdeck shuffle(const tactions& actions, int decksize = DECKSIZE) {
    tdeck deck(decksize);
    iota(deck.begin(), deck.end(), 0);

    for (const auto& a : actions) {
        if (a.first == eaction::STACK) {
            ::stack(deck);
        } else if (a.first == eaction::CUT) {
            cut(deck, a.second);
        } else if (a.first == eaction::DEAL) {
            deal(deck, a.second);
        }
    }

    return deck;
}

ostream& operator<< (ostream& os, const tdeck& v) {
    bool first = true;
    for (const auto i : v) {
        os << (first ? "" : " ") << i;
        first = false;
    }
    return os;
}


int main() {

    stringstream ss(R"==(deal into new stack
cut -2
deal with increment 7
cut 8
cut -4
deal with increment 7
cut 3
deal with increment 9
deal with increment 3
cut -1
)==");

    auto actions = parse(cin);
    auto deck = ::shuffle(actions);

    for (int i=0; i<deck.size(); ++i) {
        if (deck[i] == 2019) {
            cout << "part1: "<< i << endl;
            break;
        }
    }
}
