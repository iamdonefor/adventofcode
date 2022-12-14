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
}
