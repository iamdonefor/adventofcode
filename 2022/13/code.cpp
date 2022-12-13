#include "all.h"

struct tpacket {
    const static int NOTI{-1};

    int i = NOTI;
    vector<tpacket> l;

    bool is_int() const { return !is_list(); }
    bool is_list() const { return i == NOTI; }
    int size() const { return l.size(); }

    int cmp(const tpacket& other) const {
        if (is_int() && other.is_int()) {
            return i > other.i ? 1
            : i < other.i ? -1
            : 0;
        }

        if (is_list() && other.is_list()) {
            for (int i=0; i<min(size(), other.size()); ++i) {
                if (const auto result = l[i].cmp(other.l[i]); result) {
                    return result;
                }
            }
            return sign(size() - other.size());
        }

        tpacket intaslist{
            NOTI,
            {{is_list() ? other.i : i}}
        };

        return is_list() ? this->cmp(intaslist) : intaslist.cmp(other);
    }

    bool operator== (const tpacket& other) const {
        return cmp(other) == 0;
    }

    bool operator< (const tpacket& other) const {
        return cmp(other) < 0;
    }
};

struct thelper {
    string s;
    tpacket p;

    bool operator== (const thelper& other) const {
        return p.cmp(other.p) == 0;
    }

    bool operator< (const thelper& other) const {
        return p.cmp(other.p) < 0;
    }
};

using tinput = vector<thelper>;

tpacket parse_string(const string& s, int& i) {
    tpacket result;

    for(;;) {
        if (s[i] == '[') {
            result.l.push_back(parse_string(s, ++i));
        }
        if (s[i] == ']') {
            ++i;
            break;
        }
        if (s[i] == ',') {
            ++i;
            continue;
        }

        int number{0};
        for (; isdigit(s[i]); ++i) {
            number = number * 10 + s[i] - '0';
        }
        result.l.push_back({number});
    }

    return result;
}

tpacket parse_string(const string& s) {
    int i{1};
    return parse_string(s, i);
}


tinput parse_input(istream& is) {
    tinput result;
    for (string s; getline(is, s);) {
        string s1, s2;

        getline(is, s1);
        getline(is, s2);

        result.push_back({s1, parse_string(s1)});
        result.push_back({s2, parse_string(s2)});
    }
    return result;
}

stringstream test{R"(
[1,1,3,1,1]
[1,1,5,1,1]

[[1],[2,3,4]]
[[1],4]

[9]
[[8,7,6]]

[[4,4],4,4]
[[4,4],4,4,4]

[7,7,7,7]
[7,7,7]

[]
[3]

[[[]]]
[[]]

[1,[2,[3,[4,[5,6,7]]]],8,9]
[1,[2,[3,[4,[5,6,0]]]],8,9]
)"};

int solution1(const tinput& input) {
    int s{0};

    for (int i=0; i<input.size(); i+=2) {
        if (input[i].p < input[i+1].p) {
            s+=(i/2+1);
        }
    }
    return s;
}

int solution2(const tinput& input) {
    vector<thelper> v{input};
    const tpacket div1{
        tpacket::NOTI,
        {
            {
                tpacket::NOTI,
                {{2}}
            },
        }
    };
    const tpacket div2{
        tpacket::NOTI,
        {
            {
                tpacket::NOTI,
                {{6}}
            },
        }
    };

    const thelper helper1{string{"[[2]]"}, div1};
    const thelper helper2{"[[6]]", div2};
    v.push_back(helper1);
    v.push_back(helper2);

    sort(v.begin(), v.end());

    const auto& it1 = find(v.begin(), v.end(), helper1);
    const auto& it2 = find(v.begin(), v.end(), helper2);

    return (it2 - v.begin() + 1) * (it1 - v.begin() + 1);
}

int main() {
    const auto ti = parse_input(test);
    const auto i = parse_input(cin);
    cout << solution1(ti) << endl;
    cout << solution1(i) << endl;

    cout << solution2(ti) << endl;
    cout << solution2(i) << endl;
}
