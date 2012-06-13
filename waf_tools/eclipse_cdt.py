import sys
import os
import Utils
import Logs
from Constants import APPNAME
from xml.dom.minidom import Document
import Options
import Build

oe_cdt = 'org.eclipse.cdt'
cdt_mk = oe_cdt + '.make.core'
cdt_core = oe_cdt + '.core'
cdt_bld = oe_cdt + '.build.core'

def add(doc, parent, tag, value = None):
    el = doc.createElement(tag)
    if (value):
        if type(value) == type(str()):
            el.appendChild(doc.createTextNode(value))
        elif type(value) == type(dict()):
            setAttributes(el, value)
    parent.appendChild(el)
    return el

def addDictionary(doc, parent, k, v):
    dictionary = add(doc, parent, 'dictionary')
    add(doc, dictionary, 'key', k)
    add(doc, dictionary, 'key', v)
    return dictionary

def setAttributes(node, attrs):
    for k, v in attrs.items():
        node.setAttribute(k, v)

def _create_project(executable, appname):
    doc = Document()
    projectDescription = doc.createElement('projectDescription')
    add(doc, projectDescription, 'name', appname)
    add(doc, projectDescription, 'comment')
    add(doc, projectDescription, 'projects')
    buildSpec = add(doc, projectDescription, 'buildSpec')
    buildCommand = add(doc, buildSpec, 'buildCommand')
    add(doc, buildCommand, 'name', oe_cdt + '.managedbuilder.core.genmakebuilder')
    add(doc, buildCommand, 'triggers', 'clean,full,incremental,')
    arguments = add(doc, buildCommand, 'arguments')
    # the default make-style targets are overwritten by the .cproject values
    dictionaries = {
            cdt_mk + '.contents': cdt_mk + '.activeConfigSettings',
            cdt_mk + '.enableAutoBuild': 'false',
            cdt_mk + '.enableCleanBuild': 'true',
            cdt_mk + '.enableFullBuild': 'true',
            }
    for k, v in dictionaries.items():
        addDictionary(doc, arguments, k, v)

    natures = add(doc, projectDescription, 'natures')
    nature_list = """
        core.ccnature
        managedbuilder.core.ScannerConfigNature
        managedbuilder.core.managedBuildNature
        core.cnature
    """.split()
    for n in nature_list:
        add(doc, natures, 'nature', oe_cdt + '.' + n)

    add(doc, natures, 'nature', 'org.python.pydev.pythonNature')

    doc.appendChild(projectDescription)
    return doc.toxml()

def addTarget(doc, buildTargets, executable, name, buildTarget, runAllBuilders=True):
    target = add(doc, buildTargets, 'target',
                    {'name': name,
                     'path': '',
                     'targetID': oe_cdt + '.build.MakeTargetBuilder'})
    add(doc, target, 'buildCommand', executable)
    add(doc, target, 'buildArguments', None)
    add(doc, target, 'buildTarget', buildTarget)
    add(doc, target, 'stopOnError', 'true')
    add(doc, target, 'useDefaultCommand', 'false')
    add(doc, target, 'runAllBuilders', str(runAllBuilders).lower())

def _create_cproject(executable, waf, appname, workspace_includes,
        cpppath, source_dirs=[]):
    doc = Document()
    doc.appendChild(doc.createProcessingInstruction('fileVersion', '4.0.0'))
    cconf_id = cdt_core + '.default.config.1'
    cproject = doc.createElement('cproject')
    storageModule = add(doc, cproject, 'storageModule',
            {'moduleId': cdt_core + '.settings'})
    cconf = add(doc, storageModule, 'cconfiguration', {'id':cconf_id})

    storageModule = add(doc, cconf, 'storageModule',
            {'buildSystemId': oe_cdt + '.managedbuilder.core.configurationDataProvider',
             'id': cconf_id,
             'moduleId': cdt_core + '.settings',
             'name': 'Default'})

    add(doc, storageModule, 'externalSettings')

    extensions = add(doc, storageModule, 'extensions')
    extension_list = """
        VCErrorParser
        MakeErrorParser
        GCCErrorParser
        GASErrorParser
        GLDErrorParser
    """.split()
    ext = add(doc, extensions, 'extension',
                {'id': cdt_core + '.ELF', 'point':cdt_core + '.BinaryParser'})
    for e in extension_list:
        ext = add(doc, extensions, 'extension',
                {'id': cdt_core + '.' + e, 'point':cdt_core + '.ErrorParser'})

    storageModule = add(doc, cconf, 'storageModule',
            {'moduleId': 'cdtBuildSystem', 'version': '4.0.0'})
    config = add(doc, storageModule, 'configuration',
                {'artifactName': appname,
                 'id': cconf_id,
                 'name': 'Default',
                 'parent': cdt_bld + '.prefbase.cfg'})
    folderInfo = add(doc, config, 'folderInfo',
                        {'id': cconf_id+'.', 'name': '/', 'resourcePath': ''})

    toolChain = add(doc, folderInfo, 'toolChain',
            {'id': cdt_bld + '.prefbase.toolchain.1',
             'name': 'No ToolChain',
             'resourceTypeBasedDiscovery': 'false',
             'superClass': cdt_bld + '.prefbase.toolchain'})

    targetPlatform = add(doc, toolChain, 'targetPlatform',
            { 'binaryParser': 'org.eclipse.cdt.core.ELF',
              'id': cdt_bld + '.prefbase.toolchain.1', 'name': ''})

    waf_build = '"%s" build'%(waf)
    waf_clean = '"%s" clean'%(waf)
    builder = add(doc, toolChain, 'builder',
                    {'autoBuildTarget': waf_build,
                     'command': executable,
                     'enableAutoBuild': 'false',
                     'cleanBuildTarget': waf_clean,
                     'enableIncrementalBuild': 'true',
                     'id': cdt_bld + '.settings.default.builder.1',
                     'incrementalBuildTarget': waf_build,
                     'managedBuildOn': 'false',
                     'name': 'Gnu Make Builder',
                     'superClass': cdt_bld + '.settings.default.builder'})

    for tool_name in ("Assembly", "GNU C++", "GNU C"):
        tool = add(doc, toolChain, 'tool',
                {'id': cdt_bld + '.settings.holder.1',
                 'name': tool_name,
                 'superClass': cdt_bld + '.settings.holder'})
        if cpppath or workspace_includes:
            incpaths = cdt_bld + '.settings.holder.incpaths'
            option = add(doc, tool, 'option',
                    {'id': incpaths+'.1',
                     'name': 'Include Paths',
                     'superClass': incpaths,
                     'valueType': 'includePath'})
            for i in workspace_includes:
                add(doc, option, 'listOptionValue',
                            {'builtIn': 'false',
                            'value': '"${workspace_loc:/%s/%s}"'%(appname, i)})
            for i in cpppath:
                add(doc, option, 'listOptionValue',
                            {'builtIn': 'false',
                            'value': '"%s"'%(i)})
    if source_dirs:
        sourceEntries = add(doc, config, 'sourceEntries')
        for i in source_dirs:
             add(doc, sourceEntries, 'entry',
                        {'excluding': i,
                        'flags': 'VALUE_WORKSPACE_PATH|RESOLVED',
                        'kind': 'sourcePath',
                        'name': ''})
             add(doc, sourceEntries, 'entry',
                        {
                        'flags': 'VALUE_WORKSPACE_PATH|RESOLVED',
                        'kind': 'sourcePath',
                        'name': i})

    storageModule = add(doc, cconf, 'storageModule',
                        {'moduleId': cdt_mk + '.buildtargets'})
    buildTargets = add(doc, storageModule, 'buildTargets')
    def addTargetWrap(name, runAll):
        return addTarget(doc, buildTargets, executable, name,
                            '"%s" %s'%(waf, name), runAll)
    addTargetWrap('configure', True)
    addTargetWrap('dist', False)
    addTargetWrap('install', False)
    addTargetWrap('check', False)

    storageModule = add(doc, cproject, 'storageModule',
                        {'moduleId': 'cdtBuildSystem',
                         'version': '4.0.0'})

    project = add(doc, storageModule, 'project',
                {'id': '%s.null.1'%appname, 'name': appname})

    doc.appendChild(cproject)
    return doc.toxml()

def _create_pydevproject(appname, system_path, user_path):
    # create a pydevproject file
    doc = Document()
    doc.appendChild(doc.createProcessingInstruction('eclipse-pydev', 'version="1.0"'))
    pydevproject = doc.createElement('pydev_project')
    prop = add(doc, pydevproject,
                   'pydev_property',
                   'python %d.%d'%(sys.version_info[0], sys.version_info[1]))
    prop.setAttribute('name', 'org.python.pydev.PYTHON_PROJECT_VERSION')
    prop = add(doc, pydevproject, 'pydev_property', 'Default')
    prop.setAttribute('name', 'org.python.pydev.PYTHON_PROJECT_INTERPRETER')
    # add waf's paths
    wafadmin = [p for p in system_path if p.find('wafadmin') != -1]
    if wafadmin:
        prop = add(doc, pydevproject, 'pydev_pathproperty',
                {'name':'org.python.pydev.PROJECT_EXTERNAL_SOURCE_PATH'})
        for i in wafadmin:
            add(doc, prop, 'path', i)
    if user_path:
        prop = add(doc, pydevproject, 'pydev_pathproperty',
                {'name':'org.python.pydev.PROJECT_SOURCE_PATH'})
        for i in user_path:
            add(doc, prop, 'path', '/'+appname+'/'+i)

    doc.appendChild(pydevproject)
    return doc.toxml()

def create_cproject(appname, build=None, workspace_includes=[], pythonpath=[]):
    """
    Create the Eclipse CDT .project and .cproject files
    @param appname The name that will appear in the Project Explorer
    @param build The BuildContext object to extract includes from
    @param workspace_includes Optional project includes to prevent
          "Unresolved Inclusion" errors in the Eclipse editor
    @param pythonpath Optional project specific python paths
    """
    cpppath = []
    source_dirs = []
    if build:
        cpppath = build.env['CPPPATH']
        Utils.pprint('YELLOW', 'Generating Eclipse CDT project files')
        for task in build.all_task_gen:
            l = Utils.to_list(getattr(task, "includes", ''))
            sources = Utils.to_list(getattr(task, 'source', ''))
            features = Utils.to_list(getattr(task, 'features', ''))
            is_cc = 'cc' in features or 'cxx' in features
            base = task.path.srcpath(task.env)
            bldpath = task.path.bldpath(task.env)
            base = os.path.normpath(os.path.join(build.bldnode.name, base))
            if is_cc:
                for source in set(os.path.normpath(os.path.join(base, os.path.dirname(source))) for source in sources):
                    if source not in source_dirs:
                        source_dirs.append(source)
            for p in l:
                inc = os.path.normpath(os.path.join(base, p))
                bld = os.path.normpath(os.path.join(build.bldnode.name, bldpath, p))
                if inc not in workspace_includes:
                    workspace_includes.append(inc)
                    workspace_includes.append(bld)
                if is_cc and inc not in source_dirs:
                    source_dirs.append(inc)
        build.all_task_gen = []
        build.task_manager.groups = []
    waf = os.path.abspath(sys.argv[0])
    project = _create_project(sys.executable, appname)
    f = open('.project', 'w')
    f.write(project)
    f.close()

    project = _create_cproject(sys.executable,
            waf,
            appname,
            workspace_includes,
            cpppath,
            source_dirs)
    f = open('.cproject', 'w')
    f.write(project)
    f.close()

    project = _create_pydevproject(appname, sys.path, pythonpath)
    f = open('.pydevproject', 'w')
    f.write(project)
    f.close()

def detect(conf):
    try:
        should_generate = Options.options.generate_eclipse
    except:
        conf.fatal("Add set_options(opt): opt.tool_options('eclipse_cdt')")
    return True

def set_options(opt):
    opt.add_option('--generate-eclipse',
            action='store_true',
            help='Generate Eclipse CDT project files (use with "build")',
            default=False)

def builder(f):
    "decorator: attach new BuildContext functions"
    setattr(Build.BuildContext, f.__name__, f)
    return f

@builder
def check_generate_eclipse(self, projectname=None, pythonpath=[]):
    try:
        if Options.options.generate_eclipse:
            appname = getattr(Utils.g_module, APPNAME,
                    os.path.basename(self.srcnode.abspath()))
            self.env['ECLIPSE_CDT_PROJECT'] = appname
            self.env['ECLIPSE_PYTHON_PATH'] = pythonpath
            self.add_pre_fun(pre_build)
    except:
         raise Utils.WafError("Add configure(conf): conf.check_tool('eclipse_cdt')")

def pre_build(bld):
    create_cproject(bld.env['ECLIPSE_CDT_PROJECT'],
            build=bld,
            pythonpath=bld.env['ECLIPSE_PYTHON_PATH'])

__all__ = ['create_cproject']
