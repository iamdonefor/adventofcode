#include "all.h"
using namespace std;

class TGraph {
public:
    using TVerticeId = int;
    using TVerticeName = string;

    TVerticeId atoi(const string& name) const { return atoi_.at(name); }
    string itoa(const TVerticeId& id) const { return itoa_.at(id); }

    size_t size() const { return size_; }
    TVerticeId start() const { return start_; }
    TVerticeId end() const { return end_; }

    bool is_small(TVerticeId id) const { return small_.count(id) > 0; }
    const set<TVerticeId>& adj(TVerticeId id) const { return adj_.at(id); }

    void add(const string& sv1, const string& sv2) {
        auto v1 = add_vertex(sv1);
        auto v2 = add_vertex(sv2);

        adj_[v1].insert(v2);
        adj_[v2].insert(v1);
    }

private:
    TVerticeId add_vertex(const string& v) {
        int vid;
        if (auto it = atoi_.find(v); it == atoi_.end()) {
            vid = size_++;

            itoa_[vid] = v;
            atoi_[v] = vid;

            if (v == "start") { start_ = vid; }
            if (v == "end") { end_ = vid; }
            if (islower(v.front())) { small_.insert(vid); }
        } else {
            vid = it->second;
        }
        return vid;
    }

    map<TVerticeId, string> itoa_;
    map<string, TVerticeId> atoi_;
    map<TVerticeId, set<TVerticeId>> adj_;
    set<TVerticeId> small_;

    size_t size_{0};
    TVerticeId start_;
    TVerticeId end_;
};

class TRoute {
public:
    using tvid = TGraph::TVerticeId;

    TRoute(const TGraph& g, bool twice = false) : g_(g), twice_(twice) { add(g_.start()); };
    TRoute(const TRoute& o)
    : g_(o.g_)
    , twice_(o.twice_)
    , path_(o.path_)
    , seen_ (o.seen_)
    {}

    void add(tvid id) {
        if (g_.is_small(id) && seen(id)) {
            twice_ = false;
        }
        seen_.insert(id);
        path_.push_back(id);
    }

    bool can_visit(tvid id) {
        if (id == g_.start() || id == g_.end()) {
            return false;
        }

        if (g_.is_small(id) && seen(id)) {
            return twice_;
        }

        return true;
    }
    bool seen(tvid id) const { return seen_.count(id) > 0; }
    tvid back() const { return path_.back(); }

    string to_string() const {
        bool first = true;
        string result;
        result += "("s + (twice_ ? "+" : "-") + ") ";
        for (const auto id : path_) {
            result += first ? "" : " - ";
            result += g_.itoa(id);
            first = false;
        }
        return result;
    }
private:
    const TGraph& g_;
    set<tvid> seen_;
    vector<tvid> path_;

    bool twice_{false};
};

using TInput = TGraph;

int64_t solution(const TInput& g, bool twice = false) {
    deque<TRoute> routes;

    int64_t nroutes{0};

    for (auto id : g.adj(g.start())) {
        routes.push_back(TRoute(g, twice));
        routes.back().add(id);
    }

    int generation{1};
    while (!routes.empty()) {
        // cout << generation++ << " : -------------------------------" << endl;
        int curr = routes.size();
        for (int i=0; i<curr; ++i) {
            auto route = routes.front();
            routes.pop_front();

            // cout << "route: " << route.to_string() << endl;

            for (auto id : g.adj(route.back())) {
                if (id == g.end()) {
                    ++nroutes;
                    // cout << "DONE (" << nroutes << ") " << route.to_string() << " - end" << endl;;
                    continue;
                }
                if (!route.can_visit(id)) {
                    // cout << "route cant go to: " << g.itoa(id) << endl;
                    continue;
                }

                // cout << "route continues to: " << g.itoa(id) << endl;
                routes.push_back(TRoute(route));
                routes.back().add(id);
            }
        }
    }

    return nroutes;
}

TInput parse_input(istream& in) {
    string s;
    TInput result;
    while (getline(in, s)) {
        auto split_pos = s.find('-');
        result.add(s.substr(0, split_pos), s.substr(split_pos+1));
    }

    return result;
}

bool verify() {
    stringstream input_stream{R"===(start-A
start-b
A-c
A-b
b-d
A-end
b-end)==="};

    stringstream input_stream2{R"===(dc-end
HN-start
start-kj
dc-start
dc-HN
LN-dc
HN-end
kj-sa
kj-HN
kj-dc)==="};


    auto input1 = parse_input(input_stream);
    auto input2 = parse_input(input_stream2);
    bool part1 = (10 == solution(input1) && 19 == solution(input2));
    bool part2 = (36 == solution(input1, true) && 103 == solution(input2, true));
    return part1 && part2;
}

int main() {
    assert(verify());

    auto input = parse_input(cin);
    cout << "part1: " << solution(input) << endl;
    cout << "part2: " << solution(input, true) << endl;
}
