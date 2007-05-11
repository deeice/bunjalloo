# create gcc object from raw binary file
$(BUILD)/%.$(RODATASUFF): %.raw
	@$(OBJCOPY) $(OBJCOPYFLAGS) $(notdir $<) $(BUILD)/$(notdir $@)

# create gcc object from palette file
$(BUILD)/%.pal.$(RODATASUFF): %.pal
	@$(OBJCOPY) $(OBJCOPYFLAGS) $(notdir $<) $(BUILD)/$(notdir $@)

# create gcc object from map file
$(BUILD)/%.map.$(RODATASUFF): %.map
	@$(OBJCOPY) $(OBJCOPYFLAGS) $(notdir $<) $(BUILD)/$(notdir $@)

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
