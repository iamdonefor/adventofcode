#include "all.h"
using namespace std;

class SnailNumber {
public:
    using SnailNumberPtr = unique_ptr<SnailNumber>;

    SnailNumber(const string& s) {
        stringstream ss{s};
        init(ss, nullptr);
    }
    SnailNumber(stringstream& ss, SnailNumber* parent) {
        init(ss, parent);
    }

    int64_t get() const { assert(is_child()); return v_; }
    int64_t add_value(int64_t value) { assert(is_child()); v_ += value; }
    bool is_child() const { return left() == nullptr && right() == nullptr; }

    const SnailNumber* const left() const { return left_.get(); }
    const SnailNumber* const right() const { return right_.get(); }
    const SnailNumber* const parent() const { return parent_; }

    SnailNumber* next() {
        if (parent()->left() == this) {
            return parent()->right()->begin();
        } else {
            auto p = parent();
            while (p->parent() && p == p->parent()->right()) {
                p = p->parent();
            }

            return p->parent() ? p->parent()->right()->begin() : nullptr;
        }
    }

    SnailNumber* prev() {
        if (parent()->right() == this) {
            return parent()->left()->end();
        } else {
            auto p = parent();
            while (p->parent() && p == p->parent()->left()) {
                p = p->parent();
            }

            return p->parent() ? p->parent()->left()->end() : nullptr;
        }
    }

    SnailNumber* begin() const {
        for (auto* p=this; p; p=p->left()) { if (p->is_child()) return const_cast<SnailNumber*>(p); }
        return nullptr;
    }

    SnailNumber* end() const {
        for (auto* p=this; p; p=p->right()) { if (p->is_child()) return const_cast<SnailNumber*>(p); }
        return nullptr;
    }

    string to_string() const {
        return repr_;
    }

    bool explode(int depth=0) {
        if (depth >= EXPLODE_DEPTH && !is_child()) {
            assert(left_->is_child());
            assert(right_->is_child());

            if (auto left = left_->prev(); left) {
                left->add_value(left_->get());
            }
            if (auto right = right_->next(); right) {
                right->add_value(right_->get());
            }

            left_.reset(nullptr);
            right_.reset(nullptr);

            return true;
        }

        if (is_child()) { return false; }
        return left_->explode(depth+1) || right_->explode(depth+1);
    }

    bool split() {
        if (is_child() && get() >= SPLIT_VALUE) {
            // auto parent = n->prev;
            // auto node = new SnailNumber(parent);
            // node->left = new SnailNumber(node, n->V / 2);
            // node->right = new SnailNumber(node, (n->V + 1) / 2);
            //
            // if (n->prev->left == n) {
            //     n->prev->left = node;
            // } else {
            //     n->prev->right = node;
            // }
            //
            // delete n;
            return false;
        }

        if (is_child()) { return false; }
        return left_->split() || right_->split();
    }

    int64_t magnitude() const {
        if (is_child()) { return get(); }
        return 3*left()->magnitude() + 2*right()->magnitude();
    }

private:
    const static int EXPLODE_DEPTH{4};
    const static int SPLIT_VALUE{10};

    unique_ptr<SnailNumber> left_{nullptr};
    unique_ptr<SnailNumber> right_{nullptr};
    const SnailNumber* parent_{nullptr};

    string repr_;
    int64_t v_{0};

    void init(stringstream& ss, const SnailNumber* parent) {
        parse(ss, parent);
        reduce();

        stringstream builder;
        to_string(builder);
        repr_ = builder.str();
    }

    void reduce() {
        while (true) {
            while(explode()) continue;
            while(split()) continue;
            break;
        }
    }

    void parse(stringstream& ss, const SnailNumber* parent) {
        parent_ = parent;
        while (true) {
            char c;
            ss.get(c);

            switch (c) {
                case '[': { left_ = make_unique<SnailNumber>(ss, this); break; }
                case ']': { return; }
                case ',': { right_ = make_unique<SnailNumber>(ss, this); break; }
                default: {
                    assert(c >= '0' && c <= '9');
                    v_ = c - '0';
                    return;
                }
            }
        }
    }

    void to_string(stringstream& builder) const {
        if (is_child()) {
            builder << v_;
        } else {
            builder << "[";
            left_->to_string(builder);
            builder << ",";
            right_->to_string(builder);
            builder << "]";
        }
    }
};

SnailNumber operator+(const SnailNumber& l, const SnailNumber& r) {
    stringstream sum;
    sum << "[" << l.to_string() << "," << r.to_string() << "]";
    return SnailNumber(sum.str());
}

vector<string> get_input(istream& in) {
    vector<string> result;
    for (string s; getline(in, s);) {
        result.push_back(s);
    }
    return result;
}

int64_t homework(const vector<string>& numbers) {
    auto s = SnailNumber(numbers[0]);
    for (int i=1; i<numbers.size(); ++i) {
        s = s + SnailNumber(numbers[i]);
        cout << s.to_string() << endl;
    }
    return s.magnitude();
}

// int64_t get_max_magnitude(const vector<string>& numbers) {
//     int64_t max_magnitude = -1;
//     for (int i=0; i<numbers.size(); ++i) {
//     for (int j=0; j<numbers.size(); ++j) {
//         if (i == j) { continue; }
//
//         auto si = parseSnail(numbers[i]);
//         auto sj = parseSnail(numbers[j]);
//         auto result = add(si, sj);
//
//         max_magnitude = max(max_magnitude, result->magnitude());
//     }}
//
//     return max_magnitude;
// }

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

    // auto input = get_input(cin);
    // cout << "part1: " << homework(input) << endl;
    // cout << "part2: " << get_max_magnitude(input) << endl;
}
