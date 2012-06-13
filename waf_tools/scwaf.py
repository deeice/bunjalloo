import Options
import Build
import Utils
import cxx
import cc
import os
import TaskGen
import Environment
import re
from Configure import conf

def option(func):
    "decorator: attach new options functions"
    setattr(Options.Handler, func.__name__, func)
    return func

def build(func):
    "decorator: attach new builder functions"
    setattr(Build.BuildContext, func.__name__, func)
    return func

def get_features(files):
    features = None
    for f in Utils.to_list(files):
        ext = '.' + f.split('.')[-1]
        if ext in cxx.EXT_CXX:
            return 'cxx'
        if ext in cc.EXT_CC:
            features = 'cc'
    return features

uppercase_re = re.compile('[A-Z0-9_]+')

def parse_task_gen_args(args):
    env = {}
    attr = {}
    for key, value in args.iteritems():
        if uppercase_re.match(key):
            env[key] = value
        else:
            attr[key] = value
    return env, attr

def apply_env_values(task_gen, env):
    for key, value in env.iteritems():
        task_gen.env[key] = value

CC_EXT_FILTER = cc.EXT_CC + cxx.EXT_CXX

def _prog_or_lib(self, features, target, source, **kwargs):
    env, attr = parse_task_gen_args(kwargs)
    here = self.path.abspath()
    join = os.path.join
    isdir = os.path.isdir
    isfile = os.path.isfile
    use_find = ' '.join(s for s in Utils.to_list(source) if isdir(join(here, s)))
    source_files = ' '.join(s for s in Utils.to_list(source) if isfile(join(here, s)))
    tg = self.new_task_gen(
            features=features,
            target=target,
            includes=Utils.to_list(use_find),
            source=source_files,
            **attr)
    tg.find_sources_in_dirs(use_find, exts=CC_EXT_FILTER)
    apply_env_values(tg, env)
    return tg

@build
def static_library(self, target, source, **kwargs):
    """ Add a library to the project using the specified source directories. """
    kwargs['export_incdirs'] = Utils.to_list(source)
    return _prog_or_lib(self, 'cxx cstaticlib',
            target, source, **kwargs)

@build
def program(self, target, source, **kwargs):
    """ Add a program to the project using the specified source directories. """
    return _prog_or_lib(self, 'cxx cprogram',
            target, source, **kwargs)

@build
def set_variant(self, variant_name):
    """ Set the current variant for the build """
    set_env = getattr(Build.BuildContext, 'set_env')
    setattr(Build.BuildContext, 'env', property(lambda x: x.env_of_name(variant_name), set_env))

@TaskGen.taskgen
def include_directories(self, includes, export=True):
    """ Add include directories to the build. """
    already_includes = Utils.to_list(self.includes)
    already_includes.extend(Utils.to_list(includes))
    self.includes = already_includes

    if export:
        exported = Utils.to_list(self.export_incdirs)
        exported.extend(Utils.to_list(includes))
        self.export_incdirs = exported

@TaskGen.taskgen
def source_directories(self, sources, export=True):
    """ Add source directories to the build. """
    self.find_sources_in_dirs(sources)
    self.include_directories(sources, export)

@conf
def addenv(self, env_name, is_copy=False):
    if is_copy:
        variant_env = self.env.copy()
    else:
        variant_env = Environment.Environment()
    self.set_env_name(env_name, variant_env)
    variant_env.set_variant(env_name)
