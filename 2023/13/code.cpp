#include "all.h"

using namespace std;
using namespace advent;

using tmatrix = vector<string>;
using tinput = vector<tmatrix>;

tinput parse_input(istream&& is) {
    tinput result;

    result.push_back({});
    for (string s; getline(is, s); ) {
        if (s.empty()) {
            result.push_back({});
            continue;
        }

        result.back().push_back(s);
    }

    return result;
}

tinput parse_input(istream& is) {
    return parse_input(move(is));
}

stringstream test{R"(#.##..##.
..#.##.#.
##......#
##......#
..#.##.#.
..##..##.
#.#.##.#.

#...##..#
#....#..#
..##..###
#####.##.
#####.##.
..##..###
#....#..#
)"};

optional<int64_t> check_vertical(const tmatrix& matrix) {
    const int X = matrix.front().size();
    const int Y = matrix.size();

    for (int line = 0; line < X-1; ++line) {
        bool found = true;
        // cout << min(line, X - line - 2) << endl;
        for (int x=0; x <= min(line, X - line - 2) && found; ++x) {
            for (int y=0; y < Y && found; ++y) {
                // cout << matrix[y][line-x] << " " << matrix[y][line+ x + 1] << endl;
                if (matrix[y][line-x] != matrix[y][line+x+1]) {
                    found = false;
                }
            }

        }
        if (found) {
            // cout << "found vertical " << line << endl;
            return line + 1;
        }
    }

    return nullopt;
}

optional<int64_t> check_horizontal(const tmatrix& matrix) {
    const int X = matrix.front().size();
    const int Y = matrix.size();

    for (int line = 0; line < Y-1; ++line) {
        bool found = true;
        // cout << min(line, X - line - 2) << endl;
        for (int y=0; y <= min(line, Y - line - 2) && found; ++y) {
            for (int x=0; x < X && found; ++x) {
                // cout << matrix[y][line-x] << " " << matrix[y][line+ x + 1] << endl;
                if (matrix[line-y][x] != matrix[line+y+1][x]) {
                    found = false;
                }
            }

        }
        if (found) {
            // cout << "found horizontal " << line << endl;
            return 100 * (line + 1);
        }
    }

    return nullopt;
}

optional<int64_t> lookup_vertical_smudge(const tmatrix& matrix) {
    const int X = matrix.front().size();
    const int Y = matrix.size();

    for (int line = 0; line < X-1; ++line) {
        int p = 0;
        // cout << min(line, X - line - 2) << endl;
        for (int x=0; x <= min(line, X - line - 2) && (p < 2); ++x) {
            for (int y=0; y < Y && (p < 2); ++y) {
                // cout << matrix[y][line-x] << " " << matrix[y][line+ x + 1] << endl;
                if (matrix[y][line-x] != matrix[y][line + x + 1]) {
                    ++p;
                }
            }

        }
        if (p == 1) {
            cout << "found vertical smudge " << line << endl;
            return (line + 1);
        }
    }

    return nullopt;
}

optional<int64_t> lookup_horizontal_smudge(const tmatrix& matrix) {
    const int X = matrix.front().size();
    const int Y = matrix.size();

    for (int line = 0; line < Y-1; ++line) {
        int p = 0;
        // cout << min(line, X - line - 2) << endl;
        for (int y=0; y <= min(line, Y - line - 2) && (p < 2); ++y) {
            for (int x=0; x < X && (p < 2); ++x) {
                // cout << matrix[y][line-x] << " " << matrix[y][line+ x + 1] << endl;
                if (matrix[line-y][x] != matrix[line+y+1][x]) {
                    ++p;
                }
            }

        }
        if (p == 1) {
            cout << "found horizontal smudge " << line << endl;
            return 100 * (line + 1);
        }
    }

    return nullopt;
}

int64_t check(const tmatrix& matrix) {
    auto result1 = check_vertical(matrix);
    auto result2 = check_horizontal(matrix);

    if (!result1 && !result2) {
        for (const auto& s : matrix) {
            cout << s << endl;
        }
        cout << endl;
    }

    return result1 ? *result1 : result2 ? *result2 : 0;
}

int64_t check_smudge(const tmatrix& matrix) {
    auto result1 = lookup_vertical_smudge(matrix);
    auto result2 = lookup_horizontal_smudge(matrix);

    if ( (!result1 && !result2) || (result1 && result2)) {
        for (const auto& s : matrix) {
            cout << s << endl;
        }
        cout << endl;
    }

    return result1 ? *result1 : result2 ? *result2 : 0;
}


int64_t solution1(const tinput& data) {
    int i = 0;
    int result = 0;
    for (const auto& d : data) {
        cout << i++ << endl;
        result += check(d);
    }
    return result;
}

int64_t solution2(const tinput& data) {
    int i = 0;
    int result = 0;
    for (const auto& d : data) {
        cout << i++ << endl;
        result += check_smudge(d);
    }
    return result;
}

int main() {
    const auto& testi = parse_input(test);
    cout << solution1(testi) << endl;

    const auto& cini = parse_input(cin);
    cout << solution1(cini) << endl;

    cout << solution2(testi) << endl;
    cout << solution2(cini) << endl;
}
