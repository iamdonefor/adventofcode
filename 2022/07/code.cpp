#include "all.h"

struct tfile {
    string name;
    bool is_dir;
    uint64_t size;
    tfile* parent;
    vector<tfile> content;
};

using tinput = tfile;

tinput parseInput(istream& is) {
    tinput fs{
        "/",
        true,
        0,
        nullptr,
    };
    tfile* current = &fs;

    string s;
    for (bool skip_read = false; skip_read || getline(is, s); ) {
        skip_read = false;
        if (s.empty()) { continue; }

        if (s.substr(0, 5) == "$ cd ") {
            const auto dir_name = s.substr(5);
            if (dir_name == "..") {
                current = current->parent;
            } else {
                current = &(*find_if(
                    current->content.begin(),
                    current->content.end(),
                    [dir_name](const auto& f) {
                        return f.name == dir_name;
                    }
                ));
            }
        }

        if (s == "$ ls") {
            skip_read = true;
            for (;getline(is, s);) {
                if (s[0] == '$') {
                    break;
                }

                if (s.substr(0, 4) == "dir ") {
                    current->content.push_back({
                        s.substr(4),
                        true,
                        0,
                        current
                    });
                } else {
                    char filename[1024];
                    int size;
                    sscanf(s.data(), "%u %s", &size, filename);
                    current->content.push_back({
                        string{filename},
                        false,
                        size,
                        current
                    });
                }
            }
        }
    }

    return fs;
}


int64_t traverseFs(tfile& fs, uint64_t& answer, vector<uint64_t>& alldirssizes, string prefix={}) {
    for (auto& c : fs.content) {
        if (c.is_dir) {
            // cout << prefix + c.name << endl;
            fs.size += traverseFs(c, answer, alldirssizes, prefix + c.name + "/");
        } else {
            fs.size += c.size;
            cout << prefix + c.name << " " << c.size << endl;
        }
    }

    if (fs.size <= 100000) {
        answer += fs.size;
    }

    alldirssizes.push_back(fs.size);
    return fs.size;
}

stringstream test{R"(
$ ls
dir a
14848514 b.txt
8504156 c.dat
dir d
$ cd a
$ ls
dir e
29116 f
2557 g
62596 h.lst
$ cd e
$ ls
584 i
$ cd ..
$ cd ..
$ cd d
$ ls
4060174 j
8033020 d.log
5626152 d.ext
7214296 k
)"};

int main() {
    {
        auto fs = parseInput(test);
        vector<uint64_t> all;

        uint64_t answer{0};
        traverseFs(fs, answer, all);
        cout << fs.size << endl;
        cout << answer << endl;

        sort(all.begin(), all.end());
        const auto needtofree = 30000000 - (70000000 - fs.size);
        cout << *upper_bound(all.begin(), all.end(), needtofree) << endl;
    }
    {
        auto fs = parseInput(cin);
        vector<uint64_t> all;

        uint64_t answer{0};
        traverseFs(fs, answer, all);
        cout << fs.size << endl;
        cout << answer << endl;

        sort(all.begin(), all.end());
        const auto needtofree = 30000000 - (70000000 - fs.size);
        cout << *upper_bound(all.begin(), all.end(), needtofree) << endl;
    }
}
