# Includable for c1-like makefiles.
VERSION = 0.1
# Resources.
TARGET  = combo
SRC     = $(wildcard *.c) $(ASRC)
HDR     = $(wildcard *.h) $(AHDR)
OBJ     = $(SRC:.c=.o)
TARDIR  = $(TARGET)-$(VERSION)
TAR     = $(TARDIR).tar
# Directories.
ROOTDIR   = 
DIRPREFIX = usr/local
MANPREFIX = share/man
MAN1PREFIX= man1
BINPREFIX = bin
DESTDIR = $(ROOTDIR)/$(DIRPREFIX)
# Program name.
# Additional sources.
ASRC =
# Additional headers.
AHDR =
# Includes.
INC =
# Dynamic/static libraries.
LIB =
# Defines to get from C-preprocessor.
DEFINE   = -DVERSION=\"$(VERSION)\"
# Warnings.
WARN     =
# Optimiziation flags.
OPTF     = -O3
# Compilation Flags.
DBGFLAGS = $(INC) -O0     -Wall   $(DEFINE) -g
CFLAGS   = $(INC) $(OPTF) $(WARN) $(DEFINE)
LDFLAGS  = $(LIBS)

# Compiler and linker.
CC = tcc
LD = $(CC)
