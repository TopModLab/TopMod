#ifndef _PYTHON_HIGHLIGHTER_H_
#define _PYTHON_HIGHLIGHTER_H_

#include <QSyntaxHighlighter>
#include <QTextEdit>
#include <QVector>

class PythonHighlighter : public QSyntaxHighlighter {
 Q_OBJECT

 public :
  PythonHighlighter(QTextEdit *parent = 0);
  ~PythonHighlighter( ) { };

 protected :
  void highlightBlock(const QString &text);
  void compilePattern( );

private :
  QVector<QRegExp> pythonPatterns;
  QRegExp stringPattern;
  QRegExp commentPattern;
  QRegExp functionPattern;
  QRegExp tuplePattern;
  QRegExp promptPattern;

  QStringList pythonKeywords;

  QTextCharFormat pythonFormat;
  QTextCharFormat commentFormat;
  QTextCharFormat stringFormat;
  QTextCharFormat functionFormat;
  QTextCharFormat tupleFormat;
  QTextCharFormat promptFormat;
};

#endif // _PYTHON_HIGHLIGHTER_H_
