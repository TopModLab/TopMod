AUTOMAKE_OPTIONS = gnu
lib_LTLIBRARIES = libvecmat.la

include_HEADERS = \
	Matrix3x3.hh \
	Matrix4x4.hh \
	Quaternion.hh \
	Vector.hh \
	Vector2d.hh \
	Vector3d.hh \
	Vector4d.hh

libvecmat_la_SOURCES = \
	Matrix3x3.cc \
	Matrix4x4.cc \
	Vector.cc \
	Vector2d.cc \
	Vector3d.cc \
	Vector4d.cc

libvecmat_la_LDFLAGS =
	-release $(VERSION)

libvecmat_la_CXXFLAGS = \
	-Wall -Wno-non-virtual-dtor -g -O3 -fPIC
