#include "all.h"
using namespace std;
#include <fstream>
#include "td.h"

class tintcode {
public:
    using taddr = int;
    using tvalue = int64_t;
    using tmemory = unordered_map<taddr, tvalue>;

    tintcode() : tintcode(string{}) {}
    tintcode(const string& s, istream& is = cin, ostream& os = cout)
    : is_(is)
    , os_(os)
    {
        stringstream ss{s};
        from_stream(ss);
    }
    tintcode(istream& ss, istream& is = cin, ostream& os = cout)
    : is_(is)
    , os_(os)
    {
        from_stream(ss);
    }
    const tmemory& run() {
        while (decode_and_execute());
        return memory_;
    }
    tintcode& patch(taddr offset, tvalue value) {
        memory_[offset] = value;
        return *this;
    }
    tintcode& trace(int level = 7) {
        trace_ = level;
        return *this;
    }
    tintcode& ascii() {
        ascii_ = true;
        return *this;
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
        REL = 9,
        BRENNSCHLUSS = 99,
    };

    enum pmodes {
        POSITION = 0,
        IMMEDIATE = 1,
        RELATIVE = 2,
    };

    void from_stream(istream& is) {
        tvalue value;
        char comma;

        string s;
        getline(is, s);
        stringstream ss{s};

        for(taddr i=0; ss >> value; ++i) {
            memory_[i] = value;
            ss >> comma;
        }

        // getline(ss, s);
    }

    template <taddr A>
    tvalue& fetch(tvalue op) {
        auto addr = ip + A;
        auto mode = ((op / 100) / static_cast<tvalue>(pow(10, A - 1))) % 10;

        switch (mode) {
            case POSITION: {
                if (trace_ > 2) cout << "mode: " << mode << ", addr: " << addr << endl;
                return memory_[memory_[addr]];
            }
            case RELATIVE: {
                if (trace_ > 2) cout << "mode: " << mode << ", addr: " << addr << endl;
                return memory_[memory_[addr] + base_];
            }
            case IMMEDIATE: {
                if (trace_ > 2) cout << "mode: " << mode << ", addr: " << addr << endl;
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

        if (trace_ > 0)
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
                if (ascii_) {
                    char c;
                    is_.get(c);
                    fetch<1>(op) = c;
                } else {
                    tvalue v;
                    is_ >> v;
                    fetch<1>(op) = v;
                }
                ip += 2;
                break;
            }

            case OUT: {
                if (ascii_) {
                    tvalue v = fetch<1>(op);
                    os_ << static_cast<char>(v);
                } else {
                    os_ << fetch<1>(op) << " ";
                }
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
                auto left = fetch<1>(op);
                auto right = fetch<2>(op);

                if ((opcode == LT && left < right)
                 || (opcode == EQ && left == right)) {
                    fetch<3>(op) = 1;
                } else {
                    fetch<3>(op) = 0;
                }

                ip += 4;
                break;
            }

            case REL: {
                base_ += fetch<1>(op);
                ip += 2;
                break;
            }

            default: {
                assert(false);
                break;
            }
        }

        return true;
    }

    taddr ip{0};
    taddr base_{0};
    tmemory memory_;
    int trace_{0};
    bool ascii_{false};

    istream& is_;
    ostream& os_;
};

ostream& operator<< (ostream& os, const tintcode::tmemory& vv) {
    bool first = true;
    for (const auto v : vv) {
        cout << (first ? "" : ", ") << v.first << ": " << v.second;
        first = false;
    }
    return os;
}

void verify() {
    {
        stringstream in{"7 8 9"};
        stringstream out;
        for (int i=0; i<3; ++i) {
            tintcode ic(ntd::test_if, in, out);
            ic.run();
        }

        assert(out.str() == ntd::ok_if);
    }

    {
        stringstream out;
        tintcode ic(ntd::test_quine, cin, out);
        ic.run();
        assert(out.str() == ntd::ok_quine);
    }
}

int main() {
    verify();

    fstream fs("day25.txt");
    // stringstream in("A,B,A,C,B,C,B,C,A,C\nR,12,L,6,R,12\nL,8,L,6,L,10\nR,12,L,10,L,6,R,10\nn\n");

    tintcode ic(fs, cin);
    ic.ascii().run();
}
