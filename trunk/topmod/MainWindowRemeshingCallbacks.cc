#include "MainWindow.hh"

void MainWindow::perform_remeshing()
{
     // Generic callback for all remeshing schemes
     // This simply calls the generic remeshing subrouting in DLFLWindow class
     // which uses the currently selected remeshing scheme to call the appropriate function
     // The scheme should be set when the corresponding option is selected from the menu
  mainWindow->performRemeshing();
}

void MainWindow::subdivide_face()
{
  mainWindow->subDivideSelectedFaces();
  mainWindow->redraw();
}

// Controls for remeshing schemes
void MainWindow::toggleDooSabinEdgeFlag(int state)
{
  DLFLWindow::doo_sabin_check = bool(state);
}

void MainWindow::changeRoot4Twist(double value)
{
  DLFLWindow::twist_factor = value;
}

void MainWindow::changeRoot4Weight(double value)
{
  DLFLWindow::weight_factor = value;
}

void MainWindow::changeVertexCuttingOffset(double value)
{
  DLFLWindow::vertex_cutting_offset = value;
}

void MainWindow::changePentagonalOffset(double value)
{
  DLFLWindow::pentagonal_offset = value;
}

void MainWindow::changePentagonalScaleFactor(double value)
{
  DLFLWindow::pentagonal_scale = value;
}

// Added by Doug
void MainWindow::changeStarOffset(double value)
{
  DLFLWindow::star_offset = value;
}

void MainWindow::changeFractalOffset(double value)
{
  DLFLWindow::fractal_offset = value;
}

// End additions by Doug

// Added by Eric
void MainWindow::changeSubStellateAmount(double value)
{
  DLFLWindow::substellate_height = value;
}

void MainWindow::changeSubStellateCurve(double value)
{
  DLFLWindow::substellate_curve = value;
}

// End additions by Eric
void MainWindow::toggleAccurateDualFlag(int state)
{
  DLFLWindow::accurate_dual = !bool(state);
}

void MainWindow::toggleUseQuadsFlag(int state)
{
  DLFLWindow::use_quads = bool(state);
}

// Added by Bei & Cansin

void MainWindow::changeDomeSubdivisionLength(double value)
{
  DLFLWindow::domeLength_factor = value;
}

void MainWindow::changeDomeSubdivisionScale(double value)
{
  DLFLWindow::domeScale_factor = value;
}

void MainWindow::changeDual1264Scale(double value)
{
  DLFLWindow::dual1264_scale_factor = value;
}

void MainWindow::changeDooSabinBCnewScale(double value)
{
  DLFLWindow::dooSabinBCnewScale_factor = value;
}

void MainWindow::changeDooSabinBCnewLength(double value)
{
  DLFLWindow::dooSabinBCnewLength_factor = value;
}

void MainWindow::changeLoopStyleSubdivisionLength(double value)
{
  DLFLWindow::loopLength_factor = value;
}
