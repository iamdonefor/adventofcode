 #include "all.h"

using namespace std;
using namespace advent;

using tinput = vector<vector<int>>;

tinput parse_input(istream&& is) {
    tinput result;

    for (string s; getline(is, s); ) {
        if (s.empty()) { continue; }
        
        result.push_back({});
        stringstream ss{s};

        while (ss && ss.peek() != EOF) {
            int x;
            ss >> x;
            result.back().push_back(x);
        }
    }

    return result;
}

tinput parse_input(istream& is) {
    return parse_input(move(is));
}

stringstream test{R"(0 3 6 9 12 15
1 3 6 10 15 21
10 13 16 21 30 45
)"};

stringstream test1{R"(
10 13 16 21 30 45
)"};


int64_t calculate_seq(vector<int> input, bool fronts=false) {
    vector<int> output;
    vector<int> backs;

    for (;;) {
        if (fronts) {
            backs.push_back(input.front());
        } else {
            backs.push_back(input.back());
        }
        for (int i=1; i<input.size(); ++i) {
            output.push_back(input[i] - input[i-1]);
        }
        if (all_of(output.begin(), output.end(), [](const auto& x) { return x == 0; })) {
            int64_t result = 0;
            if (fronts) {
                result = backs.back();
                for (int i=backs.size() - 2; i >= 0; --i) {
                    result = backs[i] - result;
                }
            } else {
                for (const auto& b: backs) {
                    result += b;
                }
            }
            // cout << result << endl;
            return result;
        }
        input = move(output);
    }

    return 0;
}

int64_t solution1(const tinput& data) {
    int64_t result{0};

    for (const auto& d : data) {
        result += calculate_seq(d);
    }

    return result;
}


int64_t solution2(const tinput& data) {
    int64_t result{0};

    for (const auto& d : data) {
        result += calculate_seq(d, true);
    }

    return result;
}

int main() {
    const auto& testi = parse_input(test);
    const auto& cini = parse_input(cin);

    cout << solution1(testi) << endl;
    cout << solution1(cini) << endl;

    cout << solution2(testi) << endl;
    cout << solution2(cini) << endl;
}