#include "all.h"
using namespace std;

bool trace{true};

class tintcode {
public:
    using tmemory = vector<int>;

    tintcode() {}
    tintcode(const tmemory& memory) : memory_(memory) {}
    tintcode(const string& s) {
        stringstream ss{s};
        from_stream(ss);
    }
    tintcode(istream& ss) {
        from_stream(ss);
    }
    const tmemory& run() {
        while (decode_and_execute());
        return memory_;
    }
    void patch(int offset, int value) {
        memory_[offset] = value;
    }
private:
    enum opcodes {
        ADD = 1,
        MUL = 2,
        IN = 3,
        OUT = 4,
        JT = 5,
        JF = 6,
        LT = 7,
        EQ = 8,
        BRENNSCHLUSS = 99,
    };
    enum pmodes {
        POSITION = 0,
        IMMEDIATE = 1,
    };

    void from_stream(istream& ss) {
        int value;
        char comma;
        while(ss >> value) {
            memory_.push_back(value);
            ss >> comma;
        }
    }

    template <int A>
    int& fetch(int op) {
        auto addr = ip + A;
        auto mode = ((op / 100) / static_cast<int>(pow(10, A - 1))) % 10;

        switch (mode) {
            case POSITION: {
                return memory_[memory_[addr]];
            }
            case IMMEDIATE: {
                return memory_[addr];
            }
            default: {
                assert(false);
            }
        }
    }

    bool decode_and_execute() {
        auto op = memory_[ip];
        auto opcode = op % 100;

        if (trace)
            cout << "ip: " << ip << ", " << opcode << endl;

        switch(opcode) {
            case BRENNSCHLUSS: { return false; }

            case ADD: {
                fetch<3>(op) = fetch<1>(op) + fetch<2>(op);
                ip += 4;
                break;
            }

            case MUL: {
                fetch<3>(op) = fetch<1>(op) * fetch<2>(op);
                ip += 4;
                break;
            }

            case IN: {
                fetch<1>(op) = 5;
                ip += 2;
                break;
            }

            case OUT: {
                cout << fetch<1>(op) << " ";
                ip += 2;
                break;
            }

            case JT:
            case JF: {
                auto value = fetch<1>(op);
                if ((opcode == JT && value != 0)
                 || (opcode == JF && value == 0)) {
                    ip = fetch<2>(op);
                } else {
                    ip += 3;
                }

                break;
            }

            case EQ:
            case LT: {
                int left = fetch<1>(op);
                int right = fetch<2>(op);

                if ((opcode == LT && left < right)
                 || (opcode == EQ && left == right)) {
                    fetch<3>(op) = 1;
                } else {
                    fetch<3>(op) = 0;
                }

                ip += 4;
                break;
            }
            
            default: {
                assert(false);
                break;
            }
        }

        return true;
    }

    int ip{0};
    vector<int> memory_;
};

ostream& operator<< (ostream& os, const vector<int>& vv) {
    bool first = true;
    for (const auto v : vv) {
        cout << (first ? "" : ", ") << v;
        first = false;
    }
    return os;
}

int main() {
    // tintcode ic("3,21,1008,21,8,20,1005,20,22,107,8,21,20,1006,20,31,1106,0,36,98,0,0,1002,21,125,20,4,20,1105,1,46,104,999,1105,1,46,1101,1000,1,20,4,20,1105,1,46,98,99");

    tintcode ic(cin);
    ic.run();
    cout << endl;
}
