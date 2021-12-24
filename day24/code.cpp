#include "all.h"
#include <cstring> // strcmp oh my
using namespace std;

#include "test_data.h"

enum eopcode {
    NOP,
    INP,
    ADD,
    RADD,
    MUL,
    RMUL,
    DIV,
    RDIV,
    MOD,
    RMOD,
    EQL,
    REQL
};

struct tinstruction {
    eopcode op{NOP};
    int8_t op1{0};
    int8_t op2{0};
};

using tprogram = vector<tinstruction>;
using tinput = array<int, 14>;
using tregisters = array<int64_t, 4>;

tprogram compile(istream& in) {
    tprogram program;

    char op[4];

    for (string s; getline(in, s);) {
        tinstruction i;
        int8_t op1{0};
        int op2{0};

        if (s[0] == 'i' && s[1] == 'n' && s[2] == 'p') {
            assert(sscanf(s.data(), "inp %c", &op1) == 1);
            i.op = INP;
            i.op1 = op1 - 'w';
            program.push_back(move(i));
            continue;
        }

        // register/value version
        if (sscanf(s.data(), "%s %c %d", op, &op1, &op2) == 3) {
            if (strcmp(op, "add") == 0) { i.op = ADD, i.op1 = op1 - 'w'; i.op2 = op2; }
            if (strcmp(op, "mul") == 0) { i.op = MUL, i.op1 = op1 - 'w'; i.op2 = op2; }
            if (strcmp(op, "div") == 0) { i.op = DIV, i.op1 = op1 - 'w'; i.op2 = op2; }
            if (strcmp(op, "mod") == 0) { i.op = MOD, i.op1 = op1 - 'w'; i.op2 = op2; }
            if (strcmp(op, "eql") == 0) { i.op = EQL, i.op1 = op1 - 'w'; i.op2 = op2; }

            program.push_back(move(i));
            continue;
        }

        if (sscanf(s.data(), "%s %c %c", op, &op1, &op2) == 3) {
            if (strcmp(op, "add") == 0) { i.op = RADD, i.op1 = op1 - 'w'; i.op2 = op2 - 'w'; }
            if (strcmp(op, "mul") == 0) { i.op = RMUL, i.op1 = op1 - 'w'; i.op2 = op2 - 'w'; }
            if (strcmp(op, "div") == 0) { i.op = RDIV, i.op1 = op1 - 'w'; i.op2 = op2 - 'w'; }
            if (strcmp(op, "mod") == 0) { i.op = RMOD, i.op1 = op1 - 'w'; i.op2 = op2 - 'w'; }
            if (strcmp(op, "eql") == 0) { i.op = REQL, i.op1 = op1 - 'w'; i.op2 = op2 - 'w'; }

            program.push_back(move(i));
            continue;
        }
    }

    return program;
}

tprogram compile(const string& instr) {
    stringstream in{instr};
    return compile(in);
}

template <typename T>
tregisters execute(const tprogram& program, const T& input, const tregisters& init_regs = {0, 0, 0, 0}) {
    tregisters regs = init_regs;
    size_t inpp = 0;

    for (const auto& i : program) {
        switch (i.op) {
            case INP: { assert(inpp < input.size()); regs[i.op1] = input[inpp++]; break; }

            case ADD: { regs[i.op1] += i.op2; break; }
            case RADD: { regs[i.op1] += regs[i.op2]; break; }

            case MUL: { regs[i.op1] *= i.op2; break; }
            case RMUL: { regs[i.op1] *= regs[i.op2]; break; }

            case DIV: { assert(i.op2 != 0); regs[i.op1] /= i.op2; break; }
            case RDIV: { assert(regs[i.op2] != 0); regs[i.op1] /= regs[i.op2]; break; }

            case MOD: { assert(regs[i.op1] >= 0 && i.op2 > 0); regs[i.op1] = regs[i.op1] % i.op2; break; }
            case RMOD: { assert(regs[i.op1] >= 0 && regs[i.op2] > 0); regs[i.op1] = regs[i.op1] % regs[i.op2]; break; }

            case EQL: { regs[i.op1] = (regs[i.op1] == i.op2) ? 1 : 0; break; }
            case REQL: { regs[i.op1] = (regs[i.op1] == regs[i.op2]) ? 1 : 0; break; }

            case NOP: break;
        }
    }

    return regs;
}

void ut() {
    tinput t1 = {5, 15};
    tinput t2 = {13, 10};

    auto p1 = compile(ntd::inv);
    auto p2 = compile(ntd::x3);
    auto p3 = compile(ntd::bits);
    assert(execute(p1, t1)[1] == -5);
    assert(execute(p2, t1)[3] == 1);
    assert(execute(p2, t2)[3] == 0);
    assert(execute(p3, t1) == tregisters({0, 1, 0, 1}));
    assert(execute(p3, t2) == tregisters({1, 1, 0, 1}));
}

template <size_t S>
ostream& operator<< (ostream& os, const array<int, S>& inp) {
    for (const auto i : inp) {
        os << i;
    }
    return os;
}

ostream& operator<< (ostream& os, const tregisters& regs) {
    for (int i=0; i<4; ++i) {
        cout << static_cast<char>(i + 'w') << ": " << regs[i] << " ";
    }

    return os;
}

// 1. each iteration of input gets input value into w
// 2. at the beginning of each iteration z has the value of previous iterations
// 3. x assigned z % 26, then incremented by value and compared to input
// 4. z is divided by 26
// 5. if x is zero, then z multiplied by 1 otherwize by 25
// 6. the very last command is add z y so both z and y must be 0
// 7. we have positive values in 7 increments and negative n 7 others
//    so each negative addition must set z % 26 - v equal to w

// i found all possible prefixes of length 5 (my input has 3 increments and 2 increments)
// and then bruteforce the last 9 digits  


template <size_t S>
void run_all(const tprogram& program) {
    // array<int, S> a;
    int64_t count{0};
    tinput a;

    for (int i=0; i<a.size(); ++i) a[i] = 1;
    a[0] = 1;
    a[1] = 1;
    a[2] = 1;
    a[3] = 8;
    a[4] = 9;

    while (true) {
        auto regs = execute(program, a);
        if (regs[3] == 0) { // || (++count % 100000000 == 0)) {
            cout << regs << " " << a << endl;
            return;
        }

        for (int i=5; ;++i) {
            if (i >= a.size()) {
                return;
            }
            if (++a[i] <= 9) {
                break;
            } else {
                a[i] = 1;
            }
        }

    }
}

int main() {
    ut();

    // auto program = compile(ntd::input1 + ntd::input2 + ntd::input3 + ntd::input4 + ntd::input5);
    auto program = compile(ntd::full_input);
    run_all<9>(program);
}
