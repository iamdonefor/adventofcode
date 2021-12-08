#include "all.h"

using namespace std;

class TSegment {
public:
    TSegment() = default;
    TSegment(string s) : Value(move(s))
    {
        sort(Value.begin(), Value.end());
    }

    int Size() const { return Value.size(); }

    TSegment operator- (const TSegment& other) const {
        vector<int> left(7, 0);
        for (const auto c : Value) { left[c - 'a'] = 1; }
        for (const auto c : other.Get()) { left[c - 'a'] -= 1; }

        string result;
        for (int i = 0; i < left.size(); ++i) {
            if (left[i] == 1) {
                result.push_back('a' + i);
            }
        }
        return TSegment(result);
    }

    bool operator== (const TSegment& other) const {
        return Value == other.Value;
    }

    bool operator< (const TSegment& other) const {
        return Value < other.Value;
    }

    string Get() const { return Value; }

    friend ostream& operator<< (ostream& os, const TSegment& s) {
        os << "{ " << s.Get() << " }";
        return os;
    }

private:
    string Value{};
};

map<int, TSegment> decipher_lines(const vector<string>& lines) {
    vector<TSegment> len5;
    vector<TSegment> len6;
    map<int, TSegment> known;

    for (const auto& s : lines) {
        switch (s.size()) {
            case 2:
                known[1] = TSegment(s);
                break;
            case 3:
                known[7] = TSegment(s);
                break;
            case 4:
                known[4] = TSegment(s);
                break;
            case 5:
                len5.emplace_back(s);
                break;
            case 6:
                len6.emplace_back(s);
                break;
            case 7:
                known[8] = TSegment(s);
                break;

        }
    }

    assert(len5.size() == 3);
    assert(len6.size() == 3);

    for (const auto& s : len6) {
        if ((s - known[1]).Size() == 5) {
            known[6] = s;
        }
    }

    for (const auto& s : len5) {
        if ((s - known[1]).Size() == 3) {
            known[3] = s;
            continue;
        }
        if ((known[6] - s).Size() == 1) {
            known[5] = s;
            continue;
        }
        if ((known[6] - s).Size() == 2) {
            known[2] = s;
            continue;
        }
        assert(false);
    }

    for (const auto& s : len6) {
        if (s == known[6]) {
            continue;
        }
        if ((s - known[5]).Size() == 1) {
            known[9] = s;
            continue;
        }
        if ((s - known[5]).Size() == 2) {
            known[0] = s;
            continue;
        }
        assert(false);
    }

    return known;
}

int decode(const string& ins) {
    stringstream input{ins};

    vector<string> segments(10);
    for (int i=0; i<10; ++i) {
        input >> segments[i];
    }

    const map<int, TSegment> encode_map = decipher_lines(segments);
    // for (const auto& [k,v] : encode_map) {
    //     cout << k << " : " << v << endl;
    // }

    map<TSegment, int> decode_map;
    for (const auto& [k,v] : encode_map) {
        decode_map[v] = k;
    }
    assert(decode_map.size() == 10);

    string s;
    input >> s; // " | "

    int result = 0;
    for (int i=0; i<4; ++i) {
        input >> s;
        result = (10* result) + decode_map.at(s);
    }

    return result;
}

bool verify() {
    return decode("be cfbegad cbdgef fgaecd cgeb fdcge agebfd fecdb fabcd edb | fdgacbe cefdb cefbgd gcbe")
        == 8394;
}

int main(int argc, char** argv) {
    verify();

    string s;
    vector<int> sums(10, 0);
    int sum_part1;
    int sum_part2;

    while (getline(cin, s)) {
        auto x = decode(s);
        sum_part2 += x;

        for (int i=0; i<4; ++i) {
            if (x % 10 == 1 || x % 10 == 4 || x % 10 == 7 || x % 10 == 8) {
                sum_part1++;
            }
            x /= 10;
        }
    }

    cout << "part1: " << sum_part1 << endl;
    cout << "part2: " << sum_part2 << endl;
}
