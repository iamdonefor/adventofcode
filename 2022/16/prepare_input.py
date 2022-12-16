#!/usr/bin/env python
import re
import sys

test_input = '''Valve AA has flow rate=0; tunnels lead to valves DD, II, BB
Valve BB has flow rate=13; tunnels lead to valves CC, AA
Valve CC has flow rate=2; tunnels lead to valves DD, BB
Valve DD has flow rate=20; tunnels lead to valves CC, AA, EE
Valve EE has flow rate=3; tunnels lead to valves FF, DD
Valve FF has flow rate=0; tunnels lead to valves EE, GG
Valve GG has flow rate=0; tunnels lead to valves FF, HH
Valve II has flow rate=0; tunnels lead to valves AA, JJ
Valve JJ has flow rate=21; tunnel leads to valve II
Valve HH has flow rate=22; tunnel leads to valve GG
'''

header = '''#pragma once

struct tinput_node {
    string name;
    int rate;
    vector<string> adj;
};
'''

def parse(name, data):
    print "vector<tinput_node> " + name + "{"
    for s in data.split('\n'):
        if not s: continue
        pattern = "Valve ([A-Z]{2}) has flow rate=(\d+); tunnel[s]? lead[s]? to valve[s]? (.*)"
        name, rate, valves_raw = re.match(pattern, s).groups()
        valves = ['"' + x.strip() + '"' for x in valves_raw.split(",")]
        print "{ \"" + name + "\", {}".format(rate) + ", { " + ", ".join(valves) + " }},"
    print("};")
    print

def main(stream):
    print header
    parse("test_data", test_input)
    parse("prod_data", open("input").read())


if __name__ == '__main__':
    main(sys.stdin)
