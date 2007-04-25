AUTOMAKE_OPTIONS = gnu
lib_LTLIBRARIES = libarcball.la

DEFAULT_INCLUDES = -I../vecmat
 
include_HEADERS = \
	Arcball.hh \
	BallMath.hh \
	DollyControl.hh \
	TransControl.hh \
	ZoomControl.hh 

libarcball_la_SOURCES = \
	Arcball.cc \
	BallMath.cc

libarcball_la_LDFLAGS = \
	-release $(VERSION)

libarcball_la_CXXFLAGS = \
	-Wall -Wno-non-virtual-dtor -g -O3 -fPIC
