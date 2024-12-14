#include "all.h"

using namespace std;
using namespace advent;

#include "data.h"

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
    
map<tuple<tvertice, tvertice>, int> make_distances(const tgraph<tvertice>& graph) {
    map<tuple<tvertice, tvertice>, int> result;
    
    const auto vertices = vector<tvertice>{graph.vertices().begin(), graph.vertices().end()};

    for (size_t i=0; i<vertices.size(); ++i) {
    for (size_t j=0; j<vertices.size(); ++j) {
        if (i == j) { continue; }
        result[make_tuple(vertices[i], vertices[j])] = graph.bfs({vertices[i]}, vertices[j]);
    }}

    return result;
}

struct tstate {
    int time{0};
    int64_t valves{0};
    tvertice where;
    int64_t volume{0};
    int64_t speed{0};
};

const static int TL{30};

int64_t solution(const tgraph<tvertice>& graph) {
    const auto& distances = make_distances(graph);
    vector<tvertice> vertices;

    copy_if(graph.vertices().begin(), graph.vertices().end(), back_inserter(vertices),
        [](const auto& v) {
            return v.rate > 0;
        }
    );

    assert(vertices.size() < 63);
    cout << vertices.size() << endl;

    deque<tstate> states;
    states.push_back({.where = {"AA"}});

    while(!states.empty()) {
    for (const auto& state : states) {
        cout << state.where << " " << state.time << " " << state.valves << endl;
    }
    getchar();
        for (size_t i = states.size(); i > 0; --i) {
            const auto& state = states.front();
            states.pop_front();

            for (size_t valve = 0; valve < vertices.size(); ++valve) {
                if (!((1 << valve) & state.valves)) {
                    cout << state.where << " " << vertices[valve] << endl;
                    int extra_time = distances.at(tie(state.where, vertices[valve])) + 1;

                    cout << extra_time << endl;
                    if (state.time + extra_time > TL) {
                        continue; // XXX push end state
                    }

                    auto nstate = state;

                    nstate.time += extra_time;
                    nstate.valves |= (1 << valve);
                    nstate.where = vertices[valve];
                    nstate.volume += (state.speed * extra_time);
                    nstate.speed += state.speed + vertices[valve].rate;

                    states.push_back(move(nstate));
                }
            }
        }
    }

    return 0;
}

int main() {
    cout << solution(create_graph(test_data)) << endl;

    // auto graph = create_graph(test_data);
    // cout << "digraph {";
    // for (const auto& v : graph.vertices()) {
    //     for (const auto& vv : graph.adj(v)) {
    //         cout << '"' << v << "\" -> \"" << vv << "\";" << endl;
    //     }
    // }
    // cout << "}" << endl;


}