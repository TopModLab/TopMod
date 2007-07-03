#include "PythonHighlighter.hh"

PythonHighlighter::PythonHighlighter(QTextEdit *parent) 
  : QSyntaxHighlighter(parent) {
  pythonKeywords << "and" << "assert" << "break" << "class" << "continue" << "def"
								 << "del" << "elif" << "else" << "except" << "exec" << "finally"
								 << "for" << "from" << "global" << "if" << "import" << "in"
								 << "is" << "lambda" << "not" << "or" << "pass" << "print" << "raise"
								 << "return" << "try" << "while" << "yield" << "True" << "False";

  //dlflKeywords << "insert_edge" << "delete_edge" << "collapse_edge" << "subdivide_edge";

  promptFormat.setForeground(QColor("greenyellow"));
  promptFormat.setFontWeight(QFont::Bold);
  promptFormat.setFontItalic(true);

  functionFormat.setForeground(QColor("royalblue"));
  functionFormat.setFontWeight(QFont::Bold);

  tupleFormat.setForeground(QColor("lightskyblue"));
  tupleFormat.setFontWeight(QFont::Bold);
  //tupleFormat.setFontItalic(true);

  pythonFormat.setForeground(QColor("coral"));
  pythonFormat.setFontWeight(QFont::Bold);

  stringFormat.setForeground(QColor("crimson"));
  commentFormat.setForeground(QColor("orangered"));
  commentFormat.setFontItalic(true);
  compilePattern( );
}

PythonHighlighter::~PythonHighlighter( ) {  
	
}

void PythonHighlighter::compilePattern() {
  pythonPatterns.clear();
  QString delim("\\b");
  QStringList::iterator lit = pythonKeywords.begin();
  for (; lit != pythonKeywords.end(); lit++) {
    pythonPatterns.push_back(QRegExp(delim + *lit + delim));
  }

  stringPattern  = QRegExp("\".*\"");
  commentPattern = QRegExp("#.*");
  functionPattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
  tuplePattern = QRegExp("(\\((\\d+,)+\\d+\\))|(\\[\\d+(,\\d+)*\\])");
  promptPattern = QRegExp("dlfl>");
}

void PythonHighlighter::highlightBlock(const QString& text) {
  if (text.startsWith(">")) return;

  for( int p = 0; p < pythonPatterns.size(); p++) {
    const QRegExp& expression = pythonPatterns[p];
    int index = text.indexOf(expression);
    while (index >= 0) {
      int length = expression.matchedLength();
      setFormat(index, length, pythonFormat);
      index = text.indexOf(expression, index + length);
    }
  }

  int index = text.indexOf(stringPattern);
  while (index >= 0) {
    int length = stringPattern.matchedLength();
    setFormat(index, length, stringFormat);
    index = text.indexOf(stringPattern, index + length);
  }

  index = text.indexOf(commentPattern);
  while (index >= 0) {
    int length = commentPattern.matchedLength();
    setFormat(index, length, commentFormat);
    index = text.indexOf(commentPattern, index + length);
  }

  index = text.indexOf(functionPattern);
  while (index >= 0) {
    int length = functionPattern.matchedLength();
    setFormat(index, length, functionFormat);
    index = text.indexOf(functionPattern, index + length);
  }

  index = text.indexOf(tuplePattern);
  while (index >= 0) {
    int length = tuplePattern.matchedLength();
    setFormat(index, length, tupleFormat);
    index = text.indexOf(tuplePattern, index + length);
  }

  index = text.indexOf(promptPattern);
  while (index >= 0) {
    int length = promptPattern.matchedLength();
    setFormat(index, length, promptFormat);
    index = text.indexOf(promptPattern, index + length);
  }
}

