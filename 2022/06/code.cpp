#include "all.h"

using tinput = string;

tinput parseInput(istream& is) {
    tinput result;

    for (string s; getline(is, s); ) {
        result = s;
        // if (sscanf(s.data(), "%d,%d", &x, &y) == 2)
    }

    return result;
}

int findWindow(const string& s, int diffCount) {
    vector<int> state(256, 0);
    int diffChars{0};

    for (int i=0; i<s.size(); ++i) {
        if (!state[s[i]]++) {
            ++diffChars;
        }
        if (i >= diffCount && !(--state[s[i-diffCount]])) {
            --diffChars;
        }
        if (diffChars == diffCount) {
            return i+1;
        }
    }

    return -1;
}

stringstream test{R"(zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw)"};

int main() {
    cout << findWindow(parseInput(test), 14) << endl;
    cout << findWindow(parseInput(cin), 14) << endl;
}
