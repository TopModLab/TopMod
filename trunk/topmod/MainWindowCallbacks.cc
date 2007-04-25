/* $Id: MainWindowCallbacks.cc,v 4.13 2004/01/20 22:18:36 vinod Exp $ */

// Definitions for callback functions in the MainWindow class
// All these are static methods

#include "MainWindow.hh"

// File Menu.
void MainWindow::open_file()
{
  mainWindow->openFile();
  mainWindow->redraw();
}

void MainWindow::save_file()
{
  mainWindow->saveFile(false,false);
}

void MainWindow::save_file_with_normals()
{
     // Save the file with normal information.
     // Relevant only for OBJ files. DLFL files always have normal information.
  mainWindow->saveFile(true,false);
}

void MainWindow::save_file_with_tex_coords()
{
     // Save the file with texture coordinates
     // Relevant only for OBJ files. DLFL files always have texture coordinates.
  mainWindow->saveFile(false,true);
}

void MainWindow::save_file_extended()
{
     // Saves with all extra information. Currently only normals and tex coords
     // Relevant only for OBJ files. DLFL files always have normals and texture coordinates.
  mainWindow->saveFile(true,true);
}

void MainWindow::load_texture()
{
  // char * texfile = fl_file_chooser("Load Texture","*.{jpg,png}","");
  // if ( texfile != NULL )
  //    {
  //      textured->setTexture(texfile);
  //      texturedlit->setTexture(texfile);
  //      mainWindow->getActive()->valid(0); // To ensure new texture is loaded into OpenGL
  //      mainWindow->redraw();
  //    }
}

void MainWindow::print_info()
{
  mainWindow->printSummary();
}

void MainWindow::print_flist()
{
  mainWindow->printFaceList();
}

void MainWindow::print_vlist()
{
  mainWindow->printVertexList();
}

void MainWindow::print_elist()
{
  mainWindow->printEdgeList();
}

void MainWindow::quit()
{
  mainWindow->cleanupForExit();
  exit(0);
}

void MainWindow::undo()
{
  mainWindow->undo();
  mainWindow->redraw();
}

void MainWindow::redo()
{
  mainWindow->redo();
  mainWindow->redraw();
}

// Display Menu.
void MainWindow::toggle_vertices()
{
  mainWindow->toggleVertices();
  mainWindow->redraw();
}

void MainWindow::toggle_silhouette()
{
  mainWindow->toggleSilhouette();
  mainWindow->redraw();
}

void MainWindow::toggle_wireframe()
{
  mainWindow->toggleWireframe();
  mainWindow->redraw();
}

void MainWindow::toggle_grid()
{
  mainWindow->toggleGrid();
  mainWindow->redraw();
}

void MainWindow::toggle_axes()
{
  mainWindow->toggleAxes();
  mainWindow->redraw();
}

void MainWindow::use_normal_renderer()
{
  mainWindow->setRenderer(normal);
  //mainWindow->getActive()->invalidate();
  mainWindow->redraw();
}

void MainWindow::use_lighted_renderer()
{
  mainWindow->recomputeNormals();
  mainWindow->setRenderer(lit);
  // mainWindow->getActive()->invalidate();
  mainWindow->redraw();
}

void MainWindow::use_shaded_renderer()
{
  mainWindow->setRenderer(shaded);
  // mainWindow->getActive()->invalidate();
  mainWindow->redraw();
}

void MainWindow::use_textured_renderer()
{
  mainWindow->setRenderer(textured);
  // mainWindow->getActive()->invalidate();
  mainWindow->redraw();
}

void MainWindow::use_textured_lit_renderer()
{
  mainWindow->setRenderer(texturedlit);
  // mainWindow->getActive()->invalidate();
  mainWindow->redraw();
}

void MainWindow::use_patch_renderer()
{
  mainWindow->setRenderer(patch);
  // mainWindow->getActive()->invalidate();
  mainWindow->redraw();
}

// Object Menu.
void MainWindow::subdivide_all_edges()
{
   mainWindow->subDivideAllEdges();
   mainWindow->redraw();
}

void MainWindow::planarize_all_faces()
{
   mainWindow->planarizeFaces();
   mainWindow->redraw();
}

void MainWindow::make_object_spherical()
{
   mainWindow->spheralizeObject();
   mainWindow->redraw();
}

void MainWindow::make_object_smooth()
{
   mainWindow->smoothMesh();
   mainWindow->redraw();
}

void MainWindow::create_crust()
{
  mainWindow->createCrust();
  mainWindow->redraw();
}

void MainWindow::compute_lighting()
{
  mainWindow->recomputeLighting();
  mainWindow->redraw();
}

void MainWindow::compute_normals_and_lighting()
{
  mainWindow->recomputeNormals();
  mainWindow->redraw();
}

void MainWindow::assign_texture_coordinates()
{
  mainWindow->randomAssignTexCoords();
  mainWindow->redraw();
}

// Selection Menu.
void MainWindow::select_vertex()
{
  mainWindow->setMode(DLFLWindow::SelectVertex);
}

void MainWindow::select_face()
{
  mainWindow->setMode(DLFLWindow::SelectFace);
}

void MainWindow::select_edge()
{
  mainWindow->setMode(DLFLWindow::SelectEdge);
}

void MainWindow::select_corner()
{
  mainWindow->setMode(DLFLWindow::SelectFaceVertex);
}

void MainWindow::exit_selection_mode()
{
  mainWindow->setMode(DLFLWindow::NormalMode);
}

void MainWindow::exit_modes()
{
  mainWindow->setMode(DLFLWindow::NormalMode);
}

// Basics.
void MainWindow::toggleDeleteEdgeCleanupFlag(int state)
{
	// QMessageBox::about(this, tr("About TopMod"),tr("%1").arg(state));
	DLFLWindow::delete_edge_cleanup = bool(state);
}

void MainWindow::changeNumSubDivs(int value)
{
  DLFLWindow::num_e_subdivs = value;
}

void MainWindow::translatex(double value)
{
  mainWindow->translatex(value);
  mainWindow->redraw();
}

void MainWindow::translatey(double value)
{
  mainWindow->translatey(value);
  mainWindow->redraw();
}

void MainWindow::translatez(double value)
{
  mainWindow->translatez(value);
  mainWindow->redraw();
}

void MainWindow::scalex(double value)
{
  mainWindow->scalex(value);
  mainWindow->redraw();
}

void MainWindow::scaley(double value)
{
  mainWindow->scaley(value);
  mainWindow->redraw();
}

void MainWindow::scalez(double value)
{
  mainWindow->scalez(value);
  mainWindow->redraw();
}

void MainWindow::freeze_transforms()
{
  mainWindow->freezeTransforms();
  mainWindow->redraw();
}

void MainWindow::changeTileTexNum(int value)
{
  DLFLWindow::tile_tex_n = value;
}

// Extrusions.
void MainWindow::changeExtrudeLength(double value)
{
  DLFLWindow::extrude_dist = value;
}

void MainWindow::changeExtrudeRotation(int value)
{
  DLFLWindow::extrude_rot = value;
}

void MainWindow::changeExtrudeScale(double value)
{
  DLFLWindow::extrude_scale = value;
}

void MainWindow::changeNumExtrusions(int value)
{
  DLFLWindow::num_extrusions = value;
}

void MainWindow::toggleDualMeshEdgesFlag(int state)
{
  DLFLWindow::dual_mesh_edges_check = bool(state);
}

void MainWindow::toggleHexagonalizeDodecaExtrudeFlag(int state)
{
  DLFLWindow::hexagonalize_dodeca_extrude = bool(state);
}

// Stellation
void MainWindow::changeStellateLength(double value)
{
  DLFLWindow::extrude_dist = value;
}


// Holes and Handles.
void MainWindow::changeMaxSegments(int value)
{
  DLFLWindow::max_segments = value;
}

void MainWindow::changeNumSegments(int value)
{
	DLFLWindow::num_segments = value;
}

void MainWindow::changeNumSegments2(int value){
	
}

void MainWindow::changeMaxSegments2(int value){
	
}

void MainWindow::changeWeight1(double value)
{
  DLFLWindow::nwt1 = value;
}

void MainWindow::toggleSymmetricWeightsFlag(int state)
{
	DLFLWindow::symmetric_weights = bool(state);
}

void MainWindow::changeWeight2(double value)
{
	DLFLWindow::nwt2 = value;
}

void MainWindow::changeExtraTwists(int value)
{
  DLFLWindow::num_extra_twists = value;
}

// Crust Modeling.
void MainWindow::changeCrustScaleFactor(double value)
{
  DLFLWindow::crust_scale_factor = value;
}

void MainWindow::changeCrustThickness(double value)
{
  DLFLWindow::crust_thickness = value;
}

void MainWindow::toggleCrustCleanupFlag(int state)
{
  DLFLWindow::crust_cleanup = bool(state);
}

void MainWindow::crust_modeling1()
{
  mainWindow->setMode(DLFLWindow::CrustModeling);
  mainWindow->createCrust(true);
  mainWindow->redraw();
}

void MainWindow::crust_modeling2()
{
  mainWindow->setMode(DLFLWindow::CrustModeling);
  mainWindow->createCrust(false);
  mainWindow->redraw();
}

//column modeling and wireframe modeling
void MainWindow::changeWireframeThickness(double value)
{
  DLFLWindow::wireframe_thickness = value;
}

void MainWindow::changeColumnThickness(double value)
{
  DLFLWindow::column_thickness = value;
}

void MainWindow::changeColumnSegments(int value)
{
  DLFLWindow::column_segments = value;
}

void MainWindow::wireframe_modeling()
{
  mainWindow->makeWireframe();
  mainWindow->setMode(DLFLWindow::NormalMode);
  mainWindow->redraw();
}

void MainWindow::column_modeling()
{
  mainWindow->makeWireframeWithColumns();
  mainWindow->setMode(DLFLWindow::NormalMode);
  mainWindow->redraw();
}

//sierpinsky
void MainWindow::create_sierpinsky_tetrahedra()
{
  mainWindow->multiConnectMidpoints();
  mainWindow->redraw();
}


// Multi-face handle
void MainWindow::mfh_use_closest_edge_algo()
{
  DLFLWindow::mfh_algo = DLFLWindow::ClosestEdge;
}

void MainWindow::mfh_use_convex_hull_algo()
{
  DLFLWindow::mfh_algo = DLFLWindow::ConvexHull;
}

void MainWindow::changeMultiFaceHandleScaleFactor(double value)
{
  DLFLWindow::mfh_scale_factor = value;
}

void MainWindow::changeMultiFaceHandleExtrudeDist(double value)
{
  DLFLWindow::mfh_extrude_dist = value;
}

void MainWindow::toggleMultiFaceHandleUseMaxOffsetFlag(int state)
{
  DLFLWindow::mfh_use_max_offsets = bool(state);
}

void MainWindow::create_multi_face_handle()
{
  mainWindow->createMultiFaceHandle();
  mainWindow->redraw();
}

//menger sponge operations
void MainWindow::changeMengerSpongeThickness(double value)
{
  DLFLWindow::sponge_thickness = value;
}

void MainWindow::changeMengerSpongeCollapseThreshold(double value)
{
  DLFLWindow::sponge_collapse_threshold = value;
}

void MainWindow::toggleMengerSpongeFractionalThicknessFlag(int state)
{
  DLFLWindow::sponge_fractional_thickness = bool(state);
}

void MainWindow::create_menger_sponge()
{
  mainWindow->createSponge();
  mainWindow->redraw();
}

//sculpting operations from ozgur
void MainWindow::perform_cutting()
{
  mainWindow->performCutting();
  mainWindow->redraw();
}

void MainWindow::create_convexhull()
{
  mainWindow->createConvexHull();
  mainWindow->redraw();
}

void MainWindow::create_dual_convexhull()
{
  mainWindow->createDualConvexHull();
  mainWindow->redraw();
}

void MainWindow::changeCutOffsetE(double value)
{
  DLFLWindow::cutOffsetE_factor= value;
}

void MainWindow::changeCutOffsetV(double value)
{
  DLFLWindow::cutOffsetV_factor= value;
}

void MainWindow::changePNormalBendS(double value)
{
  DLFLWindow:: pnormalBendS_factor= value;
}

void MainWindow::changePNormalBendT(double value)
{
  DLFLWindow:: pnormalBendT_factor= value;
}

void MainWindow::toggleGlobalCut(int state){    //ozgur
	DLFLWindow::global_cut = bool(state);	
}
void MainWindow::toggleSelectedCut(int state){  //ozgur
	DLFLWindow::selected_cut = bool(state);
}

void MainWindow::changeTiltPlane1(double value){
	
}
void MainWindow::changeTiltPlane2(double value){
	
}




