#include "../all.h"

using namespace std;

pair<int, int> parsepath(string_view s) {
    vector<pair<int, int>> result;

    int x = 0;
    int y = 0;

    for (int i=0; i<s.size(); ++i) {
        if (s[i] == 'e') {
            x += 2;
        } else if (s[i] == 'w') {
            x -= 2;
        } else if (s[i] == 's' && s[i+1] == 'e') {
            x += 1;
            y -= 1;
            ++i;
        } else if (s[i] == 's' && s[i+1] == 'w') {
            x -= 1;
            y -= 1;
            ++i;
        } else if (s[i] == 'n' && s[i+1] == 'e') {
            x += 1;
            y += 1;
            ++i;
        } else if (s[i] == 'n' && s[i+1] == 'w') {
            x -= 1;
            y += 1;
            ++i;
        }
    }

    return make_pair(x, y);
}

int processtiles(const vector<string> paths) {
    set<pair<int, int>> seen;

    for (const auto& path : paths) {
        const auto path_ends = parsepath(path);
        cout << "path: " << path << " {" << path_ends.first << ", " << path_ends.second << "}" << endl;;

        if (seen.count(path_ends)) {
            seen.erase(path_ends);
        } else {
            seen.insert(path_ends);
        }
    }

    return seen.size();
}

int main() {
    vector<string> input;

    string s;
    while (getline(cin, s)) {
        input.push_back(s);
    }

    cout << "total: " << input.size() << " paths" << endl;

    cout << processtiles(input) << " black tiles" << endl;
}
