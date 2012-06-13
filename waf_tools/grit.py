""" Waf tool for generating image data using grit """
import os
import sys
import devkitarm
try:
    import Task
    import Utils
    from scwaf import build, apply_env_values, parse_task_gen_args
    from TaskGen import feature, after, before

    @build
    def transform_image(self, source, **kwargs):
        env, attr = parse_task_gen_args(kwargs)
        tg = self.new_task_gen(features='grit', source=source, **attr)
        apply_env_values(tg, env)
        return tg

    @feature('grit')
    @before('apply_core')
    def apply_core_grit(self):
        self.meths.remove('apply_core')
        do_apply_grit(self)

except ImportError:
    pass

def do_pallete_grit(self):
    get_source = self.path.find_resource
    get_node = self.path.find_or_declare
    palette = get_palette(self.env['GRITFLAGS'])
    task = self.create_task('grit')
    ext = grit_ext(self.env['GRITFLAGS'])
    source = Utils.to_list(self.source)
    task.set_inputs([get_source(src) for src in source])
    outputs = []
    for inp in task.inputs:
        outputs.extend([inp.change_ext(e) for e in ext])

    here = os.path.dirname(source[0])
    outputs.extend([get_node(os.path.join(here, palette + e)) for e in ext])
    task.set_outputs(outputs)
    self.tasks.append(task)

def do_listy_grit(self):
    get_node = self.path.find_or_declare
    get_source = self.path.find_resource
    for src in Utils.to_list(self.source):
        task = self.create_task('grit')
        src_node = get_source(src)
        task.set_inputs(src_node)
        ext = grit_ext(self.env['GRITFLAGS'])
        task.set_outputs([src_node.change_ext(e) for e in ext])
        self.tasks.append(task)

def do_apply_grit(self):
    get_node = self.path.find_or_declare
    get_source = self.path.find_resource
    palette = get_palette(self.env['GRITFLAGS'])
    if palette:
        do_pallete_grit(self)
    else:
        do_listy_grit(self)

def get_palette(flags):
    flags = Utils.to_list(flags)
    if '-pS' not in flags:
        return None
    for f in flags:
        if f.startswith('-O'):
            return f[2:]
    return None

def grit_ext(flags):
    flags = Utils.to_list(flags)
    out = ''
    noheader = '-fh!' in flags
    try:
        fh = flags.index('-fh')
        if fh != -1:
            out += ' .h'
    except ValueError:
        pass
    try:
        ft = flags.index('-ft')
        if flags[ft+1] == 'b':
            out += ' .img.bin'
        if flags[ft+1] == 'c':
            out += ' .c'
    except ValueError:
        pass
    try:
        ft = flags.index('-ftb')
        if ft != -1:
            out += ' .img.bin'
    except ValueError:
        pass
    try:
        ft = flags.index('-ftc')
        if ft != -1:
            out += ' .c'
    except ValueError:
        pass
    result = out.split()
    if '.c' in out and '.h' not in out and not noheader:
        result += ['.h']
    return result

def setup(bld):
    grit_str = sys.executable + " " + os.path.abspath(__file__) + \
            "  ${SRC[0].bld_dir(env)} ${GRIT} ${SRC} ${GRITFLAGS}"
    Task.simple_task_type('grit', grit_str, before='cc cxx')

def detect(conf):
    dka_bin = devkitarm.get_devkitarm_bin()
    grit = conf.find_program('grit', path_list=[dka_bin], var='GRIT', mandatory=True)
    v = conf.env
    if not v['GRITFLAGS']:
        v['GRITFLAGS'] = '-pw 16 -gB 8 -m! -ft b -fh! -q'.split()

if __name__ == '__main__':
    # wrap grit command to avoid chdir issues
    # arg1 = new path
    # arg2 = grit cmd
    # arg3+ = grit args
    import subprocess
    try:
        args = []
        # convert filenames to absolute file names
        for arg in sys.argv[2:]:
            if arg.startswith('..'):
                arg = os.path.abspath(arg)
            args.append(arg)

        os.chdir(sys.argv[1])
        proc = subprocess.Popen(args)
        proc.wait()
    except Exception,e:
        sys.stderr.write(str(e) + '\n')
        sys.exit(1)
