
export OBJCOPY :=
export OBJCOPYFLAGS := -I binary -O elf32-littlearm -B arm --rename-section .data=.rodata,readonly,contents

all:
	@$(MAKE) --no-print-dir -C img
	@$(MAKE) --no-print-dir -C snd

clean:
	@$(MAKE) --no-print-dir -C img clean
	@$(MAKE) --no-print-dir -C snd clean
