#include "all.h"

using namespace std;

class TLanternFish {
public:
    const static int NewFishDelay{8};
    const static int DefaultSpawnDelay{6};

    TLanternFish() : TLanternFish(NewFishDelay) {}
    TLanternFish(int initDays) : DaysToSpawn(initDays) {}

    int DayPassed() { //1 if new fish shoul be spawned, 0 otherwize
        auto hadSpawned = DaysToSpawn-- ? 0 : 1;
        if (DaysToSpawn < 0) {
            DaysToSpawn = DefaultSpawnDelay;
        }
        return hadSpawned;
    }

private:
    int DaysToSpawn;
};

// oopsie woopsie, so quadratic
int simulate(const vector<int>& initPopulation, int days = 80) {
    vector<TLanternFish> fishes;

    for (const auto& d : initPopulation) {
        fishes.emplace_back(d);
    }

    for (int d = 0; d < days; ++d) {
        cout << "day: " << d << " " << ", fishes: " << fishes.size() << endl;
        int newFishes = 0;
        for (auto& fish : fishes) {
            newFishes += fish.DayPassed();
        }
        for (int i=0; i<newFishes; ++i) {
            fishes.emplace_back();
        }
    }

    return fishes.size();
}

int64_t simulate_fast(const vector<int>& initPopulation, int daysToGo = 80) {
    vector<int64_t> fishTimers(10, 0);

    const static int newSpawnTime{8};
    const static int spawnTime{6};

    for (const auto i : initPopulation) {
        ++fishTimers[i];
    }

    int64_t totalFishes;
    for (int d = 0; d < daysToGo; ++d) {
        fishTimers[newSpawnTime + 1] = fishTimers.front();
        fishTimers[spawnTime + 1] += fishTimers.front();

        for (int i=0; i<=newSpawnTime; ++i) {
            swap(fishTimers[i], fishTimers[i+1]);
        }

        totalFishes = accumulate(fishTimers.begin(), fishTimers.begin() + newSpawnTime + 1, static_cast<int64_t>(0));
        // cout << "End of day: " << d+1 << ", total fishes: " << totalFishes << endl;
    }

    return totalFishes;
}

bool verify() {
    const vector<int> init{3,4,3,1,2};

    return 5934 == simulate_fast(init) // part1
        && 26984457539ll == simulate_fast(init, 256); // part2
}

int main(int argc, char** argv) {
    assert(verify());

    vector<int> init;
    while (cin) {
        int v; char c;
        cin >> v >> c;
        init.push_back(v);
    }

    int days = argc > 1 ? stol(argv[1]) : 80;

    cout << "Total fishes spawned: " << simulate_fast(init, days) << endl;
}
