#!/usr/bin/env python3

# Copyright (c) 2015-2020 Agalmic Ventures LLC (www.agalmicventures.com)
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

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
	'ipcConsoleServer.clientConnected': ('SUCCESS', '<IpcConsoleServer> Connected new client %(clientPid)s'),
	'ipcConsoleServer.outOfOrderSequence': ('ERROR', '<IpcConsoleServer> Received out of order sequence number from client %(clientPid)s: got %(sequenceNumber)s vs last %(lastSequenceNumber)s'),
	'ipcConsoleServer.duplicateSequence': ('ERROR', '<IpcConsoleServer> Received duplicate sequence number from client %(clientPid)s: got %(sequenceNumber)s vs last %(lastSequenceNumber)s'),
	'ipcConsoleServer.missingSequence': ('WARNING', '<IpcConsoleServer> Missing sequence numbers from client %(clientPid)s: got %(sequenceNumber)s vs last %(lastSequenceNumber)s'),

	'mainLoop.enter': ('ALERT', '<ApplicationContext> Entering main loop...'),
	'mainLoop.exit': ('ALERT', '<ApplicationContext> Exiting main loop after %(updates)s updates...'),

	'shutdown.shutdownActions': ('INFO', '<ApplicationContext> Running shutdown actions...'),
	'shutdown.shutdownActionsComplete': ('SUCCESS', '<ApplicationContext> Shutdown actions complete.'),
	'shutdown.shuttingDown': ('INFO', '<ApplicationContext> Shutting down...'),

	'startup.software': ('INFO', '<ApplicationContext> Detected... OS: %(os)s | Hostname: %(hostname)s | CWD: %(cwd)s.'),
	'startup.pid': ('INFO', '<ApplicationContext> Detected... PID: %(pid)s | PPID: %(ppid)s'),
	'startup.hardware': ('INFO', '<ApplicationContext> Detected... %(processorCount)s CPU cores | %(physicalMemory)s bytes of RAM [%(pageSize)s per page].'),
	'startup.initialized': ('SUCCESS', '<ApplicationContext> Initialized.'),

	'timeSeriesReplayer.initializing': ('INFO', '<TimeSeriesReplayer> Initializing...'),
	'timeSeriesReplayer.initialized': ('SUCCESS', '<TimeSeriesReplayer> Initialized.'),
	'timeSeriesReplayer.unorderedSource': ('ERROR', '<TimeSeriesReplayer> Removing source %(name)s for out of order times: next %(nextTime)s < last %(time)s.'),
	'timeSeriesReplayer.finishedSource': ('INFO', '<TimeSeriesReplayer> Finished source: %(name)s.'),
	'timeSeriesReplayer.finished': ('SUCCESS', '<TimeSeriesReplayer> Finished.'),
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
	for n, line in enumerate(sys.stdin):
		try:
			if line == '':
				time.sleep(0.1)
				continue
			elif line[0] == '{':
				try:
					line = line.replace('\":nan', '\":NaN') #XXX: sort of a hack, but this works to fix how C++ outputs NaN
					value = json.loads(line)
				except ValueError:
					print(line)
					continue

				handleLine(value)
			else:
				print(line)
		except UnicodeDecodeError:
			print('Unicode error on line %d' % n)
		except KeyboardInterrupt:
			if quitting:
				break
			else:
				quitting = True
				continue
		except Exception as e: # @suppress the general exception warning, it's important to let nothing through
			#print('Exception thrown: %s -- %s' % (type(e), e))
			break

	return 0

if __name__ == '__main__':
	sys.exit(main())
