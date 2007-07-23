rem run in the cmd prompt or powershell by typing .\makeall.bat
@echo on
cd include
qmake
mingw32-make
cd ..
qmake
mingw32-make