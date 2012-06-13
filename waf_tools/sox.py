#!/usr/bin/python
import subprocess
import Logs
import Task
from os.path import basename
from TaskGen import extension

def parse_help(help_text):
    have_wav = False
    have_one = False
    for line in help_text:
        if line.upper().find('FILE FORMATS') != -1:
            if 'wav' in line.split(':')[1].split():
                have_wav = True
        elif line.find('-1') != -1:
            have_one = True
    return have_one, have_wav

def get_help(soxprog):
    process_pipe = subprocess.Popen([soxprog, '-h'],
            stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    process_pipe.wait()
    v = process_pipe.stdout.readlines()
    v.extend(process_pipe.stderr.readlines())
    return v

def detect(conf):
    sox = conf.find_program('sox', var='SOX')
    if not sox:
        return None
    have_one, have_wav = parse_help(get_help(conf.env['SOX']))
    flags = " -r 16000 -s -c 1 "
    extra = "-1" if have_one else "-b"
    flags += extra
    if not conf.env['SOXFLAGS']:
        conf.env['SOXFLAGS'] = flags.split()
    conf.check_message(basename(sox), 'has wav file format', have_wav)
    if not have_wav:
        Logs.error("""sox does not have wav. run `sudo apt-get install libsox-fmt-all'?""")
        conf.env['SOX'] = None
        return None

def setup(build):
    sox_str = "${SOX} ${SRC} ${SOXFLAGS} ${TGT}"
    Task.simple_task_type('sox', sox_str, before='padbin')

@extension('.wav')
def convert_wav(self, node):
    task = self.create_task('sox')
    task.set_inputs(node)
    task.set_outputs(node.change_ext('.raw'))
