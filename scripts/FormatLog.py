#!/usr/bin/env python3

import argparse
import json
import sys

nc = '\033[00m'
red = '\033[1;31m'
green = '\033[0;32m'
yellow = '\033[0;33m'
blue = '\033[0;34m'

levelColors = {
	'CRITICAL': '\033[0;33;41m', #Yellow with a red background
	'ERROR': red,
	'WARNING': '\033[38;5;202m',
	'ALERT': yellow,
	'SUCCESS': green,
	'CONFIG': '\033[0;36m', #Cyan
	'INFO': nc,
	'DETAIL': nc,
	'JSON': nc,
	'TRACE': nc,
	'UNKNOWN': nc,
}

def handleLine(line):
	#Get basics
	n = line.get('n', 0)
	time = line.get('t', 0)
	timeSec, timeNs = divmod(time, 1000 * 1000 * 1000)
	level = line.get('level', 'UNKNOWN')

	#Colorize
	color = levelColors.get(level)

	message = line.get('message')
	if message is None:
		return

	print('%s[%05d] [%s.%09d] %8s - %s%s' % (color, n, timeSec, timeNs, level, message, nc))

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
