/* $Id: DLFLAppWindow.cc,v 4.15 2004/01/20 17:17:16 vinod Exp $ */

// Source code for DLFLAppWindow class

#include "DLFLAppWindow.hh"

int DLFLAppWindow::MIN_W = 1200;                        // Minimum width for app window
int DLFLAppWindow::MIN_H = 630;                        // Minimum height for app window
int DLFLAppWindow::OPT_W = 250;                        // Options panel width
int DLFLAppWindow::MODE_TILE_H = 130;                  // Mode tile height
int DLFLAppWindow::SUBGRP_TILE_H = 100;                 // Remeshing sub-group tile height
int DLFLAppWindow::MENU_H = 30;                        // Menubar height
int DLFLAppWindow::STATUS_H = 30;                      // Status window height

ShadedRendererPtr DLFLAppWindow::shaded;                // ShadedRenderer
NormalRendererPtr DLFLAppWindow::normal;                // NormalRenderer
LitRendererPtr DLFLAppWindow::lit;                        // LitRenderer
TexturedRendererPtr DLFLAppWindow::textured;          // TexturedRenderer
TexturedLitRendererPtr DLFLAppWindow::texturedlit; // TexturedLitRenderer
PatchRendererPtr DLFLAppWindow::patch;		       // PatchRenderer

Fl_Menu_Bar * DLFLAppWindow::menubar;
Fl_Choice * DLFLAppWindow::basics;
Fl_Choice * DLFLAppWindow::extrude;
Fl_Choice * DLFLAppWindow::crust;
Fl_Choice * DLFLAppWindow::remeshing;
Fl_Choice * DLFLAppWindow::handles;
Fl_Choice * DLFLAppWindow::highgenus;
Fl_Choice * DLFLAppWindow::texturing;

Fl_Choice * DLFLAppWindow::mfh_algo_menu;

Fl_Group * DLFLAppWindow::control_panel;
Fl_Group * DLFLAppWindow::mode_panel;
Fl_Group * DLFLAppWindow::roller_panel;
Fl_Group * DLFLAppWindow::remeshing_subgrp_panel;
Fl_Group * DLFLAppWindow::mode_sel_tile;
Fl_Group * DLFLAppWindow::blank;

Fl_Group * DLFLAppWindow::delete_edge_tile;
Fl_Group * DLFLAppWindow::subdivide_edge_tile;
Fl_Group * DLFLAppWindow::subdivide_face_tile;
Fl_Group * DLFLAppWindow::tile_texturing_tile;
Fl_Group * DLFLAppWindow::transforms_tile;
Fl_Group * DLFLAppWindow::extrude_tile;
Fl_Group * DLFLAppWindow::stellate_tile;
Fl_Group * DLFLAppWindow::crust_modeling1_tile;
Fl_Group * DLFLAppWindow::crust_modeling2_tile;
Fl_Group * DLFLAppWindow::wireframe_modeling_tile;
Fl_Group * DLFLAppWindow::column_modeling_tile;

Fl_Group * DLFLAppWindow::hole_tile;
Fl_Group * DLFLAppWindow::handle_tile;
Fl_Group * DLFLAppWindow::multi_face_handle_tile;
Fl_Group * DLFLAppWindow::sierpinsky_tile;
Fl_Group * DLFLAppWindow::mengersponge_tile;

Fl_Value_Input * DLFLAppWindow::numSubDivs_input;
Fl_Value_Input * DLFLAppWindow::tileTexNum_input;
Fl_Value_Input * DLFLAppWindow::extrudeLength_input;
Fl_Value_Input * DLFLAppWindow::extrudeRotation_input;
Fl_Value_Input * DLFLAppWindow::extrudeScale_input;
Fl_Value_Input * DLFLAppWindow::numExtrusions_input;
Fl_Value_Input * DLFLAppWindow::stellateLength_input;
Fl_Value_Input * DLFLAppWindow::numSegments_input;
Fl_Value_Input * DLFLAppWindow::maxSegments_input;
Fl_Value_Input * DLFLAppWindow::numSegments2_input;
Fl_Value_Input * DLFLAppWindow::maxSegments2_input;
Fl_Value_Input * DLFLAppWindow::weight1_input;
Fl_Value_Input * DLFLAppWindow::weight2_input;
Fl_Value_Input * DLFLAppWindow::extraTwists_input;
Fl_Value_Input * DLFLAppWindow::multiFaceHandleScaleFactor_input;
Fl_Value_Input * DLFLAppWindow::multiFaceHandleExtrudeDist_input;
Fl_Value_Input * DLFLAppWindow::crustScaleFactor_input;
Fl_Value_Input * DLFLAppWindow::crustThickness_input;
Fl_Value_Input * DLFLAppWindow::wireframeThickness_input;
Fl_Value_Input * DLFLAppWindow::columnThickness_input;
Fl_Value_Input * DLFLAppWindow::columnSegments_input;
Fl_Value_Input * DLFLAppWindow::root4Twist_input;
Fl_Value_Input * DLFLAppWindow::root4Weight_input;
Fl_Value_Input * DLFLAppWindow::vertexCuttingOffset_input;
Fl_Value_Input * DLFLAppWindow::pentagonalOffset_input;
Fl_Value_Input * DLFLAppWindow::pentagonalScaleFactor_input;
Fl_Value_Input * DLFLAppWindow::starOffset_input; // Doug
Fl_Value_Input * DLFLAppWindow::fractalOffset_input; // Doug
Fl_Value_Input * DLFLAppWindow::substellate_amount_input; // Eric
Fl_Value_Input * DLFLAppWindow::substellate_curve_input; // Eric
Fl_Value_Input * DLFLAppWindow::mengerSpongeThickness_input;
Fl_Value_Input * DLFLAppWindow::mengerSpongeCollapseThreshold_input;
Fl_Value_Input * DLFLAppWindow::translatex_input;
Fl_Value_Input * DLFLAppWindow::translatey_input;
Fl_Value_Input * DLFLAppWindow::translatez_input;
Fl_Value_Input * DLFLAppWindow::scalex_input;
Fl_Value_Input * DLFLAppWindow::scaley_input;
Fl_Value_Input * DLFLAppWindow::scalez_input;

Fl_Roller * DLFLAppWindow::numSubDivs_roller;
Fl_Roller * DLFLAppWindow::tileTexNum_roller;
Fl_Roller * DLFLAppWindow::extrudeLength_roller;
Fl_Roller * DLFLAppWindow::extrudeRotation_roller;
Fl_Roller * DLFLAppWindow::extrudeScale_roller;
Fl_Roller * DLFLAppWindow::numExtrusions_roller;
Fl_Roller * DLFLAppWindow::stellateLength_roller;
Fl_Roller * DLFLAppWindow::numSegments_roller;
Fl_Roller * DLFLAppWindow::maxSegments_roller;
Fl_Roller * DLFLAppWindow::numSegments2_roller;
Fl_Roller * DLFLAppWindow::maxSegments2_roller;
Fl_Roller * DLFLAppWindow::weight1_roller;
Fl_Roller * DLFLAppWindow::weight2_roller;
Fl_Roller * DLFLAppWindow::extraTwists_roller;
Fl_Roller * DLFLAppWindow::multiFaceHandleScaleFactor_roller;
Fl_Roller * DLFLAppWindow::multiFaceHandleExtrudeDist_roller;
Fl_Roller * DLFLAppWindow::crustScaleFactor_roller;
Fl_Roller * DLFLAppWindow::crustThickness_roller;
Fl_Roller * DLFLAppWindow::wireframeThickness_roller;
Fl_Roller * DLFLAppWindow::columnThickness_roller;
Fl_Roller * DLFLAppWindow::columnSegments_roller;
Fl_Roller * DLFLAppWindow::root4Twist_roller;
Fl_Roller * DLFLAppWindow::root4Weight_roller;
Fl_Roller * DLFLAppWindow::vertexCuttingOffset_roller;
Fl_Roller * DLFLAppWindow::pentagonalOffset_roller;
Fl_Roller * DLFLAppWindow::pentagonalScaleFactor_roller;
Fl_Roller * DLFLAppWindow::starOffset_roller; // Doug
Fl_Roller * DLFLAppWindow::fractalOffset_roller; // Doug
Fl_Roller * DLFLAppWindow::substellate_amount_roller; // Eric
Fl_Roller * DLFLAppWindow::substellate_curve_roller; // Eric
Fl_Roller * DLFLAppWindow::mengerSpongeThickness_roller;
Fl_Roller * DLFLAppWindow::mengerSpongeCollapseThreshold_roller;
Fl_Roller * DLFLAppWindow::translatex_roller;
Fl_Roller * DLFLAppWindow::translatey_roller;
Fl_Roller * DLFLAppWindow::translatez_roller;
Fl_Roller * DLFLAppWindow::scalex_roller;
Fl_Roller * DLFLAppWindow::scaley_roller;
Fl_Roller * DLFLAppWindow::scalez_roller;

Fl_Check_Button * DLFLAppWindow::deleteEdgeCleanupCheck_button;
Fl_Check_Button * DLFLAppWindow::symmetricWeightsCheck_button;
Fl_Check_Button * DLFLAppWindow::dualMeshEdgesCheck_button;
Fl_Check_Button * DLFLAppWindow::dooSabinEdgeCheck_button;
Fl_Check_Button * DLFLAppWindow::accurateDualCheck_button;
Fl_Check_Button * DLFLAppWindow::useQuadsCheck_button;
Fl_Check_Button * DLFLAppWindow::multiFaceHandleUseMaxOffsetCheck_button;
Fl_Check_Button * DLFLAppWindow::mengerSpongeFractionalThicknessCheck_button;
Fl_Check_Button * DLFLAppWindow::crustCleanupCheck_button1;
Fl_Check_Button * DLFLAppWindow::crustCleanupCheck_button2;
Fl_Check_Button * DLFLAppWindow::hexagonalizeDodecaExtrudeCheck_button;

DLFLWindowPtr DLFLAppWindow::mainWindowPtr;            // Pointer to the main window


static Fl_Choice * createPullDownMenu(int x, int y, int w, int h,
                                      const char * name,
                                      const Fl_Menu_Item * items)
{
  Fl_Choice * pdmenu = new Fl_Choice(x, y, w, h, name);
  pdmenu->align(FL_ALIGN_TOP_LEFT);
  pdmenu->color(50);
  pdmenu->selection_color(50);
  pdmenu->copy(items); // Force array copy
  pdmenu->hide();
  return pdmenu;
}

static Fl_Check_Button * createCheckButton(int x, int y, int w, int h,
                                           const char * name,
                                           bool value,
                                           Fl_Callback * callback)
{
  Fl_Check_Button * btn = new Fl_Check_Button(x, y, w, h, name);
  btn->align(FL_ALIGN_RIGHT); btn->when(FL_WHEN_RELEASE);
  btn->value(value); btn->callback(callback);
  return btn;
}

static Fl_Value_Input * createValueInput(int x, int y, int w, int h,
                                         double min, double max, double step,
                                         double value,
                                         Fl_Callback * callback)
{
  Fl_Value_Input * valueinput = new Fl_Value_Input(x, y, w, h);
  valueinput->minimum(min); valueinput->maximum(max); valueinput->step(step);
  valueinput->value(value);
  valueinput->callback(callback);
  return valueinput;
}

static Fl_Roller * createRoller(int x, int y, int w, int h,
                                const char * name,
                                double min, double max, double step,
                                double value,
                                Fl_Callback * callback)
{
  Fl_Roller * roller = new Fl_Roller(x, y, w, h, name);
  roller->type(FL_HORIZONTAL); roller->align(FL_ALIGN_TOP_LEFT);
  roller->minimum(min); roller->maximum(max); roller->step(step);
  roller->value(value);
  roller->callback(callback);
  return roller;
}

static void createNumericControls(int x, int y, int w, int h,
                                  const char * name,
                                  double min, double max, double step,
                                  double value,
                                  Fl_Callback * callback,
                                  Fl_Value_Input *& valueinput,
                                  Fl_Roller *& roller)
{
  valueinput = createValueInput(x+5,y,50,25,min,max,step,value,callback);
  roller = createRoller(x+65,y,165,25,name,min,max,step,value,callback);
}

void DLFLAppWindow :: createModeSelectionPanel(int x, int y, int w, int h)
{
     // Create mode selection panel, which is part of the control panel
     // x,y specifies the left top corner of the Fl_Group widget
     // w and h are the width and height of the Fl_Group widget respectively.
  mode_panel = new Fl_Group(x+1,y+1,w-2,h-1);
  mode_panel->box(FL_EMBOSSED_BOX);

     // Add various pull-down menus to the mode panel
  {
    Fl_Menu_Item mode_menu [] =
      {
        {"Select Mode", 0, (Fl_Callback*)normal_mode, 0, FL_MENU_DIVIDER},
        {"Basics", 0,(Fl_Callback*)basics_mode, 0, 0},
        {"Extrusions", 0,(Fl_Callback*)extrude_mode, 0, FL_MENU_DIVIDER},
        {"Remeshing", 0,(Fl_Callback*)remeshing_mode, 0, 0},
        {"High Genus Modeling", 0,(Fl_Callback*)highgenus_mode, 0, 0},
        {"Texturing", 0,(Fl_Callback*)texturing_mode, 0, 0},
        {0}
      };
    Fl_Choice* modes = createPullDownMenu(x+10, 55, w-20, 25, "Group", mode_menu);
    modes->show();
  }
  {
    Fl_Menu_Item basics_menu [] =
      {
        {"Select Basic Operation", 0, (Fl_Callback*)exit_modes, 0, FL_MENU_DIVIDER},
        {"Insert Edge", 0, (Fl_Callback*)insert_edge_mode, 0, 0},
        {"Delete Edge", 0, (Fl_Callback*)delete_edge_mode, 0, 0},
        {"Collapse Edge", 0, (Fl_Callback*)collapse_edge_mode, 0, 0},
        {"Subdivide Edge", 0, (Fl_Callback*)subdivide_edge_mode, 0, 0},
        {"Connect Edges", 0, (Fl_Callback*)connect_edge_mode, 0, 0},
        {"Splice Corners", 0, (Fl_Callback*)splice_corners_mode, 0, 0},
#ifndef DLFL_RELEASE
        {"Transforms", 0, (Fl_Callback*)transforms_mode, 0, 0},
#endif /* DLFL_RELEASE */
        {0}
      };
    basics = createPullDownMenu(x+10, 110, w-20, 25, "Basics", basics_menu);
  }
  {
    Fl_Menu_Item menuitems [] =
      {
        {"Select Extrusion", 0, (Fl_Callback*)exit_modes, 0, FL_MENU_DIVIDER},
#ifndef DLFL_RELEASE
        {"Extrude Face (Doo-Sabin)", 0, (Fl_Callback*)extrude_face_doosabin, 0, 0},
#endif /* DLFL_RELEASE */
        {"Cubical Extrude", 0, (Fl_Callback*)extrude_face, 0, 0},
        {"Dodecahedral Extrude", 0, (Fl_Callback*)extrude_face_dodeca, 0, 0},
        {"Icosahedral Extrude", 0, (Fl_Callback*)extrude_face_icosa, 0, 0},
        {"Octahedral Extrude", 0, (Fl_Callback*)extrude_dual_face, 0, FL_MENU_DIVIDER},
        {"Stellate (Tetrahedral Extrude)", 0, (Fl_Callback*)stellate_face, 0, 0},
        {"Double Stellate", 0, (Fl_Callback*)double_stellate_face, 0, 0},
        {0}
      };
    extrude = createPullDownMenu(x+10, 110, w-20, 25, "Extrude", menuitems);
  }
  {
    Fl_Menu_Item menuitems [] =
      {
        {"Select Remeshing Scheme", 0, (Fl_Callback*)exit_modes, 0, FL_MENU_DIVIDER},
        {"Dual", 0, (Fl_Callback*)dual, 0, FL_MENU_DIVIDER},
        {"Conversion Schemes", 0, 0, 0, FL_MENU_INACTIVE},
        {"3-Conversion Schemes", 0, (Fl_Callback*)tri_conversion, 0, 0},
        {"4-Conversion Schemes", 0, (Fl_Callback*)quad_conversion, 0, 0},
        {"5-Conversion Schemes", 0, (Fl_Callback*)pent_conversion, 0, FL_MENU_DIVIDER},
        {"Preservation Schemes", 0, 0, 0, FL_MENU_INACTIVE},
        {"3-Preservation Schemes", 0, (Fl_Callback*)tri_preservation, 0, 0},
        {"4-Preservation Schemes", 0, (Fl_Callback*)quad_preservation, 0, 0},
        {"5-Preservation Schemes", 0, (Fl_Callback*)pent_preservation, 0, 0},
        {"6-Preservation Schemes", 0, (Fl_Callback*)hex_preservation, 0, FL_MENU_DIVIDER},
        {"Miscellaneous Schemes", 0, (Fl_Callback*)misc_remeshing, 0, 0},
        {0}
      };
    menuitems[2].labelcolor(FL_BLUE); menuitems[2].labelfont(FL_HELVETICA_BOLD);
    menuitems[6].labelcolor(FL_BLUE); menuitems[6].labelfont(FL_HELVETICA_BOLD);
    remeshing = createPullDownMenu(x+10, 110, w-20, 25, "Remeshing", menuitems);
  }
  {
    Fl_Menu_Item menuitems [] =
      {
        {"Select High-Genus Operation", 0, (Fl_Callback*)exit_modes, 0, FL_MENU_DIVIDER},
        {"Interactive Operations", 0, 0, 0, FL_MENU_INACTIVE},
        {"Add Hole/Handle", 0, (Fl_Callback*)add_hole_mode1, 0, 0},
        {"Add Hole/Handle (closest vertex)", 0, (Fl_Callback*)add_hole_mode2, 0, 0},
        {"Add Handle (shape interpolation)", 0, (Fl_Callback*)add_handle_mode, 0, FL_MENU_DIVIDER},
        {"Semi-Automatic Operations", 0, 0, 0, FL_MENU_INACTIVE},
        {"Rind Modeling (Scaling)", 0, (Fl_Callback*)crust_modeling_mode1, 0, 0},
        {"Rind Modeling (Thickness)", 0, (Fl_Callback*)crust_modeling_mode2, 0, FL_MENU_DIVIDER},
        {"Automatic Operations", 0, 0, 0, FL_MENU_INACTIVE},
        {"Wireframe Modeling", 0, (Fl_Callback*)wireframe_modeling_mode, 0, 0},
        {"Column Modeling", 0, (Fl_Callback*)column_modeling_mode, 0, 0},
        {"Sierpinsky", 0, (Fl_Callback*)sierpinsky_mode, 0, FL_MENU_DIVIDER},
        {"Experimental Operations", 0, 0, 0, FL_MENU_INACTIVE},
        {"Create Multi-face Handle", 0, (Fl_Callback*)multi_face_handle_mode, 0, 0},
        {"Menger Sponge", 0, (Fl_Callback*)mengersponge_mode, 0, 0},
        {0}
      };
    menuitems[1].labelcolor(FL_BLUE); menuitems[1].labelfont(FL_HELVETICA_BOLD);
    menuitems[5].labelcolor(FL_BLUE); menuitems[5].labelfont(FL_HELVETICA_BOLD);
    menuitems[8].labelcolor(FL_BLUE); menuitems[8].labelfont(FL_HELVETICA_BOLD);
    menuitems[12].labelcolor(FL_BLUE); menuitems[12].labelfont(FL_HELVETICA_BOLD);
    highgenus = createPullDownMenu(x+10, 110, w-20, 25, "High Genus Modeling", menuitems);
  }
  {
    Fl_Menu_Item texturing_menu [] =
      {
        {"Select Texturing Operation", 0, (Fl_Callback*)exit_modes, 0, FL_MENU_DIVIDER},
        {"Tile Texturing", 0, (Fl_Callback*)tile_texturing_mode, 0, 0},
        {0}
      };
    texturing = createPullDownMenu(x+10, 110, w-20, 25, "Texturing",texturing_menu);
  }
  mode_panel->end();
}

void DLFLAppWindow :: createBasicModeTiles(int x, int y, int w, int h)
{
     // Create tiles for the Basic mode
     // Each tile is created as a new group
     // This function should be called right after creating a new group
     // for the outer panel

     // Delete edge tile
  {
    Fl_Group* o = delete_edge_tile = new Fl_Group(x, y, w, h);
    o->color(48); o->selection_color(48);
    o->resizable(NULL);
    {
      Fl_Group* o = new Fl_Group(x+10, y+25, w-20, 50);
      deleteEdgeCleanupCheck_button = createCheckButton(x+15,y+25,20,25,
                                                        "Cleanup",
                                                        DLFLWindow::delete_edge_cleanup,
                                                        toggleDeleteEdgeCleanupFlag);
      o->end();
    }
    o->end(); o->hide();
  }
  
     // Edge subdivision tile
  {
    Fl_Group* o = subdivide_edge_tile = new Fl_Group(x, y, w, h);
    o->color(48); o->selection_color(48);
    o->resizable(NULL);
    {
      Fl_Group* o = new Fl_Group(x+10, y+25, w-20, 50);
      {
        numSubDivs_input = createValueInput(x+15, y+25, 50, 25,
                                            1,10,1,
                                            DLFLWindow::num_e_subdivs,
                                            changeNumSubDivs);
        numSubDivs_roller = createRoller(x+75, y+25, 165, 25,
                                         "Number of Subdivisions:",
                                         1,10,1,
                                         DLFLWindow::num_e_subdivs,
                                         changeNumSubDivs);
      }
      o->end();
    }
    o->end(); o->hide();
  }

     // Transforms tile
  {
    Fl_Group* o = transforms_tile = new Fl_Group(x, y, w, h);
    o->color(48); o->selection_color(48);
    o->resizable(NULL);
    {
      Fl_Group* o = new Fl_Group(x+10, y+25, w-20, 50);
      {
        translatex_input = createValueInput(x+15, y+25, 50, 25,
                                            -100,100,0.5,
                                            0.0,
                                            translatex);
        translatex_roller = createRoller(x+75, y+25, 165, 25,
                                         "X Position:",
                                         -100,100,0.5,
                                         0.0,
                                         translatex);
      }
      o->end();
    }
    {
      Fl_Group* o = new Fl_Group(x+10, y+90, w-20, 50);
      {
        translatey_input = createValueInput(x+15, y+90, 50, 25,
                                            -100,100,0.5,
                                            0.0,
                                            translatey);
        translatey_roller = createRoller(x+75, y+90, 165, 25,
                                         "Y Position:",
                                         -100,100,0.5,
                                         0.0,
                                         translatey);
      }
      o->end();
    }
    {
      Fl_Group* o = new Fl_Group(x+10, y+155, w-20, 50);
      {
        translatez_input = createValueInput(x+15, y+155, 50, 25,
                                            -100,100,0.5,
                                            0.0,
                                            translatez);
        translatez_roller = createRoller(x+75, y+155, 165, 25,
                                         "Z Position:",
                                         -100,100,0.5,
                                         0.0,
                                         translatez);
      }
      o->end();
    }
    {
      Fl_Group* o = new Fl_Group(x+10, y+220, w-20, 50);
      {
        scalex_input = createValueInput(x+15, y+220, 50, 25,
                                        0.1,10,0.1,
                                        1.0,
                                        scalex);
        scalex_roller = createRoller(x+75, y+220, 165, 25,
                                     "X Scale Factor:",
                                     0.1,10,0.1,
                                     1.0,
                                     scalex);
      }
      o->end();
    }
    {
      Fl_Group* o = new Fl_Group(x+10, y+285, w-20, 50);
      {
        scaley_input = createValueInput(x+15, y+285, 50, 25,
                                        0.1,10,0.1,
                                        1.0,
                                        scaley);
        scaley_roller = createRoller(x+75, y+285, 165, 25,
                                     "Y Scale Factor:",
                                     0.1,10,0.1,
                                     1.0,
                                     scaley);
      }
      o->end();
    }
    {
      Fl_Group* o = new Fl_Group(x+10, y+350, w-20, 50);
      {
        scalez_input = createValueInput(x+15, y+350, 50, 25,
                                        0.1,10,0.1,
                                        1.0,
                                        scalez);
        scalez_roller = createRoller(x+75, y+350, 165, 25,
                                     "Z Scale Factor:",
                                     0.1,10,0.1,
                                     1.0,
                                     scalez);
      }
      o->end();
    }
    {
      Fl_Button* o = new Fl_Button((w-140)/2, y+415, 140, 35, "Freeze Transforms");
      o->callback((Fl_Callback*)freeze_transforms);
    }
    o->end(); o->hide();
  }
}

void DLFLAppWindow :: createExtrusionModeTiles(int x, int y, int w, int h)
{
     // Extrusions tile
  {
    Fl_Group* o = extrude_tile = new Fl_Group(x, y, w, h);
    o->color(48); o->selection_color(48);
    o->resizable(NULL);
    {
      Fl_Group* o = new Fl_Group(x+10, y+25, w-20, 50);
      {
        extrudeLength_input = createValueInput(x+15, y+25, 50, 25,
                                               -100,100,0.1,
                                               DLFLWindow::extrude_dist,
                                               changeExtrudeLength);
        extrudeLength_roller = createRoller(x+75, y+25, 165, 25,
                                            "Length:",
                                            -100,100,0.1,
                                            DLFLWindow::extrude_dist,
                                            changeExtrudeLength);
      }
      o->end();
    }
    {
      Fl_Group* o = new Fl_Group(x+10, y+90, w-20, 50);
      {
        extrudeRotation_input= createValueInput(x+15, y+90, 50, 25,
                                                0.0,360.0,1.0,
                                                DLFLWindow::extrude_rot,
                                                changeExtrudeRotation);
        extrudeRotation_roller = createRoller(x+75, y+90, 165, 25,
                                              "Rotation:",
                                              0.0,360.0,1.0,
                                              DLFLWindow::extrude_rot,
                                              changeExtrudeRotation);
      }
      o->end();
    }
    {
      Fl_Group* o = new Fl_Group(x+10, y+155, w-20, 50);
      {
        extrudeScale_input = createValueInput(x+15, y+155, 50, 25,
                                              0.0, 10.0, 0.01,
                                              DLFLWindow::extrude_scale,
                                              changeExtrudeScale);
        extrudeScale_roller = createRoller(x+75, y+155, 165, 25,
                                           "Scale:",
                                           0.0, 10.0, 0.01,
                                           DLFLWindow::extrude_scale,
                                           changeExtrudeScale);
      }
      o->end();
    }
    {
      Fl_Group* o = new Fl_Group(x+10, 360, w-20, 50);
      {
        numExtrusions_input = createValueInput(x+15, y+220, 50, 25,
                                               1, 50, 1,
                                               DLFLWindow::num_extrusions,
                                               changeNumExtrusions);
        numExtrusions_roller = createRoller(x+75, y+220, 165, 25,
                                            "Segments:",
                                            1, 50, 1,
                                            DLFLWindow::num_extrusions,
                                            changeNumExtrusions);
      }
      o->end();
    }
    {
      Fl_Group* o = new Fl_Group(x+10, y+285, w-20, 50);
      {
           // Note: Both check buttons are created at the same location
           // since only one of them will be used at a time.
        dualMeshEdgesCheck_button = createCheckButton(x+15, y+285, 20, 25,
                                                      "Mesh flat edges",
                                                      DLFLWindow::dual_mesh_edges_check,
                                                      toggleDualMeshEdgesFlag);
        hexagonalizeDodecaExtrudeCheck_button = createCheckButton(x+15, y+285, 20, 25,
                                                                  "Hexagonalize",
                                                                  DLFLWindow::hexagonalize_dodeca_extrude,
                                                                  toggleHexagonalizeDodecaExtrudeFlag);
        hexagonalizeDodecaExtrudeCheck_button->hide();
      }
      o->end();
    }
    o->end(); o->hide();
  }

     // Stellations tile
  {
    Fl_Group* o = stellate_tile = new Fl_Group(x, y, w, h);
    o->color(48); o->selection_color(48);
    o->resizable(NULL);
    {
      Fl_Group* o = new Fl_Group(x+10, y+25, w-20, 50);
      {
        stellateLength_input = createValueInput(x+15, y+25, 50, 25,
                                                -100.0, 100.0, 0.1,
                                                DLFLWindow::extrude_dist,
                                                changeStellateLength);
        stellateLength_roller = createRoller(x+75, y+25, 165, 25,
                                             "Length:",
                                             -100.0, 100.0, 0.1,
                                             DLFLWindow::extrude_dist,
                                             changeStellateLength);
      }
      o->end();
    }
    o->end(); o->hide();
  }
}

void DLFLAppWindow :: createTexturingModeTiles(int x, int y, int w, int h)
{
     // Current production version does not include slider for tiling number
     // It is always two
     // Tile Texturing tile
  {
    Fl_Group* o = tile_texturing_tile = new Fl_Group(x, y, w, h);
    o->color(48); o->selection_color(48);
    o->resizable(NULL);
#ifndef DLFL_RELEASE
    {
      Fl_Group* o = new Fl_Group(x+10, y+25, w-20, 50);
      {
        tileTexNum_input = createValueInput(x+15, y+25, 50, 25,
                                            2,8,1,
                                            DLFLWindow::tile_tex_n,
                                            changeTileTexNum);
        tileTexNum_roller = createRoller(x+75, y+25, 165, 25,
                                         "Tiling Number:",
                                         2,8,1,
                                         DLFLWindow::tile_tex_n,
                                         changeTileTexNum);
      }
      o->end();
    }
#endif    
    {
      Fl_Button* o = new Fl_Button((w-140)/2, y+90, 140, 35, "Assign Tex Coords");
      o->callback((Fl_Callback*)assign_tile_texture_coords);
    }
    o->end(); o->hide();
  }
}

void DLFLAppWindow :: createHighGenusModeTiles(int x, int y, int w, int h)
{
     // Hole/Handle tile
  {
    Fl_Group* o = hole_tile = new Fl_Group(x, y, w, h);
    o->color(48); o->selection_color(48);
    o->resizable(NULL);
    {
      Fl_Group* o = new Fl_Group(x+10, y+25, w-20, 50);
      {
        numSegments_input = createValueInput(x+15, y+25, 50, 25,
                                             1, 100, 1,
                                             DLFLWindow::num_segments,
                                             changeNumSegments);
        numSegments_roller = createRoller(x+75, y+25, 165, 25,
                                          "Number of Segments:",
                                          1, 100, 1,
                                          DLFLWindow::num_segments,
                                          changeNumSegments);
      }
      o->end();
    }
    {
      Fl_Group* o = new Fl_Group(x+10, y+90, w-20, 50);
      {
        maxSegments_input = createValueInput(x+15, y+90, 50, 25,
                                             -1, DLFLWindow::num_segments, 1,
                                             DLFLWindow::max_segments,
                                             changeMaxSegments);
        maxSegments_roller = createRoller(x+75, y+90, 165, 25,
                                          "# of Segments to Connect:",
                                          -1, DLFLWindow::num_segments, 1,
                                          DLFLWindow::max_segments,
                                          changeMaxSegments);
      }
      o->end();
    }
    o->end(); o->hide();
  }

     // Curved Handle tile
  {
    Fl_Group* o = handle_tile = new Fl_Group(x, y, w, h);
    o->color(48); o->selection_color(48);
    o->resizable(NULL);
    {
      Fl_Group* o = new Fl_Group(x+10, y+25, w-20, 50);
      {
        numSegments2_input = createValueInput(x+15, y+25, 50, 25,
                                              1, 100, 1,
                                              DLFLWindow::num_segments,
                                              changeNumSegments);
        numSegments2_roller = createRoller(x+75, y+25, 165, 25,
                                           "Number of Segments:",
                                           1, 100, 1,
                                           DLFLWindow::num_segments,
                                           changeNumSegments);
      }
      o->end();
    }
    {
      Fl_Group* o = new Fl_Group(x+10, y+90, w-20, 50);
      {
        maxSegments2_input = createValueInput(x+15, y+90, 50, 25,
                                              -1, DLFLWindow::num_segments, 1,
                                              DLFLWindow::max_segments,
                                              changeMaxSegments);
        maxSegments2_roller = createRoller(x+75, y+90, 165, 25,
                                           "# of Segments to Connect:",
                                           -1, DLFLWindow::num_segments, 1,
                                           DLFLWindow::max_segments,
                                           changeMaxSegments);
      }
      o->end();
    }
    {
      Fl_Group* o = new Fl_Group(x+10, y+155, w-20, 50);
      {
        weight1_input = createValueInput(x+15, y+155, 50, 25,
                                         -100.0, 100.0, 0.5,
                                         DLFLWindow::nwt1,
                                         changeWeight1);
        weight1_roller = createRoller(x+75, y+155, 165, 25,
                                      "Weight 1:",
                                      -100.0, 100.0, 0.5,
                                      DLFLWindow::nwt1,
                                      changeWeight1);
      }
      o->end();
    }
    {
      Fl_Group* o = new Fl_Group(x+10, y+220, w-20, 50);
      {
        symmetricWeightsCheck_button = createCheckButton(x+15, y+220, 20, 25,
                                                         "Symmetric Weights",
                                                         DLFLWindow::symmetric_weights,
                                                         toggleSymmetricWeightsFlag);
      }
      o->end();
    }
    {
      Fl_Group* o = new Fl_Group(x+10, y+285, w-20, 50);
      {
        weight2_input = createValueInput(x+15, y+285, 50, 25,
                                         -100.0, 100.0, 0.5,
                                         DLFLWindow::nwt2,
                                         changeWeight2);
        weight2_input->deactivate();
        weight2_roller = createRoller(x+75, y+285, 165, 25,
                                      "Weight 2:",
                                      -100.0, 100.0, 0.5,
                                      DLFLWindow::nwt2,
                                      changeWeight2);
        weight2_roller->deactivate();
      }
      o->end();
    }
    {
      Fl_Group* o = new Fl_Group(x+10, y+350, w-20, 50);
      {
        extraTwists_input = createValueInput(x+15, y+350, 50, 25,
                                             0.0, 10.0, 1.0,
                                             DLFLWindow::num_extra_twists,
                                             changeExtraTwists);
        extraTwists_roller = createRoller(x+75, y+350, 165, 25,
                                          "Extra Twists:",
                                          0.0, 10.0, 1.0,
                                          DLFLWindow::num_extra_twists,
                                          changeExtraTwists);
      }
      o->end();
    }
    o->end(); o->hide();
  }

     // Multi-Face handle tile
  {
    Fl_Group* o = multi_face_handle_tile = new Fl_Group(x, y, w, h);
    o->color(48); o->selection_color(48);
    o->resizable(NULL);
    {
      Fl_Group* o = new Fl_Group(x+10, y+25, w-20, 50);
      {
        Fl_Menu_Item menuitems [] =
          {
            {"Convex Hull", 0, (Fl_Callback*)mfh_use_convex_hull_algo, 0, 0},
            {"Closest Edge", 0, (Fl_Callback*)mfh_use_closest_edge_algo, 0, 0},
            {0}
          };
        mfh_algo_menu = createPullDownMenu(10, y+25, w-20, 25, "Algorithm", menuitems);
        mfh_algo_menu->show();
      }
      o->end();
    }
    {
      Fl_Group* o = new Fl_Group(x+10, y+90, w-20, 50);
      {
        multiFaceHandleScaleFactor_input = createValueInput(x+15, y+90, 50, 25,
                                                            0.0, 5.0, 0.01,
                                                            DLFLWindow::mfh_scale_factor,
                                                            changeMultiFaceHandleScaleFactor);
        multiFaceHandleScaleFactor_roller = createRoller(x+75, y+90, 165, 25,
                                                         "Scale Factor:",
                                                         0.0, 5.0, 0.01,
                                                         DLFLWindow::mfh_scale_factor,
                                                         changeMultiFaceHandleScaleFactor);
      }
      o->end();
    }
    {
      Fl_Group* o = new Fl_Group(x+10, y+155, w-20, 50);
      {
        multiFaceHandleExtrudeDist_input = createValueInput(x+15, y+155, 50, 25,
                                                            -2.0, 2.0, 0.1,
                                                            DLFLWindow::mfh_extrude_dist,
                                                            changeMultiFaceHandleExtrudeDist);
        multiFaceHandleExtrudeDist_roller = createRoller(x+75, y+155, 165, 25,
                                                         "Extrude Distance:",
                                                         -2.0, 2.0, 0.1,
                                                         DLFLWindow::mfh_extrude_dist,
                                                         changeMultiFaceHandleExtrudeDist);
      }
      o->end();
    }
    {
      Fl_Group* o = new Fl_Group(x+10, y+220, w-20, 50);
      {
        multiFaceHandleUseMaxOffsetCheck_button = createCheckButton(x+15, y+220, 20, 25,
                                                                    "Use Max. Offsets",
                                                                    DLFLWindow::mfh_use_max_offsets,
                                                                    toggleMultiFaceHandleUseMaxOffsetFlag);
      }
      o->end();
    }
    {
      Fl_Button* o = new Fl_Button((w-140)/2, y+285, 140, 50, "Connect Selected\nFaces");
      o->callback((Fl_Callback*)create_multi_face_handle);
    }
    o->end(); o->hide();
  }

     // Crust modeling tile 1
  {
    Fl_Group* o = crust_modeling1_tile = new Fl_Group(x, y, w, h);
    o->color(48); o->selection_color(48);
    o->resizable(NULL);
    {
      Fl_Group* o =  new Fl_Group(x+10, y+25, w-20, 50);
      {
        crustScaleFactor_input =  createValueInput(x+15, y+25, 50, 25,
                                                   0.0, 10.0, 0.01,
                                                   DLFLWindow::crust_scale_factor,
                                                   changeCrustScaleFactor);
        crustScaleFactor_roller = createRoller(x+75, y+25, 165, 25,
                                               "Scale Factor:",
                                               0.0, 10.0, 0.01,
                                               DLFLWindow::crust_scale_factor,
                                               changeCrustScaleFactor);
      }
      o->end();
    }
    {
      Fl_Button* o = new Fl_Button((w-90)/2, y+90, 90, 35, "Create Crust");
      o->callback((Fl_Callback*)crust_modeling1);
    }
    {
      Fl_Group* o = new Fl_Group(x+10, y+155, w-20, 50);
      {
        crustCleanupCheck_button1 = createCheckButton(x+15, y+155, 20, 25,
                                                      "Cleanup when peeling",
                                                      DLFLWindow::crust_cleanup,
                                                      toggleCrustCleanupFlag);
      }
      o->end();
    }
    o->end(); o->hide();
  }

     // Crust modeling tile 2
  {
    Fl_Group* o = crust_modeling2_tile = new Fl_Group(x, y, w, h);
    o->color(48); o->selection_color(48);
    o->resizable(NULL);
    {
      Fl_Group* o = new Fl_Group(x+10, y+25, w-20, 50);
      {
        crustThickness_input = createValueInput(x+15, y+25, 50, 25,
                                                -1.0, 1.0, 0.01,
                                                DLFLWindow::crust_thickness,
                                                changeCrustThickness);
        crustThickness_roller = createRoller(x+75, y+25, 165, 25,
                                             "Thickness:",
                                             -1.0, 1.0, 0.01,
                                             DLFLWindow::crust_thickness,
                                             changeCrustThickness);
      }
      o->end();
    }
    {
      Fl_Button* o = new Fl_Button((w-90)/2, y+90, 90, 35, "Create Crust");
      o->callback((Fl_Callback*)crust_modeling2);
    }
    {
      Fl_Group* o = new Fl_Group(x+10, y+155, w-20, 50);
      {
        crustCleanupCheck_button2 = createCheckButton(x+15, y+155, 20, 25,
                                                      "Cleanup when peeling",
                                                      DLFLWindow::crust_cleanup,
                                                      toggleCrustCleanupFlag);
      }
      o->end();
    }
    o->end(); o->hide();
  }

     // Wireframe modeling tile
  {
    Fl_Group* o = wireframe_modeling_tile = new Fl_Group(x, y, w, h);
    o->color(48); o->selection_color(48);
    o->resizable(NULL);
    {
      Fl_Group* o = new Fl_Group(x+10, y+25, w-20, 50);
      {
        wireframeThickness_input = createValueInput(x+15, y+25, 50, 25,
                                                    0.0, 1.0, 0.01,
                                                    DLFLWindow::wireframe_thickness,
                                                    changeWireframeThickness);
        wireframeThickness_roller = createRoller(x+75, y+25, 165, 25,
                                                 "Thickness:",
                                                 0.0, 1.0, 0.01,
                                                 DLFLWindow::wireframe_thickness,
                                                 changeWireframeThickness);
      }
      o->end();
    }
    {
      Fl_Button* o = new Fl_Button((w-120)/2, y+90, 120, 35, "Create Wireframe");
      o->callback((Fl_Callback*)wireframe_modeling);
    }
    o->end(); o->hide();
  }

     // Column modeling tile
  {
    Fl_Group* o = column_modeling_tile = new Fl_Group(x, y, w, h);
    o->color(48); o->selection_color(48);
    o->resizable(NULL);
    {
      Fl_Group* o = new Fl_Group(x+10, y+25, w-20, 50);
      {
        columnThickness_input = createValueInput(x+15, y+25, 50, 25,
                                                 0.0, 1.0, 0.01,
                                                 DLFLWindow::column_thickness,
                                                 changeColumnThickness);
        columnThickness_roller = createRoller(x+75, y+25, 165, 25,
                                              "Thickness:",
                                              0.0, 1.0, 0.01,
                                              DLFLWindow::column_thickness,
                                              changeColumnThickness);
      }
      o->end();
    }
    {
      Fl_Group* o = new Fl_Group(x+10, y+90, w-20, 50);
      {
        columnSegments_input = createValueInput(x+15, y+90, 50, 25,
                                                4, 100, 2,
                                                DLFLWindow::column_segments,
                                                changeColumnSegments);
        columnSegments_roller = createRoller(x+75, y+90, 165, 25,
                                             "Number of Segments:",
                                             4, 100, 2,
                                             DLFLWindow::column_segments,
                                             changeColumnSegments);
      }
      o->end();
    }
    {
      Fl_Button* o = new Fl_Button((w-120)/2, y+155, 120, 50, "Create Wireframe\nWith Columns");
      o->callback((Fl_Callback*)column_modeling);
    }
    o->end(); o->hide();
  }

     // Sierpinsky tile
  {
    Fl_Group* o = sierpinsky_tile = new Fl_Group(x, y, w, h);
    o->color(48); o->selection_color(48);
    o->resizable(NULL);
    {
      Fl_Button* o = new Fl_Button((w-140)/2, y+25, 140, 50, "Create Sierpinsky\nTetrahedra");
      o->callback((Fl_Callback*)create_sierpinsky_tetrahedra);
    }
    o->end(); o->hide();
  }

     // Menger Sponge tile
  {
    Fl_Group* o = mengersponge_tile = new Fl_Group(x, y, w, h);
    o->color(48); o->selection_color(48);
    o->resizable(NULL);
    {
      Fl_Group* o = new Fl_Group(x+10, y+25, w-20, 50);
      {
        mengerSpongeThickness_input = createValueInput(x+15, y+25, 50, 25,
                                                       0.01, 1.0, 0.01,
                                                       DLFLWindow::sponge_thickness,
                                                       changeMengerSpongeThickness);
        mengerSpongeThickness_roller = createRoller(x+75, y+25, 165, 25,
                                                    "Thickness:",
                                                    0.01, 1.0, 0.01,
                                                    DLFLWindow::sponge_thickness,
                                                    changeMengerSpongeThickness);
      }
      o->end();
    }
    {
      Fl_Group* o = new Fl_Group(x+10, y+90, w-20, 50);
      {
        mengerSpongeCollapseThreshold_input = createValueInput(x+15, y+90, 50, 25,
                                                               0.0, 10.0, 0.1,
                                                               DLFLWindow::sponge_collapse_threshold,
                                                               changeMengerSpongeCollapseThreshold);
        mengerSpongeCollapseThreshold_roller = createRoller(x+75, y+90, 165, 25,
                                                            "Edge Collapse\nThreshold Factor:",
                                                            0.0, 10.0, 0.1,
                                                            DLFLWindow::sponge_collapse_threshold,
                                                            changeMengerSpongeCollapseThreshold);
      }
      o->end();
    }
       /*
    {
      Fl_Group* o = new Fl_Group(x+10, y+90, w-20, 50);
      {
        mengerSpongeFractionalThicknessCheck_button = createCheckButton(x+15, y+90, 20, 25,
                                                                        "Fractional thickness",
                                                                        DLFLWindow::sponge_fractional_thickness,
                                                                        toggleMengerSpongeFractionalThicknessFlag);
      }
      o->end();
    }
       */
    {
      Fl_Button* o = new Fl_Button((w-140)/2, y+155, 140, 50, "Create\nMenger Sponge");
      o->callback((Fl_Callback*)create_menger_sponge);
    }
    o->end(); o->hide();
  }

     // Dual tile - this is part of remeshing but is created here because it is a different size
  {
    Fl_Group* o = dual_tile = new Fl_Group(x, y, w, h);
    o->color(48); o->selection_color(48);
    o->resizable(NULL);
    {
      Fl_Group* o = new Fl_Group(x+10, y+25, w-20, 50);
      {
        accurateDualCheck_button = createCheckButton(x+15, y+25, 20, 25,
                                                     "Use Faster Method\n(Less accurate)",
                                                     !DLFLWindow::accurate_dual,
                                                     toggleAccurateDualFlag);
      }
      o->end();
    }
    {
      Fl_Button* o = new Fl_Button((w-100)/2, y+90, 100, 35, "Create Dual");
      o->callback((Fl_Callback*)perform_remeshing);
    }
    o->end(); o->hide();
  }
}

void DLFLAppWindow :: createRollerPanel(int x, int y, int w, int h)
{
     // Panel with all the rollers and buttons which is a part of the control panel
     // x,y specifies the left top corner of the Fl_Group widget
     // w and h are the width and height of the Fl_Group widget respectively.
     // All rollers and buttons are within groups that are children of this widget
  roller_panel = new Fl_Group(x+1,y+1,w-2,h-2);
  roller_panel->box(FL_EMBOSSED_BOX);
  
     // Blank tile for filler
  {
    Fl_Group* o = blank = new Fl_Group(x+1, y+1, w-2, h-2);
    o->show();
    o->end();
  }

  createBasicModeTiles(x,y,w,h);
  createExtrusionModeTiles(x,y,w,h);
  createTexturingModeTiles(x,y,w,h);
  createRemeshingMenus(x,y,w,SUBGRP_TILE_H-1);
  createRemeshingTiles(x,y+SUBGRP_TILE_H+1,w,h-SUBGRP_TILE_H-1);
  createHighGenusModeTiles(x,y,w,h);
  roller_panel->end();
}

void DLFLAppWindow :: createControlWidgets(int w, int h)
{
     // Control panel on left. All control widgets are children of the control panel
  control_panel = new Fl_Group(0,MENU_H,OPT_W,h-MENU_H);
  control_panel->box(FL_EMBOSSED_FRAME);

     // Mode selection tile
  createModeSelectionPanel(0,MENU_H,OPT_W,MODE_TILE_H);

  int roller_panel_y = MODE_TILE_H + MENU_H;
  int roller_panel_h = h - roller_panel_y;
  createRollerPanel(0,roller_panel_y,OPT_W,roller_panel_h);

  mode_panel->show();
  roller_panel->show();
  
  control_panel->end();
  control_panel->resizable(roller_panel);
  control_panel->show();
}

void DLFLAppWindow :: destroyControlWidgets(void)
{
  delete basics;
  delete extrude;
  delete handles;
  delete crust;
  delete remeshing;
  delete remeshing_subgrp_panel;
  delete highgenus;
  
  delete control_panel;
  delete roller_panel;
  delete mode_sel_tile;
  delete blank;

  delete subdivide_edge_tile;
  delete subdivide_face_tile;
  delete tile_texturing_tile;
  delete transforms_tile;
  delete extrude_tile;
  delete stellate_tile;
  delete crust_modeling1_tile;
  delete crust_modeling2_tile;
  delete wireframe_modeling_tile;

  delete doo_sabin_tile;
  delete root_4_tile;
  delete vertex_cutting_tile;
  delete pentagonal_tile;
  delete dual_tile;
  delete star_tile; // Doug
  delete fractal_tile; // Doug
  delete substellate2_tile; // Eric

  delete hole_tile;
  delete handle_tile;
  delete multi_face_handle_tile;
  delete sierpinsky_tile;
  delete mengersponge_tile;

  delete numSubDivs_input;
  delete tileTexNum_input;
  delete extrudeLength_input;
  delete extrudeRotation_input;
  delete extrudeScale_input;
  delete stellateLength_input;
  delete numExtrusions_input;
  delete numSegments_input;
  delete maxSegments_input;
  delete numSegments2_input;
  delete maxSegments2_input;
  delete weight1_input;
  delete weight2_input;
  delete extraTwists_input;
  delete multiFaceHandleScaleFactor_input;
  delete multiFaceHandleExtrudeDist_input;
  delete crustScaleFactor_input;
  delete crustThickness_input;
  delete wireframeThickness_input;
  delete root4Twist_input;
  delete root4Weight_input;
  delete vertexCuttingOffset_input;
  delete pentagonalOffset_input;
  delete starOffset_input; // Doug
  delete fractalOffset_input; // Doug
  delete substellate_amount_input; // Eric
  delete substellate_curve_input; // Eric
  delete mengerSpongeThickness_input;
  delete mengerSpongeCollapseThreshold_input;
  delete translatex_input;
  delete translatey_input;
  delete translatez_input;
  delete scalex_input;
  delete scalex_input;
  delete scalex_input;

  delete numSubDivs_roller;
  delete tileTexNum_roller;
  delete extrudeLength_roller;
  delete extrudeRotation_roller;
  delete extrudeScale_roller;
  delete numExtrusions_roller;
  delete stellateLength_roller;
  delete numSegments_roller;
  delete maxSegments_roller;
  delete numSegments2_roller;
  delete maxSegments2_roller;
  delete weight1_roller;
  delete weight2_roller;
  delete extraTwists_roller;
  delete multiFaceHandleScaleFactor_roller;
  delete multiFaceHandleExtrudeDist_roller;
  delete crustScaleFactor_roller;
  delete crustThickness_roller;
  delete wireframeThickness_roller;
  delete root4Twist_roller;
  delete root4Weight_roller;
  delete vertexCuttingOffset_roller;
  delete pentagonalOffset_roller;
  delete starOffset_roller; // Doug
  delete fractalOffset_roller; // Doug
  delete substellate_amount_roller; // Eric
  delete substellate_curve_roller; // Eric
  delete mengerSpongeThickness_roller;
  delete mengerSpongeCollapseThreshold_roller;
  delete translatex_roller;
  delete translatey_roller;
  delete translatez_roller;
  delete scalex_roller;
  delete scalex_roller;
  delete scalex_roller;

  delete symmetricWeightsCheck_button;
  delete dooSabinEdgeCheck_button;
  delete dualMeshEdgesCheck_button;
  delete hexagonalizeDodecaExtrudeCheck_button;
  delete useQuadsCheck_button;
  delete multiFaceHandleUseMaxOffsetCheck_button;
  delete mengerSpongeFractionalThicknessCheck_button;
}

void DLFLAppWindow :: createMenubar(int w, int h)
{
  Fl_Menu_Item mainmenuitems[] =
    {
      {"File", 0, 0, 0, FL_SUBMENU},
        {"Open", 0, (Fl_Callback*)open_file, 0, 0},
        {"Save", 0, 0, 0, FL_SUBMENU},
          {"Save (default)", 0, (Fl_Callback*)save_file, 0, 0},
          {"Save (with normals)", 0, (Fl_Callback*)save_file_with_normals, 0, 0},
          {"Save (with texture coords)", 0, (Fl_Callback*)save_file_with_tex_coords, 0, 0},
          {"Save (extended)", 0, (Fl_Callback*)save_file_extended, 0, 0},
        {0},
        {"Load Texture", 0, (Fl_Callback*)load_texture, 0, 0},
        {"Print Information", 0, (Fl_Callback*)print_info, 0, 0},
        {"Print Face List", 0, (Fl_Callback*)print_flist, 0, 0},
        {"Print Vertex List", 0, (Fl_Callback*)print_vlist, 0, 0},
        {"Print Edge List", 0, (Fl_Callback*)print_elist, 0, 0},
        {"Quit", 0, (Fl_Callback*)quit, 0, 0},
      {0},
      {"Edit", 0, 0, 0, FL_SUBMENU},
        {"Undo", FL_CTRL+'z', (Fl_Callback*)undo, 0, 0},
        {"Redo", FL_CTRL+'x', (Fl_Callback*)redo, 0, 0},
      {0},
      {"Display", 0, 0, 0, FL_SUBMENU},
        {"Show Vertices", 0, (Fl_Callback*)toggle_vertices, 0, FL_MENU_TOGGLE},
        {"Show Silhouette", 0, (Fl_Callback*)toggle_silhouette, 0, FL_MENU_TOGGLE},
        {"Show Wireframe", 0, (Fl_Callback*)toggle_wireframe, 0, FL_MENU_TOGGLE|FL_MENU_VALUE},
        {"Show Grid", 0, (Fl_Callback*)toggle_grid, 0, FL_MENU_TOGGLE},
        {"Show Coordinate Axes", 0, (Fl_Callback*)toggle_axes, 0, FL_MENU_TOGGLE|FL_MENU_VALUE},
      {0},
      {"Renderer", 0, 0, 0, FL_SUBMENU},
#ifndef DLFL_RELEASE
        {"Normal Renderer", 0, (Fl_Callback*)use_normal_renderer, 0, FL_MENU_RADIO},
#endif /* DLFL_RELEASE */
        {"Shaded Renderer", 0, (Fl_Callback*)use_shaded_renderer, 0, FL_MENU_RADIO},
        {"Lighted Renderer", 0, (Fl_Callback*)use_lighted_renderer, 0, FL_MENU_RADIO|FL_MENU_VALUE},
        {"Textured Renderer", 0, (Fl_Callback*)use_textured_renderer, 0, FL_MENU_RADIO},
        {"Textured-Lighted Renderer", 0, (Fl_Callback*)use_textured_lit_renderer, 0, FL_MENU_RADIO},
        {"Patch Renderer", 0, (Fl_Callback*)use_patch_renderer, 0, FL_MENU_RADIO},
      {0},
#ifndef DLFL_RELEASE
      {"Object", 0, 0, 0, FL_SUBMENU},
        {"Subdivide All Edges", 0, (Fl_Callback*)subdivide_all_edges, 0, 0},
        {"Planarize All Faces", 0, (Fl_Callback*)planarize_all_faces, 0, 0},
        {"Make Object Spherical", 0, (Fl_Callback*)make_object_spherical, 0, 0},
        {"Make Object Smooth", 0, (Fl_Callback*)make_object_smooth, 0, 0},
        {"Create a Crust", 0, (Fl_Callback*)create_crust, 0, 0},
        {"Compute Lighting", 0, (Fl_Callback*)compute_lighting, 0, 0},
        {"Compute Normals and Lighting", 0, (Fl_Callback*)compute_normals_and_lighting, 0, 0},
        {"Assign Texture Coordinates", 0, (Fl_Callback*)assign_texture_coordinates, 0, 0},
      {0},
#endif /* DLFL_RELEASE */
      {"Selection", 0, 0, 0, FL_SUBMENU},
        {"Select a Vertex", 0, (Fl_Callback*)select_vertex, 0, 0},
        {"Select a Face", 0, (Fl_Callback*)select_face, 0, 0},
        {"Select an Edge", 0, (Fl_Callback*)select_edge, 0, 0},
        {"Select a Corner", 0, (Fl_Callback*)select_corner, 0, 0},
        {"Exit Selection Mode", 0, (Fl_Callback*)exit_selection_mode, 0, 0},
      {0},
      {0}
    };

  menubar = new Fl_Menu_Bar(0,0,w,MENU_H);
  menubar->labeltype(FL_NO_LABEL);
  menubar->copy(mainmenuitems); // Use copy - the array of menu items is local to this function
  menubar->global();
}

/*
  $Log: DLFLAppWindow.cc,v $
  Revision 4.15  2004/01/20 17:17:16  vinod
  Tiling number tile is now excluded in the release version

  Revision 4.14  2004/01/20 09:30:01  vinod
  Added column modeling tile. Reorganized high genus menu.
  Added Edit menu with Undo and Redo.

  Revision 4.13  2004/01/20 00:59:33  vinod
  Renamed some schemes in the menus. Moved dual tile creation code to this file

  Revision 4.12  2004/01/19 20:22:27  vinod
  Moved remeshing tile definitions to DLFLAppWindowRemeshingTiles.cc

  Revision 4.11  2004/01/19 18:07:27  vinod
  Reorganized remeshing menus and tiles. Moved code related to remeshing to separate files

  Revision 4.10  2004/01/13 09:20:01  vinod
  Added controls and callbacks for scale factor
  for pentagon preserving remeshing scheme

  Revision 4.9  2004/01/13 02:50:00  vinod
  Added tiles, controls and callbacks for:
  stellate and double stellate remeshing schemes
  double stellation, dodecahedral and icosahedral extrusions

  Revision 4.8  2004/01/12 20:11:27  vinod
  Separated creation of control widgets into separate groups based on mode

  Revision 4.7  2004/01/12 05:00:41  vinod
  Removed unused widgets. Adjusted formatting

  Revision 4.6  2004/01/12 02:28:09  vinod
  Cleaned up code. Added static functions for adding widgets.
  Rollers, check buttons and value input widgets are now added using static functions.
  Removed unused controls.

  Revision 4.5  2004/01/09 14:54:45  vinod
  Changed names of toggle button callbacks.

  Revision 4.4  2004/01/02 21:02:45  vinod
  Additions by Doug: Tiles, control widgets and callbacks for star, sqrt3 and fractal subdivision schemes

  Revision 4.3  2004/01/02 19:43:57  vinod
  Added Collapse Edge operation to Basics group.
  Added macros for conditional include of non-release menu options

  Revision 4.2  2004/01/01 07:51:35  vinod
  Reorganized menus. Renamed some menu items

  Revision 4.1  2003/12/28 05:29:39  vinod
  Added tile for delete edge. Added check button for
  delete edge cleanup flag toggle. Removed second delete edge mode.

  Revision 4.0  2003/12/26 01:58:52  vinod
  Major version sync.

  Revision 3.14  2003/12/26 01:50:46  vinod
  Combined roller and input callbacks.
  Added separate texturing mode. Minor re-organization and renaming of menus.

  Revision 3.13  2003/12/07 19:18:25  vinod
  Added check button for crust cleanup

  Revision 3.12  2003/12/07 01:49:45  vinod
  Added check button and callback for dual accuracy flag

  Revision 3.11  2003/12/05 15:56:54  vinod
  Bug fix: Added missing end() for symmetric weights group. Changed weight limits to allow negative weights

  Revision 3.10  2003/12/01 09:01:36  vinod
  Added options, controls and callbacks for translation and rotation

  Revision 3.9  2003/11/29 22:04:38  vinod
  Added control and callback for max. no. of connections
  for regular face connection operation

  Revision 3.8  2003/11/29 04:21:51  vinod
  Added check button controls and callbacks
  for symmetric hermitian handle weights

  Revision 3.7  2003/11/28 19:43:38  vinod
  Added controls and callbacks for setting max. # of connections for curved handles

  Revision 3.6  2003/11/27 00:22:29  vinod
  Changed threshold angle to threshold factor in Menger sponge creation

  Revision 3.5  2003/11/26 18:26:03  vinod
  Changed collapse threshold in Menger sponge creation to angle instead of length

  Revision 3.4  2003/11/23 22:44:56  vinod
  Added controls for menger sponge edge collapse threshold

  Revision 3.3  2003/11/23 22:22:26  vinod
  Commented out fractional thickness toggle button for Menger sponge

  Revision 3.2  2003/11/21 15:26:55  vinod
  Added check button for fractional thickness toggle for menger sponge creation

  Revision 3.1  2003/11/19 16:10:07  vinod
  Added High Genus group and controls for sierpinsky and menger sponge

  Revision 3.0  2003/09/19 16:06:47  vinod
  Major version update

  Revision 2.25  2003/09/06 19:51:21  vinod
  Added menu for choosing multi-face-handle algorithm

  Revision 2.24  2003/08/24 20:25:40  vinod
  Changed range and step size for various controls

  Revision 2.23  2003/08/24 01:52:45  vinod
  Changed all sliders to rollers

  Revision 2.22  2003/07/07 16:12:41  vinod
  Added check button for use_max_offsets for multi-connect

  Revision 2.21  2003/04/25 15:47:28  vinod
  Added widgets for multi-face handle operations

  Revision 2.20  2003/04/14 16:51:13  vinod
  Added widgets for multi-face handle creation

  Revision 2.19  2003/04/14 16:16:24  vinod
  Made various tiles not resizable to prevent widgets from getting resized when window is resized

  Revision 2.18  2003/03/26 05:14:55  vinod
  Changed slider range for extrusions to allow negative values

  Revision 2.17  2003/02/01 19:04:04  vinod
  Changed save option to be submenu allowing save with multiple options

  Revision 2.16  2003/01/25 08:11:09  vinod
  Ensured centering of all buttons. Added Subdivide Selected Faces button

  Revision 2.15  2003/01/20 21:43:35  vinod
  Name change from skeleton modeling to wireframe modeling

  Revision 2.14  2003/01/05 21:10:28  vinod
  Added controls and variables for skeleton modeling

  Revision 2.13  2002/12/27 06:33:43  vinod
  Changed min/max ranges for various sliders and input boxes

  Revision 2.12  2002/12/18 16:21:25  vinod
  Added face subdivision and tile texturing controls

  Revision 2.11  2002/12/17 03:42:59  vinod
  Added separate controls and callbacks for stellation length. Fixed comment logs

  Revision 2.10  2002/12/17 03:18:37  vinod
  See comments for previous revision

  Revision 2.9  2002/12/17 03:11:07  vinod
  Additions by Paul
  Tiles for stellateFace, extrudeDualFace, vertex cutting subdivide
  pentagonal subdivide
  Sliders for controls for extrudeDualFace, stellateFace

  Revision 2.8  2002/12/17 02:44:59  vinod
  DLFLWindow.hh

  Revision 2.7  2002/11/07 08:20:18  vinod
  Added check button for edge check flag for dooSabin subdivision

  Revision 2.6  2002/10/31 06:08:19  vinod
  Added controls and callbacks for simplest subdivision scheme

  Revision 2.5  2002/10/30 04:43:54  vinod
  Added menu item to load texture, set proper initial value for menu items

  Revision 2.4  2002/10/28 06:51:23  vinod
  Reorganized control panel widgets.

  Revision 2.3  2002/10/22 21:19:26  vinod
  Changed sizes of various windows
  Added controls for dual
  Changed all Fl_Tile to Fl_Group
  Fixed position of all widgets
  Added renderers

  Revision 2.2  2002/10/17 02:19:04  vinod
  Added definitions for static members.
  Widgets are now static members. Added w,h parameters to create... methods
  Static parameters from DLFLWindow class are now properly referenced

  Revision 2.1  2002/10/16 14:13:52  vinod
  Source code for DLFLAppWindow class

*/
