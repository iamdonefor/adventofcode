#include "all.h"

using namespace std;
using namespace advent;

using tinput = vector<int>;

tinput parse_input(istream&& is) {
    tinput result;

    for (string s; getline(is, s); ) {
        if (s.empty()) { continue; }
        // if (sscanf(s.data(), "%ld,%ld", &x, &y) == 2) {
        // } else {
        //      throw runtime_error("invalid input");
        // }
    }

    return result;
}

tinput parse_input(istream& is) {
    return parse_input(move(is));
}

stringstream test{R"(
)"};

int64_t solution1(const tinput& data) {
    return 0;
}

int main() {
    const auto& testi = parse_input(test);
    cout << solution1(testi) << endl;

    const auto& cini = parse_input(cin);
    cout << solution1(cini) << endl;
}
