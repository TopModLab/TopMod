/*
*
* ***** BEGIN GPL LICENSE BLOCK *****
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software  Foundation,
* Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*
* The Original Code is Copyright (C) 2005 by xxxxxxxxxxxxxx
* All rights reserved.
*
* The Original Code is: all of this file.
*
* Contributor(s): none yet.
*
* ***** END GPL LICENSE BLOCK *****
*
* Short description of this file
*
* name of .hh file containing function prototypes
*
*/

#include "PythonHighlighter.hh"

PythonHighlighter::PythonHighlighter(QTextEdit *parent) 
  : QSyntaxHighlighter(parent) {
  pythonKeywords << "and" << "assert" << "break" << "class" << "continue" << "def"
								 << "del" << "elif" << "else" << "except" << "exec" << "finally"
								 << "for" << "from" << "global" << "if" << "import" << "in"
								 << "is" << "lambda" << "not" << "or" << "pass" << "print" << "raise"
								 << "return" << "try" << "while" << "yield" << "True" << "False";

  promptFormat.setForeground(QColor("greenyellow"));
  promptFormat.setFontWeight(QFont::Bold);
  promptFormat.setFontItalic(true);

  functionFormat.setForeground(QColor("royalblue"));
  functionFormat.setFontWeight(QFont::Bold);

  tupleFormat.setForeground(QColor("lightskyblue"));
  tupleFormat.setFontWeight(QFont::Bold);

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

  stringPattern  = QRegExp("(\".*\")|'.*'");
  functionPattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
  tuplePattern = QRegExp("(\\((-?\\d+(\\.\\d*)?,[ ]*)+-?\\d+(\\.\\d*)?\\))|(\\[(-?\\d+(\\.\\d*)?,[ ]*)+-?\\d+(\\.\\d*)?\\])");
  promptPattern = QRegExp("dlfl>");
  commentPattern = QRegExp("#.*");
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

	// Must be last to highlight correctly
  index = text.indexOf(commentPattern);
  while (index >= 0) {
    int length = commentPattern.matchedLength();
    setFormat(index, length, commentFormat);
    index = text.indexOf(commentPattern, index + length);
  }
}

