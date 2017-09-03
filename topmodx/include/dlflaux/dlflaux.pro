TEMPLATE = lib
CONFIG -= qt
CONFIG += staticlib #dll # build shared library
# CONFIG += debug warn_off create_prl
CONFIG += debug warn_off create_prl
TARGET = dlflaux
INCLUDEPATH += .. ../vecmat ../dlflcore
CONFIG(debug, debug|release) {
	LIB_CONF_SUB_DIR = Debug
} else {
	LIB_CONF_SUB_DIR = Debug
}
DESTDIR = ../../lib/$${LIB_CONF_SUB_DIR}

macx {
 # compile release + universal binary
 #QMAKE_LFLAGS += -F../../lib
 #LIBS += -framework vecmat -framework dlflcore
 #CONFIG += x86 ppc
 #CONFIG += lib_bundle
 #QMAKE_BUNDLE_EXTENSION = .framework
#} else:unix {
 QMAKE_LFLAGS += -L../../lib/$${LIB_CONF_SUB_DIR}
 #LIBS += -lvecmat -ldlflcore
}

HEADERS += \
	DLFLCast.h  \
	DLFLConnect.h  \
	DLFLConvexHull.h  \
	DLFLCrust.h  \
	DLFLDual.h  \
	DLFLExtrude.h  \
	DLFLMeshSmooth.h  \
	DLFLMultiConnect.h  \
	DLFLSculpting \
	DLFLSubdiv.h

SOURCES += \
	DLFLCast.cc  \
	DLFLConnect.cc  \
	DLFLConvexHull.cc  \
	DLFLCrust.cc  \
	DLFLDual.cc  \
	DLFLExtrude.cc  \
	DLFLMeshSmooth.cc  \
	DLFLMultiConnect.cc  \
	DLFLSculpting.cc \
	DLFLSubdiv.cc
