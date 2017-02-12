
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

	#TODO

	Logs.info('Pre-build complete.')

def _postBuild(ctx):
	Logs.info('Post-build...')

	#TODO

	Logs.info('Post-build complete.')

#################### Commands ####################

def options(ctx):
	Logs.info('Loading options...')

	_loadTools(ctx)

	ctx.add_option('-d', '--debug', dest='debug', default=False, action='store_true', help='Debug mode')
	ctx.add_option('-s', '--symbols', dest='symbols', default=False, action='store_true', help='Debug symbols (on by default in debug mode)')

	Logs.info('Options loaded.')

def configure(ctx):
	Logs.info('Configuring...')

	_loadTools(ctx)

	#Platform checks
	isMac = os.uname()[0] == 'Darwin'

	#Setup the environment
	ctx.env.INCLUDES += ['.', 'src']
	ctx.env.CXXFLAGS = [
		'-std=c++11',
		'-Wall',
		'-Wextra',
		'-pedantic',
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

	#Setup libraries
	ctx.env.LIB = [
		'pthread',
	]

	if isMac:
		#Find a version of boost installed by brew
		boostRootPath = '/usr/local/Cellar/boost'
		boostVersion = os.listdir(boostRootPath)[-1]
		boostPath = os.path.join(boostRootPath, boostVersion)
		boostLibPath = os.path.join(boostPath, 'lib')
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
		Logs.info('Building version %s' % version)
	except subprocess.CalledProcessError:
		version = 'NotInGit'
		Logs.warn('Building vout of version control, so no version string is available')
	ctx.env.VERSION = version

	ctx.add_pre_fun(_preBuild)
	ctx.add_post_fun(_postBuild)

	ctx.recurse('src')
	#TODO: ctx.recurse('test')

def test(ctx, valgrind=False):
	stars = '*' * 30
	Logs.info('%s Running Unit Tests %s' % (stars, stars))

	binary = 'build/test/WerkTest'
	if not os.path.exists(binary):
		raise RuntimeError('Missing binary: %s' % binary)

	if valgrind:
		binary = 'valgrind --error-exitcode=99 ' + binary

	exitCode = os.system(binary)
	if exitCode != 0:
		raise RuntimeError('Non-zero return: %s -> %d' % (binary, exitCode))

def valgrind(ctx):
	test(ctx, valgrind=True)

def profile(ctx):
	stars = '*' * 30
	Logs.info('%s Running Profiles %s' % (stars, stars))

	binary = 'build/test/WerkProfile'
	if not os.path.exists(binary):
		raise RuntimeError('Missing binary: %s' % binary)

	exitCode = os.system(binary)
	if exitCode != 0:
		raise RuntimeError('Non-zero return: %s -> %d' % (binary, exitCode))
