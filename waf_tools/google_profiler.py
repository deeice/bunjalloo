"""
Usage for CPU profiling:

configure --with-profiler
compile the code
run the program as follows:

    $ CPUPROFILE=myfile.log ../_build_/default/path/to/prog

View the offending functions with:

    $ pprof --text ../_build_/default/path/to/prog myfile.log
    $ pprof --gv ../_build_/default/path/to/prog myfile.log

"""
import Options

def set_options(opt):
    opt.add_option(
            '--with-profiler',
            action='store_true',
            help='Use Google CPU profiler (http://code.google.com/p/google-perftools/)',
            default=False)
    opt.add_option(
            '--with-tcmalloc',
            action='store_true',
            help='Use Google Heap profiler (http://code.google.com/p/google-perftools/)',
            default=False)

def detect(conf):
    conf.env['USE_PROFILER'] = ''
    result = conf.check(lib='profiler',
            mandatory=Options.options.with_profiler)
    if Options.options.with_profiler:
        conf.env['USE_PROFILER'] = 'PROFILER'

    result = conf.check(lib='tcmalloc',
            mandatory=Options.options.with_profiler)
    if Options.options.with_tcmalloc:
        conf.env['USE_PROFILER'] += ' TCMALLOC'
