#include "all.h"
using namespace std;

using TPolyMap = map<pair<char, char>, char>;

string polymerize(const string& poly, const TPolyMap& m) {
    string result;
    result.reserve(poly.size() * 2);

    for (int i=1; i < poly.size(); ++i) {
        result.push_back(poly[i-1]);
        result.push_back(m.at({poly[i-1], poly[i]}));
    }

    result.push_back(poly.back());
    return result;
}

int part1(const string& poly, const TPolyMap& m) {
    string result{poly};
    for (int i=0; i<10; ++i) {
        result = polymerize(result, m);
        cout << i+1 << " : " << result.size() << " " << result << endl;
    }

    vector<int> counter(26, 0);
    for (const auto c : result) {
        counter[c - 'A'] += 1;
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
    auto result1 = part1(poly, m);

    return result1 == 1588;
}

int main() {
    assert(verify());

    auto [poly, m] = parse_input(cin);
    cout << "part1: " << part1(poly, m) << endl;
}
