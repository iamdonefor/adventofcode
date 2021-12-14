#include "all.h"
using namespace std;

using TPolyMap = map<pair<char, char>, char>;
using TPolyPairs = map<pair<char, char>, int64_t>;

TPolyPairs polymerize(const string& poly, const TPolyMap& m, int steps) {
    map<pair<char, char>, int64_t> poly_pairs;

    for (int i=1; i<poly.size(); ++i) {
        ++poly_pairs[{poly[i-1], poly[i]}];
    }

    for (int i=0; i<steps; ++i) {
        map<pair<char, char>, int64_t> next_poly_pairs;

        for (const auto& [p, count] : poly_pairs) {
            auto midc = m.at(p);
            next_poly_pairs[{p.first, midc}] += count;
            next_poly_pairs[{midc, p.second}] += count;
        }

        poly_pairs = move(next_poly_pairs);
    }

    return poly_pairs;
}

vector<int64_t> count_letters(const string& poly, const TPolyPairs& pairs) {
    vector<int64_t> counter(26, 0);

    for (const auto& it : pairs) {
        counter[it.first.first - 'A'] += it.second;
        counter[it.first.second - 'A'] += it.second;
    }

    counter[poly.front() - 'A'] += 1;
    counter[poly.back() - 'A'] += 1;

    for (auto& i : counter) { i /= 2; }

    return counter;
}

int64_t solve(const string& poly, const TPolyMap& m, int steps=10) {
    auto poly_pairs = polymerize(poly, m, steps);
    auto counter = count_letters(poly, poly_pairs);

    counter.erase(remove(counter.begin(), counter.end(), 0), counter.end());
    auto [mmin, mmax] = minmax_element(counter.begin(), counter.end());

    return *mmax - *mmin;
}

pair<string, TPolyMap> parse_input(istream& in) {
    string poly;
    TPolyMap m;

    getline(in, poly);

    string s;
    while (getline(in, s)) {
        char from0, from1, to;
        if (sscanf(s.data(), "%c%c -> %c", &from0, &from1, &to) == 3) {
            m[{from0, from1}] = to;
            continue;
        }
    }

    return {poly, m};
}

bool verify() {
    stringstream input_stream{R"===(NNCB

CH -> B
HH -> N
CB -> H
NH -> C
HB -> C
HC -> B
HN -> C
NN -> C
BH -> H
NC -> B
NB -> B
BN -> B
BB -> N
BC -> B
CC -> N
CN -> C)==="};

    auto [poly, m] = parse_input(input_stream);
    auto result1 = solve(poly, m);
    auto result2 = solve(poly, m, 40);

    return result1 == 1588 && result2 == 2188189693529;
}

int main() {
    assert(verify());

    auto [poly, m] = parse_input(cin);
    cout << "part1: " << solve(poly, m) << endl;
    cout << "part2: " << solve(poly, m, 40) << endl;
}
