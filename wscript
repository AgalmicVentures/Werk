
import os

#################### waf Options ####################

NAME = 'TemplateCpp'
VERSION = '1.0'

top = '.'
out = 'build'

#################### Helpers ####################

def _loadTools(ctx):
	#Generic C++ compiler
	ctx.load('compiler_cxx')

	#Header dependencies
	ctx.load('c_preproc')

	#TODO: other tools?

def _preBuild(ctx):
	pass #TODO

def _postBuild(ctx):
	pass #TODO

#################### Commands ####################

def options(ctx):
	_loadTools(ctx)

	ctx.add_option('-d', '--debug', dest='debug', default=False, action='store_true', help='Debug mode')
	ctx.add_option('-s', '--symbols', dest='symbols', default=False, action='store_true', help='Debug symbols (on by default in debug mode)')

def configure(ctx):
	_loadTools(ctx)

	#Platform checks
	isMac = os.uname()[0] == 'Darwin'

	#Setup the environment
	ctx.env.INCLUDES += ['.', 'src']
	ctx.env.CXXFLAGS = [
		'-std=c++0x',
		'-Wall',
		'-Wextra',
		'-pedantic',
	]

	#Include symbols for debugging, or when explicitly requested
	if ctx.options.debug or ctx.options.symbols:
		ctx.env.CXXFLAGS.append('-g3')

	#Debugging
	if ctx.options.debug:
		ctx.env.DEFINES.append('DEBUG')
	else:
		ctx.env.CXXFLAGS.append('-O3')
		#XXX: ctx.env.CXXFLAGS.append('-Os')
		ctx.env.DEFINES.append('NDEBUG') #Causes asserts to compile out: http://www.cplusplus.com/reference/cassert/assert/

	#Setup libraries
	ctx.env.LIB = [
		#'pthread',
	]
	if isMac:
		ctx.env.LIBPATH.append('/usr/local/Cellar/boost/1.57.0/lib')

def build(ctx):
	ctx.add_pre_fun(_preBuild)
	ctx.add_post_fun(_postBuild)

	ctx.recurse('src')
	#TODO: ctx.recurse('test')

def runTests(ctx):
	stars = '*' * 30
	print
	print '%s Running Unit Tests %s' % (stars, stars)
	print
	#TODO: os.system("build/unit_tests")

