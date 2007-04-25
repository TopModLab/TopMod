/* $Id: DLFLAppWindowRemeshingTiles.cc,v 4.5 2004/01/20 22:19:01 vinod Exp $ */

// Subroutines to create all remeshing menus and tiles
#include "DLFLAppWindow.hh"

Fl_Choice * DLFLAppWindow::tri_remeshing_menu;
Fl_Choice * DLFLAppWindow::quad_remeshing_menu;
Fl_Choice * DLFLAppWindow::pent_remeshing_menu;
Fl_Choice * DLFLAppWindow::tri_preserving_menu;
Fl_Choice * DLFLAppWindow::quad_preserving_menu;
Fl_Choice * DLFLAppWindow::pent_preserving_menu;
Fl_Choice * DLFLAppWindow::hex_preserving_menu;
Fl_Choice * DLFLAppWindow::misc_remeshing_menu;

Fl_Group * DLFLAppWindow::generic_remeshing_tile;
Fl_Group * DLFLAppWindow::doo_sabin_tile;
Fl_Group * DLFLAppWindow::root_4_tile;
Fl_Group * DLFLAppWindow::vertex_cutting_tile;
Fl_Group * DLFLAppWindow::pentagonal_tile;
Fl_Group * DLFLAppWindow::pentagonal2_tile;
Fl_Group * DLFLAppWindow::dual_tile;
Fl_Group * DLFLAppWindow::star_tile; // Doug
Fl_Group * DLFLAppWindow::fractal_tile; // Doug
Fl_Group * DLFLAppWindow::substellate2_tile; // Eric
Fl_Group * DLFLAppWindow::doo_sabin_bc_new_tile; // Bei & Cansin
Fl_Group * DLFLAppWindow::loopstyle_sub_tile; // Bei & Cansin
Fl_Group * DLFLAppWindow::dual1264_tile; // Bei & Cansin
Fl_Group * DLFLAppWindow::dome_sub_tile; // Bei & Cansin

Fl_Value_Input * DLFLAppWindow::dooSabinBCnewScale_input; // Bei & Cansin
Fl_Value_Input * DLFLAppWindow::dooSabinBCnewLength_input; // Bei & Cansin
Fl_Value_Input * DLFLAppWindow::loopStyleSubdivisionLength_input; // Bei & Cansin
Fl_Value_Input * DLFLAppWindow::dual1264Scale_input; // Bei & Cansin
Fl_Value_Input * DLFLAppWindow::domeSubdivisionLength_input; // Bei & Cansin
Fl_Value_Input * DLFLAppWindow::domeSubdivisionScale_input; // Bei & Cansin
Fl_Roller * DLFLAppWindow::dooSabinBCnewScale_roller; // Bei & Cansin
Fl_Roller * DLFLAppWindow::dooSabinBCnewLength_roller; // Bei & Cansin
Fl_Roller * DLFLAppWindow::loopStyleSubdivisionLength_roller; // Bei & Cansin
Fl_Roller * DLFLAppWindow::dual1264Scale_roller; // Bei & Cansin
Fl_Roller * DLFLAppWindow::domeSubdivisionLength_roller; // Bei & Cansin
Fl_Roller * DLFLAppWindow::domeSubdivisionScale_roller; // Bei & Cansin

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

void DLFLAppWindow :: createRemeshingMenus(int x, int y, int w, int h)
{
  remeshing_subgrp_panel = new Fl_Group(x+1,y,w-2,h);
  remeshing_subgrp_panel->box(FL_EMBOSSED_BOX);
  {
    Fl_Menu_Item menuitems [] =
      {
        {"Select Remeshing Scheme", 0, (Fl_Callback*)hide_remeshing_tiles, 0, FL_MENU_DIVIDER},
        {"Triangle Conversion Schemes", 0, 0, 0, FL_MENU_INACTIVE},
        {"Root-3", 0, (Fl_Callback*)root3_subdivision, 0, 0}, // To be implemented
	{"Dual of Vertex-Truncation", 0, (Fl_Callback*)sqrt3_subdivision, 0, 0}, // Doug
        {"Stellation", 0, (Fl_Callback*)global_stellation, 0, 0}, // To be implement
	{"Double stellation", 0, (Fl_Callback*)star_subdivision, 0, 0}, // Doug
        {"12.6.4", 0, (Fl_Callback*)generic1264_subdivision, 0, FL_MENU_DIVIDER},
        {"Valent-3 Conversion Schemes", 0, 0, 0, FL_MENU_INACTIVE},
        {"Honeycomb (Dual of Root-3)", 0, (Fl_Callback*)honeycomb_subdivision, 0, 0},
        {"Vertex-Truncation", 0, (Fl_Callback*)vertex_cutting_subdivision, 0, 0},
        {"Dual of 12.6.4", 0, (Fl_Callback*)dual_generic1264_subdivision, 0, 0},
        {0}
      };
    menuitems[1].labelcolor(FL_BLUE); menuitems[1].labelfont(FL_HELVETICA_BOLD);
    menuitems[7].labelcolor(FL_BLUE); menuitems[7].labelfont(FL_HELVETICA_BOLD);
    tri_remeshing_menu = createPullDownMenu(10, y+25, w-20, 25,
                                            "3-Conversion Schemes",menuitems);
  }
  {
    Fl_Menu_Item menuitems [] =
      {
        {"Select Remeshing Scheme", 0, (Fl_Callback*)hide_remeshing_tiles, 0, FL_MENU_DIVIDER},
        {"Quad. Conversion Schemes", 0, 0, 0, FL_MENU_INACTIVE},
        {"Linear Vertex Insertion", 0, (Fl_Callback*)vertex_insertion_subdivision, 0, 0},
        {"Catmull-Clark", 0, (Fl_Callback*)catmull_clark_subdivision, 0, 0},
        {"Stellate with edge removal", 0, (Fl_Callback*)stellate_subdivision, 0, FL_MENU_DIVIDER}, // Eric
        {"Valent-4 Conversion Schemes", 0, 0, 0, FL_MENU_INACTIVE},
        {"Doo-Sabin", 0, (Fl_Callback*)doo_sabin_subdivision, 0, 0},
#ifndef DLFL_RELEASE
        {"Corner-Cutting", 0, (Fl_Callback*)corner_cutting_subdivision, 0, 0},
#endif /* DLFL_RELEASE */
        {"Simplest", 0, (Fl_Callback*)simplest_subdivision, 0, 0},
        {0}
      };
    menuitems[1].labelcolor(FL_BLUE); menuitems[1].labelfont(FL_HELVETICA_BOLD);
    menuitems[5].labelcolor(FL_BLUE); menuitems[5].labelfont(FL_HELVETICA_BOLD);
    quad_remeshing_menu = createPullDownMenu(10, y+25, w-20, 25,
                                             "4-Conversion Schemes",menuitems);
  }
  {
    Fl_Menu_Item menuitems [] =
      {
        {"Select Remeshing Scheme", 0, (Fl_Callback*)hide_remeshing_tiles, 0, FL_MENU_DIVIDER},
        {"Pentagon Conversion Schemes", 0, 0, 0, FL_MENU_INACTIVE},
        {"Pentagonalization", 0, (Fl_Callback*)pentagonal_subdivision, 0, 0},
        {"Cubic Pentagonalization", 0, (Fl_Callback*)cubic_pentagonal_subdivision, 0, FL_MENU_DIVIDER},
        {"Valent-5 Conversion Schemes", 0, 0, 0, FL_MENU_INACTIVE},
        {"Dual-Pentagonalization", 0, (Fl_Callback*)dual_pentagonal_subdivision, 0, 0},
        {0}
      };
    menuitems[1].labelcolor(FL_BLUE); menuitems[1].labelfont(FL_HELVETICA_BOLD);
    menuitems[4].labelcolor(FL_BLUE); menuitems[4].labelfont(FL_HELVETICA_BOLD);
    pent_remeshing_menu = createPullDownMenu(10, y+25, w-20, 25,
                                             "5-Conversion Schemes",menuitems);
  }
  {
    Fl_Menu_Item menuitems [] =
      {
        {"Select Remeshing Scheme", 0, (Fl_Callback*)hide_remeshing_tiles, 0, FL_MENU_DIVIDER},
        {"Triangle Preservation Schemes", 0, 0, 0, FL_MENU_INACTIVE},
        {"Loop-Style Remeshing", 0, (Fl_Callback*)loop_style_subdivision, 0, 0},
        {"Loop Subdivision", 0, (Fl_Callback*)loop_subdivision, 0, FL_MENU_DIVIDER},
        {"Valent-3 Preservation Schemes", 0, 0, 0, FL_MENU_INACTIVE},
        {"Dual of Loop-Style Remeshing", 0, (Fl_Callback*)root_4_subdivision, 0, 0},
        {"Dual of Loop Subdivision", 0, (Fl_Callback*)dual_loop_subdivision, 0, 0},
        {0}
      };
    menuitems[1].labelcolor(FL_BLUE); menuitems[1].labelfont(FL_HELVETICA_BOLD);
    menuitems[4].labelcolor(FL_BLUE); menuitems[4].labelfont(FL_HELVETICA_BOLD);
    tri_preserving_menu = createPullDownMenu(10, y+25, w-20, 25,
                                             "3-Preservation Schemes",menuitems);
  }
  {
    Fl_Menu_Item menuitems [] =
      {
        {"Select Remeshing Scheme", 0, (Fl_Callback*)hide_remeshing_tiles, 0, FL_MENU_DIVIDER},
        {"Quad. Preservation Schemes", 0, 0, 0, FL_MENU_INACTIVE},
#ifndef DLFL_RELEASE
        {"Global extrude", 0, (Fl_Callback*)global_extrusion, 0, 0}, // To be implemented
#endif /* DLFL_RELEASE */
        {"Checkerboard Remeshing", 0, (Fl_Callback*)checkerboard_remeshing, 0, FL_MENU_DIVIDER}, // To be implemented
        {"Valent-4 Preservation Schemes", 0, 0, 0, FL_MENU_INACTIVE},
#ifndef DLFL_RELEASE
        {"Dual of global extrude", 0, (Fl_Callback*)dual_global_extrusion, 0, 0}, // To be implemented
#endif /* DLFL_RELEASE */
        {"Dual of Checkerboard Remeshing", 0, (Fl_Callback*)dual_checkerboard_remeshing, 0, 0}, // To be implemented
        {0}
      };
    menuitems[1].labelcolor(FL_BLUE); menuitems[1].labelfont(FL_HELVETICA_BOLD);
#ifndef DLFL_RELEASE
    menuitems[4].labelcolor(FL_BLUE); menuitems[4].labelfont(FL_HELVETICA_BOLD);
#else
    menuitems[3].labelcolor(FL_BLUE); menuitems[3].labelfont(FL_HELVETICA_BOLD);
#endif /* DLFL_RELEASE */
    quad_preserving_menu = createPullDownMenu(10, y+25, w-20, 25,
                                              "4-Preservation Schemes",menuitems);
  }
  {
    Fl_Menu_Item menuitems [] =
      {
        {"Select Remeshing Scheme", 0, (Fl_Callback*)hide_remeshing_tiles, 0, FL_MENU_DIVIDER},
        {"Pentagon Preservation Schemes", 0, 0, 0, FL_MENU_INACTIVE},
        {"Pentagon Preserving", 0, (Fl_Callback*)pentagonal2_subdivision, 0, FL_MENU_DIVIDER},
        {"Valent-5 Preservation Schemes", 0, 0, 0, FL_MENU_INACTIVE},
        {"Dual of Pentagon Preservation", 0, (Fl_Callback*)dual_pentagonal2_subdivision, 0, 0},
        {0}
      };
    menuitems[1].labelcolor(FL_BLUE); menuitems[1].labelfont(FL_HELVETICA_BOLD);
    menuitems[3].labelcolor(FL_BLUE); menuitems[3].labelfont(FL_HELVETICA_BOLD);
    pent_preserving_menu = createPullDownMenu(10, y+25, w-20, 25,
                                              "5-Preservation Schemes",menuitems);
  }
  {
    Fl_Menu_Item menuitems [] =
      {
        {"Select Remeshing Scheme", 0, (Fl_Callback*)hide_remeshing_tiles, 0, FL_MENU_DIVIDER},
        {"Hexagon Preservation Schemes", 0, 0, 0, FL_MENU_INACTIVE},
        {"Dual of Loop-Style Remeshing", 0, (Fl_Callback*)root_4_subdivision, 0, FL_MENU_DIVIDER},
        {"Valent-6 Preservation Schemes", 0, 0, 0, FL_MENU_INACTIVE},
        {"Loop-Style Remeshing", 0, (Fl_Callback*)loop_style_subdivision, 0, 0},
        {0}
      };
    menuitems[1].labelcolor(FL_BLUE); menuitems[1].labelfont(FL_HELVETICA_BOLD);
    menuitems[3].labelcolor(FL_BLUE); menuitems[3].labelfont(FL_HELVETICA_BOLD);
    hex_preserving_menu = createPullDownMenu(10, y+25, w-20, 25,
                                             "6-Preservation Schemes",menuitems);
  }
  {
    Fl_Menu_Item menuitems [] =
      {
        {"Select Remeshing Scheme", 0, (Fl_Callback*)hide_remeshing_tiles, 0, FL_MENU_DIVIDER},
	{"Fractal", 0, (Fl_Callback*)fractal_subdivision, 0, 0}, // Doug
        {"Double Stellate with Edge Removal", 0, (Fl_Callback*)twostellate_subdivision, 0, 0}, // Eric
        {"Doo-Sabin(BC)", 0, (Fl_Callback*)doo_sabin_subdivision_bc, 0, 0}, // Bei & Cansin
	{"Doo-Sabin(BC new)", 0, (Fl_Callback*)doo_sabin_subdivision_bc_new, 0, 0}, // Bei & Cansin
	{"Dome", 0, (Fl_Callback*)dome_subdivision, 0, 0}, // Bei & Cansin
        {"Localized operations", 0, 0, 0, FL_MENU_INACTIVE},
        {"Subdivide Face", 0, (Fl_Callback*)subdivide_face_mode, 0, 0},
        {0}
      };
    menuitems[6].labelcolor(FL_BLUE); menuitems[6].labelfont(FL_HELVETICA_BOLD);
    misc_remeshing_menu = createPullDownMenu(10, y+25, w-20, 25, "Miscellaneous Schemes",menuitems);
  }
  remeshing_subgrp_panel->end();
  remeshing_subgrp_panel->hide();
}

void DLFLAppWindow :: createRemeshingTiles(int x, int y, int w, int h)
{
     // Face subdivision tile
  {
    Fl_Group* o = subdivide_face_tile = new Fl_Group(x, y, w, h);
    o->color(48); o->selection_color(48);
    o->resizable(NULL);
    {
      Fl_Group* o = new Fl_Group(x+10, y+25, w-20, 50);
      {
        useQuadsCheck_button = createCheckButton(x+15, y+25, 20, 25,
                                                 "Use Quads (off->Triangles)",
                                                 DLFLWindow::use_quads,
                                                 toggleUseQuadsFlag);
      }
      o->end();
    }
    {
      Fl_Button* o = new Fl_Button((w-140)/2, y+90, 140, 50, "Subdivide Selected\nFaces");
      o->callback((Fl_Callback*)subdivide_face);
    }
    o->end(); o->hide();
  }

     // Generic Remeshing tile
     // Used for all remeshing schemes which do not require any numeric or boolean flags
     // The appropriate remeshing scheme will be selected by the menu callback.
  {
    Fl_Group* o = generic_remeshing_tile = new Fl_Group(x, y, w, h);
    o->color(48); o->selection_color(48);
    o->resizable(NULL);
    {
      Fl_Button* o = new Fl_Button((w-90)/2, y+25, 90, 50, "Perform\nRemeshing");
      o->callback((Fl_Callback*)perform_remeshing);
    }
    o->end(); o->hide();
  }

     // Doo-Sabin subdivision tile
  {
    Fl_Group* o = doo_sabin_tile = new Fl_Group(x, y, w, h);
    o->color(48); o->selection_color(48);
    o->resizable(NULL);
    {
      Fl_Group* o = new Fl_Group(x+10, y+25, w-20, 50);
      {
        dooSabinEdgeCheck_button = createCheckButton(x+15, y+25, 20, 25,
                                                     "Check for multiple edges",
                                                     DLFLWindow::doo_sabin_check,
                                                     toggleDooSabinEdgeFlag);
      }
      o->end();
    }
    {
      Fl_Button* o = new Fl_Button((w-90)/2, y+90, 90, 50, "Perform\nRemeshing");
      o->callback((Fl_Callback*)perform_remeshing);
    }
    o->end(); o->hide();
  }

     // Root-4 subdivision tile
  {
    Fl_Group* o = root_4_tile = new Fl_Group(x, y, w, h);
    o->color(48); o->selection_color(48);
    o->resizable(NULL);
    {
      Fl_Group* o = new Fl_Group(x+10, y+25, w-20, 50);
      {
        root4Twist_input = createValueInput(x+15, y+25, 50, 25,
                                            0.0, 1.0, 0.01,
                                            DLFLWindow::twist_factor,
                                            changeRoot4Twist);
        root4Twist_roller = createRoller(x+75, y+25, 165, 25,
                                         "Twist:",
                                         0.0, 1.0, 0.01,
                                         DLFLWindow::twist_factor,
                                         changeRoot4Twist);
      }
      o->end();
    }
    {
      Fl_Group* o = new Fl_Group(x+10, y+90, w-20, 50);
      {
        root4Weight_input = createValueInput(x+15, y+90, 50, 25,
                                             0.0, 1.0, 0.01,
                                             DLFLWindow::weight_factor,
                                             changeRoot4Weight);
        root4Weight_roller = createRoller(x+75, y+90, 165, 25,
                                          "Weight:",
                                          0.0, 1.0, 0.01,
                                          DLFLWindow::weight_factor,
                                          changeRoot4Weight);
      }
      o->end();
    }
    {
      Fl_Button* o = new Fl_Button((w-90)/2, y+155, 90, 50, "Perform\nRemeshing");
      o->callback((Fl_Callback*)perform_remeshing);
    }
    o->end(); o->hide();
  }

     // Vertex-cutting subdivision tile
  {
    Fl_Group* o = vertex_cutting_tile = new Fl_Group(x, y, w, h);
    o->color(48); o->selection_color(48);
    o->resizable(NULL);
    {
      Fl_Group* o = new Fl_Group(x+10, y+25, w-20, 50);
      {
        vertexCuttingOffset_input = createValueInput(x+15, y+25, 50, 25,
                                                     0.0, 0.5, 0.01,
                                                     DLFLWindow::vertex_cutting_offset,
                                                     changeVertexCuttingOffset);
        vertexCuttingOffset_roller = createRoller(x+75, y+25, 165, 25,
                                                  "Offset:",
                                                  0.0, 0.5, 0.01,
                                                  DLFLWindow::vertex_cutting_offset,
                                                  changeVertexCuttingOffset);
      }
      o->end();
    }
    {
      Fl_Button* o = new Fl_Button((w-90)/2, y+90, 90, 50, "Perform\nRemeshing");
      o->callback((Fl_Callback*)perform_remeshing);
    }
    o->end(); o->hide();
  }

     // Pentagonal subdivision tile
  {
    Fl_Group* o = pentagonal_tile = new Fl_Group(x, y, w, h);
    o->color(48); o->selection_color(48);
    o->resizable(NULL);
    {
      Fl_Group* o = new Fl_Group(x+10, y+25, w-20, 50);
      {
        pentagonalOffset_input = createValueInput(x+15, y+25, 50, 25,
                                                  0.0, 1.0, 0.01,
                                                  DLFLWindow::pentagonal_offset,
                                                  changePentagonalOffset);
        pentagonalOffset_roller = createRoller(x+75, y+25, 165, 25,
                                               "Offset:",
                                               0.0, 1.0, 0.01,
                                               DLFLWindow::pentagonal_offset,
                                               changePentagonalOffset);
      }
      o->end();
    }
    {
      Fl_Button* o = new Fl_Button((w-90)/2, y+90, 90, 50, "Perform\nRemeshing");
      o->callback((Fl_Callback*)perform_remeshing);
    }
    o->end(); o->hide();
  }

     // Pentagonal2 subdivision tile
  {
    Fl_Group* o = pentagonal2_tile = new Fl_Group(x, y, w, h);
    o->color(48); o->selection_color(48);
    o->resizable(NULL);
    {
      Fl_Group* o = new Fl_Group(x+10, y+25, w-20, 50);
      {
        pentagonalScaleFactor_input = createValueInput(x+15, y+25, 50, 25,
                                                       0.0, 1.0, 0.01,
                                                       DLFLWindow::pentagonal_scale,
                                                       changePentagonalScaleFactor);
        pentagonalScaleFactor_roller = createRoller(x+75, y+25, 165, 25,
                                                    "Scale Factor:",
                                                    0.0, 1.0, 0.01,
                                                    DLFLWindow::pentagonal_scale,
                                                    changePentagonalScaleFactor);
      }
      o->end();
    }
    {
      Fl_Button* o = new Fl_Button((w-90)/2, y+90, 90, 50, "Perform\nRemeshing");
      o->callback((Fl_Callback*)perform_remeshing);
    }
    o->end(); o->hide();
  }

     // Dual tile - Is now created in DLFLAppWindow.cc since size is different

     // Star subdivision tile -- Doug
  {
    Fl_Group* o = star_tile = new Fl_Group(x, y, w, h);
    o->color(48); o->selection_color(48);
    o->resizable(NULL);
    {
      Fl_Group* o = new Fl_Group(x+10, y+25, w-20, 50);
      {
        starOffset_input = createValueInput(x+15, y+25, 50, 25,
                                            0.0, 1.0, 0.01,
                                            DLFLWindow::star_offset,
                                            changeStarOffset);
        starOffset_roller = createRoller(x+75, y+25, 165, 25,
                                         "Offset:",
                                         0.0, 1.0, 0.01,
                                         DLFLWindow::star_offset,
                                         changeStarOffset);
      }
      o->end();
    }
    {
      Fl_Button* o = new Fl_Button((w-90)/2, y+90, 90, 50, "Perform\nRemeshing");
      o->callback((Fl_Callback*)perform_remeshing);
    }
    o->end(); o->hide();
  }

     // Fractal subdivision tile -- Doug
  {
    Fl_Group* o = fractal_tile = new Fl_Group(x, y, w, h);
    o->color(48); o->selection_color(48);
    o->resizable(NULL);
    {
      Fl_Group* o = new Fl_Group(x+10, y+25, w-20, 50);
      {
        fractalOffset_input = createValueInput(x+15, y+25, 50, 25,
                                               0.0, 2.0, 0.01,
                                               DLFLWindow::fractal_offset,
                                               changeFractalOffset);
        fractalOffset_roller = createRoller(x+75, y+25, 165, 25,
                                            "Multiplier:",
                                            0.0, 2.0, 0.01,
                                            DLFLWindow::fractal_offset,
                                            changeFractalOffset);
      }
      o->end();
    }
    {
      Fl_Button* o = new Fl_Button((w-90)/2, y+90, 90, 50, "Perform\nRemeshing");
      o->callback((Fl_Callback*)perform_remeshing);
    }
    o->end(); o->hide();
  }

      // stellate2 subdivision tile - Eric
  {
    Fl_Group* o = substellate2_tile = new Fl_Group(x, y, w, h);
    o->color(48); o->selection_color(48);
    o->resizable(NULL);
    {
      Fl_Group* o = new Fl_Group(x+10, y+25, w-20, 50);
      {
        substellate_amount_input = createValueInput(x+15, y+25, 50, 25,
                                                    -1.0, 1.0, 0.01,
                                                    DLFLWindow::substellate_height,
                                                    changeSubStellateAmount);
        substellate_amount_roller = createRoller(x+75, y+25, 165, 25,
                                                 "Height:",
                                                 -1.0, 1.0, 0.01,
                                                 DLFLWindow::substellate_height,
                                                 changeSubStellateAmount);
      }
      o->end();
    }
    {
      Fl_Group* o = new Fl_Group(x+10, y+90, w-20, 50);
      {
        substellate_curve_input = createValueInput(x+15, y+90, 50, 25,
                                                   -1.0, 1.0, 0.01,
                                                   DLFLWindow::substellate_curve,
                                                   changeSubStellateCurve);
        substellate_curve_roller = createRoller(x+75, y+90, 165, 25,
                                                "Curve:",
                                                -1.0, 1.0, 0.01,
                                                DLFLWindow::substellate_curve,
                                                changeSubStellateCurve);
      }
      o->end();
    }
    {
      Fl_Button* o = new Fl_Button((w-90)/2, y+155, 90, 50, "Perform\nRemeshing");
      o->callback((Fl_Callback*)perform_remeshing);
    }
    o->end(); o->hide();
  }

     // Dome subdivision tile - Bei & Cansin
  {
    Fl_Group* o = dome_sub_tile = new Fl_Group(x, y, w, h);
    o->color(48); o->selection_color(48);
    o->resizable(NULL);
    {
      Fl_Group* o = new Fl_Group(x+10, y+25, w-20, 50);
      {
        domeSubdivisionLength_input = createValueInput(x+15, y+25, 50, 25,
                                                       0.0, 2.0, 0.01,
                                                       DLFLWindow::domeLength_factor,
                                                       changeDomeSubdivisionLength);
        domeSubdivisionLength_roller = createRoller(x+75, y+25, 165, 25,
                                                    "Length:",
                                                    0.0, 2.0, 0.01,
                                                    DLFLWindow::domeLength_factor,
                                                    changeDomeSubdivisionLength);
      }
      o ->end();
    }
    {
      Fl_Group* o = new Fl_Group(x+10, y+90, w-20, 50);
      {
        domeSubdivisionScale_input = createValueInput(x+15, y+90, 50, 25,
                                                      0.0, 2.0, 0.01,
                                                      DLFLWindow::domeScale_factor,
                                                      changeDomeSubdivisionScale);
        domeSubdivisionScale_roller = createRoller(x+75, y+90, 165, 25,
                                                   "Scale:",
                                                   0.0, 2.0, 0.01,
                                                   DLFLWindow::domeScale_factor,
                                                   changeDomeSubdivisionScale);
      }
      o-> end();
    }
    {
      Fl_Button* o = new Fl_Button((w-90)/2, y+155, 90, 50, "Perform\nRemeshing");
      o->callback((Fl_Callback*)perform_remeshing);
    }
    o -> end();
    o -> hide();
  }

     // SubHW subdivision tile - Bei & Cansin
  {
    Fl_Group* o = dual1264_tile = new Fl_Group(x, y, w, h);
    o->color(48); o->selection_color(48);
    o->resizable(NULL);
    {
      Fl_Group* o = new Fl_Group(x+10, y+25, w-20, 50) ;
      {
        dual1264Scale_input = createValueInput(x+15, y+25, 50, 25,
                                               0.0, 1.0, 0.01,
                                               DLFLWindow::dual1264_scale_factor,
                                               changeDual1264Scale);
        dual1264Scale_roller = createRoller(x+75, y+25, 165, 25,
                                            "Scale:",
                                            0.0, 1.0, 0.01,
                                            DLFLWindow::dual1264_scale_factor,
                                            changeDual1264Scale);
      }
      o-> end();
    }
    {
      Fl_Button* o = new Fl_Button((w-90)/2, y+90, 90, 50, "Perform\nRemeshing");
      o->callback((Fl_Callback*)perform_remeshing);
    }
    o -> end();
    o -> hide();
  }

     // Loop subdivision tile - Bei & Cansin
  {
    Fl_Group* o = loopstyle_sub_tile = new Fl_Group(x, y, w, h);
    o->color(48); o->selection_color(48);
    o->resizable(NULL);
    {
      Fl_Group* o = new Fl_Group(x+10, y+25, w-20, 50);
      {
        loopStyleSubdivisionLength_input = createValueInput(x+15, y+25, 50, 25,
                                                            0.0, 1.0, 0.01,
                                                            DLFLWindow::loopLength_factor,
                                                            changeLoopStyleSubdivisionLength);
        loopStyleSubdivisionLength_roller = createRoller(x+75, y+25, 165, 25,
                                                         "Length:",
                                                         0.0, 1.0, 0.01,
                                                         DLFLWindow::loopLength_factor,
                                                         changeLoopStyleSubdivisionLength);
      }
      o->end();
    }
    {
      Fl_Button* o = new Fl_Button((w-90)/2, y+90, 90, 50, "Perform\nRemeshing");
      o->callback((Fl_Callback*)perform_remeshing);
    }
    o->end();
    o->hide();
  }

     // Doo-Sabin(BC new) subdivision tile - Ben & Cansin
  {
    Fl_Group* o = doo_sabin_bc_new_tile = new Fl_Group(x, y, w, h);
    o->color(48); o->selection_color(48);
    o->resizable(NULL);
    {
      Fl_Group* o = new Fl_Group(x+10, y+25, w-20, 50) ;
      {
        dooSabinBCnewScale_input = createValueInput(x+15, y+25, 50, 25,
                                                    0.0, 2.0, 0.01,
                                                    DLFLWindow::dooSabinBCnewScale_factor,
                                                    changeDooSabinBCnewScale);
        dooSabinBCnewScale_roller = createRoller(x+75, y+25, 165, 25,
                                                 "Scale:",
                                                 0.0, 2.0, 0.01,
                                                 DLFLWindow::dooSabinBCnewScale_factor,
                                                 changeDooSabinBCnewScale);
      }
      o-> end();
    }
    {
      Fl_Group* o = new Fl_Group(x+10, y+90, w-20, 50) ;
      {
        dooSabinBCnewLength_input = createValueInput(x+15, y+90, 50, 25,
                                                     0.0, 1.0, 0.01,
                                                     DLFLWindow::dooSabinBCnewLength_factor,
                                                     changeDooSabinBCnewLength);
        dooSabinBCnewLength_roller = createRoller(x+75, y+90, 165, 25,
                                                  "Length:",
                                                  0.0, 1.0, 0.01,
                                                  DLFLWindow::dooSabinBCnewLength_factor,
                                                  changeDooSabinBCnewLength);
      }
      o-> end();
    }
    {
      Fl_Button* o = new Fl_Button((w-90)/2, y+155, 90, 50, "Perform\nRemeshing");
      o->callback((Fl_Callback*)perform_remeshing);
    }
    o->end();
    o->hide();
  }
}


/*
  $Log: DLFLAppWindowRemeshingTiles.cc,v $
  Revision 4.5  2004/01/20 22:19:01  vinod
  Added Loop and Dual Loop subdivision schemes

  Revision 4.4  2004/01/20 07:26:28  vinod
  Changed name of subhw to dual1264

  Revision 4.3  2004/01/20 01:00:47  vinod
  Changed names of some remeshing schemes in menus. Moved dual tile creation code to DLFLAppWindow.cc

  Revision 4.2  2004/01/19 20:24:02  vinod
  Moved remeshing tile definitions here.
  Added definitions for controls for schemes by Bei & Cansin.
  Added tiles for schemes by Bei & Cansin.

  Revision 4.1  2004/01/19 18:08:44  vinod
  Code for creating menus and tiles for remeshing. Extracted from DLFLAppWindow.cc and modified and extended.

*/
