TEMPLATE = lib
CONFIG -= qt
CONFIG += dll # build shared library
CONFIG += debug warn_off
TARGET = dlflcore
INCLUDEPATH += .. ../vecmat
DESTDIR = ../../lib

macx {
 # compile release + universal binary
 LIBS += -L/usr/local/lib -L../../lib -lvecmat
 CONFIG += x86
} else:unix {
 LIBS += -L/usr/local/lib -L../../lib -lvecmat
}

HEADERS += \
	DLFLCommon.hh \
	DLFLCore.hh \
	DLFLCoreExt.hh \
	DLFLEdge.hh \
	DLFLFace.hh \
	DLFLFaceVertex.hh \
	DLFLMaterial.hh \
	DLFLObject.hh \
	DLFLVertex.hh

SOURCES += \
	DLFLCommon.cc \
	DLFLCore.cc \
	DLFLCoreExt.cc \
	DLFLEdge.cc \
	DLFLFace.cc \
	DLFLFaceVertex.cc \
	DLFLFile.cc \
	DLFLObject.cc \
	DLFLVertex.cc
