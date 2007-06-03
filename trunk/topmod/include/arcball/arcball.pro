TEMPLATE = lib
CONFIG -= qt
CONFIG += opengl
CONFIG += dll # build shared library
CONFIG += debug warn_off
TARGET = arcball
DESTDIR = ../../lib
INCLUDEPATH += ../vecmat

macx {
 # compile release + universal binary
 LIBS += -L/usr/local/lib -L../../lib -lvecmat
 CONFIG += x86
} else:unix {
 LIBS += -L/usr/local/lib -L../../lib -lvecmat
}

HEADERS += \
	Arcball.hh \
	BallMath.hh \
	DollyControl.hh \
	TransControl.hh \
	ZoomControl.hh 

SOURCES += \
	Arcball.cc \
	BallMath.cc
