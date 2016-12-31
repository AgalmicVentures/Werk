
import os

from waflib import Logs

#################### waf Options ####################

NAME = 'Werk'
VERSION = '1.0'

top = '.'
out = 'build'

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
		#'pthread',
	]
	if isMac:
		ctx.env.LIBPATH.append('/usr/local/Cellar/boost/1.58.0/lib')

	Logs.info('Configured.')

def build(ctx):
	ctx.add_pre_fun(_preBuild)
	ctx.add_post_fun(_postBuild)

	ctx.recurse('src')
	#TODO: ctx.recurse('test')

def test(ctx):
	stars = '*' * 30
	Logs.info('%s Running Unit Tests %s' % (stars, stars))
	os.system("build/test/WerkTest")

