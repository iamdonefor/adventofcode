#include "../all.h"

using namespace std;

constexpr auto is(const char* s) {
    int result = 0;
    while (*s) { result = (100 * result + *s++) % 997; }
    return result;
}

int main() {
    int depth = 0;
    int true_depth = 0;
    int aim = 0;
    int x = 0;

    while(cin) {
        string command;
        int value;

        cin >> command >> value;
        switch(is(command.c_str())) {
            case (is("forward")): {
                x += value;
                true_depth = max(true_depth + aim * value, 0);
                break;
            }
            case (is("up")): {
                depth = max(depth - value, 0);
                aim -= value;
                break;
            }
            case (is("down")): {
                depth += value;
                aim += value;
                break;
            }
            default:
                break;
        }
    }

    cout << "depth: " << depth << ", x: " << x << ", answer is: " << depth * x << endl;
    cout << "true_depth: " << true_depth << ", x: " << x << ", answer is: " << true_depth * x << endl;
}
