"""
Usage for code coverage:

configure --with-gcov
Compile the code and run the tests or run the program
Generate the coverage files:

    $ cd _build_
    $ gcov -o default/path/to/src SourceFile_N  # object file less the .o
    $ cp SourceFile.cpp.gcov ../path/to/src

    $ cd _build_
    $ lcov -b . -c -d default -o coverage.info
    $ genhtml coverage.info
"""
import Options
import Task

def set_options(opt):
    opt.add_option(
            '--with-gcov',
            action='store_true',
            help='Use gcov for profiling',
            default=False)

def setup(bld):
    # compile cc and cxx with absolute paths when using gcov
    for name in ('cc', 'cxx'):
        task = Task.TaskBase.classes[name]
        line = task.run.code.replace('SRC', 'SRC[0].abspath(env)')
        (fun, v) = Task.compile_fun(name, line, False)
        fun.code = line
        task.run = fun

def detect(conf):
    conf.find_program('gcov')
    gcov_flags = '-fprofile-arcs -ftest-coverage'.split()
    conf.env['CXXFLAGS'] = conf.env['CFLAGS'] = \
            conf.env['LINKFLAGS'] = gcov_flags

