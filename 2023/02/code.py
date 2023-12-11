#!/usr/bin/env python

from io import StringIO
from collections import defaultdict
import sys

test = StringIO('''
Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue
Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red
Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red
Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green
''')


def parse(data):
	colors = defaultdict(int)
	games = []

	for s in data.split("\n"):
		s = s.strip()
		if not s: continue

		gameStr, gamesStr = s.split(':')
		gameN = int(gameStr[5:])

		games.append({"red":0, "blue":0, "green":0})
		current = games[-1]

		for game in gamesStr.split(";"):
			for clue in game.split(","):
				n, color = clue[1:].split(" ")
				n = int(n)
				colors[color] = max(colors[color], n)
				current[color] = max(current[color], n)

	return colors, games


def good_game(game):
	return game["red"] < 13 and game["green"] < 14 and game["blue"] < 15


def main(stream):
	data = stream.read()
	colors, games = parse(data)

	s = 0
	for i, game in enumerate(games, 1):
		if good_game(game):
			s += i
	print(s)

	s = 0
	for game in games:
		power = game["red"] * game["blue"] * game["green"]
		s += power
	print(s)


if __name__ == '__main__':
	main(test)
	main(sys.stdin)