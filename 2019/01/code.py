def get_fuel(mass):
    fuel = mass // 3 - 2
    return fuel > 0 and fuel or 0

def get_total_fuel(mass):
    total_mass = 0

    while True:
        mass = get_fuel(mass)
        if mass == 0:
            break
        total_mass += mass

    return total_mass

def main():
    modules_fuel = list(int(x) for x in open("input.txt").readlines())

    part1 = sum(get_fuel(x) for x in modules_fuel)
    part2 = sum(get_total_fuel(x) for x in modules_fuel)

    return part1, part2

if __name__ == '__main__':
    print(main())
