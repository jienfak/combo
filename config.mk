# Includable for c1-like makefiles.
VERSION = 0.1
# Artifacts.
TGT     = combo
SRC     = $(wildcard *.c) $(ADDSRC)
HDR     = $(wildcard *.h) $(ADDHDR)
ADDSRC  =
ADDHDR  =
OBJ     = $(SRC:.c=.o)
# Tarball.
TARDIR  = $(TGT)-$(VERSION)
TARARC  = $(TARDIR).tar
TAREXT  = gz
TAROUT  = $(TARARC).$(TAREXT)

# Manual.
MANSECT = 1
MANUAL  = $(TGT).$(MANSECT)
# Directories.
ROOTDIR       = 
DIRPREFIX     = usr/local
MANPREFIX     = share/man
MANSECTPREFIX = man$(MANSECT)
BINPREFIX     = bin
DESTDIR       = $(ROOTDIR)/$(DIRPREFIX)
# Includes.
INCFLAGS =
# Dynamic/static libraries.
LIBFLAGS =
# Defines to get from C-preprocessor.
DEFFLAGS = -DVERSION=\"$(VERSION)\"
# Warnings.
WRNFLAGS =
# Optimiziation flags.
OPTFLAGS = -O3
# Compilation Flags.
DBGFLAGS = -g $(INCFLAGS) -O0         -Wall       $(DEFFLAGS)
CFLAGS   =    $(INCFLAGS) $(OPTFLAGS) $(WRNFLAGS) $(DEFFLAGS)
LDFLAGS  = $(LIBFLAGS)

# Compiler and linker.
CC = tcc
LD = $(CC)
