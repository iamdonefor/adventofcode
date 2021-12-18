#include "all.h"
using namespace std;

struct SnailNumber {
    SnailNumber* left{nullptr};
    SnailNumber* right{nullptr};
    SnailNumber* prev{nullptr};

    int V{0};

    SnailNumber() = default;
    SnailNumber(SnailNumber* prev_) : prev(prev_) {}
    SnailNumber(SnailNumber* prev_, int v_) : prev(prev_), V(v_) {}

    bool is_child() const {
        return left == nullptr && right == nullptr;
    }

    SnailNumber* next() {
        if (prev->left == this) {
            return prev->right->begin();
        } else {
            auto p = prev;
            while (p->prev && p == p->prev->right) {
                p = p->prev;
            }

            return p->prev ? p->prev->right->begin() : nullptr;
        }
    }

    SnailNumber* rnext() {
        if (prev->right == this) {
            return prev->left->end();
        } else {
            auto p = prev;
            while (p->prev && p == p->prev->left) {
                p = p->prev;
            }

            return p->prev ? p->prev->left->end() : nullptr;
        }
    }

    SnailNumber* begin() {
        for (auto* p=this; p; p=p->left) {
            if (p->is_child()) {
                return p;
            }
        }
        return nullptr;
    }

    SnailNumber* end() {
        for (auto* p=this; p; p=p->right) {
            if (p->is_child()) {
                return p;
            }
        }
        return nullptr;
    }

    void free(SnailNumber* n) {
        SnailNumber* zero = new SnailNumber(this, 0);

        if (n == left) {
            delete left;
            left = zero;
        } else {
            delete right;
            right = zero;
        }
    }

    void to_string(stringstream& builder) {
        if (is_child()) {
            builder << V;
        } else {
            builder << "[";
            left->to_string(builder);
            builder << ",";
            right->to_string(builder);
            builder << "]";
        }
    }

    string to_string() {
        stringstream builder;
        to_string(builder);
        return builder.str();
    }

    int64_t magnitude() {
        if (is_child()) { return V; }
        return 3*left->magnitude() + 2*right->magnitude();
    }
};

const int EXPLODE{4};
const int SPLIT{10};

bool reduce_once(SnailNumber* n, bool do_split, int depth=0) {
    if (n == nullptr) { return false; }

    if (depth >= EXPLODE && !n->is_child()) {
        assert(n->left->is_child());
        assert(n->right->is_child());

        if (auto left = n->left->rnext(); left) {
            left->V += n->left->V;
        }
        if (auto right = n->right->next(); right) {
            right->V += n->right->V;
        }

        n->prev->free(n);

        return true;
    }

    if (do_split && n->is_child() && n->V >= SPLIT) {
        auto parent = n->prev;
        auto node = new SnailNumber(parent);
        node->left = new SnailNumber(node, n->V / 2);
        node->right = new SnailNumber(node, (n->V + 1) / 2);

        if (n->prev->left == n) {
            n->prev->left = node;
        } else {
            n->prev->right = node;
        }

        delete n;
        return true;
    }

    if (reduce_once(n->left, do_split, depth+1)) { return true; }
    if (reduce_once(n->right, do_split, depth+1)) { return true; }
    return false;
}

bool reduce(SnailNumber* n) {
    while (true) {
        if (reduce_once(n, false)) {
            continue;
        };
        if (reduce_once(n, true)) {
            continue;
        };
        break;
    }
    return true;
}

SnailNumber* parseSnail(stringstream& in, SnailNumber* prev) {
    SnailNumber *root = new SnailNumber(prev);

    while (true) {
        char c;
        in.get(c);

        switch (c) {
            case '[': {
                root->left = parseSnail(in, root);
                break;
            }
            case ']': {
                return root;
            }
            case ',': {
                root->right = parseSnail(in, root);
                break;
            }
            default: {
                assert(c >= '0' && c <= '9');
                root->V = c - '0';
                return root;
            }
        }
    }

    return root;
}

SnailNumber* add(SnailNumber* l, SnailNumber* r) {
    SnailNumber* root = new SnailNumber(nullptr);
    assert(l->prev == nullptr && r->prev == nullptr);

    root->left = l;
    root->right = r;
    l->prev = r->prev = root;

    reduce(root);
    return root;
}

SnailNumber* parseSnail(const string& s) {
    stringstream input(s);
    return parseSnail(input, nullptr);
}

vector<string> get_input(istream& in) {
    vector<string> result;
    for (string s; getline(in, s);) {
        result.push_back(s);
    }
    return result;
}

int64_t homework(const vector<string>& numbers) {
    auto s = parseSnail(numbers[0]);
    for (int i=1; i<numbers.size(); ++i) {
        s = add(s, parseSnail(numbers[i]));
    }
    return s->magnitude();
}

int64_t get_max_magnitude(const vector<string>& numbers) {
    int64_t max_magnitude = -1;
    for (int i=0; i<numbers.size(); ++i) {
    for (int j=0; j<numbers.size(); ++j) {
        if (i == j) { continue; }

        auto si = parseSnail(numbers[i]);
        auto sj = parseSnail(numbers[j]);
        auto result = add(si, sj);

        max_magnitude = max(max_magnitude, result->magnitude());
    }}

    return max_magnitude;
}

bool verify() {
    vector<string> test{
        "[[[0,[5,8]],[[1,7],[9,6]]],[[4,[1,2]],[[1,4],2]]]",
        "[[[5,[2,8]],4],[5,[[9,9],0]]]",
        "[6,[[[6,2],[5,6]],[[7,6],[4,7]]]]",
        "[[[6,[0,7]],[0,9]],[4,[9,[9,0]]]]",
        "[[[7,[6,4]],[3,[1,3]]],[[[5,5],1],9]]",
        "[[6,[[7,3],[3,2]]],[[[3,8],[5,7]],4]]",
        "[[[[5,4],[7,7]],8],[[8,3],8]]",
        "[[9,3],[[9,9],[6,[4,9]]]]",
        "[[2,[[7,7],7]],[[5,8],[[9,3],[0,2]]]]",
        "[[[[5,2],5],[8,[3,7]]],[[5,[7,5]],[4,4]]]",
    };

    return homework(test) == 4140;
}



int main() {
    assert(verify());

    auto input = get_input(cin);
    cout << "part1: " << homework(input) << endl;
    cout << "part2: " << get_max_magnitude(input) << endl;
}
