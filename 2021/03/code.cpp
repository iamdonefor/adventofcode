#include "../all.h"

using namespace std;

int get_by_bit_criteria(const vector<int>& in, int bit, bool most) {
    vector<int> ones;
    vector<int> zeroes;

    if (in.size() == 1) {
        return in.front();
    }

    auto mask = (1 << bit);
    for (const auto i : in) {
        if (i & mask) {
            ones.push_back(i);
        } else {
            zeroes.push_back(i);
        }
    }

    if ((most && ones.size() < zeroes.size())
        || (!most && zeroes.size() <= ones.size())) {
        return get_by_bit_criteria(zeroes, bit - 1, most);
    } else {
        return get_by_bit_criteria(ones, bit - 1, most);
    }
}

int main() {
    const int M{12};
    vector<int> c(M, 0);
    vector<int> input;

    while(cin) {
        string value;
        cin >> value;

        if (value.empty()) {
            continue;
        }

        auto v = stol(value, 0, 2);
        input.push_back(v);
        for (int i=M-1; i>=0; --i) {
            c[i] += (v & 1) ? 1 : -1;
            v >>= 1;
        }
    }

    int gamma = 0;
    for (int i=0; i<M; ++i) {
        gamma <<= 1;
        if (c[i] >= 0) {
            gamma |= 1;
        }
    }

    int mask = (1 << M) - 1;
    int eps = gamma ^ mask;
    cout << gamma << " " << eps << " " << " " << mask << " " << "answer: "<< gamma * eps << endl;

    int oxy = get_by_bit_criteria(input, M-1, true);
    int co2 = get_by_bit_criteria(input, M-1, false);

    cout << oxy << " " << co2 << ", answer: " << oxy * co2 << endl;
}
