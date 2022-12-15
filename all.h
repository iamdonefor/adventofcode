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
    template <typename T>
    T sign(const T& v) {
        if (v > 0) return T{1};
        if (v < 0) return T{-1};
        return T{0};
    }

    std::vector<std::string_view> split(std::string_view s, std::string_view by);
}
