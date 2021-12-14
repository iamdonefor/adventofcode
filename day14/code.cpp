#include "all.h"
using namespace std;

using TPolyMap = map<pair<char, char>, char>;

vector<int64_t> polymerize(const string& poly, const TPolyMap& m, int steps) {
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

        int64_t s{0};
        for (const auto it : next_poly_pairs) {
            s += it.second;
        }
        cout << "length after step: " << i + 1 << " = " << s + 1 << endl;

        poly_pairs = move(next_poly_pairs);
    }

    vector<int64_t> counter(26);
    for (const auto& it : poly_pairs) {
        counter[it.first.first - 'A'] += it.second;
        counter[it.first.second - 'A'] += it.second;
    }

    return counter;
}

int64_t solve(const string& poly, const TPolyMap& m, int steps=10) {
    auto counter = polymerize(poly, m, steps);
    counter[poly.front() - 'A'] += 1;
    counter[poly.back() - 'A'] += 1;

    for (auto& i : counter) {
        i /= 2;
    }

    auto newend = remove(counter.begin(), counter.end(), 0);
    auto [mmin, mmax] = minmax_element(counter.begin(), newend);
    return *mmax - *mmin;
}

pair<string, TPolyMap> parse_input(istream& in) {
    string poly;
    TPolyMap m;

    getline(in, poly);

    string s;
    while (getline(in, s)) {
        char from0, from1, to;
        if (sscanf(s.data(), "%c%c -> %s", &from0, &from1, &to) == 3) {
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
