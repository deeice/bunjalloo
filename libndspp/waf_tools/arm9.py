""" Set up the ARM9 compile flags """
import arm

def detect(conf):
  """ Detect devkitArm for ARM9 """
  arm.setup_tool(conf, 9)
