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
