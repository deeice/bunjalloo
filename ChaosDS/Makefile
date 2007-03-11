#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM)
endif

SHELL = bash

include $(DEVKITARM)/ds_rules

export TARGET		:=	$(shell basename $(CURDIR))
export TOPDIR		:=	$(CURDIR)


#---------------------------------------------------------------------------------
# path to tools - this can be deleted if you set the path in windows
#---------------------------------------------------------------------------------
export PATH		:=	$(DEVKITARM)/bin:$(PATH)

.PHONY: $(TARGET).arm7 $(TARGET).arm9 docs cscope.out

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
all: cscope.out $(TARGET).ds.gba

$(TARGET).ds.gba	: $(TARGET).nds

#---------------------------------------------------------------------------------
$(TARGET).nds	:	$(TARGET).arm7 $(TARGET).arm9
	@ndstool	-c $(TARGET).nds -7 $(TARGET).arm7 -9 $(TARGET).arm9 \
	  -b common/data/icon_palette.bmp "$(TARGET);By Richard Quirk;_" > /dev/null

#---------------------------------------------------------------------------------
$(TARGET).arm7	: arm7/$(TARGET).elf
$(TARGET).arm9	: arm9/$(TARGET).elf

#---------------------------------------------------------------------------------
arm7/$(TARGET).elf:
	$(MAKE) -C arm7
	
#---------------------------------------------------------------------------------
export PALETTES := $(shell ls -d $(TOPDIR)/common/img/palette*)

arm9/$(TARGET).elf:
	$(MAKE) -C common
	$(MAKE) -C arm9

#---------------------------------------------------------------------------------
clean:
	$(MAKE) -C arm9 clean
	$(MAKE) -C arm7 clean
	$(MAKE) -C common clean
	rm -f $(TARGET).ds.gba $(TARGET).nds $(TARGET).arm7 $(TARGET).arm9

wmb:
	wmb $(TARGET).nds

#tags: $(shell ls arm[7-9]/source/*.{cpp,h}) 
#	ctags -R .

cscope.out : $(shell find .. $(DEVKITPRO)/libnds/ -name '*.[chs]' -o -name '*.cpp' -o -name '*.inl')
	@rm -f cscope.files
	@find .. $(DEVKITPRO)/libnds/ -name '*.[chs]' -o -name '*.cpp' -o -name '*.inl' > cscope.files
	@-cscope -b -k -q


docs:
	doxygen chaos.doxy.cfg
