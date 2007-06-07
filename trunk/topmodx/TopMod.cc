#include "TopMod.hh"

TopMod::~TopMod(){
	
}

TopMod::TopMod(int & argc, char ** argv, bool GUIenabled )
	: QApplication(argc,argv,GUIenabled){
		
	setApplicationName("TopMod");

	mainWindow = new MainWindow();

	QString locale = QLocale::system().name();
	QTranslator translator;
	translator.load(QString("topmod_") + locale);
	installTranslator(&translator);

	processEvents( );

	mainWindow->resize( 1000, 800 );
	mainWindow->show();
}

bool TopMod::event(QEvent *event){

	switch (event->type()) {
	case 116:// case QEvent::FileOpen:
		mainWindow->loadFile(static_cast<QFileOpenEvent *>(event)->file());        
		return true;
	default:
		return QApplication::event(event);
	};
}