#include "all.h"

using namespace std;

int main() {
    string s;
    vector<int> data;

    while (getline(cin, s)) {
        data.push_back(stol(s));
    }

    int sc{0}, wc{0};

    for (int i=1; i<data.size(); ++i) {
        if (data[i] > data[i-1]) {
            ++sc;
        }

        if (i >= 3) {
            if (data[i] > data[i-3]) {
                ++wc;
            }
        }
    }

    cout << "single: " << sc << ", window: " << wc << endl;
}
