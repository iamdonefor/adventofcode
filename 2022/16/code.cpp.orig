#include "all.h"

using namespace std;
using namespace advent;

#include "data.h"

class eow : public exception { using exception::exception; };

struct tvertice{
    char name[2];
    int rate;

    tvertice() {}
    tvertice(const char* n, int r = 0) {
        name[0] = n[0];
        name[1] = n[1];
        // name[2] = n[2];
        rate = r;
    }

    bool operator==(const tvertice& other) const {
        return name[0] == other.name[0]
            && name[1] == other.name[1];
    }

    bool operator<(const tvertice& other) const {
        return name[0] < other.name[0] || (name[0] == other.name[0] && name[1] < other.name[1]);
    }

    friend ostream& operator<< (ostream& os, const tvertice& v) {
        os << v.name[0] << v.name[1] << ":" << to_string(v.rate);
        return os;
    }
};

tgraph<tvertice> create_graph(const vector<tinput_node> data) {
    tgraph<tvertice> graph;
    map<tvertice, tvertice> helper;

    for (const auto& node : data) {
        graph.add(tvertice{node.name.c_str(), node.rate});
        helper[{node.name.c_str(), node.rate}] = {node.name.c_str(), node.rate};
    }

    for (const auto& node : data) {
    for (const auto& adj : node.adj) {
        graph.add(
            {node.name.c_str()},
            helper[tvertice{adj.c_str()}]
        );
    }}

    return graph;
}

int64_t worker(const tgraph<tvertice>& graph, vector<tvertice> seed, vector<tvertice> check) {
    const static int TRESH{30};

    const auto vertices = graph.vertices();
    map<pair<tvertice, tvertice>, int> dst;

    const auto run = [&vertices](int i) {
        auto it = vertices.begin();
        for (; --i >=0; ++it) {}
        return *it;
    };

    for (int i=0; i<vertices.size(); ++i) {
    for (int j=0; j<vertices.size(); ++j) {
        if (i == j) { continue; }
        dst[make_pair(run(i), run(j))] = graph.bfs({run(i)}, run(j));
    }}

    const auto init_time =
        1
        + dst.at({tvertice{"AA"}, seed[0]})
        + 1
        + dst.at({seed[0], seed[1]})
        ;

    const auto init_score = seed[0].rate * (dst.at({tvertice{seed[0].name}, tvertice{seed[1].name}}) + 1);
    const auto init_pps = seed[0].rate + seed[1].rate;

    int64_t result{0};
    sort(check.begin(), check.end());
    int64_t p{0};

    for (;;) {
        auto ctime = init_time;
        auto pps = init_pps;
        int64_t score = init_score;
        auto prev = seed[1];

        for (const auto& v : check) {
            auto run_and_open_time = dst.at({prev, v}) + 1;
            if (run_and_open_time + ctime > TRESH) {
                break;
            }

            score += run_and_open_time * pps;
            pps += v.rate;
            ctime += run_and_open_time;
            prev = v;
        }

        if (ctime <= TRESH) {
            score += pps * (TRESH - ctime);
        }

        result = max(result, score);
        try {
            next_permutatation(check);
        } catch(...) {
            break;
        }
        if (++p % 100000000 == 0) {
            cout << seed[0] << "," << seed[1] << " " << p << endl;
        }
    }

    return result;
}

int64_t run_workers(const tgraph<tvertice>& graph) {
    vector<tvertice> non_zero;

    for (const auto& v : graph.vertices()) {
        if (v.rate > 0) {
            non_zero.push_back(v);
        }
    }

    vector<future<int64_t>> workers;
    for (const auto& v1 : non_zero) {
    for (const auto& v2 : non_zero) {
        if (v1 == v2) { continue; }

        vector<tvertice> seed{v1, v2};
        vector<tvertice> check;

        copy_if(non_zero.begin(), non_zero.end(), back_inserter(check),
            [&seed](const auto& v) {
                return find(seed.begin(), seed.end(), v) == seed.end();
            });

        auto f = async(std::launch::async, worker, graph, move(seed), move(check));
        workers.push_back(move(f));
    }}

    int64_t result{0};
    for (auto& f : workers) {
        auto x = f.get();
        result = max(x, result);
        cout << x << endl;
    }

    return result;
}

int main() {
    // cout << worker(create_graph(test_data)
    cout << run_workers(create_graph(prod_data)) << endl;
    //     , {{"DD", 20}, {"BB", 13}}
    //     , {{"JJ", 21}, {"HH", 22}, {"EE", 3}, {"CC", 2}}
    // ) << endl;

    // cout << prod_data.size() << endl;


    // auto graph = create_graph(test_data);
    // cout << "digraph {";
    // for (const auto& v : graph.vertices()) {
    //     for (const auto& vv : graph.adj(v)) {
    //         cout << '"' << v << "\" -> \"" << vv << "\";" << endl;
    //     }
    // }
    // cout << "}" << endl;


}
