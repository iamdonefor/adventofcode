#include "all.h"
using namespace std;

const static int64_t MOD{20201227};
const static int64_t SUB{7};

int64_t find_loop(int64_t target) {
    int64_t v{1};
    int64_t loops{0};

    for (;;) {
        ++loops;
        v = (v * SUB) % MOD;
        if (v == target) {
            break;
        }
    }

    return loops;
}

int64_t loop(int64_t value, int64_t loops) {
    int64_t v{1};
    while(loops--) {
        v = (v * value) % MOD;
    }
    return v;
}

int main() {
    // int64_t card_pub = 5764801;
    // int64_t door_pub = 17807724;
    int64_t card_pub = 19774466;
    int64_t door_pub = 7290641;

    int64_t card_priv = find_loop(card_pub);
    int64_t door_priv = find_loop(door_pub);

    cout << loop(door_pub, card_priv) << endl;
    cout << loop(card_pub, door_priv) << endl;
}
