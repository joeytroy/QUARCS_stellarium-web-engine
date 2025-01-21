import os
import glob
from SCons.Script import Environment, Variables, BoolVariable, EnumVariable

# Define build variables
vars = Variables()
vars.AddVariables(
    EnumVariable('mode', 'Build mode', 'debug', allowed_values=('debug', 'release', 'profile')),
    BoolVariable('es6', 'Create ES6 js module', False),
    BoolVariable('werror', 'Warnings as error', True),
)

# Set up environment
env = Environment(variables=vars, tools=['default', 'gcc'])

env.Append(CFLAGS=[
    '-Wall', '-std=gnu11', '-D_GNU_SOURCE', '-Wno-unknown-pragmas',
    '-Wno-missing-braces', '-Wno-unused-command-line-argument',
])
env.Append(CXXFLAGS=[
    '-Wall', '-std=gnu++11', '-Wno-unknown-pragmas', '-Wno-unused-function',
])
env.Append(LINKFLAGS=[
    '-s', 'ALLOW_MEMORY_GROWTH=1',
    '-s', 'ALLOW_TABLE_GROWTH=1',
    '-s', 'RESERVED_FUNCTION_POINTERS=10',
    '-s', 'EXPORT_NAME=StelWebEngine',
    '-s', 'USE_WEBGL2=1',
    '--no-entry',  # Suppress main requirement
    '-s', 'ERROR_ON_UNDEFINED_SYMBOLS=0',  # Allow undefined symbols
    '-s', 'LLD_REPORT_UNDEFINED=1',  # Debug undefined symbols
])
env.Append(CCFLAGS=['-include', 'config.h'])

# Add optional warnings as errors
if env['werror']:
    env.Append(CCFLAGS='-Werror')

# Set build mode flags
if env['mode'] == 'debug':
    env.Append(CCFLAGS=['-g', '-O0', '-DCOMPILE_TESTS'])
if env['mode'] == 'profile':
    env.Append(CCFLAGS=['-g'])
if env['mode'] == 'release':
    env.Append(CCFLAGS=['-O3', '-DNDEBUG'])

# Include directories
env.Append(CPPPATH=[
    'ext_src/uthash', 'src', 'src/utils', 'ext_src/stb', 'ext_src/erfa',
    'ext_src/json', 'ext_src/zlib', 'ext_src/inih', 'ext_src/nanovg',
    'ext_src/md4c', 'ext_src/webp', 'ext_src/webp/src', 'ext_src/sgp4',
    'ext_src/sgp4/cpp',
])

# Handle sources
sources = glob.glob('src/**/*.c', recursive=True)
sources += glob.glob('ext_src/**/*.c', recursive=True)

# Add SGP4-specific sources
sgp4_sources = [
    'ext_src/sgp4/sgp4unit.cpp',
    'ext_src/sgp4/sgp4io.cpp',
    'ext_src/sgp4/sgp4ext.cpp',
]
sources += ['src/sgp4.cpp']

# Avoid including src/libtess2.c multiple times
sources = [s for s in sources if 'src/libtess2.c' not in s]

# Check for EMCC environment variable
emcc = os.environ.get('EMCC', 'emcc')
if not emcc:
    raise EnvironmentError("EMCC is not set. Please ensure Emscripten is installed and `emcc` is in your PATH.")

env.Replace(CC=emcc, CXX=emcc, LINK=emcc)

# Add JS-specific linker flags
env.Append(LINKFLAGS=[
    '--pre-js', 'src/js/pre.js',
    '--pre-js', 'src/js/obj.js',
    '--pre-js', 'src/js/geojson.js',
    '--pre-js', 'src/js/canvas.js',
    '-s', 'EXPORTED_FUNCTIONS=["_sgp4", "_sgp4_twoline2rv", "_sgp4_get_satepoch", "_sgp4_get_perigree_height"]',
])

# Handle SGP4 Library
env.Append(LIBPATH=['/usr/local/lib'])  # Library path for SGP4
env.Append(LIBS=['m', 'sgp4'])  # Math library and SGP4

# Build targets
program = env.Program(target='build/stellarium-web-engine.js', source=sources)
env.Depends(program, glob.glob('src/js/*.js'))
env.Alias('js', program)

# Clean target
env.Alias('clean', None, 'scons -c')
