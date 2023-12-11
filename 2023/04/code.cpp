#include "all.h"

using namespace std;
using namespace advent;

struct tcard {
    int card;
    vector<int> winners;
    vector<int> guesses;
};

using tinput = vector<tcard>;

tinput parse_input(istream&& is) {
    tinput result;
    int card;
    string winners(1000, 0);
    string guesses(1000, 0);

    for (string s; getline(is, s); ) {
        if (sscanf(s.data(), "Card %d: %[0-9 ] | %[0-9 ]", &card, winners.data(), guesses.data()) == 3) {
            // cout << card << " " << winners << " " << guesses << endl;
        } else {
             throw runtime_error("invalid input");
        }

        tcard newcard{.card = card};
        stringstream winstream{winners};
        while (winstream) {
            int x;
            winstream >> x;
            if (x > 0) { newcard.winners.push_back(x); }
        }

        stringstream guessstream{guesses};
        while (guessstream) {
            int x;
            guessstream >> x;
            if (x > 0) { newcard.guesses.push_back(x); }
        }

        // cout << newcard.winners.size() << " " << newcard.guesses.size() << endl;
        result.push_back(newcard);
    }

    return result;
}

tinput parse_input(istream& is) {
    return parse_input(move(is));
}

int get_score(const tinput& input) {
    int score = 0;
    for (const auto& card : input) {
        unordered_set wins(card.winners.begin(), card.winners.end());
        int card_score = 0;
        for (const auto n : card.guesses) {
            if (wins.count(n) > 0) {
                card_score = card_score ? card_score * 2 : 1;
            }
        }

        score += card_score;
    }
    return score;
}

int get_cards(const tinput& input) {
    vector<int> total_wins(input.size(), 1);

    for (int i=0; i<input.size(); ++i) {
        const auto& card = input[i];
        unordered_set wins(card.winners.begin(), card.winners.end());
        int card_score = 0;
        for (const auto n : card.guesses) {
            if (wins.count(n) > 0) {
                ++card_score;
            }
        }
        // cout << "card: " << i << " " << card_score << endl;
        for (int j=1; j<=card_score; ++j) {
            total_wins[i + j] += total_wins[i];
        }
    }

    return accumulate(total_wins.begin(), total_wins.end(), 0);
}

stringstream test{R"(Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19
Card 3:  1 21 53 59 44 | 69 82 63 72 16 21 14  1
Card 4: 41 92 73 84 69 | 59 84 76 51 58  5 54 83
Card 5: 87 83 26 28 32 | 88 30 70 12 93 22 82 36
Card 6: 31 18 13 56 72 | 74 77 10 23 35 67 36 11
)"};

int main() {
    // cout << get_score(parse_input(test)) << endl;
    // cout << get_score(parse_input(cin)) << endl;
    cout << get_cards(parse_input(test)) << endl;
    cout << get_cards(parse_input(cin)) << endl;
}

