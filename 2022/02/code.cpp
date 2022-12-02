#include "all.h"
using namespace std;

namespace {
    enum EFigure {
        ROCK = 1,
        LOSE = 1,
        PAPER = 2,
        DRAW = 2,
        SCISSORS = 3,
        WIN = 3,
    };

    const static uint64_t WIN_SCORE{6};
    const static uint64_t DRAW_SCORE{3};
    const static uint64_t LOSE_SCORE{0};

    using TGuide = vector<pair<EFigure, EFigure>>;
}

EFigure decodeFigure(char c) {
    if (c == 'A' || c == 'X') {
        return EFigure::ROCK;
    }
    if (c == 'B' || c == 'Y') {
        return EFigure::PAPER;
    }
    if (c == 'C' || c == 'Z') {
        return EFigure::SCISSORS;
    }

    throw runtime_error("wrong data");
}

uint64_t getGameScore(EFigure opp, EFigure mine) {
    if (opp == mine) {
        return static_cast<uint64_t>(mine) + DRAW_SCORE;
    }

    switch (opp) {
        case EFigure::ROCK: {
            return static_cast<uint64_t>(mine) + (mine == EFigure::PAPER ? WIN_SCORE : LOSE);
        }
        case EFigure::PAPER: {
            return static_cast<uint64_t>(mine) + (mine == EFigure::SCISSORS ? WIN_SCORE : LOSE_SCORE);
        }
        case EFigure::SCISSORS: {
            return static_cast<uint64_t>(mine) + (mine == EFigure::ROCK ? WIN_SCORE : LOSE_SCORE);
        }
    }

    throw logic_error("wrong data");
}

EFigure selectCorrespondingFigure(EFigure figure, EFigure result) {
    if (result == EFigure::DRAW) {
        return figure;
    }

    switch (figure) {
        case EFigure::ROCK: {
            return (result == EFigure::LOSE ? EFigure::SCISSORS : EFigure::PAPER);
        }
        case EFigure::PAPER: {
            return (result == EFigure::LOSE ? EFigure::ROCK : EFigure::SCISSORS);
        }
        case EFigure::SCISSORS: {
            return (result == EFigure::LOSE ? EFigure::PAPER : EFigure::ROCK);
        }
    }

    throw logic_error("selectCorrespondingFigure");
}

uint64_t getUpdatedGameScore(EFigure opp, EFigure mine) {
    switch (mine) {
        case EFigure::LOSE: {
            return LOSE_SCORE + static_cast<uint64_t>(selectCorrespondingFigure(opp, LOSE));
        }
        case EFigure::DRAW: {
            return DRAW_SCORE + static_cast<uint64_t>(selectCorrespondingFigure(opp, DRAW));
        }
        case EFigure::WIN: {
            return WIN_SCORE + static_cast<uint64_t>(selectCorrespondingFigure(opp, WIN));
        }
    }

    throw logic_error("getUpdatedGameScore");
}

uint64_t getTournamentScore(const TGuide& guide, function<uint64_t (EFigure, EFigure)> scorer) {
    uint64_t result{0};

    for (const auto& game : guide) {
        result += scorer(game.first, game.second);
    }

    return result;
}

TGuide parse_input(istream& in) {
    TGuide result;

    for (string s; getline(in, s); ) {
        char opp, mine;
        if (sscanf(s.data(), "%c %c", &opp, &mine) == 2) {
            result.push_back({decodeFigure(opp), decodeFigure(mine)});
            continue;
        }
    }

    return result;
}

stringstream test{R"(
A Y
B X
C Z
)"};

int main() {
    // cout << getTournamentScore(parse_input(test), getGameScore) << endl;
    // cout << getTournamentScore(parse_input(cin), getGameScore) << endl;
    cout << getTournamentScore(parse_input(test), getUpdatedGameScore) << endl;
    cout << getTournamentScore(parse_input(cin), getUpdatedGameScore) << endl;
}
