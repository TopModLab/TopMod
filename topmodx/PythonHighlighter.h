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
*/

#ifndef _PYTHON_HIGHLIGHTER_H_
#define _PYTHON_HIGHLIGHTER_H_

#include <QSyntaxHighlighter>
#include <QTextEdit>
#include <QVector>

class PythonHighlighter : public QSyntaxHighlighter {
 Q_OBJECT

 public :
  PythonHighlighter(QTextEdit *parent = 0);
  ~PythonHighlighter();

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
