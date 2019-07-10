# Makefile for one-bin C programs.
include config.mk
INFO = options artifacts paths

.PHONY: all clean install uninstall info
all : options strip

dbg : dbgflags info $(TARGET)
	$(LD) -o $(TARGET) $(LDFLAGS) $(OBJ)

dbgflags :
	$(eval CFLAGS = $(DBGFLAGS))

options :
	@echo "'$(TARGET)' '$@':"
	@echo "CFLAGS   = '$(CFLAGS)' ;"
	@echo "LDFLAGS  = '$(LDFLAGS)' ;"
	@echo "CC       = '$(CC)' ;"
	@echo "LD       = '$(LD)' ;"

artifacts :
	@echo "'$(TARGET)' '$@':"
	@echo "SRC      = '$(SRC)' ;"
	@echo "HDR      = '$(HDR)' ;"
	@echo "OBJ      = '$(OBJ)' ;"
	@echo "TARDIR   = '$(TARDIR);'"
	@echo "TAR      = '$(TAR)' "

paths :
	@echo "'$(TARGET)' '$@':"
	@echo "ROOTDIR  = '$(ROOTDIR)';"
	@echo "DIRPREFIX= '$(DIRPREFIX)';"
	@echo "MANPREFIX= '$(MANPREFIX)'"
	@echo "DESTDIR  = '$(DESTDIR)';"

info : $(INFO)


$(TARGET) : $(OBJ)
	@echo "[$@]"
	$(LD) -o $@ $(OBJ) $(CFLAGS)

strip : $(TARGET)
	strip -v $<

.c.o : $(SRC)
	@echo "[$@]"
	$(CC) -o $@ $(CFLAGS) -c $<

$(SRC) : $(HDR)

clean: artifacts distclean resclean

distclean :
	rm -vrf $(TAR) $(TARDIR) $(TAR).gz

resclean :
	rm -vrf $(OBJ) $(TARGET)

dist : $(TAR).gz

$(TAR).gz : $(TAR)
	gzip $(TAR)

$(TAR) : $(TARDIR)
	tar -vcf $(TAR) $(TARDIR)

$(TARDIR) : resclean
	mkdir -pv $(TARDIR)
	cp -vR $(wildcard *) $(TARDIR)

install : paths $(TARGET)
	install $(TARGET) $(DESTDIR)/$(BINPREFIX)/
	sed "s/VERSION/$(VERSION)/g" <$(TARGET).1>$(DESTDIR)/$(MANPREFIX)/$(MAN1PREFIX)/$(TARGET).1

uninstall :
	rm -vrf $(DESTDIR)/$(BINPREFIX)/$(TARGET)
	rm -vrf $(DESTDIR)/$(MANPREFIX)/$(MAN1PREFIX)/$(TARGET).1
