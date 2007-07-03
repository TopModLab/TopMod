
QT += opengl xml
CONFIG += qt debug warn_off assistant link_prl

# exclude verse python or spacenav drivers
# or include them with CONFIG += 
CONFIG -=  WITH_PYTHON WITH_SPACENAV WITH_VERSE
CONFIG += WITH_PYTHON 
DEFINES += TOPMOD_VERSION GPU_OK

TEMPLATE = app

SUBDIRS = include

# TopModd will be the name for the debug version, 
# and TopMod will be the release version
CONFIG(debug, debug|release) {
 TARGET = TopMod
} else {
 TARGET = TopMod
}

DEPENDPATH += \
	   lang \
	   include \
	   include/Graphics \ 
	   include/Light \
	   include/vecmat \
	   include/arcball \
	   include/dlflcore \
	   include/dlflaux \
	   include/verse \

INCLUDEPATH += \
	    include \	      
	    include/Graphics \ 
	    include/Light \
	    include/vecmat \
	    include/arcball \
	    include/dlflcore \
	    include/dlflaux \
	    include/verse

CONFIG(WITH_VERSE){
	message("VERSE will be included")
	DEFINES *= WITH_VERSE
}

CONFIG(WITH_PYTHON){
	message("PYTHON support will be included")
	DEFINES *= WITH_PYTHON
}

CONFIG(WITH_SPACENAV){
	message("SPACENAV support will be included")
	DEFINES *= WITH_SPACENAV
}

macx {
	# either compile the mac version as an app bundle or a console app
	# tell it to load a custom info.plist file here
	QMAKE_INFO_PLIST    = Info.plist
	MACOSX_DEPLOYMENT_TARGET = 10.2
	# compile release + universal binary 
	CONFIG += x86

	# for cg gpu shading
	INCLUDEPATH += /Library/Frameworks/Cg.framework/Versions/1.0 
	QMAKE_LFLAGS += -L/Library/Frameworks/Cg.framework 
	LIBS += -framework Cg

	#QMAKE_LFLAGS += -F./lib	
	#LIBS += -framework vecmat -framework arcball -framework dlflcore -framework dlflaux
	QMAKE_LFLAGS += -L./lib
	LIBS += -lvecmat -larcball -ldlflcore -ldlflaux -framework CoreFoundation

	#PRIVATE_FRAMEWORKS.files = ./lib/vecmat.framework ./lib/arcball.framework ./lib/dlflcore.framework ./lib/dlflaux.framework
	#PRIVATE_FRAMEWORKS.path = Contents/Frameworks
	#QMAKE_BUNDLE_DATA += PRIVATE_FRAMEWORKS

	INCLUDEPATH += /usr/include
	QMAKE_LFLAGS += -L/usr/lib

	CONFIG(WITH_PYTHON){
		INCLUDEPATH += /Library/Frameworks/Python.framework/Versions/2.5/include/python2.5 
		QMAKE_LFLAGS += -L/Library/Frameworks/Python.framework 
		LIBS += -framework Python
	}
	CONFIG(WITH_SPACENAV){
		INCLUDEPATH += /Library/Frameworks/3DconnexionClient.framework/Versions/A/Headers
		LIBS += -framework 3DconnexionClient
		QMAKE_LFLAGS += -L/Library/Frameworks/3DconnexionClient.framework 
	}
	CONFIG(WITH_VERSE){
		LIBS += -lverse
	}
} else:unix {
	CONFIG -= WITH_SPACENAV WITH_VERSE
	QMAKE_LFLAGS += -L./lib
	LIBS += -lvecmat -larcball -ldlflcore -ldlflaux

	CONFIG(WITH_PYTHON){
		INCLUDEPATH += /usr/include/python2.5
		LIBS += -lpython2.5 -L/usr/lib/python2.5/config
	}
	CONFIG(WITH_SPACENAV){

	}
	CONFIG(WITH_VERSE){

	}
} else:win32 {
	CONFIG -= WITH_SPACENAV
	LIBS += -L.\lib -lvecmat -larcball -ldlflcore -ldlflaux

	CONFIG(WITH_PYTHON){
	 INCLUDEPATH += C:\Python25\include
	 QMAKE_LFLAGS += -LC:\Python25\libs
	 LIBS += -lpython25
	}
	CONFIG(WITH_SPACENAV){

	}
	CONFIG(WITH_VERSE){
		LIBS += -lverse
		INCLUDEPATH += C:\verse\include
		QMAKE_LFLAGS += -LC:\verse\lib
	}
}

# Input
HEADERS += \
	DLFLScriptEditor.hh \
	TopModPreferences.hh \
	TdxDeviceWrappers.hh \
	CommandCompleter.hh \
	DLFLLocator.hh \
	GLWidget.hh \
	TopMod.hh \
	MainWindow.hh \
	GeometryRenderer.hh \
	DLFLLighting.hh \
	qcumber.hh \
	qshortcutdialog.hh \
	qshortcutmanager.hh \
	ui_shortcutdialog.h \
	ui_stylesheeteditor.h \
	editor.hh \
	PythonHighlighter.hh \
	BasicsMode.hh \
	ExtrusionMode.hh \ 
	RemeshingMode.hh \
	ConicalMode.hh \ 
	HighgenusMode.hh \
	TexturingMode.hh \
	DLFLSelection.hh \
	Viewport.hh \
	TMPatchFace.hh \
	TMPatchObject.hh \
	TMPatch.hh \
	stylesheeteditor.hh \
	include/Base/BaseObject.hh \
	include/Base/Constants.hh \
	include/Base/Inlines.hh \
	include/Base/StreamIO.hh \
	include/Graphics/Camera.hh \
	include/Graphics/Color.hh \
	include/Graphics/Grid.hh \
	include/Graphics/Texture.hh \
	include/Graphics/Transform.hh \
	include/Light/AmbientLight.hh \
	include/Light/Light.hh \
	include/Light/PointLight.hh \
	include/Light/SpotLight.hh \
	CgData.hh \
	include/Camera2.hh \
	include/Camera3.hh

FORMS += shortcutdialog.ui stylesheeteditor.ui

SOURCES += \
	DLFLScriptEditor.cc \
	TopModPreferences.cc \
	TdxDeviceWrappers.cc \
	GLWidget.cc \
	TopMod.cc \
	main.cc \
	MainWindow.cc \
	MainWindowCallbacks.cc \
	MainWindowRemeshingCallbacks.cc \
	GeometryRenderer.cc \
	qshortcutdialog.cc \
	qshortcutmanager.cc \
	editor.cc \
	PythonHighlighter.cc \
	BasicsMode.cc	\
	ExtrusionMode.cc \
	RemeshingMode.cc \ 
	ConicalMode.cc \ 
	HighgenusMode.cc \
	TexturingMode.cc \
	DLFLLighting.cc \
	DLFLRenderer.cc \
	DLFLSelection.cc \
	DLFLSculpting.cc \
	DLFLUndo.cc \
	DLFLLocator.cc \
	TMPatchObject.cc \
	TMPatchFace.cc \
	stylesheeteditor.cc \
	CgData.cc \
	include/Camera2.cc \
	include/Camera3.cc

RESOURCES += application.qrc

TRANSLATIONS += \
	     lang/topmod_ca.ts \
	     lang/topmod_de.ts \
	     lang/topmod_en.ts \
	     lang/topmod_en_us.ts \
	     lang/topmod_es.ts \
	     lang/topmod_fr.ts \
	     lang/topmod_tr.ts

CONFIG(WITH_VERSE){

HEADERS += \
	include/verse/TKE_verse.h \
	include/verse/TIF_verse.h \
	include/verse/MEM_guardedalloc.h \
	include/verse/DNA_mesh_types.h \
	include/verse/DNA_meshdata_types.h \
	include/verse/DNA_scriptlink_types.h \
	include/verse/DNA_object_types.h \
	include/verse/DNA_ID.h \
	include/verse/DNA_listBase.h \
	include/verse/mydevice.h \
	include/verse/TLI_dynamiclist.h \
	include/verse/TLI_util.h \
	include/verse/TLI_toplib.h \
	include/verse/TLI_arithb.h \
	include/verse/TKE_utildefines.h \
	include/verse/TIF_space.h \
	include/verse/TIF_editmesh.h \
	include/verse/TKE_depsgraph.h \
	include/verse/TKE_global.h \
	include/verse/TKE_mesh.h \
	include/verse/TLI_editVert.h \
	include/verse/TKE_object.h \
	VerseTopMod.hh 
SOURCES += \
	include/verse/verse_session.cc \
	include/verse/mallocn.cc \
	include/verse/verse_common.cc \
	include/verse/util.cc \
	include/verse/verse_geometry_node.cc \
	include/verse/verse_mesh.cc \
	include/verse/verse_node.cc \
	include/verse/verse_object.cc \
	include/verse/verse_object_node.cc \
	VerseTopMod.cc
}
