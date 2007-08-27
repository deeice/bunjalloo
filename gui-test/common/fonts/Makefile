.PHONY: $(BUILD) all clean

LIBNAME:=$(BUILD)/libfont.a
IMGFILES := $(wildcard *.img)
PALFILES := $(wildcard *.pal)
MAPFILES := $(wildcard *.map)

OFILES := $(PALFILES:%.pal=$(BUILD)/%.pal.o) $(IMGFILES:%.img=$(BUILD)/%.img.o) $(MAPFILES:%.map=$(BUILD)/%.map.o)
HFILES := $(IMGFILES:%.img=%.h)

all: $(BUILD) $(LIBNAME) $(HFILES)

clean:
	-rm -f $(BUILD)/*.o $(BUILD)/*.a *.h

$(BUILD):
	@[ -d $@ ] || mkdir -p $@

$(LIBNAME) : $(OFILES)
	@echo create $@
	@$(AR) rcs $@ $(OFILES)

%.img.bin : %.img
	cp $< $@

%.pal.bin : %.pal
	cp $< $@

%.map.bin : %.map
	cp $< $@

%.h : %.pal
	$(shell \
	echo "#ifndef $*_h_seen" > $@ ;\
	echo "#define $*_h_seen" >> $@ ;\
	echo "#ifdef __cplusplus" >> $@ ;\
	echo "extern \"C\" {" >> $@ ;\
	echo "#endif" >> $@ ;\
        name=$$(echo _binary_$* | sed -e 's/^\([0-9]\)/_\1/' | tr . _) ;\
	echo "extern const u16" $$name"_pal_bin_end[];" >> $@ ;\
	echo "extern const u16" $$name"_pal_bin_start[];" >> $@ ;\
	echo "extern const u32" $${name}_pal_bin_size[]";" >> $@ ;\
        name=$$(echo _binary_$* | sed -e 's/^\([0-9]\)/_\1/' | tr . _) ;\
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
