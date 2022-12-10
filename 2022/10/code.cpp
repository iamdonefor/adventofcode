#include "all.h"
#include "test.h"

class eoe : public exception {};

enum class eop {
    NOOP,
    ADDX,
};

struct top {
    eop op;
    int arg;
};

using tinput = vector<top>;

class tcomputer {
private:
    class tcommand {
    public:
        tcommand(const tcomputer& computer_, int arg_)
        : computer(computer_)
        , arg(arg_)
        {}

        void virtual tick() = 0;

    protected:
        const tcomputer& computer;
        int arg;
    };

    class tnoop : public tcommand {
        using tcommand::tcommand;
        void tick() override {
            if (--ticks == 0) {
                throw eoe();
            }
        }

        int ticks{1};
    };

    class taddx : public tcommand {
        using tcommand::tcommand;
        void tick() override {
            if (--ticks == 0) {
                computer.adjust_x(arg);
                throw eoe();
            }
        }

        int ticks{2};
    };

public:
    void run(const tinput& input) {
        prepare();

        auto current = input.begin();

        for (int tick = 0; current != input.end(); ++tick) {
            x_by_tick.push_back(x);

            if (!executing) {
                command = decode(*current++);
                executing = true;
            }

            try {
                command->tick();
            } catch (eoe) {
                executing = false;
            }
        }
    }

    void adjust_x(int adj) const {
        x += adj;
    }

    int signal_strength() const {
        int result(0);
        for (int i=19; i<x_by_tick.size(); i+=40) {
            // cout << i << " " << x_by_tick[i] << endl;
            result += x_by_tick[i] * (i+1);
        }
        return result;
    }

private:
    unique_ptr<tcommand> decode(const top& op) {
        if (op.op == eop::NOOP) {
            return make_unique<tnoop>(*this, 0);
        }
        if (op.op == eop::ADDX) {
            return make_unique<taddx>(*this, op.arg);
        }
        throw logic_error("");
    }

    void prepare() {
        x_by_tick.clear();
        x = 1;
        executing = false;
    }

    unique_ptr<tcommand> command{nullptr};
    int mutable x{1};
    bool executing{false};
    vector<int> x_by_tick;
};


tinput parse_input(istream& is) {
    tinput result;

    for (string s; getline(is, s); ) {
        if (s.empty()) { continue; }

        if (string_view(s.data(), 4) == "noop") {
            result.push_back({eop::NOOP});
            continue;
        }

        top tmp{eop::ADDX};
        if (sscanf(s.data(), "addx %d", &tmp.arg) != 1) {
            throw runtime_error("bad input " + s);
        }
        result.push_back(tmp);
    }

    return result;
}

int main() {
    tcomputer computer;

    const auto& test_input = parse_input(test);
    const auto& input = parse_input(cin);

    computer.run(test_input);
    cout << computer.signal_strength() << endl;
    computer.run(input);
    cout << computer.signal_strength() << endl;
}
