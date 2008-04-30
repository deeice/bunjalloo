""" Set up the ARM7 compile flags """
import arm

def detect(conf):
  """ Detect devkitArm for ARM7 """
  arm.setup_tool(conf, 7)

