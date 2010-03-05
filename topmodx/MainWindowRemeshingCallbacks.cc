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

#include "MainWindow.hh"

// Controls for remeshing schemes
void MainWindow::toggleDooSabinEdgeFlag(int state)
{
  MainWindow::doo_sabin_check = bool(state);
}

void MainWindow::changeRoot4Twist(double value)
{
  MainWindow::twist_factor = value;
}

void MainWindow::changeRoot4Weight(double value)
{
  MainWindow::weight_factor = value;
}

void MainWindow::changeVertexCuttingOffset(double value)
{
  MainWindow::vertex_cutting_offset = value;
}

void MainWindow::changePentagonalOffset(double value)
{
  MainWindow::pentagonal_offset = value;
}

void MainWindow::changePentagonalScaleFactor(double value)
{
  MainWindow::pentagonal_scale = value;
}

// Added by Doug
void MainWindow::changeStarOffset(double value)
{
  MainWindow::star_offset = value;
}

void MainWindow::changeFractalOffset(double value)
{
  MainWindow::fractal_offset = value;
}

// End additions by Doug

// Added by Eric
void MainWindow::changeSubStellateAmount(double value)
{
  MainWindow::substellate_height = value;
}

void MainWindow::changeSubStellateCurve(double value)
{
  MainWindow::substellate_curve = value;
}

// End additions by Eric
void MainWindow::toggleAccurateDualFlag(int state)
{
  MainWindow::accurate_dual = !bool(state);
}

void MainWindow::toggleUseQuadsFlag(int state)
{
  MainWindow::use_quads = bool(state);
}

// Added by Bei & Cansin

void MainWindow::changeDomeSubdivisionLength(double value)
{
  MainWindow::domeLength_factor = value;
}

void MainWindow::changeDomeSubdivisionScale(double value)
{
  MainWindow::domeScale_factor = value;
}

void MainWindow::changeDual1264Scale(double value)
{
  MainWindow::dual1264_scale_factor = value;
}

void MainWindow::changeDooSabinBCnewScale(double value)
{
  MainWindow::dooSabinBCnewScale_factor = value;
}

void MainWindow::changeDooSabinBCnewLength(double value)
{
  MainWindow::dooSabinBCnewLength_factor = value;
}

void MainWindow::changeLoopStyleSubdivisionLength(double value)
{
  MainWindow::loopLength_factor = value;
}
