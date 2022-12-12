#include "all.h"

const static int BEGIN{0};
const static int END{25};

const static vector<int> neis{1, 0, -1, 0, 1};

class tgraph {
public:
    using tvertice = array<int, 2>;

    const set<tvertice>& adj(const tvertice& v) const {
        const static set<tvertice> empty;
        return adj_.count(v) > 0 ? adj_.at(v) : empty;
    };

    void add(const tvertice& l, const tvertice& r) {
        adj_[l].insert(r);
    };

    int bfs(const vector<tvertice>& vbegin, const tvertice& vend) const {
        deque<tvertice> q{vbegin.begin(), vbegin.end()};
        set<tvertice> seen;

        for (int step = 0; !q.empty(); ++step) {
            int current = q.size();
            for (int i=0; i<current; ++i) {
                const auto best = q.front();
                q.pop_front();

                if (best == vend) {
                    return step;
                }

                if (seen.count(best) > 0) {
                    continue;
                }
                seen.insert(best);

                for (const auto& v : adj(best)) {
                    if (seen.count(v) == 0) {
                        q.push_back(v);
                    }
                }
            }
        }

        return numeric_limits<int>::max();
    }

    void set_dimensions(int y, int x) {
        Y = y;
        X = x;
    }

private:
    map<tvertice, set<tvertice>> adj_;
    int X;
    int Y;
};

struct tinput {
    tgraph::tvertice vbegin;
    tgraph::tvertice vend;

    tgraph graph;

    vector<tgraph::tvertice> allas;
};

tinput parseInput(istream& is) {
    tinput result;
    int y = -1;

    // parse input
    vector<vector<int>> sup;
    for (string s; getline(is, s); ) {
        if (s.empty()) { continue; }
        ++y;
        sup.push_back({});

        for (int x = 0; x < s.size(); ++x) {
            if (s[x] == 'E') {
                sup.back().push_back(END);
                result.vend = {y, x};
                continue;
            }

            if (s[x] == 'S') {
                sup.back().push_back(BEGIN);
                result.vbegin = {y, x};
                continue;
            }

            sup.back().push_back(s[x] - 'a');
        }
    }

    // create graph
    int Y = sup.size();
    int X = sup.front().size();

    result.graph.set_dimensions(Y, X);

    for (int y=0; y<Y; ++y) {
    for (int x=0; x<X; ++x) {
        for (int i=0; i<5; ++i) {
            auto dy = y + neis[i];
            auto dx = x + neis[i+1];

            if (dx >= X  || dy >= Y || dx < 0 || dy < 0) {
                continue;
            }

            if (sup[y][x] - sup[dy][dx] > -2) {
                result.graph.add({y, x}, {dy, dx});
            }
        }
        if (sup[y][x] == 0) {
            result.allas.push_back({y, x});
        }
    }}

    return result;
}

stringstream test{R"(
Sabqponm
abcryxxl
accszExk
acctuvwj
abdefghi
)"};

int main() {
    const auto intest = parseInput(test);
    cout << intest.graph.bfs({intest.vbegin}, intest.vend) << endl;
    cout << intest.graph.bfs({intest.allas}, intest.vend) << endl;
    const auto in = parseInput(cin);
    cout << in.graph.bfs({in.vbegin}, in.vend) << endl;
    cout << in.graph.bfs({in.allas}, in.vend) << endl;
}
