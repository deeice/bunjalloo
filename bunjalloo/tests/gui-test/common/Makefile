export OBJCOPYFLAGS := -I binary -O elf32-littlearm -B arm --rename-section .data=.rodata,readonly,contents
export BUILD:=build.ds

all:
	@$(MAKE) --no-print-dir -C fonts

clean:
	@$(MAKE) --no-print-dir -C fonts clean
