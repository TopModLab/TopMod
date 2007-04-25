#!/usr/sbin/smake

# Makefile for the Data Structures library

#CC = /usr/local/grad/gnu/bin/g++
CC = CC
C = cc
AR = CC -ar

# Installation
LIBRARY = 
LIBRARY_DIR = $(HOME)/lib/$(OSTYPE)
HEADERS_DIR = $(HOME)/include

# Locations for non-standard libraries and header files. For test programs

LIBDIR = -L$(HOME)/lib/$(OSTYPE)
INCDIR = -I$(HOME)/include
TESTPROGS = arraytest # listtest
LIBS = 

DBGFLAG = -g
CFLAGS = $(OPTFLAG) $(DBGFLAG)

OBJS = 

HEADERS = List.hh Array.hh

default :
	@echo "Makefile for building the Data Structures class library"
	@echo "    make ds : Build the library"
	@echo "    make test : Build test programs"
	@echo "    make install : Install the library and header files"
	@echo "    make clean : Remove object files"
	@echo "    make realclean : Remove object files, executables and libraries"
	@echo "    make uninstall : Remove installed files"

ds : 
	@echo No object code to be compiled.
	@echo Use 'make install' to install header files

install :
	cp $(HEADERS) $(HEADERS_DIR)

test : $(TESTPROGS)

listtest : listtest.o
	$(CC) $(CFLAGS) listtest.o $(LIBDIR) $(LIBS) -o listtest

arraytest : arraytest.o
	$(CC) $(CFLAGS) arraytest.o $(LIBDIR) $(LIBS) -o arraytest

listtest.o : listtest.cc List.hh
	$(CC) $(CFLAGS) -c listtest.cc $(INCDIR)

arraytest.o : arraytest.cc Array.hh
	$(CC) $(CFLAGS) -c arraytest.cc $(INCDIR)

clean :
	-rm -f *.o

realclean :
	-rm -f *.o $(LIBRARY) $(TESTPROGS)

uninstall :
	-rm -f $(LIBRARY_DIR)/$(LIBRARY)
	-cd $(HEADERS_DIR); rm -f $(HEADERS);
