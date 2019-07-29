# Makefile for one-bin C programs.
.PHONY: all dbg clean install uninstall show_info  \
            show_options show_artifacts show_paths
include config.mk
SHOW_INFO = show_options show_artifacts show_paths
CLEAN     = clean_dist clean_artifacts


.SILENT: install uninstall clean $(CLEAN)

all : show_options strip

dbg : set_dbgflags show_info $(CLEAN) $(TGT)
	$(LD) -o $(TGT) $(LDFLAGS) $(OBJ)

set_dbgflags :
	$(eval CFLAGS = $(DBGFLAGS))

show_options :
	@echo "'$(TGT)' '$@':"
	@echo "CFLAGS   = '$(CFLAGS)' ;"
	@echo "LDFLAGS  = '$(LDFLAGS)' ;"
	@echo "CC       = '$(CC)' ;"
	@echo "LD       = '$(LD)' ;"

show_artifacts :
	@echo "'$(TGT)' '$@' :"
	@echo "SRC      = '$(SRC)' ;"
	@echo "HDR      = '$(HDR)' ;"
	@echo "OBJ      = '$(OBJ)' ;"
	@echo "TARDIR   = '$(TARDIR);'"
	@echo "TARARC   = '$(TARARC)'. "

show_paths :
	@echo "'$(TGT)' '$@' :"
	@echo "ROOTDIR  = '$(ROOTDIR)' ;"
	@echo "DIRPREFIX= '$(DIRPREFIX)' ;"
	@echo "MANPREFIX= '$(MANPREFIX)' ;"
	@echo "DESTDIR  = '$(DESTDIR)' ."

show_info : $(SHOW_INFO)


$(TGT) : $(OBJ)
	@echo "[$@]"
	$(LD) -o $@ $(OBJ) $(CFLAGS)

$(MANUAL) :

strip : $(TGT)
	strip -v $<

%.o : %.c
	@echo "[$@]"
	$(CC) -o $@ $(CFLAGS) -c $<

$(SRC) : $(HDR)

clean: $(CLEAN)

clean_dist :
	rm -vrf $(TARDIR) $(TARARC) $(TAROUT)

clean_artifacts :
	rm -vrf $(OBJ) $(TGT)

dist :  $(TAROUT)

$(TAROUT) : clean $(TARARC)
	gzip -f $(TARARC)

$(TARARC) : $(TARDIR)
	tar -vcf $(TARARC) $(TARDIR)

$(TARDIR) : $(DISTFILES)
	mkdir -pv $(TARDIR)
	cp -vR $(subst $(TARDIR),,$(wildcard *)) $(TARDIR)

install : $(TGT) $(MANUAL)
	install -v $(TGT) $(DESTDIR)/$(BINPREFIX)/
	sed "s/VERSION/$(VERSION)/g" <$(MANUAL)>$(DESTDIR)/$(MANPREFIX)/$(MANSECTPREFIX)/$(MANUAL)

uninstall :
	rm -vrf $(DESTDIR)/$(BINPREFIX)/$(TGT)
	rm -vrf $(DESTDIR)/$(MANPREFIX)/$(MANSECTPREFIX)/$(MANUAL)
