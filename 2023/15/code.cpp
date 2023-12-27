#include "all.h"

using namespace std;
using namespace advent;

enum {
    RM = 0,
    ADD = 1,
};

int hsh(const string& s) {
    int result = 0;
    for (const auto c : s) {
        result += c;
        result *= 17;
        result %= 256;
    }
    return result;
}

struct toper {
    int command;
    string label;
    int focal;
};

using tinput = vector<toper>;

tinput parse_input(istream&& is) {
    tinput result;

    string s;
    getline(is, s);

    for (auto op : advent::split(s, ",")) {
        if (op.back() == '-') {
            op.remove_suffix(1);
            result.push_back({RM, string{op}, 0});
            continue;
        }

        const auto& v = split(op, "=");
        result.push_back({ADD, string{v[0]}, stoi(string{v[1]})});
    }

    return result;
}

tinput parse_input(istream& is) {
    return parse_input(move(is));
}

stringstream test{R"(rn=1,cm-,qp=3,cm=2,qp-,pc=4,ot=9,ab=5,pc-,pc=6,ot=7)"};

struct tbox {
    using tlens = toper;
    list<tlens> lens;

    void rm(const string& label) {
        auto it = find_if(lens.begin(), lens.end(), [&label](const auto& v) {
            return v.label == label;
        });
        if (it == lens.end()) { return; }
        lens.erase(it);
    }

    void add(const string& label, int focal) {
        auto it = find_if(lens.begin(), lens.end(), [&label](const auto& v) {
            return v.label == label;
        });
        if (it == lens.end()) {
            lens.push_back({0, label, focal});
        } else {
            it->focal = focal;
        }
    }
};

int64_t solution2(const tinput& data) {
    vector<tbox> boxes(256);

    for (const auto d : data) {
        const auto bn = hsh(d.label);
        switch (d.command) {
        case RM:
            boxes[bn].rm(d.label);
            break;
        case ADD:
            boxes[bn].add(d.label, d.focal);
            break;
        }
    }

    int64_t fp{0};
    for (int i=0; i<256; ++i) {
        int slot = 1;
        for (const auto& l : boxes[i].lens) {
            fp += (i+1) * slot++ * l.focal; 
        }
    }
    return fp;
}

int main() {
    const auto& testi = parse_input(test);
    cout << solution2(testi) << endl;

    const auto& cini = parse_input(cin);
    cout << solution2(cini) << endl;
}
