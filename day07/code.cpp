#include "all.h"

using namespace std;

int get_correction(int n) {
    return n * (n + 1) / 2;
}

int try_depth(const vector<int>& depths, int depth) {
    int total{0};
    for (const auto d : depths) {
        total += get_correction(abs(depth - d));
    }
    return total;
}

pair<int, int> get_median(vector<int> data) {
    sort(data.begin(), data.end());
    int min_fuel = numeric_limits<int>::max();
    int min_depth = 0;
    for (int i=data.front(); i<data.back(); ++i) {
        auto fuel = try_depth(data, i);
        if (min_fuel > fuel) {
            min_fuel = fuel;
            min_depth = i;
        }
        cout << i << " : " << fuel << endl;
    }

    return {min_depth, min_fuel};
}

bool verify() {
    vector<int> init{16,1,2,0,4,2,7,1,2,14};
    return 168 == get_median(init).second;
}

// actually part1 is median and part2 is the mean value
// but brute force gets in time
int main(int argc, char** argv) {
    assert(verify());

    vector<int> init;
    while (cin) {
        int v; char c;
        cin >> v >> c;
        init.push_back(v);
    }
    cout << init.size() << endl;
    // int days = argc > 1 ? stol(argv[1]) : 80;
    auto result = get_median(init);
    cout << "Medium of data: " << result.second << ", at depth: " << result.first << endl;
}
