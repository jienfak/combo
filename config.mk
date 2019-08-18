# Includable for c1-like makefiles.
PROGNAME= combo
VERSION = 0.1
# Artifacts.
TGT     = $(PROGNAME)$(EXEEXT)
SRC     = $(wildcard *.c) $(ADDSRC)
HDR     = $(wildcard *.h) $(ADDHDR)
ADDSRC  =
ADDHDR  =
OBJ     = $(SRC:.c=.o)
# Tarball.
TARDIR  = $(TGT)-$(VERSION)
TARARC  = $(TARDIR)$(TAREXT)
TARGZARC  = $(TARARC)$(GZEXT)

# Manual.
MANSTDSECT = 1
MANUAL  = $(TGT).$(MANSECT)
# Directories.
# Includes.
INCFLAGS =
# Dynamic/static libraries.
LIBFLAGS =
# CPPreprocessor.
CPPFLAGS = -DVERSION=\"$(VERSION)\"
# Warnings.
WRNFLAGS =
# Optimiziation flags.
OPTFLAGS = -O3
# Compilation Flags.
DBGFLAGS = -g $(INCFLAGS) -O0         -Wall       $(DEFFLAGS) -DDBG
CFLAGS   =    $(INCFLAGS) $(OPTFLAGS) $(WRNFLAGS) $(DEFFLAGS)
LDFLAGS  = $(LIBFLAGS)

# Compiler and linker.
CC = tcc
LD = $(CC)
