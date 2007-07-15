TEMPLATE = lib
CONFIG -= qt
CONFIG += opengl
CONFIG += static #dll # build shared library
CONFIG += debug warn_off create_prl
TARGET = arcball
DESTDIR = ../../lib
INCLUDEPATH += ../vecmat

macx {
 # compile release + universal binary
 #QMAKE_LFLAGS += -F../../lib
 #LIBS += -framework vecmat
 CONFIG += x86 ppc
 #CONFIG += lib_bundle
 QMAKE_BUNDLE_EXTENSION = .framework
#} else:unix {
 QMAKE_LFLAGS += -L/usr/local/lib -L../../lib
 LIBS += -lvecmat
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
