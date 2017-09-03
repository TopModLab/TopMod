message (Compiler = $$QMAKE_CXX)
# QMAKE_CXX=
TEMPLATE = lib
CONFIG -= qt
CONFIG += staticlib #dll # build shared library
# CONFIG += debug warn_off create_prl
CONFIG += debug warn_off create_prl
TARGET = vecmat
CONFIG(debug, debug|release) {
	LIB_CONF_SUB_DIR = Debug
} else {
	LIB_CONF_SUB_DIR = Debug
}
DESTDIR = ../../lib/$${LIB_CONF_SUB_DIR}

macx {
 # compile release + universal binary
 CONFIG += x86 ppc
 #CONFIG += lib_bundle
 #QMAKE_BUNDLE_EXTENSION = .framework
}

HEADERS += \
	Matrix3x3.h \
	Matrix4x4.h \
	Quaternion.h \
	Vector.h \
	Vector2d.h \
	Vector3d.h \
	Vector4d.h

SOURCES += \
	Matrix3x3.cc \
	Matrix4x4.cc \
	Vector.cc \
	Vector2d.cc \
	Vector3d.cc \
	Vector4d.cc
