
NAME = 'TemplateCpp'
VERSION = '1.0'

top = '.'
out = 'build'

def options(ctx):
	ctx.load('compiler_cxx')
	ctx.load('c_preproc') #Used for header dependencies

	ctx.add_option('-d', '--debug', dest='debug', default=False, action='store_true', help='Debug mode')
	ctx.add_option('-s', '--symbols', dest='symbols', default=False, action='store_true', help='Debug symbols (on by default in debug mode)')

def configure(ctx):
	#Platform checks
	import os
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

	ctx.load('compiler_cxx')
	ctx.load('c_preproc')

def build(ctx):
	ctx.add_post_fun(runTests)

	ctx.recurse('src')
	#TODO: ctx.recurse('test')

def runTests(ctx):
	stars = '*' * 30
	print
	print '%s Running Unit Tests %s' % (stars, stars)
	print
	#TODO: os.system("build/unit_tests")

