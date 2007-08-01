#include "CommandCompleter.hh"
#include <QPixmap>
#include <QBitmap>

	/*!
	* \brief Constructor
	* 
	* @param m the actionList passed from the MainWindow class after each Mode class is instantiated
	* @param parent the MainWindow parent widget 
	* @param f window flags for the QDialog constructor
	* 
	*/
	CommandCompleter::CommandCompleter ( QWidget *m, QWidget * parent, Qt::WindowFlags f) : QDialog(parent, f) {
		setSizeGripEnabled(false);
		setWindowFlags(Qt::SplashScreen);
		setModal(true);
		//setWindowOpacity(1.0);
		this->resize(341,134);
		
		//this->setWindowFlags(Qt::FramelessWindowHint);
		//this->setAttribute(Qt::WA_NoSystemBackground);
		
		// QStyle* plastiqueStyle = new QPlastiqueStyle;
		mQuickCommandLabel = new QLabel(tr("<font color=\"white\">Type a command:</font>"));
		
		//this->setAutoFillBackground(true);
		// this->setStyle(plastiqueStyle);
		QPalette p = this->palette();
		//QBrush b(QImage(QString(":/images/commandcompleter.png")));
		//p.setColor(QPalette::Window, QColor(0,0,0,0));
		p.setBrush(QPalette::Window, QPixmap(QString(":images/commandcompleter.png")));
		//this->setBackgroundMode( Qt::NoBackground );
		this->setPalette(p);	
		
		QPixmap pixmap(":/images/commandcompleter.png");
		//this->setPixmap(pixmap);
		this->setMask(pixmap.mask());
	 
		mLineEdit = new QLineEdit(this);
		mCompleter = new QCompleter(this);

		mCompleter->setCaseSensitivity(Qt::CaseInsensitive);
		mCompleter->setCompletionMode(QCompleter::PopupCompletion);
		//loop through the actions added to the widget *m, add the text to a word list for now
		for (int i = 0; i < m->actions().count(); ++i)
			mWordList << ((QAction*)(m->actions().at(i)))->toolTip();

		mModel = new QStringListModel(mWordList,mCompleter);
		mCompleter->setModel(mModel);
		mLineEdit->setCompleter(mCompleter);
		
		QVBoxLayout *vbox = new QVBoxLayout(this);
		vbox->setContentsMargins(60,40,60,50);
		vbox->addWidget(mQuickCommandLabel);
		vbox->addWidget(mLineEdit);

		// QShortcut *shortcut = new QShortcut(QKeySequence(tr("Enter")),this);
		connect(mLineEdit, SIGNAL(editingFinished()), this, SLOT(accept()));
	}
