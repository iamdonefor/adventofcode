#include "all.h"

namespace monkey_helpers {
    template <uint64_t I>
    uint64_t add(uint64_t i) {
        return i + I;
    }

    template <uint64_t BY>
    int mul_by(uint64_t i) {
        return i * BY;
    }

    uint64_t square(uint64_t i) {
        return i * i;
    }
}

struct tmonkey {
    deque<uint64_t> items;
    function<uint64_t(uint64_t)> alert_adj;

    uint64_t check;
    vector<int> passes_to;

    uint64_t lookups{0};
};

#include "monkeys.h"

uint64_t solve(vector<tmonkey> monkeys, int nrounds, bool relief) {
    const uint64_t modulo = accumulate(monkeys.begin(), monkeys.end(), 1,
        [](const auto& l, const auto& r) {
            return l * r.check;
        }
    );

    for (int round = 0; round < nrounds; ++round) {
        for (auto& monkey : monkeys) {
            for (; !monkey.items.empty() ;) {
                auto new_item = monkey.alert_adj(monkey.items.front()) % modulo;
                monkey.items.pop_front();
                if (relief) { new_item /= 3u; }

                auto monkey_to_pass = (new_item % monkey.check == 0)
                    ? monkey.passes_to[0]
                    : monkey.passes_to[1]
                    ;

                monkeys[monkey_to_pass].items.push_back(new_item);
                ++monkey.lookups;
            }
        }
    }

    // for (int i=0; i<monkeys.size(); ++i) {
    //     cout << i << " : " << monkeys[i].lookups << endl;
    // }

    partial_sort(monkeys.begin(), monkeys.end(), monkeys.begin() + 2,
        [](const auto& lm, const auto& rm) {
            return lm.lookups > rm.lookups;
        });
    return monkeys[0].lookups * monkeys[1].lookups;
}

int main() {
    cout << solve(test_monkeys, 20, true) << endl;
    cout << solve(prod_monkeys, 20, true) << endl;
    cout << solve(test_monkeys, 10000, false) << endl;
    cout << solve(prod_monkeys, 10000, false) << endl;
}
