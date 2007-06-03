TEMPLATE = lib
CONFIG -= qt
CONFIG += dll # build shared library
CONFIG += debug warn_off
TARGET = vecmat
DESTDIR = ../../lib

HEADERS += \
	Matrix3x3.hh \
	Matrix4x4.hh \
	Quaternion.hh \
	Vector.hh \
	Vector2d.hh \
	Vector3d.hh \
	Vector4d.h

SOURCES += \
	Matrix3x3.cc \
	Matrix4x4.cc \
	Vector.cc \
	Vector2d.cc \
	Vector3d.cc \
	Vector4d.cc
