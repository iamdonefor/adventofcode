#include "../all.h"

int main() {
    string s;
    vector<int> elfes(1, 0);

    while (getline(cin, s)) {
        if (s.empty()) {
            elfes.push_back(0);
            continue;
        }
        elfes.back() += stol(s);
    }

    sort(elfes.begin(), elfes.end(), std::greater<int>());
    for (int s=0, i=0; i<3; ++i) {
        s += elfes[i];
        cout << elfes[i] << " " << s << endl;
    }
}
