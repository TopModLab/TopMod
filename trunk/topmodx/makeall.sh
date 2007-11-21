#!/bin/bash

# build libraries
cd include && qmake && make

# python stuff for pydlfl library
# only works on OS/X and maybe linux for now 
cd pydlfl
make
python dlfl_setup.py install

# go back up 2 directory levels
cd ../../

# i18n stuff
lupdate topmod.pro
lrelease topmod.pro

# build main project
qmake && make
