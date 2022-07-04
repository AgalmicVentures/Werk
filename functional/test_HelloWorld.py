
# Copyright (c) 2015-2022 Agalmic Ventures LLC (www.agalmicventures.com)
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

import subprocess
import unittest

def run(args, timeout=5):
	try:
		output = subprocess.check_output(['build/bin/HelloWorld'] + args, timeout=timeout)
	except subprocess.TimeoutExpired as e:
		output = e.output
	return output.decode('utf8')

class HelloWorldTest(unittest.TestCase):

	def test_emptyConfig(self):
		output = run(['/dev/null'], timeout=0.25)

		#TODO: what to assert?

	def test_dryRun(self):
		output = run(['-d'])

		#TODO: what to assert?

	def test_segfault(self):
		try:
			run(['functional/Segfault.ini'])
			self.assertFalse('Should not reach here; must throw the exception after segfaulting')
		except subprocess.CalledProcessError as e:
			self.assertEqual(e.returncode, -6) #-6 for segfaults
			self.assertIn('Intentionally segfaulting...', e.output.decode('utf8'))

	def test_segfaultNoSimulation(self):
		output = run(['-d', 'functional/SegfaultNoSimulation.ini'])
		self.assertIn('Command not found: segfault', output)

	#TODO: what other tests to add?
