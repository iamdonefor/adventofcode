#pragma once

const static int DIM{3};

class Coords : public vector<int> {
public:
    using vector::vector;
    Coords() : vector<int>({0, 0, 0}) {}

    Coords operator-(const Coords& other) const {
        Coords result;
        for (int i=0; i<DIM; ++i) {
            result[i] = (*this)[i] - other[i];
        }
        return result;
    }
};

struct Transform {

    Transform() = default;

    Transform(Coords axis, Coords signs)
    : Transform(move(axis), move(signs), {0, 0, 0})
    {}

    Transform(Coords axis, Coords signs, Coords corrections)
    : axis_(move(axis))
    , signs_(move(signs))
    , corrections_(move(corrections))
    {}

    Coords operator()(const Coords& c) const {
        Coords result;

        for (int i=0; i<DIM; ++i) {
            result[axis_[i]] = c[i] * signs_[axis_[i]] + corrections_[axis_[i]];
        }

        return result;
    }

    Transform set_corrections(const Coords& new_corrections) const {
        return Transform(axis_, signs_, new_corrections);
    }

    // Transform add_transform(const Transform)

    Coords axis_{0, 1, 2};
    Coords signs_{1, 1, 1};
    Coords corrections_{0, 0, 0};
};

ostream& operator<<(ostream& os, const Coords& v) {
    os << "{";
    bool first = true;
    for (const auto i : v) { os << (first ? "" : ", ") << i; first = false; }
    os << "}";
    return os;
}

ostream& operator<<(ostream& os, const Transform& t) {
    os << "<" << t.axis_ << ", " << t.signs_ << ", " << t.corrections_ << ">";
    return os;
}
