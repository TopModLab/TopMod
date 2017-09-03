TEMPLATE = lib
CONFIG -= qt
CONFIG += opengl
CONFIG += static #dll # build shared library
CONFIG += debug warn_off create_prl
TARGET = arcball

CONFIG(debug, debug|release) {
	LIB_CONF_SUB_DIR = Debug
} else {
	LIB_CONF_SUB_DIR = Debug
}
DESTDIR = ../../lib/$${LIB_CONF_SUB_DIR}

INCLUDEPATH += ../vecmat

macx {
 # compile release + universal binary
 #QMAKE_LFLAGS += -F../../lib
 #LIBS += -framework vecmat
 CONFIG += x86 ppc
 #CONFIG += lib_bundle
 QMAKE_BUNDLE_EXTENSION = .framework
#} else:unix {
 QMAKE_LFLAGS += -L/usr/local/lib -L../../lib/$${LIB_CONF_SUB_DIR}
 LIBS += -lvecmat
}

HEADERS += \
	Arcball.h \
	BallMath.h \
	DollyControl.h \
	TransControl.h \
	ZoomControl.h 

SOURCES += \
	Arcball.cc \
	BallMath.cc
