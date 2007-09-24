.PHONY: $(BUILD) all clean

GRITFLAGS := -pw 16 -gB 8 -m -ft b
IMGFILES := $(wildcard *.png)
LIBNAME:=$(BUILD)/libimg.a
OFILES := $(IMGFILES:%.png=$(BUILD)/%.pal.o) $(IMGFILES:%.png=$(BUILD)/%.img.o) $(IMGFILES:%.png=$(BUILD)/%.map.o)
HFILES := $(IMGFILES:%.png=%.h)

all: $(BUILD) $(LIBNAME) $(HFILES)

clean:
	-rm -f $(BUILD)/*.o $(BUILD)/*.a *.h

$(BUILD):
	@[ -d $@ ] || mkdir -p $@

$(LIBNAME) : $(OFILES)
	@echo create $@
	@$(AR) rcs $@ $(OFILES)

%.img.bin : %.png
	grit $< $(GRITFLAGS) -p! -m! -fh!

%.pal.bin : %.png
	grit $< $(GRITFLAGS) -g! -m! -fh!

%.map.bin : %.png
	grit $< $(GRITFLAGS) -g! -p! -fh!

%.h : %.png
	$(shell \
	echo "#ifndef $*_h_seen" > $@ ;\
	echo "#define $*_h_seen" >> $@ ;\
	echo "#ifdef __cplusplus" >> $@ ;\
	echo "extern \"C\" {" >> $@ ;\
	echo "#endif" >> $@ ;\
        name=$$(echo _binary_$* | tr . _) ;\
	echo "extern const u16" $$name"_pal_bin_end[];" >> $@ ;\
	echo "extern const u16" $$name"_pal_bin_start[];" >> $@ ;\
	echo "extern const u32" $${name}_pal_bin_size[]";" >> $@ ;\
        name=$$(echo _binary_$* | tr . _) ;\
	echo "extern const u16" $$name"_img_bin_end[];" >> $@ ;\
	echo "extern const u16" $$name"_img_bin_start[];" >> $@ ;\
	echo "extern const u32" $$name"_img_bin_size[];" >> $@ ;\
	echo "extern const u16" $$name"_map_bin_end[];" >> $@ ;\
	echo "extern const u16" $$name"_map_bin_start[];" >> $@ ;\
	echo "extern const u32" $$name"_map_bin_size[];" >> $@ ;\
	echo "#ifdef __cplusplus" >> $@ ;\
	echo "};" >> $@ ;\
	echo "#endif" >> $@ ;\
	echo "#endif" >> $@ ;\
	)

$(BUILD)/%.o: %.bin
	@$(OBJCOPY) $(OBJCOPYFLAGS) $< $@
