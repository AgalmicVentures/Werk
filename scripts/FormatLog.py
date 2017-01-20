#!/usr/bin/env python3

import argparse
import json
import sys

def handleLine(line):
	#TODO: colorize

	n = line.get('n', 0)
	time = line.get('t', 0)
	timeSec, timeNs = divmod(time, 1000 * 1000 * 1000)
	level = line.get('level', 'UNKNOWN')
	message = line.get('message')
	if message is None:
		return

	print('[%05d] [%s.%09d] %8s - %s' % (n, timeSec, timeNs, level, message))

def main():
	#TODO: parse arguments

	while True:
		try:
			line = sys.stdin.readline()
			if line == '':
				break

			value = json.loads(line)
			handleLine(value)
		except KeyboardInterrupt:
			break
		except Exception as e:
			print('Exception thrown: %s -- %s' % (type(e), e))
			break

	return 0

if __name__ == '__main__':
	sys.exit(main())
