# Setup for DLFL Python Module

from distutils.core import setup, Extension

dlfl_module = Extension( 'dlfl', 
			 sources = ['DLFLModule.cc'],
			 include_dirs = ['..','../vecmat','../dlflcore'],
			 library_dirs = ['.','../../lib'],#'/Users/stuart/Documents/courses/research/topmod/topmod/lib'],
			 libraries = ['dlflcore','vecmat'] )

setup( name = 'DLFL',
       version = '1.0',
       description = 'Python Bindings for DLFL Python Module',
       long_description = '',
       author = 'Stuart Tett',
       author_email = 'stuart@tett.net',
       maintainer = 'Texas A&M Vizualization Laboratory',
       url = 'http://www-viz.tamu.edu',
       download_url = 'http://www-viz.tamu.edu',
       ext_modules = [dlfl_module] )#,
       #packages=['TopModPkg'],
       #package_dir={'TopModPkg': 'TopModPkg'},
       #py_modules = ['TopMod'] )

