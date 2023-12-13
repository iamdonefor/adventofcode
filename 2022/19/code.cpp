#include "all.h"

using namespace std;
using namespace advent;


enum erobot {
    ore,
    clay,
    obsidian,
    geode,
};

using tores = array<int64_t, 4>;
using trobots = array<int, 4>;

struct tbleuprint {
    int index;
    unordered_map<erobot, tores> costs;
};

struct tstate {
    int ts;
    trobots robots;
    tores ores;
};

using tinput = vector<tbleuprint>;

const int TIME{24};

tinput parse_input(istream&& is) {
    tinput result;

    for (string s; getline(is, s); ) {
        if (s.empty()) { continue; }
        result.push_back({});

        int idx, o1, o2, o3, c1, o4, b1;
        if (sscanf(s.data(), "Blueprint %d: Each ore robot costs %d ore. Each clay robot costs %d ore. Each obsidian robot costs %d ore and %d clay. Each geode robot costs %d ore and %d obsidian.",
            &idx, &o1, &o2, &o3, &c1, &o4, &b1) == 7) {
        } else {
             throw runtime_error("invalid input");
        }

        result.back().index = idx;
        result.back().costs[ore] = {o1, 0, 0, 0};
        result.back().costs[clay] = {o2, 0, 0, 0};
        result.back().costs[obsidian] = {o3, c1, 0, 0};
        result.back().costs[geode] = {o4, 0, b1, 0};
    }

    return result;
}

tinput parse_input(istream& is) {
    return parse_input(move(is));
}

optional<tores> try_robot(erobot rtype, const tbleuprint& bp, tores ores) {
    const auto& need_ores = bp.costs.at(rtype);
    for (int i=0; i<4; ++i) {
        if (need_ores[i] > ores[i]) {
            return nullopt;
        }
    }

    tores result = ores;
    for (int i=0; i<4; ++i) {
        result[i] -= need_ores[i];
    }

    return result;
}

int64_t evaluate_blueprint(const tbleuprint& bp) {
    vector<tstate> states = {{ -1, {1, 0, 0, 0}, {0, 0, 0, 0} }};

    for (int time = 0; time < TIME; ++time) {
        vector<tstate> next_states;

        for (auto& state : states) {
            auto ores = state.ores;
            auto robots = state.robots;

            for (int i=0; i<4; ++i) {
                ores[i] += robots[i];
            }

            next_states.push_back(tstate{ time, robots, ores });

            if (auto result = try_robot(ore, bp, ores); result) {
                ++robots[ore];
                next_states.push_back(tstate{ time, robots, *result });
                --robots[ore];
            }

            if (auto result = try_robot(clay, bp, ores); result) {
                ++robots[clay];
                next_states.push_back(tstate{ time, robots, *result });
                --robots[clay];
            }

            if (auto result = try_robot(obsidian, bp, ores); result) {
                ++robots[obsidian];
                next_states.push_back(tstate{ time, robots, *result });
                --robots[obsidian];
            }

            if (auto result = try_robot(geode, bp, ores); result) {
                ++robots[geode];
                next_states.push_back(tstate{ time, robots, *result });
                --robots[geode];
            }
        }

        cout << time << " " << states.size() << endl;
        states = move(next_states);
    }

    return 0;
}

stringstream test{R"(Blueprint 1: Each ore robot costs 4 ore. Each clay robot costs 2 ore. Each obsidian robot costs 3 ore and 14 clay. Each geode robot costs 2 ore and 7 obsidian.
Blueprint 2: Each ore robot costs 2 ore. Each clay robot costs 3 ore. Each obsidian robot costs 3 ore and 8 clay. Each geode robot costs 3 ore and 12 obsidian.
)"};

int64_t solution1(const tinput& data) {
    evaluate_blueprint(data[0]);
    return 0;
}

int main() {
    const auto& testi = parse_input(test);
    cout << solution1(testi) << endl;

    // const auto& cini = parse_input(cin);
    // cout << solution1(cini) << endl;
}
