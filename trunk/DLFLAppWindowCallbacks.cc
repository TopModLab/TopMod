/* $Id: DLFLAppWindowCallbacks.cc,v 4.13 2004/01/20 22:18:36 vinod Exp $ */

// Definitions for callback functions in the DLFLAppWindow class
// All these are static methods

#include "DLFLAppWindow.hh"

void DLFLAppWindow :: hide_modes()
{
  basics->hide();
  extrude->hide();
  remeshing->hide();
  highgenus->hide();
  texturing->hide();
  remeshing_subgrp_panel->hide();
}

void DLFLAppWindow :: hide_tiles()
{
  blank->hide();
  delete_edge_tile->hide();
  subdivide_edge_tile->hide();
  subdivide_face_tile->hide();
  tile_texturing_tile->hide();
  extrude_tile->hide();
  stellate_tile->hide();
  hole_tile->hide();
  handle_tile->hide();
  multi_face_handle_tile->hide();
  crust_modeling1_tile->hide();
  crust_modeling2_tile->hide();
  wireframe_modeling_tile->hide();
  column_modeling_tile->hide();

  hide_remeshing_tiles(0,0);

  sierpinsky_tile->hide();
  mengersponge_tile->hide();
  transforms_tile->hide();
}

// File Menu.
void DLFLAppWindow :: open_file(Fl_Menu_*, void*)
{
  mainWindowPtr->openFile();
  mainWindowPtr->redraw();
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: save_file(Fl_Menu_*, void*)
{
  mainWindowPtr->saveFile(false,false);
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: save_file_with_normals(Fl_Menu_*, void*)
{
     // Save the file with normal information.
     // Relevant only for OBJ files. DLFL files always have normal information.
  mainWindowPtr->saveFile(true,false);
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: save_file_with_tex_coords(Fl_Menu_*, void*)
{
     // Save the file with texture coordinates
     // Relevant only for OBJ files. DLFL files always have texture coordinates.
  mainWindowPtr->saveFile(false,true);
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: save_file_extended(Fl_Menu_*, void*)
{
     // Saves with all extra information. Currently only normals and tex coords
     // Relevant only for OBJ files. DLFL files always have normals and texture coordinates.
  mainWindowPtr->saveFile(true,true);
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: load_texture(Fl_Menu_*, void*)
{
  char * texfile = fl_file_chooser("Load Texture","*.{jpg,png}","");
  if ( texfile != NULL )
     {
       textured->setTexture(texfile);
       texturedlit->setTexture(texfile);
       mainWindowPtr->getActive()->valid(0); // To ensure new texture is loaded into OpenGL
       mainWindowPtr->redraw();
       Fl :: focus(mainWindowPtr);
     }
}

void DLFLAppWindow :: print_info(Fl_Menu_*, void*)
{
  mainWindowPtr->printSummary();
}

void DLFLAppWindow :: print_flist(Fl_Menu_*, void*)
{
  mainWindowPtr->printFaceList();
}

void DLFLAppWindow :: print_vlist(Fl_Menu_*, void*)
{
  mainWindowPtr->printVertexList();
}

void DLFLAppWindow :: print_elist(Fl_Menu_*, void*)
{
  mainWindowPtr->printEdgeList();
}

void DLFLAppWindow :: quit(Fl_Menu_*, void*)
{
  mainWindowPtr->cleanupForExit();
  exit(0);
}

void DLFLAppWindow :: undo(Fl_Menu_*, void*)
{
  mainWindowPtr->undo();
  mainWindowPtr->redraw();
}

void DLFLAppWindow :: redo(Fl_Menu_*, void*)
{
  mainWindowPtr->redo();
  mainWindowPtr->redraw();
}

// Display Menu.
void DLFLAppWindow :: toggle_vertices(Fl_Menu_*, void*)
{
  mainWindowPtr->toggleVertices();
  mainWindowPtr->redraw();
}

void DLFLAppWindow :: toggle_silhouette(Fl_Menu_*, void*)
{
  mainWindowPtr->toggleSilhouette();
  mainWindowPtr->redraw();
}

void DLFLAppWindow :: toggle_wireframe(Fl_Menu_*, void*)
{
  mainWindowPtr->toggleWireframe();
  mainWindowPtr->redraw();
}

void DLFLAppWindow :: toggle_patch_wireframe(Fl_Menu_*, void*)
{
  mainWindowPtr->togglePatchWireframe();
  mainWindowPtr->redraw();
}

void DLFLAppWindow :: toggle_patch_vertices(Fl_Menu_*,void*)
{
  mainWindowPtr->togglePatchVertices();
  mainWindowPtr->redraw();
}

void DLFLAppWindow :: toggle_grid(Fl_Menu_*, void*)
{
  mainWindowPtr->toggleGrid();
  mainWindowPtr->redraw();
}

void DLFLAppWindow :: toggle_axes(Fl_Menu_*, void*)
{
  mainWindowPtr->toggleAxes();
  mainWindowPtr->redraw();
}

void DLFLAppWindow :: use_normal_renderer(Fl_Menu_*, void*)
{
  mainWindowPtr->setRenderer(normal);
  mainWindowPtr->getActive()->invalidate();
  mainWindowPtr->redraw();
}

void DLFLAppWindow :: use_lighted_renderer(Fl_Menu_*, void*)
{
  mainWindowPtr->recomputeNormals();
  mainWindowPtr->setRenderer(lit);
  mainWindowPtr->getActive()->invalidate();
  mainWindowPtr->redraw();
}

void DLFLAppWindow :: use_shaded_renderer(Fl_Menu_*, void*)
{
  mainWindowPtr->setRenderer(shaded);
  mainWindowPtr->getActive()->invalidate();
  mainWindowPtr->redraw();
}

void DLFLAppWindow :: use_textured_renderer(Fl_Menu_*, void*)
{
  mainWindowPtr->setRenderer(textured);
  mainWindowPtr->getActive()->invalidate();
  mainWindowPtr->redraw();
}

void DLFLAppWindow :: use_textured_lit_renderer(Fl_Menu_*, void*)
{
  mainWindowPtr->setRenderer(texturedlit);
  mainWindowPtr->getActive()->invalidate();
  mainWindowPtr->redraw();
}

void DLFLAppWindow :: use_bezier_renderer(Fl_Menu_*, void*)
{
  bezier->updateBezierSurface();
  mainWindowPtr->setRenderer(bezier);
  mainWindowPtr->getActive()->invalidate();
  mainWindowPtr->redraw();
}

// Object Menu.
void DLFLAppWindow :: subdivide_all_edges(Fl_Menu_*, void*)
{
   mainWindowPtr->subDivideAllEdges();
   mainWindowPtr->redraw();
}

void DLFLAppWindow :: planarize_all_faces(Fl_Menu_*, void*)
{
   mainWindowPtr->planarizeFaces();
   mainWindowPtr->redraw();
}

void DLFLAppWindow :: make_object_spherical(Fl_Menu_*, void*)
{
   mainWindowPtr->spheralizeObject();
   mainWindowPtr->redraw();
}

void DLFLAppWindow :: make_object_smooth(Fl_Menu_*, void*)
{
   mainWindowPtr->smoothMesh();
   mainWindowPtr->redraw();
}

void DLFLAppWindow :: create_crust(Fl_Menu_*, void*)
{
  mainWindowPtr->createCrust();
  mainWindowPtr->redraw();
}

void DLFLAppWindow :: compute_lighting(Fl_Menu_*, void*)
{
  mainWindowPtr->recomputeLighting();
  mainWindowPtr->redraw();
}

void DLFLAppWindow :: compute_normals_and_lighting(Fl_Menu_*, void*)
{
  mainWindowPtr->recomputeNormals();
  mainWindowPtr->redraw();
}

void DLFLAppWindow :: assign_texture_coordinates(Fl_Menu_*, void*)
{
  mainWindowPtr->randomAssignTexCoords();
  mainWindowPtr->redraw();
}

// Selection Menu.
void DLFLAppWindow :: select_vertex(Fl_Menu_*, void*)
{
  mainWindowPtr->setMode(DLFLWindow::SelectVertex);
}

void DLFLAppWindow :: select_face(Fl_Menu_*, void*)
{
  mainWindowPtr->setMode(DLFLWindow::SelectFace);
}

void DLFLAppWindow :: select_edge(Fl_Menu_*, void*)
{
  mainWindowPtr->setMode(DLFLWindow::SelectEdge);
}

void DLFLAppWindow :: select_corner(Fl_Menu_*, void*)
{
  mainWindowPtr->setMode(DLFLWindow::SelectFaceVertex);
}

void DLFLAppWindow :: exit_selection_mode(Fl_Menu_*, void*)
{
  mainWindowPtr->setMode(DLFLWindow::NormalMode);
}

void DLFLAppWindow :: exit_modes(Fl_Menu_*, void*)
{
  mainWindowPtr->setMode(DLFLWindow::NormalMode);
  hide_tiles();
  blank->show();
}

void DLFLAppWindow :: normal_mode(Fl_Menu_*, void*)
{
  mainWindowPtr->setMode(DLFLWindow::NormalMode);
  hide_modes();
  hide_tiles();
  blank->show();
}

// Modes.
void DLFLAppWindow :: basics_mode(Fl_Menu_*, void*)
{
  hide_modes();
  hide_tiles();
  basics->value(0);
  basics->show();
  blank->show();
}

void DLFLAppWindow :: bezier_mode(Fl_Menu_*,void*) // brianb
{
  mainWindowPtr->setMode(DLFLWindow::BezierMode);
  hide_modes();
  hide_tiles();
  blank->show();
}

void DLFLAppWindow :: edit_mode(Fl_Menu_*, void*)  // brianb
{
  mainWindowPtr->setMode(DLFLWindow::EditMode);
  hide_modes();
  hide_tiles();
  blank->show();
}

void DLFLAppWindow :: extrude_mode(Fl_Menu_*, void*)
{
  hide_modes();
  hide_tiles();
  extrude->value(0);
  extrude->show();
  blank->show();
}

void DLFLAppWindow :: handles_mode(Fl_Menu_*, void*)
{
  hide_modes();
  hide_tiles();
  handles->value(0);
  handles->show();
  blank->show();
}

void DLFLAppWindow :: crust_mode(Fl_Menu_*, void*)
{
  hide_modes();
  hide_tiles();
  crust->value(0);
  crust->show();
  blank->show();
}

void DLFLAppWindow :: highgenus_mode(Fl_Menu_*, void*)
{
  hide_modes();
  hide_tiles();
  highgenus->value(0);
  highgenus->show();
  blank->show();
}

void DLFLAppWindow :: texturing_mode(Fl_Menu_*, void*)
{
  hide_modes();
  hide_tiles();
  texturing->value(0);
  texturing->show();
  blank->show();
}


// Basics.
void DLFLAppWindow :: insert_edge_mode(Fl_Menu_*, void*)
{
  mainWindowPtr->setMode(DLFLWindow::InsertEdge);
  hide_tiles();
  blank->show();
}

void DLFLAppWindow :: delete_edge_mode(Fl_Menu_*, void*)
{
  mainWindowPtr->setMode(DLFLWindow::DeleteEdge);
  hide_tiles();
  delete_edge_tile->show();
}

void DLFLAppWindow :: collapse_edge_mode(Fl_Menu_*, void*)
{
  mainWindowPtr->setMode(DLFLWindow::CollapseEdge);
  hide_tiles();
  blank->show();
}

void DLFLAppWindow :: subdivide_edge_mode(Fl_Menu_*, void*)
{
  mainWindowPtr->setMode(DLFLWindow::SubDivideEdge);
  hide_tiles();
  subdivide_edge_tile->show();
}

void DLFLAppWindow :: tile_texturing_mode(Fl_Menu_*, void*)
{
  hide_tiles();
  tile_texturing_tile->show();
}

void DLFLAppWindow :: assign_tile_texture_coords(Fl_Menu_*, void*)
{
  mainWindowPtr->assignTileTexCoords();
  mainWindowPtr->redraw();
}

void DLFLAppWindow :: connect_edge_mode(Fl_Menu_*, void*)
{
  mainWindowPtr->setMode(DLFLWindow::ConnectEdges);
  hide_tiles();
  blank->show();
}

void DLFLAppWindow :: transforms_mode(Fl_Menu_*, void*)
{
  hide_tiles();
  transforms_tile->show();
}

// Extrude.
void DLFLAppWindow :: extrude_face(Fl_Menu_*, void*)
{
  mainWindowPtr->setMode(DLFLWindow::ExtrudeFace);
  hide_tiles();
  dualMeshEdgesCheck_button->hide();
  hexagonalizeDodecaExtrudeCheck_button->hide();
  extrudeRotation_input->activate();
  extrudeRotation_roller->activate();
  extrude_tile->show();
}

void DLFLAppWindow :: extrude_face_doosabin(Fl_Menu_*, void*)
{
  mainWindowPtr->setMode(DLFLWindow::ExtrudeFaceDS);
  hide_tiles();
  dualMeshEdgesCheck_button->hide();
  hexagonalizeDodecaExtrudeCheck_button->hide();
  extrudeRotation_input->activate();
  extrudeRotation_roller->activate();
  extrude_tile->show();
}

void DLFLAppWindow :: extrude_face_dodeca(Fl_Menu_*, void*)
{
  mainWindowPtr->setMode(DLFLWindow::ExtrudeFaceDodeca);
  hide_tiles();
  dualMeshEdgesCheck_button->hide();
  hexagonalizeDodecaExtrudeCheck_button->show();
  extrudeRotation_input->deactivate();
  extrudeRotation_roller->deactivate();
  extrude_tile->show();
}

void DLFLAppWindow :: extrude_face_icosa(Fl_Menu_*, void*)
{
  mainWindowPtr->setMode(DLFLWindow::ExtrudeFaceIcosa);
  hide_tiles();
  dualMeshEdgesCheck_button->hide();
  hexagonalizeDodecaExtrudeCheck_button->hide();
  extrudeRotation_input->deactivate();
  extrudeRotation_roller->deactivate();
  extrude_tile->show();
}

void DLFLAppWindow :: extrude_dual_face(Fl_Menu_*, void*)
{
  mainWindowPtr->setMode(DLFLWindow::ExtrudeDualFace);
  hide_tiles();
  dualMeshEdgesCheck_button->show();
  hexagonalizeDodecaExtrudeCheck_button->hide();
  extrudeRotation_input->activate();
  extrudeRotation_roller->activate();
  extrude_tile->show();
}

void DLFLAppWindow :: stellate_face(Fl_Menu_*, void*)
{
  mainWindowPtr->setMode(DLFLWindow::StellateFace);
  hide_tiles();
  stellate_tile->show();
}

void DLFLAppWindow :: double_stellate_face(Fl_Menu_*, void*)
{
  mainWindowPtr->setMode(DLFLWindow::DoubleStellateFace);
  hide_tiles();
  stellate_tile->show();
}


// Crust.
void DLFLAppWindow :: crust_modeling_mode1(Fl_Menu_*, void*)
{
  hide_tiles();
  crust_modeling1_tile->show();
}

void DLFLAppWindow :: crust_modeling1(Fl_Menu_*, void*)
{
  mainWindowPtr->setMode(DLFLWindow::CrustModeling);
  mainWindowPtr->createCrust(true);
  mainWindowPtr->redraw();
}

void DLFLAppWindow :: crust_modeling_mode2(Fl_Menu_*, void*)
{
  hide_tiles();
  crust_modeling2_tile->show();
}

void DLFLAppWindow :: crust_modeling2(Fl_Menu_*, void*)
{
  mainWindowPtr->setMode(DLFLWindow::CrustModeling);
  mainWindowPtr->createCrust(false);
  mainWindowPtr->redraw();
}

void DLFLAppWindow :: wireframe_modeling_mode(Fl_Menu_*, void*)
{
  hide_tiles();
  wireframe_modeling_tile->show();
}

void DLFLAppWindow :: wireframe_modeling(Fl_Menu_*, void*)
{
  mainWindowPtr->makeWireframe();
  mainWindowPtr->setMode(DLFLWindow::NormalMode);
  mainWindowPtr->redraw();
}

void DLFLAppWindow :: column_modeling_mode(Fl_Menu_*, void*)
{
  hide_tiles();
  column_modeling_tile->show();
}

void DLFLAppWindow :: column_modeling(Fl_Menu_*, void*)
{
  mainWindowPtr->makeWireframeWithColumns();
  mainWindowPtr->setMode(DLFLWindow::NormalMode);
  mainWindowPtr->redraw();
}


// Handles.
void DLFLAppWindow :: add_hole_mode1(Fl_Menu_*, void*)
{
  mainWindowPtr->setMode(DLFLWindow::ConnectFaceVertices);
  hide_tiles();
  hole_tile->show();
}

void DLFLAppWindow :: add_hole_mode2(Fl_Menu_*, void*)
{
  mainWindowPtr->setMode(DLFLWindow::ConnectFaces);
  hide_tiles();
  hole_tile->show();
}

void DLFLAppWindow :: add_handle_mode(Fl_Menu_*, void*)
{
  mainWindowPtr->setMode(DLFLWindow::HermiteConnectFaces);
  hide_tiles();
  handle_tile->show();
}

void DLFLAppWindow :: multi_face_handle_mode(Fl_Menu_*, void*)
{
  mainWindowPtr->setMode(DLFLWindow::MultiSelectFace);
  hide_tiles();
  multi_face_handle_tile->show();
}

void DLFLAppWindow :: create_multi_face_handle(Fl_Menu_*, void*)
{
  mainWindowPtr->createMultiFaceHandle();
  mainWindowPtr->redraw();
}


// High Genus Modeling
void DLFLAppWindow :: sierpinsky_mode(Fl_Menu_*, void*)
{
  hide_tiles();
  sierpinsky_tile->show();
}

void DLFLAppWindow :: create_sierpinsky_tetrahedra(Fl_Menu_*, void*)
{
  mainWindowPtr->multiConnectMidpoints();
  mainWindowPtr->redraw();
}

void DLFLAppWindow :: mengersponge_mode(Fl_Menu_*, void*)
{
  hide_tiles();
  mengersponge_tile->show();
}

void DLFLAppWindow :: create_menger_sponge(Fl_Menu_*, void*)
{
  mainWindowPtr->createSponge();
  mainWindowPtr->redraw();
}


// Rollers and Input.

// Basics.
void DLFLAppWindow :: toggleDeleteEdgeCleanupFlag(Fl_Widget * widget, void * data)
{
  deleteEdgeCleanupCheck_button = (Fl_Check_Button *)widget;
  DLFLWindow::delete_edge_cleanup = bool(deleteEdgeCleanupCheck_button->value());
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: changeNumSubDivs(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  DLFLWindow::num_e_subdivs = int(v->value());
  numSubDivs_roller->value(DLFLWindow::num_e_subdivs);
  numSubDivs_input->value(DLFLWindow::num_e_subdivs);
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: changeTileTexNum(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  int num = 
  DLFLWindow::tile_tex_n = int(v->value());
  tileTexNum_roller->value(DLFLWindow::tile_tex_n);
  tileTexNum_input->value(DLFLWindow::tile_tex_n);
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: toggleUseQuadsFlag(Fl_Widget * widget, void * data)
{
  useQuadsCheck_button = (Fl_Check_Button *)widget;
  DLFLWindow::use_quads = bool(useQuadsCheck_button->value());
  Fl :: focus(mainWindowPtr);
}


// Extrusions.
void DLFLAppWindow :: changeExtrudeLength(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  DLFLWindow::extrude_dist = v->value();
  extrudeLength_roller->value(DLFLWindow::extrude_dist);
  extrudeLength_input->value(DLFLWindow::extrude_dist);
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: changeExtrudeRotation(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  DLFLWindow::extrude_rot = v->value();
  extrudeRotation_roller->value(DLFLWindow::extrude_rot);
  extrudeRotation_input->value(DLFLWindow::extrude_rot);
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: changeExtrudeScale(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  DLFLWindow::extrude_scale = v->value();
  extrudeScale_roller->value(DLFLWindow::extrude_scale);
  extrudeScale_input->value(DLFLWindow::extrude_scale);
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: changeNumExtrusions(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  DLFLWindow::num_extrusions = int(v->value());
  numExtrusions_roller->value(DLFLWindow::num_extrusions);
  numExtrusions_input->value(DLFLWindow::num_extrusions);
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: toggleDualMeshEdgesFlag(Fl_Widget * widget, void * data)
{
  dualMeshEdgesCheck_button = (Fl_Check_Button *)widget;
  DLFLWindow::dual_mesh_edges_check = bool(dualMeshEdgesCheck_button->value());
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: toggleHexagonalizeDodecaExtrudeFlag(Fl_Widget * widget, void * data)
{
  hexagonalizeDodecaExtrudeCheck_button = (Fl_Check_Button *)widget;
  DLFLWindow::hexagonalize_dodeca_extrude = bool(hexagonalizeDodecaExtrudeCheck_button->value());
  Fl :: focus(mainWindowPtr);
}

// Stellation
void DLFLAppWindow :: changeStellateLength(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  DLFLWindow::extrude_dist = v->value();
  stellateLength_roller->value(DLFLWindow::extrude_dist);
  stellateLength_input->value(DLFLWindow::extrude_dist);
  Fl :: focus(mainWindowPtr);
}


// Holes and Handles.
void DLFLAppWindow :: changeMaxSegments(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  DLFLWindow::max_segments = int(v->value());
  maxSegments_roller->value(DLFLWindow::max_segments); 
  maxSegments_input->value(DLFLWindow::max_segments); 
  maxSegments2_roller->value(DLFLWindow::max_segments); 
  maxSegments2_input->value(DLFLWindow::max_segments); 
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: changeNumSegments(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  DLFLWindow::num_segments = int(v->value());

  numSegments_input->value(DLFLWindow::num_segments);
  numSegments_roller->value(DLFLWindow::num_segments); 
  numSegments2_input->value(DLFLWindow::num_segments);
  numSegments2_roller->value(DLFLWindow::num_segments);

  maxSegments_roller->maximum(DLFLWindow::num_segments);
  maxSegments_input->maximum(DLFLWindow::num_segments);
  maxSegments2_roller->maximum(DLFLWindow::num_segments);
  maxSegments2_input->maximum(DLFLWindow::num_segments);

  if ( DLFLWindow::max_segments > DLFLWindow::num_segments )
     {
       DLFLWindow::max_segments = DLFLWindow::num_segments;
       maxSegments_roller->value(DLFLWindow::max_segments);
       maxSegments_input->value(DLFLWindow::max_segments);
       maxSegments2_roller->value(DLFLWindow::max_segments);
       maxSegments2_input->value(DLFLWindow::max_segments);
     }

  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: changeWeight1(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  DLFLWindow::nwt1 = v->value();
  weight1_roller->value(DLFLWindow::nwt1);
  weight1_input->value(DLFLWindow::nwt1);
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: toggleSymmetricWeightsFlag(Fl_Widget * widget, void * data)
{
  symmetricWeightsCheck_button = (Fl_Check_Button *)widget;
  DLFLWindow::symmetric_weights = bool(symmetricWeightsCheck_button->value());
  if ( DLFLWindow::symmetric_weights )
     {
       weight2_input->deactivate();
       weight2_roller->deactivate();
     }
  else
     {
       weight2_input->activate();
       weight2_roller->activate();
     }
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: changeWeight2(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  DLFLWindow::nwt2 = v->value();
  weight2_roller->value(DLFLWindow::nwt2);
  weight2_input->value(DLFLWindow::nwt2);
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: changeExtraTwists(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  DLFLWindow::num_extra_twists = int(v->value());
  extraTwists_roller->value(DLFLWindow::num_extra_twists);
  extraTwists_input->value(DLFLWindow::num_extra_twists);
  Fl :: focus(mainWindowPtr);
}


// Multi-face handle
void DLFLAppWindow :: mfh_use_closest_edge_algo(Fl_Menu_*, void*)
{
  DLFLWindow::mfh_algo = DLFLWindow::ClosestEdge;
     // Disable other controls
  multiFaceHandleScaleFactor_input->deactivate();
  multiFaceHandleScaleFactor_roller->deactivate();
  multiFaceHandleExtrudeDist_input->deactivate();
  multiFaceHandleExtrudeDist_roller->deactivate();
  multiFaceHandleUseMaxOffsetCheck_button->deactivate();
}

void DLFLAppWindow :: mfh_use_convex_hull_algo(Fl_Menu_*, void*)
{
  DLFLWindow::mfh_algo = DLFLWindow::ConvexHull;
     // Enable other controls (check for max_offset check box)
  multiFaceHandleScaleFactor_input->activate();
  multiFaceHandleScaleFactor_roller->activate();
  multiFaceHandleUseMaxOffsetCheck_button->activate();
  if ( !DLFLWindow::mfh_use_max_offsets )
     {
       multiFaceHandleExtrudeDist_input->activate();
       multiFaceHandleExtrudeDist_roller->activate();
     }
}

void DLFLAppWindow :: changeMultiFaceHandleScaleFactor(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  DLFLWindow::mfh_scale_factor = v->value();
  multiFaceHandleScaleFactor_roller->value(DLFLWindow::mfh_scale_factor);
  multiFaceHandleScaleFactor_input->value(DLFLWindow::mfh_scale_factor);
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: changeMultiFaceHandleExtrudeDist(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  DLFLWindow::mfh_extrude_dist = v->value();
  multiFaceHandleExtrudeDist_roller->value(DLFLWindow::mfh_extrude_dist);
  multiFaceHandleExtrudeDist_input->value(DLFLWindow::mfh_extrude_dist);
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: toggleMultiFaceHandleUseMaxOffsetFlag(Fl_Widget * widget, void * data)
{
  multiFaceHandleUseMaxOffsetCheck_button = (Fl_Check_Button *)widget;
  DLFLWindow::mfh_use_max_offsets = bool(multiFaceHandleUseMaxOffsetCheck_button->value());
  if ( DLFLWindow::mfh_use_max_offsets )
     {
       multiFaceHandleExtrudeDist_input->deactivate();
       multiFaceHandleExtrudeDist_roller->deactivate();
     }
  else
     {
       multiFaceHandleExtrudeDist_input->activate();
       multiFaceHandleExtrudeDist_roller->activate();
     }
  Fl :: focus(mainWindowPtr);
}

// Crust Modeling.
void DLFLAppWindow :: changeCrustScaleFactor(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  DLFLWindow::crust_scale_factor = v->value();
  crustScaleFactor_roller->value(DLFLWindow::crust_scale_factor);
  crustScaleFactor_input->value(DLFLWindow::crust_scale_factor);
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: changeCrustThickness(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  DLFLWindow::crust_thickness = v->value();
  crustThickness_roller->value(DLFLWindow::crust_thickness);
  crustThickness_input->value(DLFLWindow::crust_thickness);
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: toggleCrustCleanupFlag(Fl_Widget * widget, void * data)
{
  Fl_Check_Button * btn = (Fl_Check_Button *)widget;
  DLFLWindow::crust_cleanup = bool(btn->value());
  crustCleanupCheck_button1->value(DLFLWindow::crust_cleanup);
  crustCleanupCheck_button2->value(DLFLWindow::crust_cleanup);
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: changeWireframeThickness(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  DLFLWindow::wireframe_thickness = v->value();
  wireframeThickness_roller->value(DLFLWindow::wireframe_thickness);
  wireframeThickness_input->value(DLFLWindow::wireframe_thickness);
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: changeColumnThickness(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  DLFLWindow::column_thickness = v->value();
  columnThickness_roller->value(DLFLWindow::column_thickness);
  columnThickness_input->value(DLFLWindow::column_thickness);
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: changeColumnSegments(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  DLFLWindow::column_segments = int(v->value());
  columnSegments_roller->value(DLFLWindow::column_segments);
  columnSegments_input->value(DLFLWindow::column_segments);
  Fl :: focus(mainWindowPtr);
}


void DLFLAppWindow :: changeMengerSpongeThickness(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  DLFLWindow::sponge_thickness = v->value();
  mengerSpongeThickness_roller->value(DLFLWindow::sponge_thickness);
  mengerSpongeThickness_input->value(DLFLWindow::sponge_thickness);
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: changeMengerSpongeCollapseThreshold(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  DLFLWindow::sponge_collapse_threshold = v->value();
  mengerSpongeCollapseThreshold_roller->value(DLFLWindow::sponge_collapse_threshold);
  mengerSpongeCollapseThreshold_input->value(DLFLWindow::sponge_collapse_threshold);
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: toggleMengerSpongeFractionalThicknessFlag(Fl_Widget * widget, void * data)
{
  mengerSpongeFractionalThicknessCheck_button = (Fl_Check_Button *)widget;
  DLFLWindow::sponge_fractional_thickness = bool(mengerSpongeFractionalThicknessCheck_button->value());
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: translatex(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  double x = v->value();
  translatex_input->value(x);
  translatex_roller->value(x);
  mainWindowPtr->translatex(x);
  mainWindowPtr->redraw();
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: translatey(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  double y = v->value();
  translatey_input->value(y);
  translatey_roller->value(y);
  mainWindowPtr->translatey(y);
  mainWindowPtr->redraw();
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: translatez(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  double z = v->value();
  translatez_input->value(z);
  translatez_roller->value(z);
  mainWindowPtr->translatez(z);
  mainWindowPtr->redraw();
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: scalex(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  double sx = v->value();
  scalex_input->value(sx);
  scalex_roller->value(sx);
  mainWindowPtr->scalex(sx);
  mainWindowPtr->redraw();
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: scaley(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  double sy = v->value();
  scaley_input->value(sy);
  scaley_roller->value(sy);
  mainWindowPtr->scaley(sy);
  mainWindowPtr->redraw();
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: scalez(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  double sz = v->value();
  scalez_input->value(sz);
  scalez_roller->value(sz);
  mainWindowPtr->scalez(sz);
  mainWindowPtr->redraw();
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: freeze_transforms(Fl_Widget *, void *)
{
  translatex_input->value(0); translatex_roller->value(0);
  translatey_input->value(0); translatey_roller->value(0);
  translatez_input->value(0); translatez_roller->value(0);
  scalex_input->value(1); scalex_roller->value(1);
  scaley_input->value(1); scaley_roller->value(1);
  scalez_input->value(1); scalez_roller->value(1);
  mainWindowPtr->freezeTransforms();
  mainWindowPtr->redraw();
  Fl :: focus(mainWindowPtr);
}

/*
  $Log: DLFLAppWindowCallbacks.cc,v $
  Revision 4.13  2004/01/20 22:18:36  vinod
  Added redraw after file read

  Revision 4.12  2004/01/20 09:30:29  vinod
  Added callbacks for column modeling, undo and redo

  Revision 4.11  2004/01/19 20:23:06  vinod
  Replaced calls to hide remeshing tiles by new subroutine which does the same

  Revision 4.10  2004/01/19 18:07:53  vinod
  Moved code related to remeshing to separate file

  Revision 4.9  2004/01/13 09:20:01  vinod
  Added controls and callbacks for scale factor
  for pentagon preserving remeshing scheme

  Revision 4.8  2004/01/13 02:50:30  vinod
  Added callbacks for:
  stellate and double stellate remeshing schemes
  double stellation, dodecahedral and icosahedral extrusions

  Revision 4.7  2004/01/12 20:10:17  vinod
  Removed unused callback

  Revision 4.6  2004/01/12 02:26:29  vinod
  Removed callbacks for unused controls

  Revision 4.5  2004/01/09 14:54:45  vinod
  Changed names of toggle button callbacks.

  Revision 4.4  2004/01/02 21:02:45  vinod
  Additions by Doug: Tiles, control widgets and callbacks for star, sqrt3 and fractal subdivision schemes

  Revision 4.3  2004/01/02 19:44:29  vinod
  Added collapse_edge mode callback

  Revision 4.2  2004/01/01 07:51:35  vinod
  Reorganized menus. Renamed some menu items

  Revision 4.1  2003/12/28 05:28:34  vinod
  Removed callback for second delete edge mode.
  Added callback for toggle button for delete edge cleanup flag.

  Revision 4.0  2003/12/26 01:58:52  vinod
  Major version sync.

  Revision 3.10  2003/12/26 01:48:56  vinod
  Combined roller and input callbacks. Reduced bloat.

  Revision 3.9  2003/12/07 19:18:25  vinod
  Added check button for crust cleanup

  Revision 3.8  2003/12/07 01:49:45  vinod
  Added check button and callback for dual accuracy flag

  Revision 3.7  2003/12/01 09:01:36  vinod
  Added options, controls and callbacks for translation and rotation

  Revision 3.6  2003/11/29 22:04:38  vinod
  Added control and callback for max. no. of connections
  for regular face connection operation

  Revision 3.5  2003/11/29 04:21:51  vinod
  Added check button controls and callbacks
  for symmetric hermitian handle weights

  Revision 3.4  2003/11/28 19:43:38  vinod
  Added controls and callbacks for setting max. # of connections for curved handles

  Revision 3.3  2003/11/23 22:45:07  vinod
  Added callbacks for menger sponge edge collapse threshold

  Revision 3.2  2003/11/21 15:27:13  vinod
  Added callbacks for fractional thickness toggle for menger sponge

  Revision 3.1  2003/11/19 16:10:45  vinod
  Added callbacks for sierpinsky and menger sponge controls

  Revision 3.0  2003/09/19 16:06:47  vinod
  Major version update

  Revision 2.25  2003/09/06 19:50:15  vinod
  Added callbacks for multi-face-handle algorithm choice menu

  Revision 2.24  2003/08/24 01:52:45  vinod
  Changed all sliders to rollers

  Revision 2.23  2003/07/07 16:13:07  vinod
  Added callback for use_max_offsets for multi-connect

  Revision 2.22  2003/04/25 15:47:44  vinod
  Added callbacks for multi-face handle operations

  Revision 2.21  2003/04/14 16:50:48  vinod
  Added callbacks for multi-face-handle creation

  Revision 2.20  2003/02/01 19:10:46  vinod
  Included proper boolean flags for different save options

  Revision 2.19  2003/02/01 19:04:22  vinod
  Added callbacks for save methods with multiple options

  Revision 2.18  2003/01/25 08:12:13  vinod
  Added subdivide_face callback method. Fixed visibility of dualMeshEdgesCheck_button

  Revision 2.17  2003/01/20 21:43:35  vinod
  Name change from skeleton modeling to wireframe modeling

  Revision 2.16  2003/01/05 21:10:49  vinod
  Added callbacks for skeleton modeling

  Revision 2.15  2002/12/27 06:31:40  vinod
  Bug fix in crust modeling callbacks. Boolean flags were incorrect

  Revision 2.14  2002/12/19 23:12:29  vinod
  Added check for null file name when loading textures
  Added call to valid(0) to make sure new texture gets
  initialized properly.

  Revision 2.13  2002/12/18 16:21:51  vinod
  Added face subdivision and tile texturing callbacks

  Revision 2.12  2002/12/17 03:39:11  vinod
  Added separate callbacks for stellation length controls. Fixed comment logs

  Revision 2.11  2002/12/17 03:18:37  vinod
  See comments for previous revision

  Revision 2.10  2002/12/17 03:12:03  vinod
  Additions by Paul
  Callbacks related to stellateFace, extrudeDualFace
  vertex cutting subdivide, pentagonal subdivide

  Revision 2.9  2002/12/17 02:44:59  vinod
  DLFLWindow.hh

  Revision 2.8  2002/11/07 08:20:42  vinod
  Added callback for check button for edge check flag for dooSabin subdivision

  Revision 2.7  2002/10/31 06:08:33  vinod
  Added callback for simplest subdivision scheme

  Revision 2.6  2002/10/30 04:44:40  vinod
  Changed file filter for load texture callback to include only jpg and png

  Revision 2.5  2002/10/30 04:38:09  vinod
  Added call to invalidate() for DLFLViewport when renderers are changed
  to make sure renderers are properly initialized

  Revision 2.4  2002/10/30 03:44:58  vinod
  Added callback to load textures

  Revision 2.3  2002/10/22 21:21:46  vinod
  Added callback for dual, changed name of renderer callbacks
  Added typecast for integer controls

  Revision 2.2  2002/10/17 02:21:04  vinod
  Callbacks are static. Now refer to static pointer to mainWindow

  Revision 2.1  2002/10/16 14:14:23  vinod
  Callbacks for DLFLAppWindow widgets

*/
