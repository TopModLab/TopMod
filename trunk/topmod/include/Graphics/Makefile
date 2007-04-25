#!gmake

# Makefile for the GL++ library
# NOTE: There are several features available only in GNU Make
# which are used in this makefile

# This makefile uses default built-in rules for compiling .cc files (.cc.o)
# The relevant flags are passed by defining the variables CXXFLAGS, LDFLAGS, etc.

ARFLAGS := crusv

# Installation locations
LIBRARY = 
SUBDIR := $(shell uname -r)
LIBRARY_DIR = $(HOME)/lib/$(OSTYPE)/$(SUBDIR)
HEADERS_DIR = $(HOME)/include

# Include/library directories
# These are slightly different for Linux and SGI
# The OSTYPE environment variable is used to detect which OS we are on
# For SGIs also redefine the CXX to be CC instead of the default g++
# For SGIs include flag to enable ANSI scoping rules for for-init declarations

ifeq ($(OSTYPE),linux)
  INCDIR := -I/usr/X11R6/include
  LIBDIR := -L/usr/X11R6/lib
  FLTKLIBS := -lfltk_gl -lfltk
else
  CXX := CC
  CFLAGS := -LANG:ansi-for-init-scope=ON
  INCDIR := -I$(HOME)/fltk
  LIBDIR := -L$(HOME)/fltk/lib
  FLTKLIBS := -lfltk
endif

INCDIR := $(INCDIR) -I. -I$(HOME)/include
WARNS := # -Wno-deprecated
OPTFLAG := #-O2
DBGFLAG := -g
CFLAGS := $(CFLAGS) $(WARNS) $(OPTFLAG) $(DBGFLAG) $(INCDIR)
CXXFLAGS := $(CFLAGS)

LIBDIR := $(LIBDIR) -L$(LIBRARY_DIR)
LIBS := $(FLTKLIBS) -larcball -lGL -lGLU -lXext -lX11 -lvecmat -lm
LDFLAGS := $(LIBDIR) $(LIBS)

OBJS = 
HEADERS = Transform.hh Color.hh Grid.hh Camera.hh Texture.hh

TESTPROGS = 

default :
	@echo "Makefile for building the GL++ class library"
	@echo "    make glpp : Build the GL++ class library"
	@echo "    make test : Build test program for GL++ library"
	@echo "    make install : Install the library and header files"
	@echo "    make clean : Remove object files"
	@echo "    make realclean : Remove object files, executables and libraries"
	@echo "    make uninstall : Remove installed files"

glpp :
	@echo "Nothing to build. Use 'make install' to install header files"

install :
	cp $(HEADERS) $(HEADERS_DIR)

test : 
	@echo "No separate test programs"

clean :
	-rm -f *.o

realclean :
	-rm -f *.o $(LIBRARY) $(TESTPROG)

uninstall :
	-cd $(HEADERS_DIR); rm -f $(HEADERS)
