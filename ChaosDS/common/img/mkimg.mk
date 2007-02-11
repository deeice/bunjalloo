# create gcc object from raw binary file
%.$(RODATASUFF): %.raw
	@$(OBJCOPY) $(OBJCOPYFLAGS) $(notdir $<) $(notdir $@)

# create gcc object from palette file
%.pal.$(RODATASUFF): %.pal
	@$(OBJCOPY) $(OBJCOPYFLAGS) $(notdir $<) $(notdir $@)

# create gcc object from map file
%.map.$(RODATASUFF): %.map
	@$(OBJCOPY) $(OBJCOPYFLAGS) $(notdir $<) $(notdir $@)

# create raw binary file from pcx
$(PALETTENAME) : $(CURRENTIMGS)
	$(GFX2GBA) $(GFX2GBAFLAGS) -G -D -p$(PALETTENAME)  $(CURRENTIMGS) >/dev/null

%.map : %.pcx
	$(GFX2GBA) $(GFX2GBAFLAGS) -G -P  $(CURRENTIMGS) >/dev/null

%.raw  : %.pcx
	$(GFX2GBA) $(GFX2GBAFLAGS) -P -D  $(CURRENTIMGS) >/dev/null

$(LIBNAME) : $(CURRENTOFILES)
	@echo create $@
	@$(AR) rcs $@ $(CURRENTOFILES)
