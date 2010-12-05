import Options
import os

srcdir = '.'
blddir = 'build'
VERSION = '0.1.0'

def set_options(opt): 
    opt.tool_options('compiler_cxx')

def configure(conf):
    conf.check_tool('compiler_cxx')
    conf.check_tool('node_addon')

def build(bld):
    obj = bld.new_task_gen('cxx', 'shlib', 'node_addon')
    obj.target = 'termutil'
    obj.source = ['termutil.cpp']
    obj.lib=['curses']

def shutdown():
    # HACK to get termutil.node out of build directory.
    # better way to do this?
    src='build/default/termutil.node'
    dst='termutil.node'
    if Options.commands['clean']:
        if os.path.exists(dst): 
            os.unlink(dst)
    else:
        if os.path.exists(src) and not os.path.exists(dst):
            os.symlink(src, dst) 

