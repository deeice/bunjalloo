#!/usr/bin/python
import os

def get_devkitarm_bin():
    try:
        return os.path.join(os.environ['DEVKITARM'], 'bin')
    except KeyError:
        return ''

def detect(conf):
    from Logs import warn
    if 'DEVKITARM' not in os.environ:
        warn("`DEVKITARM' variable is not set, compiler may not be found")
    def prog(name, var=None):
        arm_eabi = 'arm-eabi-%s'
        if not var:
            var = name
        var = var.upper()
        exe = conf.find_program(arm_eabi % name,
                path_list=[get_devkitarm_bin()])
        if exe:
            conf.env[var] = exe

    prog('ar')
    prog('cpp')
    prog('gcc', 'cc')
    prog('g++', 'cxx')
    prog('g++', 'link_cxx')
    prog('objcopy')
    prog('ranlib')

    # update the compiler flags
    dkp = '/'
    try:
        dkp = os.environ['DEVKITPRO']
    except KeyError:
        warn("`DEVKITPRO' variable is not set, libraries may not be found")

    ARCH = '-mthumb -mthumb-interwork'.split()
    CFLAGS = '''-O2 -ffast-math -Wall -march=armv5te -mtune=arm946e-s
                -Wno-array-bounds -fomit-frame-pointer'''.split()
    CFLAGS += ARCH
    CXXFLAGS = CFLAGS + '-fno-rtti -fno-exceptions'.split()
    LINKFLAGS = '''-specs=ds_arm9.specs -Wl,-Map,map9.map
                   -Wl,-gc-sections'''.split()
    LINKFLAGS += ARCH
    OBJCOPYFLAGS = ''' -I binary -O elf32-littlearm -B arm
        --rename-section .data=.rodata,readonly,contents '''.split()
    CCDEFINES = ['ARM9']

    libnds = os.path.join(dkp, 'libnds')
    env = conf.env
    flags = {
        'CXXFLAGS': CXXFLAGS,
        'CCFLAGS': CFLAGS,
        'CCDEFINES': CCDEFINES,
        'CXXDEFINES': CCDEFINES,
        'CPPPATH': [os.path.join(libnds, 'include')],
        'LIBPATH': [os.path.join(libnds, 'lib')],
        'LINKFLAGS': LINKFLAGS,
        'OBJCOPYFLAGS': OBJCOPYFLAGS,
        'DEVKITARM': os.getenv('DEVKITARM'),
        'DEVKITPRO': os.getenv('DEVKITPRO'),
    }
    env.update(flags)

try:
    from Configure import conf
    @conf
    def check_libnds(self):
        # need -lfat -lnds9 in that order, but fat requires nds9...
        self.check(lib='nds9', uselib_store='NDS', mandatory=True)
        self.check(lib='fat', uselib='NDS', mandatory=True)
        self.env.prepend_value('LIB_NDS', 'fat')
except ImportError:
    pass
