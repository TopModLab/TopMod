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

#include "MainWindow.h"

// Controls for remeshing schemes
void MainWindow::toggleDooSabinEdgeFlag(int state)
{
	MainWindow::sDooSabinCheck = bool(state);
}

void MainWindow::changeRoot4Twist(double value)
{
	MainWindow::sTwistFactor = value;
}

void MainWindow::changeRoot4Weight(double value)
{
	MainWindow::sWeightFactor = value;
}

void MainWindow::changeVertexCuttingOffset(double value)
{
	MainWindow::sVertexCuttingOffset = value;
}

void MainWindow::changePentagonalOffset(double value)
{
	MainWindow::sPentagonalOffset = value;
}

void MainWindow::changePentagonalScaleFactor(double value)
{
	MainWindow::sPentagonalScale = value;
}

// Added by Doug
void MainWindow::changeStarOffset(double value)
{
	MainWindow::sStarOffset = value;
}

void MainWindow::changeFractalOffset(double value)
{
	MainWindow::sFractalOffset = value;
}

// End additions by Doug

// Added by Eric
void MainWindow::changeSubStellateAmount(double value)
{
	MainWindow::sSubstellateHeight = value;
}

void MainWindow::changeSubStellateCurve(double value)
{
	MainWindow::sSubstellateCurve = value;
}

// End additions by Eric
void MainWindow::toggleAccurateDualFlag(int state)
{
	MainWindow::sAccurateDual = !bool(state);
}

void MainWindow::toggleUseQuadsFlag(int state)
{
	MainWindow::sUseQuads = bool(state);
}

// Added by Bei & Cansin

void MainWindow::changeDomeSubdivisionLength(double value)
{
	MainWindow::sDomeLengthFactor = value;
}

void MainWindow::changeDomeSubdivisionScale(double value)
{
	MainWindow::sDomeScaleFactor = value;
}

void MainWindow::changeDual1264Scale(double value)
{
	MainWindow::sDual1264ScaleFactor = value;
}

void MainWindow::changeDooSabinBCnewScale(double value)
{
	MainWindow::sDooSabinBCnewScaleFactor = value;
}

void MainWindow::changeDooSabinBCnewLength(double value)
{
	MainWindow::sDooSabinBCnewLengthFactor = value;
}

void MainWindow::changeLoopStyleSubdivisionLength(double value)
{
	MainWindow::sLoopLengthFactor = value;
}
