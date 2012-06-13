"""
Usage for CPU profiling:

configure --with-gprof
run the program as normal, it will generate a gmon.out file

    $ gprof build/default/path/to/binary gmon.out > profile

"""
import Options

def set_options(opt):
    opt.add_option(
            '--with-gprof',
            action='store_true',
            help='Use gprof for profiling',
            default=False)

def detect(conf):
    conf.find_program('gprof', mandatory=Options.options.with_gprof)
    if Options.options.with_gprof:
        conf.env['USE_PROFILER'] = ['']
        conf.env['CXXFLAGS'] = ['-g', '-pg']
        conf.env['CFLAGS'] = ['-g', '-pg']
        conf.env['LINKFLAGS'] = ['-g', '-pg']
