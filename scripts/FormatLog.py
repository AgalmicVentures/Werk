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

jsonMessageTranslations = {
	'mainLoop.enter': ('ALERT', '<ApplicationContext> Entering main loop...'),
	'mainLoop.exit': ('ALERT', '<ApplicationContext> Exiting main loop...'),

	'shutdown.shutdownActions': ('INFO', '<ApplicationContext> Running shutdown actions...'),
	'shutdown.shutdownActionsComplete': ('SUCCESS', '<ApplicationContext> Shutdown actions complete.'),
	'shutdown.shuttingDown': ('INFO', '<ApplicationContext> Shutting down...'),

	'startup.software': ('INFO', '<ApplicationContext> Detected... OS: %(os)s | Hostname: %(hostname)s | PID: %(pid)s | PPID: %(ppid)s | CWD: %(cwd)s.'),
	'startup.hardware': ('INFO', '<ApplicationContext> Detected... %(processorCount)s CPU cores | %(physicalMemory) bytes of RAM [%(pageSize) per page].'),
	'startup.initialized': ('SUCCESS', '<ApplicationContext> Initialized.'),
}

def handleLine(line):
	#Get basics
	n = line.get('n', 0)
	time = line.get('t', 0)
	timeSec, timeNs = divmod(time, 1000 * 1000 * 1000)
	level = line.get('level')

	message = None
	if level is None:
		#Handle JSON data
		data = line.get('data', {})
		messageTranslation = jsonMessageTranslations.get(data.get('type'))
		if messageTranslation is not None:
			try:
				message = messageTranslation[1] % data
				level = messageTranslation[0]
			except KeyError as e:
				message = str(e)
	else:
		message = line.get('message')

	#Colorize
	color = levelColors.get(level)

	if message is None:
			return

	print('%s[%05d] [%s.%09d] %8s - %s%s' % (color, n, timeSec, timeNs, level, message, nc))

def main():
	#Parse arguments
	parser = argparse.ArgumentParser(description='Log Formatting Script (For Humans)')
	#TODO: allow loading additional JSON translation dictionaries

	arguments = parser.parse_args(sys.argv[1:])

	quitting = False
	while True:
		try:
			line = sys.stdin.readline()
			if line == '':
				break

			value = json.loads(line)
			handleLine(value)
		except KeyboardInterrupt:
			if quitting:
				break
			else:
				quitting = True
				continue
		except Exception as e: # @suppress the general exception warning, it's important to let nothing through
			print('Exception thrown: %s -- %s' % (type(e), e))
			break

	return 0

if __name__ == '__main__':
	sys.exit(main())
