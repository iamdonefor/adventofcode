#include "all.h"

using namespace std;
using namespace advent;


struct tracerecord {
    int64_t time;
    int64_t record;
};

using tinput = vector<tracerecord>;
tinput test{
    {7, 9},
    {15, 40},
    {30, 200},
};

tinput prod{
    {54, 239},
    {70, 1142},
    {82, 1295},
    {75, 1253},
};

int64_t race_distance(int64_t hold_time, int64_t total_time) {
    if (hold_time == 0) { return 0; }
    const auto speed = hold_time;

    return (total_time - hold_time) * speed;
}

int64_t ways_to_win(const tracerecord& race) {
    int64_t ways = 0;
    for (int64_t i=0; i<race.time; ++i) {
        if (race_distance(i, race.time) > race.record) {
            ++ways;
        }
    }
    return ways;
}

int64_t solution1(const tinput& input) {
    int64_t result = 1;

    for (const auto& inp : input) {
        result *= ways_to_win(inp);
    }

    return result;
}

int64_t solve_race_binary(const tracerecord& race) {
    int64_t mid = race.time / 2;
    assert(race_distance(mid, race.time) > race.record);

    // left border
    int64_t left = 0;
    int64_t right = mid;
    while (left != right) {
        int64_t half = (left + right) / 2;
        if (race_distance(half, race.time) > race.record) {
            right = half;
        } else {
            left = half + 1;
        }
    }
    const auto true_left = left;
    // cout << left << " ";

    left = mid;
    right = race.time;

    while (left != right) {
        int64_t half = (left + right + 1) / 2;
        if (race_distance(half, race.time) > race.record) {
            left = half;
        } else {
            right = half - 1;
        }
    }
    // cout << left << endl;

    return left - true_left + 1;
}

int main() {
    cout << solution1(test) << endl;
    cout << solution1(prod) << endl;
    cout << solve_race_binary({71530, 940200}) << endl;
    cout << solve_race_binary({54708275, 239114212951253}) << endl;
}
