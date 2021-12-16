#include "all.h"
using namespace std;

class BitStream {
public:
    const static uint64_t ONE{1};
    const static uint64_t ZERO{0};

    BitStream(const string& s) {
        for (int i=1; i<s.size(); i+=2) {
            data.push_back((decode(s[i-1]) << 4) | decode(s[i]));
        }
        size = 8 * data.size();
    }

    uint64_t get_literal() {
        uint64_t result{0};

        while (true) {
            result <<= 4;
            auto chunk = get(5);

            if ((chunk & 16) == 0) {
                result += chunk;
                return result;
            }

            result += (chunk - 16);
        }
    }

    uint64_t get(int len) {
        uint64_t result{0};

        if (offset + len > size) {
            throw runtime_error("read from empty stream");
        }

        for (int i=0; i<len; ++i) {
            result <<= 1;
            result |= next_bit();
            ++offset;
        }

        return result;
    }

    uint64_t seek(int len) {
        if (offset + len > size) {
            throw runtime_error("cant seek beyond the end");
        }

        offset += len;
        return offset;
    }

private:
    uint64_t next_bit() const {
        int byte_offset = offset / 8;
        int bit_offset = 7 - (offset % 8);

        uint8_t mask = (1 << bit_offset);
        if (data[byte_offset] & mask) {
            return ONE;
        } else {
            return ZERO;
        };
    }

    uint8_t decode(char c) const {
        if (c >= 'A' && c <= 'F') { return c - 'A' + 10; }
        if (c >= '0' && c <= '9') { return c - '0'; }
        cout << "wtf: " << c << endl;
        assert(false);
    }

    vector<uint8_t> data;
    int size{0};
    int offset{0};
};

struct Packet {
    int Version{0};
    int Type{0};
    uint64_t Value{0};
    vector<unique_ptr<Packet>> Children;

    bool Evaluated{false};

    uint64_t Evaluate() {
        if (Evaluated) {
            return Value;
        }

        switch (Type) {
            case 0: {
                for (const auto& child : Children) {
                    Value += child->Evaluate();
                }
                break;
            }
            case 1: {
                Value = 1;
                for (const auto& child : Children) {
                    Value *= child->Evaluate();
                }
                break;
            }
            case 2: {
                Value = numeric_limits<uint64_t>::max();
                for (const auto& child : Children) {
                    Value = min(child->Evaluate(), Value);
                }
                break;
            }
            case 3: {
                for (const auto& child : Children) {
                    Value = max(child->Evaluate(), Value);
                }
                break;
            }
            case 5: {
                assert(Children.size() == 2);
                Value = (Children[0]->Evaluate() > Children[1]->Evaluate() ? 1 : 0);
                break;
            }
            case 6: {
                assert(Children.size() == 2);
                Value = (Children[0]->Evaluate() < Children[1]->Evaluate() ? 1 : 0);
                break;
            }
            case 7: {
                assert(Children.size() == 2);
                Value = (Children[0]->Evaluate() == Children[1]->Evaluate() ? 1 : 0);
                break;
            }
        }

        Evaluated = true;
        return Value;
    }
};

unique_ptr<Packet> decode_stream(BitStream& bs) {
    auto packet = make_unique<Packet>();

    packet->Version = bs.get(3);
    packet->Type = bs.get(3);

    switch(packet->Type) {
        case 4: {
            packet->Value = bs.get_literal();
            packet->Evaluated = true;
            break;
        }
        default: {
            if (bs.get(1)) { // decode by packets
                auto n_more = bs.get(11);
                for (int i=0; i<n_more; ++i) {
                    packet->Children.emplace_back(decode_stream(bs));
                }
            } else {
                auto l_more = bs.get(15);
                auto up_to = bs.seek(0) + l_more;

                while (bs.seek(0) < up_to) {
                    packet->Children.emplace_back(decode_stream(bs));
                }
            }

            break;
        }
    }

    return move(packet);
}

unique_ptr<Packet> decode_stream(const string& s) {
    BitStream bs{s};
    return decode_stream(bs);
}

uint64_t part1(const unique_ptr<Packet>& root) {
    uint64_t sum{0};
    for (const auto& child : root->Children) {
        sum += part1(child);
    }

    sum += root->Version;
    return sum;
}

uint64_t part2(const unique_ptr<Packet>& root) {
    return root->Evaluate();
}

string parse_input(istream& in) {
    string s;
    getline(in, s);
    return s;
}

bool verify() {
    bool check1 = (
        16 == part1(decode_stream("8A004A801A8002F478"))
        && 23 == part1(decode_stream("C0015000016115A2E0802F182340"))
        && 12 == part1(decode_stream("620080001611562C8802118E34"))
        && 31 == part1(decode_stream("A0016C880162017C3686B18A3D4780"))
    );

    bool check2 = (1 == part2(decode_stream("9C0141080250320F1802104A08")));

    return check1 && check2;
}

int main() {
    assert(verify());

    auto root = decode_stream(parse_input(cin));
    cout << "part1: " << part1(root) << endl;
    cout << "part2: " << part2(root) << endl;
}
