#include "all.h"

namespace advent {
    using namespace std;

    vector<string_view> split(string_view s, string_view by) {
        vector<string_view> result;

        while (!s.empty()) {
            const auto next_pos = s.find(by);
            const auto substr_size = (next_pos == s.npos ? s.size() : next_pos);

            result.push_back(s.substr(0, substr_size));
            s.remove_prefix(next_pos == s.npos ? s.size() : next_pos + by.size());
        }

        return result;
    }

    trange::trange(int64_t _0, int64_t _1) {
        this->operator[](0) = _0;
        this->operator[](1) = _1;
    }

    bool trange::intersects(const trange& other) const {
        if ( ((*this)[0] <= other[1] && (*this)[1] >= other[0])
            || (other[0] <= (*this)[1] && other[1] >= (*this)[0])) {
            return true;
        }
        return false;
    }

    bool trange::is_adj(const trange& other) const {
        if ( ((*this)[1] + 1 == other[0] || (*this)[1] == other[0] + 1) ) {
            return true;
        }
        return false;
    }

    trange trange::combine(const trange& other) const {
        return trange(min((*this)[0], other[0]),  max((*this)[1], other[1]));
    }

    bool trange::contains(int64_t x) const {
        return x >= (*this)[0] && x <= (*this)[1];
    }

    bool trange::contains(const trange& other) const {
        return other[0] >= (*this)[0] && other[1] <= (*this)[1];
    }

    int64_t trange::size() const { return (*this)[1] - (*this)[0] + 1; }
}
