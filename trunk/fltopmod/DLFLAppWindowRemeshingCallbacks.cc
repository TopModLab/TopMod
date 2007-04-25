/* $Id: DLFLAppWindowRemeshingCallbacks.cc,v 4.5 2004/01/20 22:19:01 vinod Exp $ */

#include "DLFLAppWindow.hh"

// All remeshing related callbacks

void DLFLAppWindow :: remeshing_mode(Fl_Menu_*, void*)
{
  hide_modes();
  hide_tiles();
  remeshing->value(0);
  remeshing->show();
  blank->show();
}

void DLFLAppWindow :: hide_remeshing_tiles(Fl_Menu_*, void*)
{
     // Modify to hide only remeshing tiles
  generic_remeshing_tile->hide();
  doo_sabin_tile->hide();
  root_4_tile->hide();
  vertex_cutting_tile->hide();
  pentagonal_tile->hide();
  pentagonal2_tile->hide();
  dual_tile->hide();
  star_tile->hide(); // Doug
  fractal_tile->hide(); // Doug
  substellate2_tile->hide(); // Eric
  doo_sabin_bc_new_tile->hide(); // Bei & Cansin
  loopstyle_sub_tile->hide(); // Bei & Cansin
  dual1264_tile->hide(); // Bei & Cansin
  dome_sub_tile->hide(); // Bei & Cansin
}

void DLFLAppWindow :: dual(Fl_Menu_*, void*)
{
  hide_remeshing_tiles(0,0);
  remeshing_subgrp_panel->hide();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::Dual);
  dual_tile->show();
}

void DLFLAppWindow :: tri_conversion(Fl_Menu_*, void*)
{
  remeshing_subgrp_panel->show();
  hide_remeshing_tiles(0,0);
  tri_remeshing_menu->show();
  tri_remeshing_menu->value(0);
  quad_remeshing_menu->hide();
  pent_remeshing_menu->hide();
  tri_preserving_menu->hide();
  quad_preserving_menu->hide();
  pent_preserving_menu->hide();
  hex_preserving_menu->hide();
  misc_remeshing_menu->hide();
}

void DLFLAppWindow :: quad_conversion(Fl_Menu_*, void*)
{
  remeshing_subgrp_panel->show();
  hide_remeshing_tiles(0,0);
  tri_remeshing_menu->hide();
  quad_remeshing_menu->show();
  quad_remeshing_menu->value(0);
  pent_remeshing_menu->hide();
  tri_preserving_menu->hide();
  quad_preserving_menu->hide();
  pent_preserving_menu->hide();
  hex_preserving_menu->hide();
  misc_remeshing_menu->hide();
}

void DLFLAppWindow :: pent_conversion(Fl_Menu_*, void*)
{
  remeshing_subgrp_panel->show();
  hide_remeshing_tiles(0,0);
  tri_remeshing_menu->hide();
  quad_remeshing_menu->hide();
  pent_remeshing_menu->show();
  pent_remeshing_menu->value(0);
  tri_preserving_menu->hide();
  quad_preserving_menu->hide();
  pent_preserving_menu->hide();
  hex_preserving_menu->hide();
  misc_remeshing_menu->hide();
}

void DLFLAppWindow :: tri_preservation(Fl_Menu_*, void*)
{
  remeshing_subgrp_panel->show();
  hide_remeshing_tiles(0,0);
  tri_remeshing_menu->hide();
  quad_remeshing_menu->hide();
  pent_remeshing_menu->hide();
  tri_preserving_menu->show();
  tri_preserving_menu->value(0);
  quad_preserving_menu->hide();
  pent_preserving_menu->hide();
  hex_preserving_menu->hide();
  misc_remeshing_menu->hide();
}

void DLFLAppWindow :: quad_preservation(Fl_Menu_*, void*)
{
  remeshing_subgrp_panel->show();
  hide_remeshing_tiles(0,0);
  tri_remeshing_menu->hide();
  quad_remeshing_menu->hide();
  pent_remeshing_menu->hide();
  tri_preserving_menu->hide();
  quad_preserving_menu->show();
  quad_preserving_menu->value(0);
  pent_preserving_menu->hide();
  hex_preserving_menu->hide();
  misc_remeshing_menu->hide();
}

void DLFLAppWindow :: pent_preservation(Fl_Menu_*, void*)
{
  remeshing_subgrp_panel->show();
  hide_remeshing_tiles(0,0);
  tri_remeshing_menu->hide();
  quad_remeshing_menu->hide();
  pent_remeshing_menu->hide();
  tri_preserving_menu->hide();
  quad_preserving_menu->hide();
  pent_preserving_menu->show();
  pent_preserving_menu->value(0);
  hex_preserving_menu->hide();
  misc_remeshing_menu->hide();
}

void DLFLAppWindow :: hex_preservation(Fl_Menu_*, void*)
{
  remeshing_subgrp_panel->show();
  hide_remeshing_tiles(0,0);
  tri_remeshing_menu->hide();
  quad_remeshing_menu->hide();
  pent_remeshing_menu->hide();
  tri_preserving_menu->hide();
  quad_preserving_menu->hide();
  pent_preserving_menu->hide();
  hex_preserving_menu->show();
  hex_preserving_menu->value(0);
  misc_remeshing_menu->hide();
}

void DLFLAppWindow :: misc_remeshing(Fl_Menu_*, void*)
{
  remeshing_subgrp_panel->show();
  hide_remeshing_tiles(0,0);
  tri_remeshing_menu->hide();
  quad_remeshing_menu->hide();
  pent_remeshing_menu->hide();
  tri_preserving_menu->hide();
  quad_preserving_menu->hide();
  pent_preserving_menu->hide();
  hex_preserving_menu->hide();
  misc_remeshing_menu->show();
  misc_remeshing_menu->value(0);
}

void DLFLAppWindow :: perform_remeshing(Fl_Menu_*, void*)
{
     // Generic callback for all remeshing schemes
     // This simply calls the generic remeshing subrouting in DLFLWindow class
     // which uses the currently selected remeshing scheme to call the appropriate function
     // The scheme should be set when the corresponding option is selected from the menu
  mainWindowPtr->performRemeshing();
}

void DLFLAppWindow :: root3_subdivision(Fl_Menu_*, void*)
{
  hide_tiles();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::Root3);
  generic_remeshing_tile->show();
}

void DLFLAppWindow :: sqrt3_subdivision(Fl_Menu_*, void*) // Doug
{
  hide_tiles();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::DualVertexTrunc);
  generic_remeshing_tile->show();
}

void DLFLAppWindow :: global_stellation(Fl_Menu_*, void*)
{
  hide_tiles();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::GlobalStellate);
  generic_remeshing_tile->show();
}

void DLFLAppWindow :: star_subdivision(Fl_Menu_*, void*) // Doug
{
  hide_tiles();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::Star);
  star_tile->show();
}

void DLFLAppWindow :: generic1264_subdivision(Fl_Menu_*, void*)
{ 
  hide_tiles();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::Generic1264);
  dual1264_tile->show();
}

void DLFLAppWindow :: honeycomb_subdivision(Fl_Menu_*, void*)
{
  hide_tiles();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::Honeycomb);
  generic_remeshing_tile->show();
}

void DLFLAppWindow :: vertex_cutting_subdivision(Fl_Menu_*, void*)
{
  hide_tiles();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::VertexTrunc);
  vertex_cutting_tile->show();
}

void DLFLAppWindow :: dual_generic1264_subdivision(Fl_Menu_*, void*)
{
  hide_tiles();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::DualGeneric1264);
  dual1264_tile->show();
}

void DLFLAppWindow :: vertex_insertion_subdivision(Fl_Menu_*, void*)
{
  hide_tiles();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::LinearVertexInsertion);
  generic_remeshing_tile->show();
}

void DLFLAppWindow :: catmull_clark_subdivision(Fl_Menu_*, void*)
{
  hide_tiles();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::CatmullClark);
  generic_remeshing_tile->show();
}

void DLFLAppWindow :: stellate_subdivision(Fl_Menu_*, void*) // Eric
{
  hide_tiles();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::ModifiedStellate);
  generic_remeshing_tile->show();
}

void DLFLAppWindow :: doo_sabin_subdivision(Fl_Menu_*, void*)
{
  hide_tiles();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::DooSabin);
  doo_sabin_tile->show();
}

void DLFLAppWindow :: corner_cutting_subdivision(Fl_Menu_*, void*)
{
  hide_tiles();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::CornerCutting);
  generic_remeshing_tile->show();
}

void DLFLAppWindow :: simplest_subdivision(Fl_Menu_*, void*)
{
  hide_tiles();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::Simplest);
  generic_remeshing_tile->show();
}

void DLFLAppWindow :: pentagonal_subdivision(Fl_Menu_*, void*)
{
  hide_tiles();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::Pentagonal);
  pentagonal_tile->show();
}

void DLFLAppWindow :: cubic_pentagonal_subdivision(Fl_Menu_*, void*)
{
  hide_tiles();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::CubicPentagonal);
  pentagonal_tile->show();
}

void DLFLAppWindow :: dual_pentagonal_subdivision(Fl_Menu_*, void*)
{
  hide_tiles();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::DualPentagonal);
  generic_remeshing_tile->show();
}

void DLFLAppWindow :: loop_style_subdivision(Fl_Menu_*, void*) // Bei & Cansin
{
  hide_tiles();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::LoopStyle);
  loopstyle_sub_tile->show();
}

void DLFLAppWindow :: loop_subdivision(Fl_Menu_*, void*)
{
  hide_tiles();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::Loop);
  generic_remeshing_tile->show();
}

void DLFLAppWindow :: dual_loop_subdivision(Fl_Menu_*, void*)
{
  hide_tiles();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::DualLoop);
  generic_remeshing_tile->show();
}

void DLFLAppWindow :: root_4_subdivision(Fl_Menu_*, void*)
{
  hide_tiles();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::Root4);
  root_4_tile->show();
}

void DLFLAppWindow :: global_extrusion(Fl_Menu_*, void*)
{
  hide_tiles();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::GlobalCubicExtrude);
  generic_remeshing_tile->show();
}

void DLFLAppWindow :: checkerboard_remeshing(Fl_Menu_*, void*)
{
  hide_tiles();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::CheckerBoard);
  generic_remeshing_tile->show();
}

void DLFLAppWindow :: dual_global_extrusion(Fl_Menu_*, void*)
{
  hide_tiles();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::DualGlobalCubicExtrude);
  generic_remeshing_tile->show();
}

void DLFLAppWindow :: dual_checkerboard_remeshing(Fl_Menu_*, void*)
{
  hide_tiles();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::DualCheckerBoard);
  generic_remeshing_tile->show();
}

void DLFLAppWindow :: pentagonal2_subdivision(Fl_Menu_*, void*)
{
  hide_tiles();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::PentagonPreserving);
  pentagonal2_tile->show();
}

void DLFLAppWindow :: dual_pentagonal2_subdivision(Fl_Menu_*, void*)
{
  hide_tiles();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::DualPentagonPreserving);
  generic_remeshing_tile->show();
}

void DLFLAppWindow :: hexagon_preserving_remeshing(Fl_Menu_*, void*)
{
  hide_tiles();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::HexagonPreserving);
  generic_remeshing_tile->show();
}

void DLFLAppWindow :: dual_hexagon_preserving_remeshing(Fl_Menu_*, void*)
{
  hide_tiles();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::DualHexagonPreserving);
  generic_remeshing_tile->show();
}

void DLFLAppWindow :: fractal_subdivision(Fl_Menu_*, void*) // Doug
{
  hide_tiles();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::Fractal);
  fractal_tile->show();
}

void DLFLAppWindow :: twostellate_subdivision(Fl_Menu_*, void*) // Eric
{
  hide_tiles();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::ModifiedDoubleStellate);
  substellate2_tile->show();
}

void DLFLAppWindow :: dome_subdivision(Fl_Menu_*, void*) // Bei & Cansin
{
  hide_tiles();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::Dome);
  dome_sub_tile->show();
}

void DLFLAppWindow :: dual1264_subdivision(Fl_Menu_*, void*) // Bei & Cansin
{
  hide_tiles();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::DualGeneric1264);
  dual1264_tile->show();
}

void DLFLAppWindow :: doo_sabin_subdivision_bc(Fl_Menu_*, void*) // Bei & Cansin
{
  hide_tiles();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::DooSabinBC);
  generic_remeshing_tile->show();
}

void DLFLAppWindow :: doo_sabin_subdivision_bc_new(Fl_Menu_*, void*) // Bei & Cansin
{
  hide_tiles();
  mainWindowPtr->setRemeshingScheme(DLFLWindow::DooSabinBCNew);
  doo_sabin_bc_new_tile->show();
}

void DLFLAppWindow :: subdivide_face_mode(Fl_Menu_*, void*)
{
  mainWindowPtr->setMode(DLFLWindow::SubDivideFace);
  hide_tiles();
  subdivide_face_tile->show();
}

void DLFLAppWindow :: subdivide_face(Fl_Menu_*, void*)
{
  mainWindowPtr->subDivideSelectedFaces();
  mainWindowPtr->redraw();
}

// Controls for remeshing schemes
void DLFLAppWindow :: toggleDooSabinEdgeFlag(Fl_Widget * widget, void * data)
{
  dooSabinEdgeCheck_button = (Fl_Check_Button *)widget;
  DLFLWindow::doo_sabin_check = bool(dooSabinEdgeCheck_button->value());
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: changeRoot4Twist(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  DLFLWindow::twist_factor = v->value();
  root4Twist_input->value(DLFLWindow::twist_factor);
  root4Twist_roller->value(DLFLWindow::twist_factor);
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: changeRoot4Weight(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  DLFLWindow::weight_factor = v->value();
  root4Weight_roller->value(DLFLWindow::weight_factor);
  root4Weight_input->value(DLFLWindow::weight_factor);
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: changeVertexCuttingOffset(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  DLFLWindow::vertex_cutting_offset = v->value();
  vertexCuttingOffset_roller->value(DLFLWindow::vertex_cutting_offset);
  vertexCuttingOffset_input->value(DLFLWindow::vertex_cutting_offset);
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: changePentagonalOffset(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  DLFLWindow::pentagonal_offset = v->value();
  pentagonalOffset_roller->value(DLFLWindow::pentagonal_offset);
  pentagonalOffset_input->value(DLFLWindow::pentagonal_offset);
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: changePentagonalScaleFactor(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  DLFLWindow::pentagonal_scale = v->value();
  pentagonalScaleFactor_roller->value(DLFLWindow::pentagonal_scale);
  pentagonalScaleFactor_input->value(DLFLWindow::pentagonal_scale);
  Fl :: focus(mainWindowPtr);
}

// Added by Doug
void DLFLAppWindow :: changeStarOffset(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  DLFLWindow::star_offset = v->value();
  starOffset_roller->value(DLFLWindow::star_offset);
  starOffset_input->value(DLFLWindow::star_offset);
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: changeFractalOffset(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  DLFLWindow::fractal_offset = v->value();
  fractalOffset_roller->value(DLFLWindow::fractal_offset);
  fractalOffset_input->value(DLFLWindow::fractal_offset);
  Fl :: focus(mainWindowPtr);
}

// End additions by Doug

// Added by Eric
void DLFLAppWindow :: changeSubStellateAmount(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  DLFLWindow::substellate_height = v->value();
  substellate_amount_roller->value(DLFLWindow::substellate_height);
  substellate_amount_input->value(DLFLWindow::substellate_height);
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow :: changeSubStellateCurve(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  DLFLWindow::substellate_curve = v->value();
  substellate_curve_roller->value(DLFLWindow::substellate_curve);
  substellate_curve_input->value(DLFLWindow::substellate_curve);
  Fl :: focus(mainWindowPtr);
}

// End additions by Eric

void DLFLAppWindow :: toggleAccurateDualFlag(Fl_Widget * widget, void * data)
{
  accurateDualCheck_button = (Fl_Check_Button *)widget;
  DLFLWindow::accurate_dual = !bool(accurateDualCheck_button->value());
  Fl :: focus(mainWindowPtr);
}


// Added by Bei & Cansin

void DLFLAppWindow ::changeDomeSubdivisionLength(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  DLFLWindow::domeLength_factor = v->value();
  domeSubdivisionLength_roller->value(DLFLWindow::domeLength_factor);
  domeSubdivisionLength_input->value(DLFLWindow::domeLength_factor);
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow ::changeDomeSubdivisionScale(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  DLFLWindow::domeScale_factor = v->value();
  domeSubdivisionScale_roller->value(DLFLWindow::domeScale_factor);
  domeSubdivisionScale_input->value(DLFLWindow::domeScale_factor);
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow ::changeDual1264Scale(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  DLFLWindow::dual1264_scale_factor = v->value();
  dual1264Scale_roller->value(DLFLWindow::dual1264_scale_factor);
  dual1264Scale_input->value(DLFLWindow::dual1264_scale_factor);
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow ::changeDooSabinBCnewScale(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  DLFLWindow::dooSabinBCnewScale_factor = v->value();
  dooSabinBCnewScale_roller->value(DLFLWindow::dooSabinBCnewScale_factor);
  dooSabinBCnewScale_input->value(DLFLWindow::dooSabinBCnewScale_factor);
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow ::changeDooSabinBCnewLength(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  DLFLWindow::dooSabinBCnewLength_factor = v->value();
  dooSabinBCnewLength_roller->value(DLFLWindow::dooSabinBCnewLength_factor);
  dooSabinBCnewLength_input->value(DLFLWindow::dooSabinBCnewLength_factor);
  Fl :: focus(mainWindowPtr);
}

void DLFLAppWindow ::changeLoopStyleSubdivisionLength(Fl_Widget * widget, void * data)
{
  Fl_Valuator * v = (Fl_Valuator *)widget;
  DLFLWindow::loopLength_factor = v->value();
  loopStyleSubdivisionLength_roller->value(DLFLWindow::loopLength_factor);
  loopStyleSubdivisionLength_input->value(DLFLWindow::loopLength_factor);
  Fl :: focus(mainWindowPtr);
}

// End additions by Bei & Cansin

/*
  $Log: DLFLAppWindowRemeshingCallbacks.cc,v $
  Revision 4.5  2004/01/20 22:19:01  vinod
  Added Loop and Dual Loop subdivision schemes

  Revision 4.4  2004/01/20 07:26:28  vinod
  Changed name of subhw to dual1264

  Revision 4.3  2004/01/20 01:01:30  vinod
  Renamed some remeshing scheme callbacks.

  Revision 4.2  2004/01/19 20:24:45  vinod
  Added hide_remeshing_tiles method.
  Added callbacks for schemes by Bei & Cansin

  Revision 4.1  2004/01/19 18:09:11  vinod
  Code for callbacks related to remeshing. Extracted from DLFLAppWindowCallbacks.cc and modified and extended.

*/
