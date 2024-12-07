#include "all.h"

using namespace std;
using namespace advent;

struct tcond {
    char what = 0;
    int lm = 0;
    int value = 0;
    string target;
};

struct tworkflow {
    string name;
    vector<tcond> conds;
};

struct tpart {
    int x;
    int m;
    int a;
    int s;
};

struct tinput {
    vector<tpart> parts;
    vector<tworkflow> workflows;
};

tinput parse_input(istream&& is) {
    tinput result;

    for (string s; getline(is, s); ) {
        if (s.empty()) { continue; }

        if (s[0] == '{') {
            result.parts.push_back({});
            auto& p = result.parts.back();

            if (sscanf(s.data(), "{x=%d,m=%d,a=%d,s=%d}", &p.x, &p.m, &p.a, &p.s) != 4) {
                throw runtime_error("invalid input");
            }
            continue;
        }

        tworkflow wf;

        const auto p = s.find("{");
        wf.name = s.substr(0, p);

        string_view flow{s.data() + p + 1};
        flow.remove_suffix(1);

        for (const auto expr : split(flow, ",")) {
            const auto expr_parts = split(expr, ":");
            if (expr_parts.size() == 1) {
                wf.conds.push_back({.target = string(expr_parts[0])});
            } else {
                auto h = expr_parts[0];
                tcond cond;

                cond.what = h[0];
                cond.lm = (h[1] == '>' ? 1 : -1);
                h.remove_prefix(2);
                cond.value = stoi(string{h});
                cond.target = string(expr_parts[1]);

                wf.conds.push_back(cond);
            }
        }

        result.workflows.push_back(wf);
    }
    return result;
}

tinput parse_input(istream& is) {
    return parse_input(move(is));
}

stringstream test{R"(px{a<2006:qkq,m>2090:A,rfg}
pv{a>1716:R,A}
lnx{m>1548:A,A}
rfg{s<537:gd,x>2440:R,A}
qs{s>3448:A,lnx}
qkq{x<1416:A,crn}
crn{x>2662:A,R}
in{s<1351:px,qqz}
qqz{s>2770:qs,m<1801:hdj,R}
gd{a>3333:R,R}
hdj{m>838:A,pv}

{x=787,m=2655,a=1222,s=2876}
{x=1679,m=44,a=2067,s=496}
{x=2036,m=264,a=79,s=2244}
{x=2461,m=1339,a=466,s=291}
{x=2127,m=1623,a=2188,s=1013}
)"};

const string& apply_wf(const tworkflow& wf, const tpart& part) {
    for (const auto& cond : wf.conds) {
        if (cond.lm == 0) { return ref(cond.target); }

        int to_comp{0};
        if (cond.what == 'a') { to_comp = part.a; }
        else if (cond.what == 'm') { to_comp = part.m; }
        else if (cond.what == 'x') { to_comp = part.x; }
        else if (cond.what == 's') { to_comp = part.s; }
    
        if (cond.lm == 1 && to_comp > cond.value) { return ref(cond.target); }
        if (cond.lm == -1 && to_comp < cond.value) { return ref(cond.target); }
    }

    throw logic_error("apply_wf");
} 

int64_t solution1(const tinput& data) {
    map<string, tworkflow> wfs;
    for (const auto& wf : data.workflows) {
        wfs[wf.name] = wf;
    }

    int64_t result{0};
    for (const auto& part : data.parts) {
        string wfname = "in";
        for (;;) {
            wfname = apply_wf(wfs.at(wfname), part);
            if (wfname == "A") {
                result += part.x + part.s + part.a + part.m;
                break;
            }
            if (wfname == "R") {
                // cout << "R" << endl;
                break;
            }
        }
    }

    return result;
}

int64_t solution2(const tinput& data) {
    map<string, tworkflow> wfs;
    for (const auto& wf : data.workflows) {
        wfs[wf.name] = wf;
    }

    deque<string> q;
    q.push_back("in");

    while(!q.empty()) {
        auto wf = wfs.at(q.front());
        q.pop_front();

        for (const auto& cond : wf.conds) {
            if (cond.target == "R" || cond.target == "A") { continue; }

            q.push_back(cond.target);

        }
        cout << wf.name << endl;
    }

    return 0;
}

int main() {
    const auto& testi = parse_input(test);
    cout << solution1(testi) << endl;
    cout << solution2(testi) << endl;

    const auto& cini = parse_input(cin);
    cout << solution1(cini) << endl;
    cout << solution2(cini) << endl;
}
