ifeq ($(strip $(OBJCOPY)),)
include $(DEVKITARM)/ds_rules
BUILD := build.ds
else
BUILD := build.sdl
endif

SUFFIXES= .wav
OUTDIR := $(BUILD)

WAVFILES  := $(wildcard *.wav)
WAVOFILES := $(WAVFILES:%.wav=$(OUTDIR)/%.all.rodata.o)
TOOLS :=../../tools
PADPROG := $(TOOLS)/padprog

LIBSND := $(OUTDIR)/libsnd.a


all: $(BUILD) $(PADPROG) $(OUTDIR) $(LIBSND) sounds.h

$(BUILD):
	test -d $(BUILD) || mkdir -p $(BUILD)

$(OUTDIR):
	@[ -d $(OUTDIR) ] || mkdir -p $(OUTDIR)


$(PADPROG):
	@$(MAKE) --no-print-dir -C $(TOOLS)

clean:
	@echo clean snd
	@rm $(OUTDIR)/*.o -f


$(LIBSND) : $(WAVOFILES) 
	$(AR) rcs $@ $^

%.raw : %.wav
	@sox $< -r 16000 -s -b -c 1 $@

%.snd : %.raw
	@$(PADPROG) $< -p 4 -u -fo $@

$(OUTDIR)/%.all.rodata.o : %.snd 
	$(OBJCOPY) $(OBJCOPYFLAGS) $< $@

sounds.h: $(WAVFILES)
	$(shell \
	echo "#ifndef sounds_h_seen" > $@ ;\
	echo "#define sounds_h_seen" >> $@ ;\
	echo "#ifdef __cplusplus" >> $@ ;\
	echo "extern \"C\" {" >> $@ ;\
	echo "#endif" >> $@ ;\
	for i in $^ ;\
	do \
        name=$$(echo _binary_$$(basename $$i) |sed -r 's/wav//g' | sed -e 's/^\([0-9]\)/_\1/' | tr . _) ;\
	echo "extern const u16" $$name"snd_end[];" >> $@ ;\
	echo "extern const u16" $$name"snd_start[];" >> $@ ;\
	echo "extern const u32" $${name}snd_size[]";" >> $@ ;\
	done ;\
	echo "#ifdef __cplusplus" >> $@ ;\
	echo "};" >> $@ ;\
	echo "#endif" >> $@ ;\
	echo "#endif" >> $@ ;\
	)
