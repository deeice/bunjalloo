.PHONY: $(BUILD) all clean

GRITFLAGS := -pw 16 -gB 8 -m -ft b
PALFILES := $(wildcard *.pal)
MAPFILES := $(wildcard *.map)
IMGFILES := $(wildcard *.img)
LIBNAME:=$(BUILD)/libfont.a
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

%.h : %.pal
	$(shell \
	echo "#ifndef $*_h_seen" > $@ ;\
	echo "#define $*_h_seen" >> $@ ;\
	echo "#ifdef __cplusplus" >> $@ ;\
	echo "extern \"C\" {" >> $@ ;\
	echo "#endif" >> $@ ;\
        name=$$(echo _binary_$* | sed -e 's/^\([0-9]\)/_\1/' | tr . _) ;\
	echo "extern const u16" $$name"_pal_end[];" >> $@ ;\
	echo "extern const u16" $$name"_pal_start[];" >> $@ ;\
	echo "extern const u32" $${name}_pal_size[]";" >> $@ ;\
        name=$$(echo _binary_$* | sed -e 's/^\([0-9]\)/_\1/' | tr . _) ;\
	echo "extern const u8" $$name"_img_end[];" >> $@ ;\
	echo "extern const u8" $$name"_img_start[];" >> $@ ;\
	echo "extern const u32" $$name"_img_size[];" >> $@ ;\
	echo "extern const u8" $$name"_map_end[];" >> $@ ;\
	echo "extern const u8" $$name"_map_start[];" >> $@ ;\
	echo "extern const u32" $$name"_map_size[];" >> $@ ;\
	echo "#ifdef __cplusplus" >> $@ ;\
	echo "};" >> $@ ;\
	echo "#endif" >> $@ ;\
	echo "#endif" >> $@ ;\
	)

$(BUILD)/%.pal.o: %.pal
	@$(OBJCOPY) $(OBJCOPYFLAGS) $< $@
$(BUILD)/%.map.o: %.map
	@$(OBJCOPY) $(OBJCOPYFLAGS) $< $@
$(BUILD)/%.img.o: %.img
	@$(OBJCOPY) $(OBJCOPYFLAGS) $< $@
