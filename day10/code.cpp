#include "../all.h"
using namespace std;

using TInput = vector<string>;

struct TCloser { char c; int corrupt_score; };
const map<char, TCloser> brothers{
        {')', {'(', 3} },
        {']', {'[', 57} },
        {'}', {'{', 1197} },
        {'>', {'<', 25137} }
};
const unordered_map<char, int> ac_scores {
    {'(', 1},
    {'[', 2},
    {'{', 3},
    {'<', 4},
};

pair<int, int64_t> get_line_score(const string& s) {
    stack<char> state;

    for (const auto c : s) {
        switch (c) {
            case '(':
            case '[':
            case '{':
            case '<':
                state.push(c);
                break;

            case ')':
            case ']':
            case '}':
            case '>':
                const auto& b = brothers.at(c);
                if (state.top() == b.c) {
                    state.pop();
                } else {
                    return {b.corrupt_score, 0};
                }
                break;
        }
    }

    int64_t ac_score = 0; // nice catch gyus, the answer overflows int
    while(!state.empty()) {
        ac_score *= 5;
        ac_score += ac_scores.at(state.top());
        state.pop();
    }

    return {0, ac_score};
}

int part1(TInput input) {
    return accumulate(input.begin(), input.end(), 0, [](int sum, const string& s) {
        return sum + get_line_score(s).first;
    });
}

int64_t part2(TInput input) {
    vector<int64_t> autocomplete_scores;
    for (const auto& s : input) {
        auto score = get_line_score(s);
        if (score.first == 0) {
            autocomplete_scores.push_back(score.second);
        }
    }

    sort(autocomplete_scores.begin(), autocomplete_scores.end());

    return autocomplete_scores[autocomplete_scores.size() / 2];
}

TInput parse_input(istream& in) {
    vector<string> result;
    string s;
    while (getline(in, s)) {
        result.emplace_back(move(s));
    }
    return result;
}

bool verify() {
    stringstream input_stream{R"--([({(<(())[]>[[{[]{<()<>>
[(()[<>])]({[<{<<[]>>(
{([(<{}[<>[]}>{[]{[(<()>
(((({<>}<{<{<>}{[]{[]{}
[[<[([]))<([[{}[[()]]]
[{[{({}]{}}([{[{{{}}([]
{<[[]]>}<{[{[{[]{()[[[]
[<(<(<(<{}))><([]([]()
<{([([[(<>()){}]>(<<{{
<{([{{}}[<[[[<>{}]]]>[]])--"};

    auto input = parse_input(input_stream);
    return 26397 == part1(input) && 288957 == part2(input);
}

int main() {
    assert(verify());

    auto input = parse_input(cin);
    cout << "Part1: " << part1(input) << endl;
    cout << "Part2: " << part2(input) << endl;
}
