TEMPLATE = lib
CONFIG -= qt
CONFIG += staticlib #dll # build shared library
CONFIG += release warn_off create_prl
TARGET = dlflcore
INCLUDEPATH += .. ../vecmat
DESTDIR = ../../lib

macx {
 # compile release + universal binary
 #QMAKE_LFLAGS += -F../../lib
 #LIBS += -framework vecmat
 CONFIG += x86
 #CONFIG += lib_bundle
 #QMAKE_BUNDLE_EXTENSION = .framework
#} else:unix {
 QMAKE_LFLAGS += -L../../lib
 #LIBS += -lvecmat
}
else:win32 {

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
        DLFLFileAlt.cc \
	DLFLObject.cc \
	DLFLVertex.cc
