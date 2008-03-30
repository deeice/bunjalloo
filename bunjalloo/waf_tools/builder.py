#!/usr/bin/env python
# encoding: utf-8
import os

def build(bld):
  APPNAME = os.path.basename(bld.m_curdirnode.srcpath(bld.env()))
  arm9font = bld.create_obj('bin2o')
  arm9font.source = """
                      common/fonts/vera.img
                      common/fonts/vera.map
                      common/fonts/vera.pal
                     """
  arm9font.target = 'vera'
  sdlfont = arm9font.clone('sdl')

  arm9 = bld.create_obj('cpp', 'program')
  arm9.find_sources_in_dirs('arm9')
  arm9.includes += ' .'
  arm9.uselib_local = 'bwt vera'
  sdl = arm9.clone('sdl')
  arm9.uselib = 'ARM9'
  arm9.target = APPNAME+'-arm9'
  sdl.target = APPNAME

  arm9bin = bld.create_obj('objcopy')
  arm9bin.source = arm9.target
  arm9bin.target = arm9.target+'.bin'

  nds = bld.create_obj('ndstool')
  nds.source = arm9bin.target
  nds.target = APPNAME+'.nds'
