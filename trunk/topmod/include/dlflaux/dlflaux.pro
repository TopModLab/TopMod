TEMPLATE = lib
CONFIG -= qt
CONFIG += dll # build shared library
CONFIG += debug warn_off
TARGET = dlflaux
INCLUDEPATH += .. ../vecmat ../dlflcore
DESTDIR = ../../lib

macx {
 # compile release + universal binary
 LIBS += -L/usr/local/lib -L../../lib -lvecmat -ldlflcore
 CONFIG += x86
} else:unix {
 LIBS += -L/usr/local/lib -L../../lib -lvecmat -ldlflcore
}

HEADERS += \
	DLFLCast.hh  \
	DLFLConnect.hh  \
	DLFLConvexHull.hh  \
	DLFLCrust.hh  \
	DLFLDual.hh  \
	DLFLExtrude.hh  \
	DLFLMeshSmooth.hh  \
	DLFLMultiConnect.hh  \
	DLFLSubdiv.hh

SOURCES += \
	DLFLCast.cc  \
	DLFLConnect.cc  \
	DLFLConvexHull.cc  \
	DLFLCrust.cc  \
	DLFLDual.cc  \
	DLFLExtrude.cc  \
	DLFLMeshSmooth.cc  \
	DLFLMultiConnect.cc  \
	DLFLSubdiv.cc
