
# Copyright (c) 2015-2017 Agalmic Ventures LLC (www.agalmicventures.com)
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

import os

from waflib import Logs

#################### waf Options ####################

NAME = 'Werk'
VERSION = '1.0'

top = '.'
out = 'build'

versionCommand = 'git rev-parse HEAD'.split(' ')

#################### Helpers ####################

stars = '*' * 20

def _loadTools(ctx):
	#Generic C++ compiler
	ctx.load('compiler_cxx')

	#Header dependencies
	ctx.load('c_preproc')

	#Assembly language
	ctx.load('asm')

	#Parser building
	ctx.load('bison')
	ctx.load('flex')

def _preBuild(ctx):
	Logs.info('Pre-build...')

	#Nothing to do at the moment

	Logs.info('Pre-build complete.')

def _postBuild(ctx):
	Logs.info('Post-build...')

	environment = ctx.env.ENVIRONMENT
	if environment == 'debug':
		Logs.error('Finished building: %s DEBUG [%s] %s' % (stars, ctx.env.VERSION, stars))
	else:
		Logs.info('Finished building: %s RELEASE [%s] %s' % (stars, ctx.env.VERSION, stars))

	Logs.info('Post-build complete.')

#################### Commands ####################

def options(ctx):
	Logs.info('Loading options...')

	_loadTools(ctx)

	ctx.add_option('-d', '--debug', action='store_true',
		help='Debug mode')
	ctx.add_option('-l', '--lto', action='store_true',
		help='Debug symbols (on by default in debug mode)')
	ctx.add_option('-s', '--symbols', action='store_true',
		help='Debug symbols (on by default in debug mode)')
	ctx.add_option('--valgrind', action='store_true',
		help='Enable valgrind for the action if applicable')

	Logs.info('Options loaded.')

def configure(ctx):
	Logs.info('Configuring...')

	_loadTools(ctx)

	#Platform checks
	isMac = os.uname()[0] == 'Darwin'

	#Setup the environment
	ctx.env.INCLUDES += ['.'] #, 'src']
	ctx.env.CXXFLAGS = [
		'-std=c++17',
		'-march=native',
		'-mtune=native',

		'-Wall',
		'-Wno-trigraphs',
		'-Wuninitialized',
		'-Wtype-limits',
		'-Wmissing-include-dirs',
		'-Wshadow',

		'-pedantic',
	]
	if isMac:
		ctx.env.CXXFLAGS += [
			'-Wnull-dereference',
		]
	else:
		ctx.env.CXXFLAGS += [
			'-Wconversion',
			'-Wundef',
			'-Wuseless-cast',
		]

	#Include symbols for debugging, or when explicitly requested
	if ctx.options.debug or ctx.options.symbols:
		ctx.env.CXXFLAGS.append('-g3')

	#Debugging
	if ctx.options.debug:
		ctx.msg('Build environment', '%s DEBUG %s' % (stars, stars), color='RED')

		ctx.env.CXXFLAGS.append('-O0')
		ctx.env.DEFINES.append('DEBUG')
		ctx.env.ENVIRONMENT = 'debug'
	else:
		ctx.msg('Build environment', '%s RELEASE %s' % (stars, stars), color='BOLD')

		ctx.env.CXXFLAGS.append('-O3')
		ctx.env.DEFINES.append('NDEBUG') #Causes asserts to compile out: http://www.cplusplus.com/reference/cassert/assert/
		ctx.env.ENVIRONMENT = 'release'

		if ctx.options.lto:
			ctx.env.AR = 'gcc-ar'
			ctx.env.NM = 'gcc-nm'
			ctx.env.RNALIB = 'gcc-ranlib'
			ctx.env.CXXFLAGS.append('-flto')
			ctx.env.CXXFLAGS.append('-Wl,-flto')

	#Setup libraries
	ctx.env.LIB = [
		'dl',
		'pthread',
	]

	if isMac:
		#Find a version of boost installed by brew
		boostRootPath = '/usr/local/Cellar/boost'
		try:
			boostVersion = sorted(os.listdir(boostRootPath))[-1]
		except OSError:
			Logs.error('Missing boost -- try installing with `brew install boost`')
			raise

		boostPath = os.path.join(boostRootPath, boostVersion)
		boostLibPath = os.path.join(boostPath, 'lib')
		Logs.info('Boost path: %s' % boostPath)
		ctx.env.LIBPATH.append(boostLibPath)
	else:
		ctx.env.LIB += [
			'rt',
		]

	Logs.info('Configured.')

def build(ctx):
	#Called when the dependency tree needs to be computed, not only 'build'
	#(e.g. it also is called for 'clean').

	#Check the version immediately before the build happens
	import subprocess
	try:
		version = subprocess.check_output(versionCommand).strip()
		if not isinstance(version, str):
			version = version.decode('utf8') #Python 3 compatibility
		Logs.info('Building version %s' % version)
	except subprocess.CalledProcessError:
		version = 'NotInGit'
		Logs.warn('Building out of version control, so no version string is available')
	ctx.env.VERSION = version

	ctx.add_pre_fun(_preBuild)
	ctx.add_post_fun(_postBuild)

	#Build each of the subprojects
	ctx.recurse('src')

valgrindStr = 'valgrind --error-exitcode=99 --gen-suppressions=all --suppressions=./valgrind.supp '
def test(ctx):
	stars = '*' * 30
	Logs.info('%s Running Unit Tests %s' % (stars, stars))

	binary = 'build/test/WerkTest'
	if not os.path.exists(binary):
		raise RuntimeError('Missing binary: %s' % binary)

	if ctx.options.valgrind:
		binary = valgrindStr + binary

	exitCode = os.system(binary)
	if exitCode != 0:
		raise RuntimeError('Non-zero return: %s -> %d' % (binary, exitCode))

def profile(ctx):
	stars = '*' * 30
	Logs.info('%s Running Profiles %s' % (stars, stars))

	binary = 'build/test/WerkProfile'
	if not os.path.exists(binary):
		raise RuntimeError('Missing binary: %s' % binary)

	if ctx.options.valgrind:
		binary = valgrindStr + binary + ' -- --disable-tests'

	exitCode = os.system(binary)
	if exitCode != 0:
		raise RuntimeError('Non-zero return: %s -> %d' % (binary, exitCode))
