# Simple suckless makefile.

include config.mk

TARGET = combo
SRC = $(TARGET).c
OBJ = ${SRC:.c=.o}

all: options $(TARGET)

options:
	@echo $(TARGET) build options:
	@echo "CFLAGS   = '${CFLAGS}'"
	@echo "LDFLAGS  = '${LDFLAGS}'"
	@echo "CC       = '${CC}'"
	@echo "LD       = '${LD}'"

.c.o:
	@echo CC $<
	${CC} -c ${CFLAGS} $<

${OBJ}: config.mk

$(TARGET): ${OBJ}
	@echo LD $@
	${LD} -g -o $@ ${OBJ} ${LDFLAGS}
#@strip $@

clean:
	@echo Cleaning
	@rm -f $(TARGET) ${OBJ} swarp-${VERSION}.tar.gz

dist: clean
	@echo Creating dist tarball
	@mkdir -p $(TARGET)-${VERSION}
	@cp -R LICENSE Makefile README ../config.mk ${SRC} $(TARGET)-${VERSION}
	@tar -cf $(TARGET)-${VERSION}.tar $(TARGET)-${VERSION}
	@gzip $(TARGET)-${VERSION}.tar
	@rm -rf $(TARGET)-${VERSION}

install: all
	@echo Installing executable file to ${DESTDIR}${PREFIX}/bin
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f $(TARGET) ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/$(TARGET)
	@sed "s/VERSION/$(VERSION)/g" < $(TARGET).1 > $(DESTDIR)$(MANPREFIX)/man1/$(TARGET).1

uninstall:
	@echo Removing executable file from ${DESTDIR}${PREFIX}/bin
	@rm -f ${DESTDIR}${PREFIX}/bin/$(TARGET)

.PHONY: all options clean dist install uninstall
