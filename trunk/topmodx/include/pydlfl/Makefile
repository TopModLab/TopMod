# Makefile

OS =  $(shell uname)

#ifeq ($(OS),Linux)
# setup for linux
#endif
#ifeq ($(OS),Darwin)
# setup for Mac
#endif
# Windows?

dlfl:
	@ echo Building DLFL Python Module...
	@ python dlfl_setup.py build

install:
	@ echo Installing DLFL Python Module...
	@ python dlfl_setup.py install

bundle: dlfl
	@ echo Installing PyDLFL into Mac Bundle...
	@ cp `find build -iname dlfl.so -print` ../../TopMod.app/

locinstall: dlfl
	@ echo Installing PyDLFL into TopMod directory
	@ cp `find build -iname dlfl.so -print` ../../

release: dlfl
	@ echo Installing in Windows Release directory
	@ cp \build\lib.win32-2.5\dlfl.pyd ../../release/

