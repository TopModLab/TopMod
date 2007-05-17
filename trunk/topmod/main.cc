/******************************************
 * main.cc
 * topmod 
 * 
 ******************************************/

#include <QApplication>
#include <QSplashScreen>
#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QMainWindow>
#include <QMessageBox>

#include "MainWindow.hh"

#ifdef __APPLE__

/*
#include "TdxDeviceWrappers.hh"

int TdxComputeAxes(const TdxDeviceAxes inArrayPtr)
{
    OSStatus err;
    // CFStringRef txt;
    // HIViewRef transField, rotatField;
	// const HIViewID viewID1 = { '3Dm1', k3DxCarbonDemoTextFieldID1 };
	// const HIViewID viewID2 = { '3Dm1', k3DxCarbonDemoTextFieldID2 };
    char st[128];
    
    //fprintf(stderr, "TdxComputeAxes...\n"); 
    
    // // find the refs of the EditText controls we want to write into 
    // err = HIViewFindByID(HIViewGetRoot(gWindow), viewID1, &transField);
    // assert(err == noErr);
    // err = HIViewFindByID(HIViewGetRoot(gWindow), viewID2, &rotatField);
    // assert(err == noErr);
    
    // write the values into the 1st EditText control 
    // sprintf(st, "(%hd, %hd, %hd)", inArrayPtr[0], inArrayPtr[1], inArrayPtr[2]);
    // txt = CFStringCreateWithCString(NULL, st, kCFStringEncodingUTF8);
    // err = SetControlData(transField,
    //                      kControlEntireControl,
    //                      kControlEditTextCFStringTag,
    //                      sizeof(txt),
    //                      &txt);
    // CFRelease(txt);
    // 
    // // write the values into the 2nd EditText control 
    // sprintf(st, "(%hd, %hd, %hd)", inArrayPtr[3], inArrayPtr[4], inArrayPtr[5]);
    // txt = CFStringCreateWithCString(NULL, st, kCFStringEncodingUTF8);
    // err = SetControlData(rotatField,
    //                      kControlEntireControl,
    //                      kControlEditTextCFStringTag,
    //                      sizeof(txt),
    //                      &txt);
    // CFRelease(txt);
    
    return err;
}

int TdxComputeButtons(UInt16 btnPressed, SInt16 btnState)
{
    OSStatus err;
		// QMessageBox::about(0, "About TopMod","SPACE NAV");

	//     CFStringRef txt;
	//     HIViewRef txtField;
	// const HIViewID viewID3 = { '3Dm1', k3DxCarbonDemoTextFieldID3 };
	//     char st[128];
	//     
	//     fprintf(stderr, "TdxComputeButtons...\n"); 
	//     
	// 
	//      find the ref of the EditText control we want to write into 
	//     err = HIViewFindByID(HIViewGetRoot(gWindow), viewID3, &txtField);
	//     assert(err == noErr);
	// 
	//     // write the values into the EditText control 
	//     sprintf(st, "last:%hu  status:%hd", btnPressed, btnState);
	//     txt = CFStringCreateWithCString(NULL, st, kCFStringEncodingUTF8);
	//     err = SetControlData(txtField,
	//                          kControlEntireControl,
	//                          kControlEditTextCFStringTag,
	//                          sizeof(txt),
	//                          &txt);
	//     assert(err == noErr);
	//     CFRelease(txt);
    
    return err;
}


int TdxComputeEventZero()
{
    static SInt16 zero[6];
    memset(zero, 0, sizeof(zero));
    
  //fprintf(stderr, "TdxComputeEventZero...\n");
    

    // in this case, just process the zero event like any other axes event. 
    return TdxComputeAxes(zero);
}//*/
#endif

int main( int argc, char **argv ) {

  QApplication app( argc, argv, true );

	app.setApplicationName("TopMod");
	
	QString locale = QLocale::system().name();

	QTranslator translator;
	translator.load(QString("topmod_") + locale);
	app.installTranslator(&translator);
		
	//3d connexion stuff ... isn't working right now
	// OSStatus err;
	// err = TdxInitDevice('TopM', TRUE, kConnexionClientModeTakeOver, kConnexionMaskAll);
	//pass kConnexionClientModeTakeOver as the mode and try kConnexionMaskAxis | kConnexionMaskButtons as the mask. kConnexionMaskAll should also work.

	MainWindow mainWindow;

	app.processEvents( );
	mainWindow.resize( 1000, 800 );
	mainWindow.show();

	return app.exec( );
}