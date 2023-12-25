#pragma once

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdio>
#include <deque>
#include <iomanip>
#include <ios>
#include <iostream>
#include <fstream>
#include <functional>
#include <future>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <random>
#include <set>
#include <sstream>
#include <stack>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

namespace advent {
    // using namespace std;

    template <typename T>
    T sign(const T& v) {
        if (v > 0) return T{1};
        if (v < 0) return T{-1};
        return T{0};
    }

    template<typename tvertice, typename tset=std::set<tvertice>>
    class tgraph {
    public:
        const tset& adj(const tvertice& v) const {
            const static tset empty_;
            const auto it=adj_.find(v);
            return it == adj_.end() ? empty_ : it->second;
        };

        const tset& vertices() const {
            return std::cref(vertices_);
        }

        void add(const tvertice& v) {
            vertices_.insert(v);
            adj_[v] = {};
        }

        void add(const tvertice& l, const tvertice& r) {
            vertices_.insert(r);
            vertices_.insert(l);
            adj_[l].insert(r);
        };

        int bfs(const std::vector<tvertice>& vbegin, const tvertice& vend) const {
            std::deque<tvertice> q{vbegin.begin(), vbegin.end()};
            tset seen;

            for (int step = 0; !q.empty(); ++step) {
                for (int i=0, current=q.size(); i<current; ++i) {
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

            return std::numeric_limits<int>::max();
        }

    private:
        tset vertices_;
        std::map<tvertice, tset> adj_;
    };

    template <typename T>
    void next_permutatation(T& what) {
        using eow = std::exception;

        if (what.size() == 1) {
            return;
        }

        int inversion_index = -1;
        for (int i = what.size() - 1; i > 0; --i) {
            if (what[i - 1] < what[i]) {
                inversion_index = i - 1;
                break;
            }
        }

        if (inversion_index == -1) {
            throw eow();
        }

        for (int i = what.size() - 1; i > inversion_index; --i) {
            if (what[inversion_index] < what[i]) {
                std::swap(what[i], what[inversion_index]);
                break;
            }
        }

        std::sort(what.begin() + inversion_index + 1, what.end());
        return;
    }

    std::vector<std::string_view> split(std::string_view s, std::string_view by);

    using tcoords = std::array<int64_t, 2>;
    using tcoords3 = std::array<int64_t, 3>;

    class trange : public std::array<int64_t, 2> {
    public:
        trange(int64_t _0, int64_t _1);

        bool intersects(const trange& other) const;
        bool is_adj(const trange& other) const;

        bool contains(int64_t x) const;
        bool contains(const trange& other) const;

        trange combine(const trange& other) const;
        int64_t size() const;
    };

    template <typename T>
    std::ostream& operator << (std::ostream& os, const std::vector<std::vector<T>>& v) {
        if (v.empty() || v.front().empty()) { return os; }
        for (size_t y=0; y<v.size(); ++y) {
        bool first = true;
        for (size_t x=0; x<v.front().size(); ++x) {
            os << (first ? "" : ", ") << v[y][x];
        }
        os << std::endl;
        }
        return os;
    }

}
