/* $Id: DLFLAppWindow.hh,v 4.15 2004/01/20 22:18:20 vinod Exp $ */

#ifndef _DLFL_APP_WINDOW_HH_

#define _DLFL_APP_WINDOW_HH_

/*
  Main Application Window class for the DLFL demonstration program
*/

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Double_Window.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Tile.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/fl_file_chooser.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Roller.H>

#include "DLFLWindow.hh"
#include "DLFLObject.hh"
#include "ShadedRenderer.hh"
#include "NormalRenderer.hh"
#include "LitRenderer.hh"
#include "TexturedRenderer.hh"
#include "TexturedLitRenderer.hh"
#include "PatchRenderer.hh"

class DLFLAppWindow : public Fl_Double_Window
{
  protected :

        // Default sizes
     static int MIN_W;                 // Minimum width for app window
     static int MIN_H;                // Minimum height for app window
     static int OPT_W;                          // Options panel width
     static int MODE_TILE_H;                       // Mode tile height
     static int SUBGRP_TILE_H;      // Operation sub-group tile height
     static int MENU_H;                              // Menubar height
     static int STATUS_H;                      // Status window height

        // Renderers
     static ShadedRendererPtr shaded;                // ShadedRenderer
     static NormalRendererPtr normal;                // NormalRenderer
     static LitRendererPtr lit;                        // LitRenderer
     static TexturedRendererPtr textured;          // TexturedRenderer
     static TexturedLitRendererPtr texturedlit; // TexturedLitRenderer
     static PatchRendererPtr patch;		      // PatchRenderer
     
        // Control panels and controls
     static Fl_Menu_Bar * menubar;
     static Fl_Choice * basics;
     static Fl_Choice * extrude;
     static Fl_Choice * crust;
     static Fl_Choice * remeshing;
     static Fl_Choice * handles;
     static Fl_Choice * highgenus;
     static Fl_Choice * texturing;

     static Fl_Choice * mfh_algo_menu;

        // Remeshing menus
     static Fl_Choice * tri_remeshing_menu;
     static Fl_Choice * quad_remeshing_menu;
     static Fl_Choice * pent_remeshing_menu;
     static Fl_Choice * tri_preserving_menu;
     static Fl_Choice * quad_preserving_menu;
     static Fl_Choice * pent_preserving_menu;
     static Fl_Choice * hex_preserving_menu;
     static Fl_Choice * misc_remeshing_menu;

     static Fl_Group * control_panel;
     static Fl_Group * mode_panel;
     static Fl_Group * roller_panel;
     static Fl_Group * remeshing_subgrp_panel;
     static Fl_Group * mode_sel_tile;
     static Fl_Group * blank;

     static Fl_Group * delete_edge_tile;
     static Fl_Group * subdivide_edge_tile;
     static Fl_Group * subdivide_face_tile;
     static Fl_Group * tile_texturing_tile;
     static Fl_Group * transforms_tile;
     static Fl_Group * extrude_tile;
     static Fl_Group * stellate_tile;
     static Fl_Group * crust_modeling1_tile;
     static Fl_Group * crust_modeling2_tile;
     static Fl_Group * wireframe_modeling_tile;
     static Fl_Group * column_modeling_tile; // Esan

     static Fl_Group * generic_remeshing_tile;
     static Fl_Group * doo_sabin_tile;
     static Fl_Group * root_4_tile;
     static Fl_Group * vertex_cutting_tile;
     static Fl_Group * pentagonal_tile;
     static Fl_Group * pentagonal2_tile;
     static Fl_Group * dual_tile;
     static Fl_Group * star_tile; // Doug
     static Fl_Group * fractal_tile; // Doug
     static Fl_Group * substellate2_tile; // Eric
     static Fl_Group * doo_sabin_bc_new_tile; // Bei & Cansin
     static Fl_Group * loopstyle_sub_tile; // Bei & Cansin
     static Fl_Group * dual1264_tile; // Bei & Cansin
     static Fl_Group * dome_sub_tile; // Bei & Cansin

     static Fl_Group * hole_tile;
     static Fl_Group * handle_tile;
     static Fl_Group * multi_face_handle_tile;
     static Fl_Group * sierpinsky_tile;
     static Fl_Group * mengersponge_tile;

     static Fl_Value_Input * numSubDivs_input;
     static Fl_Value_Input * tileTexNum_input;
     static Fl_Value_Input * extrudeLength_input;
     static Fl_Value_Input * extrudeRotation_input;
     static Fl_Value_Input * extrudeScale_input;
     static Fl_Value_Input * stellateLength_input;
     static Fl_Value_Input * numExtrusions_input;
     static Fl_Value_Input * numSegments_input;
     static Fl_Value_Input * maxSegments_input;
     static Fl_Value_Input * numSegments2_input;
     static Fl_Value_Input * maxSegments2_input;
     static Fl_Value_Input * weight1_input;
     static Fl_Value_Input * weight2_input;
     static Fl_Value_Input * extraTwists_input;
     static Fl_Value_Input * multiFaceHandleScaleFactor_input;
     static Fl_Value_Input * multiFaceHandleExtrudeDist_input;
     static Fl_Value_Input * crustScaleFactor_input;
     static Fl_Value_Input * crustThickness_input;
     static Fl_Value_Input * wireframeThickness_input;
     static Fl_Value_Input * columnThickness_input; // Esan
     static Fl_Value_Input * columnSegments_input; // Esan
     static Fl_Value_Input * root4Twist_input;
     static Fl_Value_Input * root4Weight_input;
     static Fl_Value_Input * vertexCuttingOffset_input;
     static Fl_Value_Input * pentagonalOffset_input;
     static Fl_Value_Input * pentagonalScaleFactor_input;
     static Fl_Value_Input * starOffset_input; // Doug
     static Fl_Value_Input * fractalOffset_input; // Doug
     static Fl_Value_Input * substellate_amount_input; // Eric
     static Fl_Value_Input * substellate_curve_input; // Eric
     static Fl_Value_Input * dooSabinBCnewScale_input; // Bei & Cansin
     static Fl_Value_Input * dooSabinBCnewLength_input; // Bei & Cansin
     static Fl_Value_Input * loopStyleSubdivisionLength_input; // Bei & Cansin
     static Fl_Value_Input * dual1264Scale_input; // Bei & Cansin
     static Fl_Value_Input * domeSubdivisionScale_input; // Bei & Cansin
     static Fl_Value_Input * domeSubdivisionLength_input; // Bei & Cansin

     static Fl_Value_Input * mengerSpongeThickness_input;
     static Fl_Value_Input * mengerSpongeCollapseThreshold_input;
     static Fl_Value_Input * translatex_input;
     static Fl_Value_Input * translatey_input;
     static Fl_Value_Input * translatez_input;
     static Fl_Value_Input * scalex_input;
     static Fl_Value_Input * scaley_input;
     static Fl_Value_Input * scalez_input;

     static Fl_Roller * numSubDivs_roller;
     static Fl_Roller * tileTexNum_roller;
     static Fl_Roller * extrudeLength_roller;
     static Fl_Roller * extrudeRotation_roller;
     static Fl_Roller * extrudeScale_roller;
     static Fl_Roller * numExtrusions_roller;
     static Fl_Roller * stellateLength_roller;
     static Fl_Roller * numSegments_roller;
     static Fl_Roller * maxSegments_roller;
     static Fl_Roller * numSegments2_roller;
     static Fl_Roller * maxSegments2_roller;
     static Fl_Roller * weight1_roller;
     static Fl_Roller * weight2_roller;
     static Fl_Roller * extraTwists_roller;
     static Fl_Roller * multiFaceHandleScaleFactor_roller;
     static Fl_Roller * multiFaceHandleExtrudeDist_roller;
     static Fl_Roller * crustScaleFactor_roller;
     static Fl_Roller * crustThickness_roller;
     static Fl_Roller * wireframeThickness_roller;
     static Fl_Roller * columnThickness_roller; // Esan
     static Fl_Roller * columnSegments_roller; // Esan
     static Fl_Roller * root4Twist_roller;
     static Fl_Roller * root4Weight_roller;
     static Fl_Roller * vertexCuttingOffset_roller;
     static Fl_Roller * pentagonalOffset_roller;
     static Fl_Roller * pentagonalScaleFactor_roller;
     static Fl_Roller * starOffset_roller; // Doug
     static Fl_Roller * fractalOffset_roller; // Doug
     static Fl_Roller * substellate_amount_roller; // Eric
     static Fl_Roller * substellate_curve_roller; // Eric
     static Fl_Roller * dooSabinBCnewScale_roller; // Bei & Cansin
     static Fl_Roller * dooSabinBCnewLength_roller; // Bei & Cansin
     static Fl_Roller * loopStyleSubdivisionLength_roller; // Bei & Cansin
     static Fl_Roller * dual1264Scale_roller; // Bei & Cansin
     static Fl_Roller * domeSubdivisionScale_roller; // Bei & Cansin
     static Fl_Roller * domeSubdivisionLength_roller; // Bei & Cansin

     static Fl_Roller * mengerSpongeThickness_roller;
     static Fl_Roller * mengerSpongeCollapseThreshold_roller;
     static Fl_Roller * translatex_roller;
     static Fl_Roller * translatey_roller;
     static Fl_Roller * translatez_roller;
     static Fl_Roller * scalex_roller;
     static Fl_Roller * scaley_roller;
     static Fl_Roller * scalez_roller;

     static Fl_Check_Button * deleteEdgeCleanupCheck_button;
     static Fl_Check_Button * symmetricWeightsCheck_button;
     static Fl_Check_Button * dooSabinEdgeCheck_button;
     static Fl_Check_Button * accurateDualCheck_button;
     static Fl_Check_Button * dualMeshEdgesCheck_button;
     static Fl_Check_Button * useQuadsCheck_button;
     static Fl_Check_Button * multiFaceHandleUseMaxOffsetCheck_button;
     static Fl_Check_Button * mengerSpongeFractionalThicknessCheck_button;
     static Fl_Check_Button * crustCleanupCheck_button1;
     static Fl_Check_Button * crustCleanupCheck_button2;
     static Fl_Check_Button * hexagonalizeDodecaExtrudeCheck_button;

     DLFLWindow mainWindow;            // Window displaying the object
     static DLFLWindowPtr mainWindowPtr;               // Pointer to the main window

  public :

        // Create/destroy the control panels and the controls
     static void createModeSelectionPanel(int x, int y, int w, int h);
     static void createBasicModeTiles(int x, int y, int w, int h);
     static void createExtrusionModeTiles(int x, int y, int w, int h);
     static void createTexturingModeTiles(int x, int y, int w, int h);
     static void createRemeshingMenus(int x, int y, int w, int h);
     static void createRemeshingTiles(int x, int y, int w, int h);
     static void createHighGenusModeTiles(int x, int y, int w, int h);
     static void createRollerPanel(int x, int y, int w, int h);
     static void createControlWidgets(int w, int h);
     static void destroyControlWidgets(void);

        // Create the menubar
     static void createMenubar(int w, int h);

        // Create the renderers
     static void createRenderers(void)
       {
         shaded = new ShadedRenderer();
         shaded->setRenderFlags(DLFLRenderer::ShowWireframe);
         
         normal = new NormalRenderer();
         normal->setRenderFlags(DLFLRenderer::ShowWireframe);
         
         lit = new LitRenderer();
         lit->setRenderFlags(DLFLRenderer::ShowWireframe);
         
         textured = new TexturedRenderer();
         textured->setRenderFlags(DLFLRenderer::ShowWireframe);
         
         texturedlit = new TexturedLitRenderer();
         texturedlit->setRenderFlags(DLFLRenderer::ShowWireframe);
					patch = new PatchRenderer();
				 patch->setRenderFlags(DLFLRenderer::ShowWireframe);
       }

     static void destroyRenderers(void)
       {
         delete shaded;
         delete normal;
         delete lit;
         delete textured;
         delete texturedlit;
	 			delete patch;
       }

     static void setMainWindow(DLFLWindowPtr wp)
       {
         mainWindowPtr = wp;
       }

     static DLFLWindowPtr getMainWindow(void)
       {
         return mainWindowPtr;
       }

        // Constructor
     DLFLAppWindow(const char * title=0)
       : Fl_Double_Window(MIN_W,MIN_H,title), mainWindow(OPT_W,MENU_H,MIN_W-OPT_W,MIN_H-MENU_H)
       {
         size_range(MIN_W,MIN_H); // Set minimum size for this window
         mainWindow.box(FL_BORDER_BOX);
         mainWindow.end();
         DLFLAppWindow::createMenubar(MIN_W,MIN_H);
         DLFLAppWindow::createControlWidgets(MIN_W,MIN_H);
         this->resizable(mainWindow);
         this->end();

         DLFLAppWindow::createRenderers();
         mainWindow.setRenderer(lit); // Default renderer is LightedRenderer

         DLFLAppWindow::setMainWindow(&mainWindow);
       }

        // Destructor
     ~DLFLAppWindow()
       {
         DLFLAppWindow::destroyControlWidgets();
         DLFLAppWindow::destroyRenderers();
         delete menubar;
            // delete mainWindow;
       }

        // Open an object from a file
     void openFile(const char * filename)
       {
         mainWindow.readObject(filename);
         mainWindow.recomputePatches();
         mainWindow.recomputeNormals();
         mainWindow.redraw();
       }

        // Open an object from a file using the alternate OBJ reader
     void openFileAlt(const char * filename)
       {
         mainWindow.readObjectAlt(filename);
         mainWindow.redraw();
       }

        // Read a texture file for use by the renderers which do texturing
     void readTexture(const char * filename)
       {
         textured->setTexture(filename);
         texturedlit->setTexture(filename);
       }

        // override show() method to show subwindows also
     void show(int argc, char ** argv)
       {
         Fl_Double_Window::show(argc,argv);
         mainWindow.show();
       }
     
     void show(void)
       {
         Fl_Double_Window::show();
         mainWindow.show();
       }

     void resize(int x, int y, int w, int h)
       {
         Fl_Double_Window::resize(x,y,w,h);
         if ( menubar ) menubar->resize(0,0,w,MENU_H);
         if ( control_panel ) control_panel->resize(0,MENU_H,OPT_W,h-MENU_H);
         mainWindow.resize(OPT_W,MENU_H,w-OPT_W,h-MENU_H);
       }

     int handle(int event)
       {
         if ( event == FL_KEYBOARD ) return mainWindow.handle(event);
         return Fl_Double_Window::handle(event);
       }

        //--- Callbacks for the controls and menu items ---//
        //--- The source for these methods are in DLFLAppWindowCallbacks.cc ---//
        // The DLFLAppWindow instance initiating the callback
        // is assumed to be specified through the user_data parameter

     static void hide_modes();
     static void hide_tiles();
     static void open_file(Fl_Menu_*, void*);
     static void save_file(Fl_Menu_*, void*);
     static void save_file_with_normals(Fl_Menu_*, void*);
     static void save_file_with_tex_coords(Fl_Menu_*, void*);
     static void save_file_extended(Fl_Menu_*, void*);
     static void load_texture(Fl_Menu_*, void*);
     static void print_info(Fl_Menu_*, void*);
     static void print_flist(Fl_Menu_*, void*);
     static void print_vlist(Fl_Menu_*, void*);
     static void print_elist(Fl_Menu_*, void*);
     static void quit(Fl_Menu_*, void*);

     static void undo(Fl_Menu_*, void*);
     static void redo(Fl_Menu_*, void*);

     static void toggle_vertices(Fl_Menu_*, void*);
     static void toggle_silhouette(Fl_Menu_*, void*);
     static void toggle_wireframe(Fl_Menu_*, void*);
     static void toggle_grid(Fl_Menu_*, void*);
     static void toggle_axes(Fl_Menu_*, void*);

     static void use_normal_renderer(Fl_Menu_*, void*);
     static void use_lighted_renderer(Fl_Menu_*, void*);
     static void use_shaded_renderer(Fl_Menu_*, void*);
     static void use_textured_renderer(Fl_Menu_*, void*);
     static void use_textured_lit_renderer(Fl_Menu_*, void*);
     static void use_patch_renderer(Fl_Menu_*, void*);
     
     static void subdivide_all_edges(Fl_Menu_*, void*);
     static void planarize_all_faces(Fl_Menu_*, void*);
     static void make_object_spherical(Fl_Menu_*, void*);
     static void make_object_smooth(Fl_Menu_*, void*);
     static void create_crust(Fl_Menu_*, void*);
     static void compute_lighting(Fl_Menu_*, void*);
     static void compute_normals_and_lighting(Fl_Menu_*, void*);
     static void assign_texture_coordinates(Fl_Menu_*, void*);

     static void select_vertex(Fl_Menu_*, void*);
     static void select_face(Fl_Menu_*, void*);
     static void select_edge(Fl_Menu_*, void*);
     static void select_corner(Fl_Menu_*, void*);
     static void exit_selection_mode(Fl_Menu_*, void*);

     static void exit_modes(Fl_Menu_*, void*);
     static void normal_mode(Fl_Menu_*, void*);
     static void basics_mode(Fl_Menu_*, void*);
     static void extrude_mode(Fl_Menu_*, void*);
     static void handles_mode(Fl_Menu_*, void*);
     static void crust_mode(Fl_Menu_*, void*);
     static void remeshing_mode(Fl_Menu_*, void*);
     static void highgenus_mode(Fl_Menu_*, void*);
     static void texturing_mode(Fl_Menu_*, void*);
     static void hide_remeshing_tiles(Fl_Menu_*, void*);

     static void insert_edge_mode(Fl_Menu_*, void*);
     static void delete_edge_mode(Fl_Menu_*, void*);
     static void collapse_edge_mode(Fl_Menu_*, void*);
     static void subdivide_edge_mode(Fl_Menu_*, void*);
     static void subdivide_face_mode(Fl_Menu_*, void*);
     static void subdivide_face(Fl_Menu_*, void*);
     static void tile_texturing_mode(Fl_Menu_*, void*);
     static void assign_tile_texture_coords(Fl_Menu_*, void*);
     static void connect_edge_mode(Fl_Menu_*, void*);
     static void transforms_mode(Fl_Menu_*, void*);
     static void splice_corners_mode(Fl_Menu_*, void*);
     
     static void extrude_face(Fl_Menu_*, void*);
     static void extrude_face_doosabin(Fl_Menu_*, void*);
     static void extrude_face_dodeca(Fl_Menu_*, void*);
     static void extrude_face_icosa(Fl_Menu_*, void*);
     static void extrude_dual_face(Fl_Menu_*, void*);
     static void stellate_face(Fl_Menu_*, void*);
     static void double_stellate_face(Fl_Menu_*, void*);

     static void crust_modeling_mode1(Fl_Menu_*, void*);
     static void crust_modeling1(Fl_Menu_*, void*);
     static void crust_modeling_mode2(Fl_Menu_*, void*);
     static void crust_modeling2(Fl_Menu_*, void*);
     static void wireframe_modeling_mode(Fl_Menu_*, void*);
     static void wireframe_modeling(Fl_Menu_*, void*);
     static void column_modeling_mode(Fl_Menu_*, void*); // Esan
     static void column_modeling(Fl_Menu_*, void*); // Esan

     static void tri_conversion(Fl_Menu_*, void*);
     static void quad_conversion(Fl_Menu_*, void*);
     static void pent_conversion(Fl_Menu_*, void*);
     static void tri_preservation(Fl_Menu_*, void*);
     static void quad_preservation(Fl_Menu_*, void*);
     static void pent_preservation(Fl_Menu_*, void*);
     static void hex_preservation(Fl_Menu_*, void*);
     static void misc_remeshing(Fl_Menu_*, void*);

     static void perform_remeshing(Fl_Menu_*, void*);
     static void root3_subdivision(Fl_Menu_*, void*);
     static void sqrt3_subdivision(Fl_Menu_*, void*); // Doug
     static void global_stellation(Fl_Menu_*, void*);
     static void star_subdivision(Fl_Menu_*, void*); // Doug
     static void generic1264_subdivision(Fl_Menu_*, void*);
     static void honeycomb_subdivision(Fl_Menu_*, void*);
     static void vertex_cutting_subdivision(Fl_Menu_*, void*);
     static void dual_generic1264_subdivision(Fl_Menu_*, void*);
     static void vertex_insertion_subdivision(Fl_Menu_*, void*);
     static void catmull_clark_subdivision(Fl_Menu_*, void*);
     static void stellate_subdivision(Fl_Menu_*, void*); // Eric
     static void doo_sabin_subdivision(Fl_Menu_*, void*);
     static void corner_cutting_subdivision(Fl_Menu_*, void*);
     static void simplest_subdivision(Fl_Menu_*, void*);
     static void pentagonal_subdivision(Fl_Menu_*, void*);
     static void cubic_pentagonal_subdivision(Fl_Menu_*, void*);
     static void dual_pentagonal_subdivision(Fl_Menu_*, void*);
     static void loop_style_subdivision(Fl_Menu_*, void*);
     static void loop_subdivision(Fl_Menu_*, void*);
     static void dual_loop_subdivision(Fl_Menu_*, void*);
     static void root_4_subdivision(Fl_Menu_*, void*);
     static void global_extrusion(Fl_Menu_*, void*);
     static void checkerboard_remeshing(Fl_Menu_*, void*);
     static void dual_global_extrusion(Fl_Menu_*, void*);
     static void dual_checkerboard_remeshing(Fl_Menu_*, void*);
     static void pentagonal2_subdivision(Fl_Menu_*, void*);
     static void dual_pentagonal2_subdivision(Fl_Menu_*, void*);
     static void hexagon_preserving_remeshing(Fl_Menu_*, void*);
     static void dual_hexagon_preserving_remeshing(Fl_Menu_*, void*);
     static void fractal_subdivision(Fl_Menu_*, void*); // Doug
     static void twostellate_subdivision(Fl_Menu_*, void*); // Eric
     static void dual(Fl_Menu_*, void*);
     static void doo_sabin_subdivision_bc(Fl_Menu_*, void*); // Bei & Cansin
     static void doo_sabin_subdivision_bc_new(Fl_Menu_*, void*); // Bei & Cansin
     static void dual1264_subdivision(Fl_Menu_*, void*); // Bei & Cansin
     static void dome_subdivision(Fl_Menu_*, void*); // Bei & Cansin

     static void add_hole_mode1(Fl_Menu_*, void*);
     static void add_hole_mode2(Fl_Menu_*, void*);
     static void add_handle_mode(Fl_Menu_*, void*);
     static void multi_face_handle_mode(Fl_Menu_*, void*);
     static void create_multi_face_handle(Fl_Menu_*, void*);
     static void mfh_use_closest_edge_algo(Fl_Menu_*, void*);
     static void mfh_use_convex_hull_algo(Fl_Menu_*, void*);

     static void sierpinsky_mode(Fl_Menu_*, void*);
     static void create_sierpinsky_tetrahedra(Fl_Menu_*, void*);
     static void mengersponge_mode(Fl_Menu_*, void*);
     static void create_menger_sponge(Fl_Menu_*, void*);

     static void toggleDeleteEdgeCleanupFlag(Fl_Widget * widget, void * data);

     static void changeNumSubDivs(Fl_Widget * widget, void * data);
     static void changeTileTexNum(Fl_Widget * widget, void * data);
     static void toggleUseQuadsFlag(Fl_Widget * widget, void * data);

     static void changeExtrudeLength(Fl_Widget * widget, void * data);
     static void changeExtrudeRotation(Fl_Widget * widget, void * data);
     static void changeExtrudeScale(Fl_Widget * widget, void * data);
     static void changeNumExtrusions(Fl_Widget * widget, void * data);
     static void changeStellateLength(Fl_Widget * widget, void * data);
     static void toggleDualMeshEdgesFlag(Fl_Widget * widget, void * data);
     static void toggleHexagonalizeDodecaExtrudeFlag(Fl_Widget * widget, void * data);

     static void changeNumSegments(Fl_Widget * widget, void * data);
     static void changeMaxSegments(Fl_Widget * widget, void * data);
     static void changeNumSegments2(Fl_Widget * widget, void * data);
     static void changeMaxSegments2(Fl_Widget * widget, void * data);
     static void changeWeight1(Fl_Widget * widget, void * data);
     static void toggleSymmetricWeightsFlag(Fl_Widget * widget, void * data);
     static void changeWeight2(Fl_Widget * widget, void * data);
     static void changeExtraTwists(Fl_Widget * widget, void * data);

     static void changeMultiFaceHandleScaleFactor(Fl_Widget * widget, void * data);
     static void changeMultiFaceHandleExtrudeDist(Fl_Widget * widget, void * data);
     static void toggleMultiFaceHandleUseMaxOffsetFlag(Fl_Widget * widget, void * data);

     static void changeCrustScaleFactor(Fl_Widget * widget, void * data);
     static void changeCrustThickness(Fl_Widget * widget, void * data);
     static void toggleCrustCleanupFlag(Fl_Widget * widget, void * data);

     static void changeWireframeThickness(Fl_Widget * widget, void * data);

     static void changeColumnThickness(Fl_Widget * widget, void * data); // Esan
     static void changeColumnSegments(Fl_Widget * widget, void * data); // Esan

     static void toggleDooSabinEdgeFlag(Fl_Widget * widget, void * data);

     static void changeRoot4Twist(Fl_Widget * widget, void * data);
     static void changeRoot4Weight(Fl_Widget * widget, void * data);

     static void changeVertexCuttingOffset(Fl_Widget * widget, void * data);

     static void changePentagonalOffset(Fl_Widget * widget, void * data);
     static void changePentagonalScaleFactor(Fl_Widget * widget, void * data);

     static void changeStarOffset(Fl_Widget * widget, void * data); // Doug

     static void changeFractalOffset(Fl_Widget * widget, void * data); // Doug

     static void changeSubStellateAmount(Fl_Widget * widget, void * data); // Eric
     static void changeSubStellateCurve(Fl_Widget * widget, void * data); // Eric

     static void changeDooSabinBCnewScale(Fl_Widget * widget, void * data); // Bei & Cansin
     static void changeDooSabinBCnewLength(Fl_Widget * widget, void * data); // Bei & Cansin
     static void changeLoopStyleSubdivisionLength(Fl_Widget * widget, void * data); // Bei & Cansin
     static void changeDual1264Scale(Fl_Widget * widget, void * data); // Bei & Cansin
     static void changeDomeSubdivisionLength(Fl_Widget * widget, void * data); // Bei & Cansin
     static void changeDomeSubdivisionScale(Fl_Widget * widget, void * data); // Bei & Cansin

     static void toggleAccurateDualFlag(Fl_Widget * widget, void * data);

     static void changeMengerSpongeThickness(Fl_Widget * widget, void * data);
     static void toggleMengerSpongeFractionalThicknessFlag(Fl_Widget * widget, void * data);
     static void changeMengerSpongeCollapseThreshold(Fl_Widget * widget, void * data);

     static void translatex(Fl_Widget * widget, void * data);
     static void translatey(Fl_Widget * widget, void * data);
     static void translatez(Fl_Widget * widget, void * data);
     static void scalex(Fl_Widget * widget, void * data);
     static void scaley(Fl_Widget * widget, void * data);
     static void scalez(Fl_Widget * widget, void * data);
     static void freeze_transforms(Fl_Widget*, void*);
};


#endif /* #ifndef _DLFL_APP_WINDOW_HH_ */


/*
  $Log: DLFLAppWindow.hh,v $
  Revision 4.15  2004/01/20 22:18:20  vinod
  Added loop and dual_loop subdivision schemes

  Revision 4.14  2004/01/20 09:29:03  vinod
  Added variables and callbacks for column modeling.
  Added callbacks for undo and redo from menubar

  Revision 4.13  2004/01/20 07:25:41  vinod
  Changed name of subhw to dual1264

  Revision 4.12  2004/01/20 00:58:30  vinod
  Renamed some remeshing schemes.
  Normals are now computed when file is read.

  Revision 4.11  2004/01/19 20:22:00  vinod
  Added tiles, controls and callbacks for schemes by Bei & Cansin

  Revision 4.10  2004/01/19 18:06:39  vinod
  Reorganized remeshing menus and tiles. Added new schemes.

  Revision 4.9  2004/01/13 09:20:01  vinod
  Added controls and callbacks for scale factor
  for pentagon preserving remeshing scheme

  Revision 4.8  2004/01/13 02:49:09  vinod
  Added tiles, controls and callbacks for:
  stellate and double stellate remeshing schemes
  double stellation, dodecahedral and icosahedral extrusions

  Revision 4.7  2004/01/12 20:10:53  vinod
  Removed unused callbacks. Added methods for creating tiles for specific modes

  Revision 4.6  2004/01/12 05:00:34  vinod
  Removed unused widgets.

  Revision 4.5  2004/01/09 14:54:45  vinod
  Changed names of toggle button callbacks.

  Revision 4.4  2004/01/02 21:02:45  vinod
  Additions by Doug: Tiles, control widgets and callbacks for star, sqrt3 and fractal subdivision schemes

  Revision 4.3  2004/01/02 19:44:29  vinod
  Added collapse_edge mode callback

  Revision 4.2  2004/01/01 07:51:35  vinod
  Reorganized menus. Renamed some menu items

  Revision 4.1  2003/12/28 05:29:25  vinod
  Added tile for delete edge. Added check button for
  delete edge cleanup flag toggle. Removed second delete edge mode.

  Revision 4.0  2003/12/26 01:58:52  vinod
  Major version sync.

  Revision 3.10  2003/12/26 01:49:49  vinod
  Combined roller and input callbacks. Added separate texturing mode.

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

  Revision 3.3  2003/11/23 22:44:56  vinod
  Added controls for menger sponge edge collapse threshold

  Revision 3.2  2003/11/21 15:26:55  vinod
  Added check button for fractional thickness toggle for menger sponge creation

  Revision 3.1  2003/11/19 16:10:27  vinod
  Added High Genus group and controls for sierpinsky and menger sponge

  Revision 3.0  2003/09/19 16:06:47  vinod
  Major version update

  Revision 2.6  2003/09/06 19:50:39  vinod
  Added callbacks for multi-face-handle algorithm choice menu

  Revision 2.5  2003/08/24 01:52:45  vinod
  Changed all sliders to rollers

  Revision 2.4  2003/07/07 16:12:41  vinod
  Added check button for use_max_offsets for multi-connect

  Revision 2.3  2003/04/25 15:47:03  vinod
  Added widgets and callbacks for multi-face handle operations

  Revision 2.2  2003/04/14 16:51:05  vinod
  Added widgets for multi-face handle creation

  Revision 2.1  2003/02/01 19:04:04  vinod
  Changed save option to be submenu allowing save with multiple options

  Revision 2.0  2003/01/25 08:49:31  vinod
  *** empty log message ***

  Revision 1.21  2003/01/25 08:10:25  vinod
  Added subdivide_face method as button callback

  Revision 1.20  2003/01/20 21:43:35  vinod
  Name change from skeleton modeling to wireframe modeling

  Revision 1.19  2003/01/05 21:10:28  vinod
  Added controls and variables for skeleton modeling

  Revision 1.18  2002/12/18 16:21:10  vinod
  Added face subdivision and tile texturing

  Revision 1.17  2002/12/17 03:41:45  vinod
  Added separate controls and callbacks for stellation length. Fixed comment logs

  Revision 1.16  2002/12/17 03:18:37  vinod
  See comments for previous revision

  Revision 1.15  2002/12/17 03:11:07  vinod
  Additions by Paul
  Tiles for stellateFace, extrudeDualFace, vertex cutting subdivide
  pentagonal subdivide
  Sliders for controls for extrudeDualFace, stellateFace

  Revision 1.14  2002/12/17 02:44:59  vinod
  DLFLWindow.hh

  Revision 1.13  2002/12/14 21:00:05  vinod
  Added openFileAlt method

  Revision 1.12  2002/11/07 08:20:18  vinod
  Added check button for edge check flag for dooSabin subdivision

  Revision 1.11  2002/10/31 06:08:19  vinod
  Added controls and callbacks for simplest subdivision scheme

  Revision 1.10  2002/10/30 03:44:26  vinod
  Added method and callback to load textures

  Revision 1.9  2002/10/28 06:51:08  vinod
  Reorganized control panel widgets. Changed base class to Fl_Double_Window

  Revision 1.8  2002/10/25 15:33:37  vinod
  Removed unnecessary include of fstream

  Revision 1.7  2002/10/25 05:32:29  vinod
  Added openFile method. No default object is read

  Revision 1.6  2002/10/25 02:54:37  vinod
  Change fstream.h to fstream as per new C++ standards

  Revision 1.5  2002/10/22 21:20:48  vinod
  Added renderers, changed Fl_Tile to Fl_Group
  Added widgets and callbacks for dual. Added handle method to send keyboard events to DLFLWindow

  Revision 1.4  2002/10/17 02:20:15  vinod
  Made widgets and callbacks static. Added static pointer to main window

  Revision 1.3  2002/10/16 14:14:08  vinod
  Added callbacks, etc.

  Revision 1.2  2002/10/08 16:15:53  vinod
  Added overrides for show and resize methods

  Revision 1.1  2002/10/07 08:25:07  vinod
  DLFL Application Window

*/
