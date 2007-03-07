SUFFIXES= .wav

%.all.rodata.o : %.wav 
	@sox $< -r 16000 -s -b -c 1 $*1.raw
	@padprog $*1.raw -p 4 -u -fo $*.raw 
	@arm-eabi-objcopy $(OBJCOPYFLAGS) $*.raw $@
	@-rm $*1.raw

