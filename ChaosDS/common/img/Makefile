
SUBDIRS:= $(shell ls palette* -d | egrep -v '^(CVS)$$')

PCX_FILES:=$(shell ls palette*/*.pcx)

all: images.h
	@for i in $(SUBDIRS); do if test -e $$i/Makefile ; then $(MAKE) --no-print-dir -C $$i all ; fi; done;

clean:
	@for i in $(SUBDIRS); do if test -e $$i/Makefile ; then $(MAKE) --no-print-dir -C $$i clean ; fi; done;
	@rm -f images.h


images.h: $(PCX_FILES)
	$(shell \
	echo "#ifndef images_h_seen" > $@ ;\
	echo "#define images_h_seen" >> $@ ;\
	echo "#ifdef __cplusplus" >> $@ ;\
	echo "extern \"C\" {" >> $@ ;\
	echo "#endif" >> $@ ;\
        for i in $(SUBDIRS) ;\
        do \
        name=$$(echo _binary_$$(basename $$i) | sed -e 's/^\([0-9]\)/_\1/' | tr . _) ;\
	echo "extern const u16" $$name"_pal_end[];" >> $@ ;\
	echo "extern const u16" $$name"_pal_start[];" >> $@ ;\
	echo "extern const u32" $${name}_pal_size[]";" >> $@ ;\
        done;\
	for i in $^ ;\
	do \
        name=$$(echo _binary_$$(basename $$i) |sed -r 's/pcx//g' | sed -e 's/^\([0-9]\)/_\1/' | tr . _) ;\
	echo "extern const u16" $$name"raw_end[];" >> $@ ;\
	echo "extern const u16" $$name"raw_start[];" >> $@ ;\
	echo "extern const u32" $${name}raw_size[]";" >> $@ ;\
	echo "extern const u16" $$name"map_end[];" >> $@ ;\
	echo "extern const u16" $$name"map_start[];" >> $@ ;\
	echo "extern const u32" $${name}map_size[]";" >> $@ ;\
	done ;\
	echo "#ifdef __cplusplus" >> $@ ;\
	echo "};" >> $@ ;\
	echo "#endif" >> $@ ;\
	echo "#endif" >> $@ ;\
	)
