/* $Id: DLFLWindow.cc,v 4.14 2004/04/18 18:19:54 vinod Exp $ */

// Source code for DLFLWindow class

#include "DLFLWindow.hh"

//-- Parameters used in various operations on the DLFL object --//
//-- See header file for explanations --//

int DLFLWindow :: drag_startx = 0;
int DLFLWindow :: drag_starty = 0;
bool DLFLWindow :: is_editing = false;

        // Edge deletion
bool DLFLWindow :: delete_edge_cleanup = true;

        // Handles
int DLFLWindow :: num_segments = 4;
int DLFLWindow :: max_segments = -1;
bool DLFLWindow :: symmetric_weights = true;
double DLFLWindow :: nwt1 = 5.0;
double DLFLWindow :: nwt2 = 5.0;
int DLFLWindow :: num_extra_twists = 0;

        // Extrusions
double DLFLWindow :: extrude_dist = 1.0;
double DLFLWindow :: extrude_rot = 0.0;
double DLFLWindow :: extrude_scale = 1.0;
int DLFLWindow :: num_extrusions = 1;
double DLFLWindow :: ds_ex_twist = 0.0;
bool DLFLWindow :: dual_mesh_edges_check = false;
bool DLFLWindow :: hexagonalize_dodeca_extrude = false;

        // Edge subdivision
int DLFLWindow :: num_e_subdivs = 2;

        // Split valence 2 vertices
double DLFLWindow :: vertex_split_offset=-0.1;

        // Crust modeling
double DLFLWindow :: crust_thickness = 0.5;
double DLFLWindow :: crust_scale_factor = 0.9;
bool DLFLWindow :: crust_cleanup = true;

        // Wireframe modeling
double DLFLWindow :: wireframe_thickness = 0.25;

        // Column modeling
double DLFLWindow :: column_thickness = 0.25;
int DLFLWindow :: column_segments = 4;

        // Subdivision
double DLFLWindow :: twist_factor = 0.0;
double DLFLWindow :: weight_factor = 0.0;
bool DLFLWindow :: doo_sabin_check = true;
double DLFLWindow :: vertex_cutting_offset = 0.25;
double DLFLWindow :: pentagonal_offset = 0.0;
double DLFLWindow :: pentagonal_scale = 0.75;
bool DLFLWindow :: accurate_dual = true;
double DLFLWindow :: checkerboard_thickness = 0.33;

       // Added by Doug
double DLFLWindow :: star_offset = 0.0;
double DLFLWindow :: fractal_offset = 1.0;

       // Added by Eric
double DLFLWindow :: substellate_height = 0.0;
double DLFLWindow :: substellate_curve = 0.0;

       // Added by bei&cansin dec 09
double DLFLWindow :: domeLength_factor = 1.0 ;
double DLFLWindow :: domeScale_factor = 1.0  ;
double DLFLWindow :: dual1264_scale_factor = 0.7 ;

       // Added by bei dec 11
double DLFLWindow :: dooSabinBCnewScale_factor = 1.0;
double DLFLWindow :: dooSabinBCnewLength_factor = 1.0;

       // Added by bei 12-11-03
double DLFLWindow :: loopLength_factor = 1.0;

       // Face subdivision
bool DLFLWindow :: use_quads = true;

        // Tile texturing
int DLFLWindow :: tile_tex_n = 2;

        // Multi-face handles
DLFLWindow::MFHAlgo DLFLWindow :: mfh_algo = DLFLWindow::ConvexHull;
double DLFLWindow :: mfh_scale_factor = 1.0;
double DLFLWindow :: mfh_extrude_dist = 0.5;
bool DLFLWindow :: mfh_use_max_offsets = false;
double DLFLWindow :: mfh_min_adj_factor = 0.01;
bool DLFLWindow :: mfh_make_connections = true;

        // Menger Sponge creation
double DLFLWindow :: sponge_thickness = 0.67; // Thickness for Menger Sponge
double DLFLWindow :: sponge_collapse_threshold = 0.0; // Threshold factor for edge collapse
bool DLFLWindow :: sponge_fractional_thickness = false; // Thickness is fractional

//-- Selection count, used as index into selection lists --//
int DLFLWindow :: num_sel_verts = 0;
int DLFLWindow :: num_sel_edges = 0;
int DLFLWindow :: num_sel_faces = 0;
int DLFLWindow :: num_sel_faceverts = 0;

void DLFLWindow :: doDrag(int x, int y)
{
  int drag_endx = Fl::event_x();
  int drag_endy = h()-Fl::event_y();

  GLdouble obj_world[3],  // Object world coordinates
           obj_window[3], // Object window coordinates 
           ms_window[3],  // Mouse start drag window
           ms_world[3],   // Mouse start drag world
           me_window[3],  // Mouse end drag window
           me_world[3];   // Mouse end drag world
  GLdouble modelMatrix[16], projMatrix[16];
  GLint viewport[4];
  GLint realy;
  DLFLVertexPtr vptr;
  Viewport* viewp;

  switch ( mode )
  {
    case EditMode:
      if (DLFLViewport::numSelectedLocators() > 0)
      {
        if (!is_editing)
        {
          undoPush();
          is_editing = true;
        }

        vptr = active->getLocatorPtr()->getActiveVertex();

        // Save previous transformations
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
  
        // Apply current transformation
        viewp = active->getViewport();
        viewp->reshape();
        viewp->apply_transform();
  
        // Get the info
        glGetIntegerv(GL_VIEWPORT, viewport);
        glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
        glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);

        obj_world[0] = vptr->getCoords()[0]; 
        obj_world[1] = vptr->getCoords()[1]; 
        obj_world[2] = vptr->getCoords()[2];

        // Project object coordinates to window coordinates (to get accurate window depth)
        gluProject(obj_world[0],obj_world[1],obj_world[2],
                   modelMatrix,projMatrix,viewport,
                   &obj_window[0],&obj_window[1],&obj_window[2]);

        // Set start and end window coordinates using depth coordinate found above
        ms_window[0] = drag_startx;  ms_window[1] = drag_starty;  ms_window[2] = obj_window[2];
        me_window[0] = drag_endx;    me_window[1] = drag_endy;    me_window[2] = obj_window[2];

        // Unproject start drag window coordinates to world coordinates
        gluUnProject(ms_window[0],ms_window[1],ms_window[2],
                     modelMatrix, projMatrix, viewport,
                     &ms_world[0],&ms_world[1],&ms_world[2]);

        // Unproject end drag window coordinates to world coordinates
        gluUnProject(me_window[0],me_window[1],me_window[2],
                     modelMatrix, projMatrix, viewport,
                     &me_world[0],&me_world[1],&me_world[2]);

        // Switch on locked axis and update object world position
        switch (active->getLocatorPtr()->getSelectedAxis())
        {
          case 0: // X-axis
            obj_world[0] = obj_world[0] + me_world[0] - ms_world[0];     
            break;

          case 1: // Y-axis
            obj_world[1] = obj_world[1] + me_world[1] - ms_world[1];                     
            break;

          case 2: // Z-axis
            obj_world[2] = obj_world[2] + me_world[2] - ms_world[2];                 
            break;

          case 3:  // User can drag freely along viewing place
          default: 
            obj_world[0] = obj_world[0] + me_world[0] - ms_world[0];             
            obj_world[1] = obj_world[1] + me_world[1] - ms_world[1];
            obj_world[2] = obj_world[2] + me_world[2] - ms_world[2];
            break;
        }

        vptr->setCoords(Vector3d(obj_world[0],obj_world[1],obj_world[2]));

        // Update patches
        //object.bezierDefaults();

        // Reset drag start points
        startDrag(drag_endx,drag_endy);

        // Restore previous transformations
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();

        redraw();
      }
      break;

    default:
      doSelection(Fl::event_x(),h()-Fl::event_y());
      break;
  }
}

   // Do selection of various entities depending on current mode
void DLFLWindow :: doSelection(int x, int y)
{
  DLFLVertexPtr svptr = NULL;
  DLFLEdgePtr septr = NULL;
  DLFLFacePtr sfptr = NULL;
  DLFLFaceVertexPtr sfvptr = NULL;
  DLFLLocatorPtr slptr = NULL; // brianb
  
  switch ( mode )
     {
       case EditMode:     // brianb
                    slptr = active->getLocatorPtr();
                    svptr = active->getLocatorPtr()->getActiveVertex();

                    if (svptr == NULL)
                    {
                      svptr = active->selectVertex(x,y);
                      slptr->setActiveVertex(svptr);
                    }
                    
                    // Test for locator selection
                    if (slptr->getActiveVertex() != NULL)
                    {
                      slptr = active->selectLocator(x,y);
                      if (slptr != NULL)
                      {
                        DLFLViewport::setSelectedLocator(0,slptr);
                        startDrag(x,y);
                      }
                      else
                      {
                        active->getLocatorPtr()->setActiveVertex(NULL);
                        DLFLViewport::clearSelectedLocators();
                      } 
                    }
                    break;

       case SelectVertex :
                    svptr = active->selectVertex(x,y);
                    DLFLViewport::setSelectedVertex(num_sel_verts,svptr);
                    break;

       case MultiSelectVertex :
                    svptr = active->selectVertex(x,y);
                    if ( !DLFLViewport::isSelected(svptr) )
                       DLFLViewport::setSelectedVertex(num_sel_verts,svptr);
                    break;

       case DeleteEdge :
       case SubDivideEdge :
       case CollapseEdge :
       case SelectEdge :
                    septr = active->selectEdge(x,y);
                    DLFLViewport::setSelectedEdge(num_sel_edges,septr);
                    break;

       case MultiSelectEdge :
                    septr = active->selectEdge(x,y);
                    if ( !DLFLViewport::isSelected(septr) )
                       DLFLViewport::setSelectedEdge(num_sel_edges,septr);
                    break;

       case SelectFace :
       case ExtrudeFace :
       case ExtrudeFaceDS :
       case ExtrudeDualFace :
       case ExtrudeFaceDodeca :
       case ExtrudeFaceIcosa :
       case StellateFace :
       case DoubleStellateFace :
       case CrustModeling :
       case ConnectFaces :
                    sfptr = active->selectFace(x,y);
                    DLFLViewport::setSelectedFace(num_sel_faces,sfptr);
                    break;

       case MultiSelectFace :
       case SubDivideFace :
                       // No duplicates allowed
                    sfptr = active->selectFace(x,y);
                    if ( !DLFLViewport::isSelected(sfptr) )
                       DLFLViewport::setSelectedFace(num_sel_faces,sfptr);
                    break;

       case SelectFaceVertex :
       case ReorderFace :
       case InsertEdge :
       case ConnectFaceVertices :
       case BezierConnectFaces :
       case HermiteConnectFaces :
                    sfptr = active->selectFace(x,y);
                    DLFLViewport::setSelectedFace(num_sel_faces,sfptr);
                    if ( sfptr )
                       {
                         sfvptr = active->selectFaceVertex(sfptr,x,y);
                         DLFLViewport::setSelectedFaceVertex(num_sel_faceverts,sfvptr);
                       }
                    break;

       case MultiSelectFaceVertex :
                    sfptr = active->selectFace(x,y);
                    DLFLViewport::setSelectedFace(num_sel_faces,sfptr);
                    if ( sfptr )
                       {
                         sfvptr = active->selectFaceVertex(sfptr,x,y);
                         if ( !DLFLViewport::isSelected(sfvptr) )
                            DLFLViewport::setSelectedFaceVertex(num_sel_faceverts,sfvptr);
                       }
                    break;

       case ConnectEdges :
                    sfptr = active->selectFace(x,y);
                    DLFLViewport::setSelectedFace(num_sel_faces,sfptr);
                    if ( sfptr )
                       {
                         septr = active->selectEdge(x,y);
                         DLFLViewport::setSelectedEdge(num_sel_edges,septr);
                       }
                    break;
     }
  if ( svptr != NULL || septr != NULL || sfptr != NULL || slptr != NULL) redraw();
}

   // Handle keyboard and mouse events
int DLFLWindow :: handle(int event)
{
  int key = Fl::event_key();
  bool ctrl = Fl::event_state(FL_CTRL);
  bool alt = Fl::event_state(FL_ALT);
  bool shift = Fl::event_state(FL_SHIFT);
  int retval = 0;

  if ( event == FL_FOCUS )
     {
          // Handle focus events since we want keyboard events also
       retval = 1;
     }
  else if ( event == FL_KEYBOARD )
     {
       if ( ctrl )
          {
            switch ( key )
               {
                 case 'b' :      // Toggle display of patches
                              togglePatchWireframe(); retval = 1; redraw();
                              break;
                 case 'p' :
                                 // Toggle display of points
                              toggleVertices(); retval = 1; redraw();
                              break;
                 case 's' :
                                 // Toggle display of silhouette
                              toggleSilhouette(); retval = 1; redraw();
                              break;
                 case 'w' :
                                 // Toggle display of wireframe
                              toggleWireframe(); retval = 1; redraw();
                              break;
                 case 'n' :
                                 // Turn off display of points, silhouette and wireframe
                              turnOffOverlays(); retval = 1; redraw();
                              break;
                 case 'r' :
                                 // Reverse displayed object
                              toggleObjectOrientation(); retval = 1; redraw();
                              break;
                 case 'v' :
                                 // Go into vertex selection mode
                              setMode(SelectVertex); retval = 1;
                              break;
                 case 'e' :
                                 // Go into edge selection mode
                              setMode(SelectEdge); retval = 1;
                              break;
                 case 'f' :
                                 // Go into face selection mode
                              setMode(SelectFace); retval = 1;
                              break;
                 case 'c' :
                                 // Go into face-vertex selection mode
                              setMode(SelectFaceVertex); retval = 1;
                              break;
                 case 'u' :
                                 // Go out of selection mode
                              setMode(NormalMode); retval = 1;
                              break;
                 case 'z' :
                                 // If shift key is pressed, toggle undo
                              if ( shift )
                                 {
                                   cout << "Toggle Undo" << endl;
                                   toggleUndo(); retval = 1;
                                 }
                              else
                                 {
                                      // Undo last operation
                                   undo(); retval = 1; redraw();
                                 }
                              break;
                 case 'x' :
                                 // Redo last undo
                              redo(); retval = 1; redraw();
                              break;
               }
          }
       else if ( alt )
          {
            switch (key)
               {
                 case 'b' :
                                 // Bezier Connect Faces (create handle)
                              setMode(BezierConnectFaces); retval = 1;
                              break;
                 case 'c' :
                                 // Connect 2 half-edges
                              setMode(ConnectEdges); retval = 1;
                              break;
                 case 'e' :
                              setMode(DeleteEdge); retval = 1;
                              break;
                 case 'f' :
                                 // Sub-divide face
                              setMode(SubDivideFace); retval = 1;
                              break;
                 case 'h' :
                                 // Connect Faces (create hole/handle)
                              if ( shift ) setMode(ConnectFaces);
                              else setMode(ConnectFaceVertices);
                              retval = 1;
                              break;
                 case 'i' :
                              setMode(InsertEdge); retval = 1;
                              break;
                 case 'l' :
                              setMode(CollapseEdge); retval = 1;
                              break;
                 case 'm' :
                                 // Hermite Connect Faces (create handle)
                              setMode(HermiteConnectFaces); retval = 1;
                              break;
                 case 'n' :
                                 // Go back to normal mode
                              setMode(NormalMode); retval = 1; redraw();
                              break;
                 case 'r' :
                                 // Reorder face
                              setMode(ReorderFace); retval = 1;
                              break;
                 case 's' :
                              setMode(SubDivideEdge); retval = 1;
                              break;
                 case 't' :
                                 // Crust Modeling
                              if ( shift ) createCrust(true);
                              else createCrust(false);
                              setMode(CrustModeling);
                              retval = 1; redraw();
                              break;
                 case 'x' :
                                 // Extrude a face
                              if ( shift ) setMode(ExtrudeFaceDS);
                              else setMode(ExtrudeFace);
                              retval = 1;
                              break;
               }
          }
       else
          {
            switch ( key )
               {
//                  case '1' :
//                               smoothMesh(); retval = 1; redraw();
//                               break;
//                  case '2' :
//                               spheralizeObject(); retval = 1; redraw();
//                               break;
//                  case '3' :
//                               planarizeFaces(); retval = 1; redraw();
//                               break;
//                  case '4' :
//                               subDivideRoot4(); retval = 1; redraw();
//                               break;
//                  case '5' :
//                               splitValence2Vertices(); retval = 1; redraw();
//                               break;
                 case FL_Escape :
                                 // Cancel current operation/selection
                              DLFLWindow::clearSelected(); retval = 1; redraw();
                              break;
                 case 'a' :
                              subDivideAllEdges(); retval = 1; redraw();
                              break;
                 case 'b' :
                              testConvexHull(); retval = 1; redraw();
                              break;
                 case 'c' :
                              if ( shift ) makeUnitCube(2.0);
                              else subDivideCornerCutting();
                              retval = 1; redraw();
                              break;
                 case 'd' :
                              subDivideDooSabin(); retval = 1; redraw();
                              break;
                 case 'e' :
                              printEdgeList(); retval = 1;
                              break;
                 case 'f' :
                              if ( shift ) subDivideAllFaces();
                              else printFaceList();
                              retval = 1; redraw();
                              break;
                 case 'g' :
                              toggleGrid(); retval = 1; redraw();
                              break;
                 case 'h' :
                              subDivideHoneycomb(); retval = 1; redraw();
                              break;
                 case 'j' :
                              edgeCleanup(); retval = 1; redraw();
                              break;
                 case 'k' :
                              subDividePentagonPreserving(); retval = 1; redraw();
                              break;
                 case 'l' :
                              recomputeLighting(); retval = 1; redraw();
                              break;
                 case 'm' :
                              if ( shift ) makeMengerSponge(1);
                              else multiConnectMidpoints();
                              retval = 1; redraw();
                              break;
                 case 'n' :
                              recomputeNormals(); retval = 1; redraw();
                              break;
                 case 'o' :
                              if ( shift ) modifiedMultiConnectCrust();
                              else multiConnectCrust();
                              retval = 1; redraw();
                              break;
                 case 'p' :
                              printSummary(); retval = 1;
                              break;
                 case 'q' :
                              cleanupForExit();
                              exit(0);
                 case 'r' :
                              if ( shift ) createCrust(true);
                              else createCrust(false);
                              retval = 1; redraw();
                              break;
                 case 's' :
                              if ( shift ) createSponge();
                              else subDivideCatmullClark();
                              retval = 1; redraw();
                              break;
                 case 't' :
                              if ( shift ) makeUnitTetrahedron(1.0);
                              else assignTileTexCoords();
                              retval = 1; redraw();
                              break;
                 case 'v' :
                              printVertexList(); retval = 1;
                              break;
                 case 'w' :
                              cleanupWingedVertices(); retval = 1; redraw();
                              break;
                 case 'x' :
                              toggleAxes(); retval = 1; redraw();
                              break;
                 case 'y' :
                              makeSierpinskiTetrahedron(2);
                              retval = 1; redraw();
                              break;
                 case 'z' :
                              makeWireframe(); retval = 1; redraw();
                              break;
               }
          }
     }
  else if ( event == FL_RELEASE || event == FL_DRAG )
     {
          // The mouse was dragged or released
          // Send this event to the subroutine handling the current event, if any
          // If we are not currently doing anything, nothing will happen
          // If ALT key is pressed we are mainpulating the view which is handled by the viewport
       if ( !alt )
          {
            if ( mode != NormalMode )
               {
                 if ( event == FL_DRAG)
                 {
                   doDrag(Fl::event_x(),Fl::event_y());
                 }
                 else if ( event == FL_RELEASE ) // Complete current operation if any
                    {
                      switch ( mode )
                         {
                           case EditMode :       // brianb
                                        is_editing = false;
                                        if ( DLFLViewport::numSelectedVertices() >= 1 )
                                           {
                                             DLFLVertexPtr vp = DLFLViewport::getSelectedVertex(0);
                                             vp->print();
                                             DLFLViewport::clearSelectedVertices();
                                             num_sel_verts = 0;
                                             redraw();
                                           }
                                        break;
                           case SelectVertex :
                                        if ( DLFLViewport::numSelectedVertices() >= 1 )
                                           {
                                             DLFLVertexPtr vp = DLFLViewport::getSelectedVertex(0);
                                             vp->print();
                                             DLFLViewport::clearSelectedVertices();
                                             num_sel_verts = 0;
                                             redraw();
                                           }
                                        break;
                           case SelectEdge :
                                        if ( DLFLViewport::numSelectedEdges() >= 1 )
                                           {
                                             DLFLEdgePtr ep = DLFLViewport::getSelectedEdge(0);
                                             ep->print();
                                             DLFLViewport::clearSelectedEdges();
                                             num_sel_edges = 0;
                                             redraw();
                                           }
                                        break;
                           case SelectFace :
                                        if ( DLFLViewport::numSelectedFaces() >= 1 )
                                           {
                                             DLFLFacePtr fp = DLFLViewport::getSelectedFace(0);
                                             fp->print();
                                             DLFLViewport::clearSelectedFaces();
                                             num_sel_faces = 0;
                                             redraw();
                                           }
                                        break;
                           case SelectFaceVertex :
                                        if ( DLFLViewport::numSelectedFaceVertices() >= 1 )
                                           {
                                             DLFLFaceVertexPtr fvp = DLFLViewport::getSelectedFaceVertex(0);
                                             fvp->print();
                                             DLFLViewport::clearSelectedFaceVertices();
                                             DLFLViewport::clearSelectedFaces();
                                             num_sel_faceverts = 0; num_sel_faces = 0;
                                             redraw();
                                           }
                                        break;
                           case InsertEdge :
                                        if ( DLFLViewport::numSelectedFaceVertices() >= 2 )
                                           {
                                             DLFLFaceVertexPtr sfvptr1, sfvptr2;
                                             sfvptr1 = DLFLViewport::getSelectedFaceVertex(0);
                                             sfvptr2 = DLFLViewport::getSelectedFaceVertex(1);
                                             if ( sfvptr1 && sfvptr2 )
                                                {
                                                  DLFLMaterialPtr mptr = sfvptr1->getFacePtr()->material();
                                                  undoPush();
                                                  object.insertEdge(sfvptr1,sfvptr2,false,mptr);
                                                  DLFLViewport::clearSelectedFaces();
                                                  DLFLViewport::clearSelectedFaceVertices();
                                                  num_sel_faceverts = 0; num_sel_faces = 0;
                                                  redraw();   
                                                }
                                           }
                                        else if ( DLFLViewport::numSelectedFaceVertices() == 1 )
                                           {
                                             num_sel_faceverts=1; num_sel_faces=1;
                                           }
                                        break;
                           case DeleteEdge :
                                        if ( DLFLViewport::numSelectedEdges() >= 1 )
                                           {
                                             DLFLEdgePtr septr = DLFLViewport::getSelectedEdge(0);
                                             if ( septr )
                                                {
                                                  undoPush();
                                                  object.deleteEdge(septr,DLFLWindow::delete_edge_cleanup);
                                                }
                                             DLFLViewport::clearSelectedEdges();
                                             redraw();
                                           }
                                        break;
                           case SubDivideEdge :
                                        if ( DLFLViewport::numSelectedEdges() >= 1 )
                                           {
                                             DLFLEdgePtr septr = DLFLViewport::getSelectedEdge(0);
                                             if ( septr )
                                                {
                                                  undoPush();
                                                  object.subDivideEdge(num_e_subdivs,septr);
                                                }
                                             DLFLViewport::clearSelectedEdges();
                                             redraw();
                                           }
                                        break;
                           case CollapseEdge :
                                        if ( DLFLViewport::numSelectedEdges() >= 1 )
                                           {
                                             DLFLEdgePtr septr = DLFLViewport::getSelectedEdge(0);
                                             if ( septr )
                                                {
                                                  undoPush();
                                                  object.collapseEdge(septr);
                                                }
                                             DLFLViewport::clearSelectedEdges();
                                             redraw();
                                           }
                                        break;
                           case ConnectFaces :
                                        if ( DLFLViewport::numSelectedFaces() >= 2 )
                                           {
                                             DLFLFacePtr sfptr1, sfptr2;
                                             sfptr1 = DLFLViewport::getSelectedFace(0);
                                             sfptr2 = DLFLViewport::getSelectedFace(1);
                                             if ( sfptr1 && sfptr2 )
                                                {
                                                  undoPush();
                                                  object.connectFaces(sfptr1,sfptr2,num_segments);
                                                  recomputeNormals();
                                                  DLFLViewport::clearSelectedFaces();
                                                  redraw();   
                                                }
                                           }
                                        else if ( DLFLViewport::numSelectedFaces() == 1 )
                                           {
                                             num_sel_faces = 1;
                                           }
                                        break;
                           case ConnectFaceVertices :
                                        if ( DLFLViewport::numSelectedFaceVertices() >= 2 )
                                           {
                                             DLFLFaceVertexPtr sfvptr1, sfvptr2;
                                             sfvptr1 = DLFLViewport::getSelectedFaceVertex(0);
                                             sfvptr2 = DLFLViewport::getSelectedFaceVertex(1);
                                             if ( sfvptr1 && sfvptr2 )
                                                {
                                                  undoPush();
                                                  object.connectFaces(sfvptr1,sfvptr2,num_segments, max_segments);
                                                  DLFLViewport::clearSelectedFaces();
                                                  DLFLViewport::clearSelectedFaceVertices();
                                                  num_sel_faceverts = 0; num_sel_faces = 0;
                                                  recomputeNormals();
                                                  redraw();   
                                                }
                                           }
                                        else if ( DLFLViewport::numSelectedFaceVertices() == 1 )
                                           {
                                             num_sel_faceverts = 1; num_sel_faces = 1;
                                           }
                                        break;
                           case ConnectEdges :
                                        if ( DLFLViewport::numSelectedEdges() >= 2 )
                                           {
                                             DLFLEdgePtr septr1, septr2;
                                             DLFLFacePtr sfptr1, sfptr2;
                                             septr1 = DLFLViewport::getSelectedEdge(0);
                                             septr2 = DLFLViewport::getSelectedEdge(1);
                                             sfptr1 = DLFLViewport::getSelectedFace(0);
                                             sfptr2 = DLFLViewport::getSelectedFace(1);
                                             if ( septr1 && septr2 )
                                                {
                                                  undoPush();
                                                  object.connectEdges(septr1,sfptr1,septr2,sfptr2);
                                                  DLFLViewport::clearSelectedEdges();
                                                  DLFLViewport::clearSelectedFaces();
                                                  num_sel_edges = 0; num_sel_faces = 0;
                                                  redraw();   
                                                }
                                           }
                                        else if ( DLFLViewport::numSelectedEdges() == 1 )
                                           {
                                             num_sel_edges = 1; num_sel_faces = 1;
                                           }
                                        break;
                           case ExtrudeFace :
                                        if ( DLFLViewport::numSelectedFaces() >= 1 )
                                           {
                                             DLFLFacePtr sfptr = DLFLViewport::getSelectedFace(0);
                                             if ( sfptr )
                                                {
                                                  undoPush();
                                                  object.extrudeFace(sfptr,extrude_dist,num_extrusions,
                                                                     extrude_rot,extrude_scale);
                                                  recomputeNormals();
                                                }
                                             DLFLViewport::clearSelectedFaces();
                                             redraw();
                                           }
                                        break;
                           case ExtrudeFaceDS :
                                        if ( DLFLViewport::numSelectedFaces() >= 1 )
                                           {
                                             DLFLFacePtr sfptr = DLFLViewport::getSelectedFace(0);
                                             if ( sfptr )
                                                {
                                                  undoPush();
                                                  object.extrudeFaceDS(sfptr,extrude_dist,num_extrusions,
                                                                       ds_ex_twist,extrude_scale);
                                                  recomputeNormals();
                                                }
                                             DLFLViewport::clearSelectedFaces();
                                             redraw();
                                           }
                                        break;
                           case ExtrudeDualFace :
                                        if ( DLFLViewport::numSelectedFaces() >= 1 )
                                           {
                                             DLFLFacePtr sfptr = DLFLViewport::getSelectedFace(0);
                                             if ( sfptr )
                                                {
                                                  undoPush();
                                                  object.extrudeDualFace(sfptr,extrude_dist,num_extrusions,
                                                                         extrude_rot,extrude_scale,
                                                                         dual_mesh_edges_check);
                                                  recomputeNormals();
                                                }
                                             DLFLViewport::clearSelectedFaces();
                                             redraw();
                                           }
                                        break;
                           case ExtrudeFaceDodeca :
                                        if ( DLFLViewport::numSelectedFaces() >= 1 )
                                           {
                                             DLFLFacePtr sfptr = DLFLViewport::getSelectedFace(0);
                                             if ( sfptr )
                                                {
                                                  undoPush();
                                                  object.extrudeFaceDodeca(sfptr,extrude_dist,num_extrusions,
                                                                           ds_ex_twist,extrude_scale,
                                                                           hexagonalize_dodeca_extrude);
                                                  recomputeNormals();
                                                }
                                             DLFLViewport::clearSelectedFaces();
                                             redraw();
                                           }
                                        break;
                           case ExtrudeFaceIcosa :
                                        if ( DLFLViewport::numSelectedFaces() >= 1 )
                                           {
                                             DLFLFacePtr sfptr = DLFLViewport::getSelectedFace(0);
                                             if ( sfptr )
                                                {
                                                  undoPush();
                                                  object.extrudeFaceIcosa(sfptr,extrude_dist,num_extrusions,
                                                                          ds_ex_twist,extrude_scale);
                                                  recomputeNormals();
                                                }
                                             DLFLViewport::clearSelectedFaces();
                                             redraw();
                                           }
                                        break;
                           case StellateFace :
                                        if ( DLFLViewport::numSelectedFaces() >= 1 )
                                           {
                                             DLFLFacePtr sfptr = DLFLViewport::getSelectedFace(0);
                                             if ( sfptr )
                                                {
                                                  undoPush();
                                                  object.stellateFace(sfptr,extrude_dist);
                                                  recomputeNormals();
                                                }
                                             DLFLViewport::clearSelectedFaces();
                                             redraw();
                                           }
                                        break;
                           case DoubleStellateFace :
                                        if ( DLFLViewport::numSelectedFaces() >= 1 )
                                           {
                                             DLFLFacePtr sfptr = DLFLViewport::getSelectedFace(0);
                                             if ( sfptr )
                                                {
                                                  undoPush();
                                                  object.doubleStellateFace(sfptr,extrude_dist);
                                                  recomputeNormals();
                                                }
                                             DLFLViewport::clearSelectedFaces();
                                             redraw();
                                           }
                                        break;
                           case CrustModeling :
                                        if ( DLFLViewport::numSelectedFaces() >= 1 )
                                           {
                                             DLFLFacePtr sfptr = DLFLViewport::getSelectedFace(0);
                                             if ( sfptr )
                                                {
                                                     // No undo for hole punching in crust modeling mode
                                                     // because the ids in the recreated object
                                                     // will be different
                                                  if ( shift )
                                                     {
                                                       object.tagMatchingFaces(sfptr);
                                                       object.punchHoles();
                                                       recomputeNormals();
                                                     }
                                                  else
                                                     object.cmMakeHole(sfptr,crust_cleanup);
//                                                  recomputeNormals();
                                                }
                                             DLFLViewport::clearSelectedFaces();
                                             redraw();
                                           }
                                        break;
                           case BezierConnectFaces :
                                        if ( DLFLViewport::numSelectedFaceVertices() >= 2 )
                                           {
                                             DLFLFaceVertexPtr sfvptr1, sfvptr2;
                                             sfvptr1 = DLFLViewport::getSelectedFaceVertex(0);
                                             sfvptr2 = DLFLViewport::getSelectedFaceVertex(1);
                                             if ( sfvptr1 && sfvptr2 )
                                                {
                                                  undoPush();
                                                  object.bezierConnectFaces(sfvptr1,sfvptr2,
                                                                            num_segments,nwt1,nwt2);
                                                  DLFLViewport::clearSelectedFaces();
                                                  DLFLViewport::clearSelectedFaceVertices();
                                                  num_sel_faceverts = 0; num_sel_faces = 0;
                                                  recomputeNormals();
                                                  redraw();   
                                                }
                                           }
                                        else if ( DLFLViewport::numSelectedFaceVertices() == 1 )
                                           {
                                             num_sel_faceverts = 1; num_sel_faces = 1;
                                           }
                                        break;
                           case HermiteConnectFaces :
                                        if ( DLFLViewport::numSelectedFaceVertices() >= 2 )
                                           {
                                             DLFLFaceVertexPtr sfvptr1, sfvptr2;
                                             sfvptr1 = DLFLViewport::getSelectedFaceVertex(0);
                                             sfvptr2 = DLFLViewport::getSelectedFaceVertex(1);
                                             if ( sfvptr1 && sfvptr2 )
                                                {
                                                  undoPush();
                                                  if ( symmetric_weights )
                                                     object.hermiteConnectFaces(sfvptr1,sfvptr2,
                                                                                num_segments,nwt1,nwt1,
                                                                                max_segments,num_extra_twists);
                                                  else
                                                     object.hermiteConnectFaces(sfvptr1,sfvptr2,
                                                                                num_segments,nwt1,nwt2,
                                                                                max_segments,num_extra_twists);
                                                  DLFLViewport::clearSelectedFaces();
                                                  DLFLViewport::clearSelectedFaceVertices();
                                                  num_sel_faceverts = 0; num_sel_faces = 0;
                                                  recomputeNormals();
                                                  redraw();
                                                }
                                           }
                                        else if ( DLFLViewport::numSelectedFaceVertices() == 1 )
                                           {
                                             num_sel_faceverts = 1; num_sel_faces = 1;
                                           }
                                        break;
                           case ReorderFace :
                                        if ( DLFLViewport::numSelectedFaceVertices() >= 1 )
                                           {
                                             DLFLFaceVertexPtr sfvptr = DLFLViewport::getSelectedFaceVertex(0);
                                             if ( sfvptr ) sfvptr->getFacePtr()->reorder(sfvptr);
                                             redraw();
                                           }
                                        break;
                           case SubDivideFace :
                           case MultiSelectFace :
                                        num_sel_faces++;
                                        break;
                         }
                    }
                 retval = 1; // Capture all non ALT modified drag/release events when not in NormalMode
               }
          }
     }
  else if ( event == FL_PUSH )
     {
       if ( !alt )
          {
               // We are doing some kind of selection
            doSelection(Fl::event_x(),h()-Fl::event_y());
            retval = 1;
          }
     }
     // If we haven't handled the event yet, try sending it to the active viewport
  if ( retval != 1 ) retval = active->handle(event);
  return retval;
}

void DLFLWindow :: testConvexHull(void)
{
     // For testing convex hull subroutine
  Vector3dArray vertices;
  vertices.resize(8);
  vertices[0].set(0,0,0);
  vertices[1].set(0,10,0);
  vertices[2].set(10,10,0);
  vertices[3].set(10,0,0);
  vertices[4].set(0,0,10);
  vertices[5].set(0,10,10);
  vertices[6].set(10,10,10);
  vertices[7].set(10,0,10);
  DLFLConvexHull convexhull;
  convexhull.createHull(vertices);

  ofstream file;
  file.open("convexhull.dlfl");
  convexhull.writeDLFL(file);
  file.close();
}

void DLFLWindow :: performRemeshing(void)
{
  switch ( remeshingscheme )
     {
       case Dual :
                    createDual();
                    break;
       case Root3 :
                    subDivideRoot3();
                    break;
       case DualVertexTrunc :
                    subDivideSqrt3();
                    break;
       case GlobalStellate :
                    globalStellate();
                    break;
       case Star :
                    subDivideStar();
                    break;
       case Generic1264 :
                    subDivide1264();
                    break;
       case Honeycomb :
                    subDivideHoneycomb();
                    break;
       case VertexTrunc :
                    subDivideVertexCutting();
                    break;
       case DualGeneric1264 :
                    subDivideDual1264();
                    break;
       case LinearVertexInsertion :
                    subDivideLinearVertexInsertion();
                    break;
       case CatmullClark :
                    subDivideCatmullClark();
                    break;
       case ModifiedStellate :
                    subStellate1();
                    break;
       case DooSabin :
                    subDivideDooSabin();
                    break;
       case CornerCutting :
                    subDivideCornerCutting();
                    break;
       case Simplest :
                    subDivideSimplest();
                    break;
       case Pentagonal :
                    subDividePentagonal();
                    break;
       case CubicPentagonal :
                    subDivideCubicPentagonal();
                    break;
       case DualPentagonal :
                    subDivideDualPentagonal();
                    break;
       case LoopStyle :
                    subDivideLoopStyle();
                    break;
       case Loop :
                    subDivideLoop();
                    break;
       case Root4 :
                    subDivideRoot4();
                    break;
       case DualLoop :
                    subDivideDualLoop();
                    break;
       case GlobalCubicExtrude :
                    cout << "NOT YET IMPLEMENTED!!!" << endl;
                    break;
       case CheckerBoard :
                    subDivideCheckerBoard();
                    break;
       case DualGlobalCubicExtrude :
                    cout << "NOT YET IMPLEMENTED!!!" << endl;
                    break;
       case DualCheckerBoard :
                    subDivideDualCheckerBoard();
                    break;
       case PentagonPreserving :
                    subDividePentagonPreserving();
                    break;
       case DualPentagonPreserving :
                    subDivideDualPentagonPreserving();
                    break;
       case HexagonPreserving :
                    subDivideRoot4();
                    break;
       case DualHexagonPreserving :
                    subDivideDualHexagonPreserving();
                    break;
       case Fractal :
                    subDivideFractal();
                    break;
       case ModifiedDoubleStellate :
                    subStellate2();
                    break;
       case Dome :
                    subDivideDome();
                    break;
       case DooSabinBC :
                    subDivideDooSabinBC();
                    break;
       case DooSabinBCNew :
                    subDivideDooSabinBCNew();
                    break;
     }
  setMode(NormalMode);
  redraw();
}

/*
  $Log: DLFLWindow.cc,v $
  Revision 4.14  2004/04/18 18:19:54  vinod
  Added hotkey to toggle object orientation

  Revision 4.13  2004/04/18 17:30:44  vinod
  Added hotkey (ESC button) to cancel current operation

  Revision 4.12  2004/02/24 20:41:44  vinod
  Added option to turn off undo using a hotkey

  Revision 4.11  2004/01/20 22:17:26  vinod
  Added Loop and DualLoop subdivision schemes

  Revision 4.10  2004/01/20 09:28:18  vinod
  Added variables for column modeling

  Revision 4.9  2004/01/20 07:24:55  vinod
  Added checkerBoardRemeshing scheme.
  Changed name of subhw to dual1264.
  Added wrapper functions for several remeshing schemes with proper handling of undo.

  Revision 4.8  2004/01/20 00:57:12  vinod
  Added implementation for some remeshing schemes

  Revision 4.7  2004/01/19 20:21:27  vinod
  Implemented some dual schemes. Added remeshing schemes by Bei & Cansin

  Revision 4.6  2004/01/19 18:06:00  vinod
  Added generic method to perform remeshing based on selected scheme.

  Revision 4.5  2004/01/17 00:54:07  vinod
  Added option to punch multiple holes in a single step using tagMatchingFaces method

  Revision 4.4  2004/01/13 02:46:44  vinod
  Changed names of dodecahedral and icosahedral extrude methods.
  Added hexagonalize flag for dodecahedral extrude

  Revision 4.3  2004/01/13 02:16:51  vinod
  Added double stellation, dodecahedral and icosahedral extrusion modes
  and user-interface hooks.
  Added variables for pentagonal subdivision (preserving) scale factor,
  and for stellate and double stellate remeshing schemes.

  Revision 4.2  2004/01/02 21:01:57  vinod
  Additions by Doug: parameters for star, sqrt3 and fractal subdivision schemes

  Revision 4.1  2003/12/28 05:27:34  vinod
  Added boolean flag for cleanup after edge deletion.
  Removed the RemoveEdge mode. Consolidated both delete edge operations into DeleteEdge mode using flag

  Revision 4.0  2003/12/26 01:58:53  vinod
  Major version sync.

  Revision 3.15  2003/12/26 01:37:58  vinod
  Removed numeric hotkeys without shift states

  Revision 3.14  2003/12/08 20:19:25  vinod
  Added hotkyes for unit tetra and Sierpinski tetra

  Revision 3.13  2003/12/08 17:05:37  vinod
  Added hotkeys for unit cube and standard Menger sponge creation

  Revision 3.12  2003/12/07 01:49:03  vinod
  Added flag for choosing dual creation method

  Revision 3.11  2003/11/29 22:04:07  vinod
  Added max. connections parameter for regular connectFaces

  Revision 3.10  2003/11/29 20:36:35  vinod
  Added subDivideAllFaces hotkey and method

  Revision 3.9  2003/11/29 19:12:45  vinod
  Added hotkey for cleanupWingedVertices method

  Revision 3.8  2003/11/29 04:20:52  vinod
  Added boolean flag for symmetric hermitian handle weights

  Revision 3.7  2003/11/27 00:22:59  vinod
  Changed threshold angle to threshold factor in Menger sponge creation

  Revision 3.6  2003/11/26 18:26:49  vinod
  Changed collapse threshold in Menger sponge creation to angle instead of length

  Revision 3.5  2003/11/23 22:45:37  vinod
  Added menger sponge edge collapse threshold parameter

  Revision 3.4  2003/11/21 15:24:28  vinod
  Added fractional thickness flag for menger sponge.
  Normals are now recomputed for most operations

  Revision 3.3  2003/11/16 06:48:44  vinod
  Added sponge_thickness class variable

  Revision 3.2  2003/10/21 06:45:04  vinod
  Added createSponge method

  Revision 3.1  2003/09/26 17:12:09  vinod
  Added multiConnectCrust and modifiedMultiConnectCrust

  Revision 3.0  2003/09/19 16:06:47  vinod
  Major version update

  Revision 2.12  2003/09/10 15:17:11  vinod
  Added hotkey for multiConnectMidpoints

  Revision 2.11  2003/09/06 19:49:49  vinod
  Added variable for multi-face-handle algorithm choice

  Revision 2.10  2003/08/24 23:11:43  vinod
  Added 2nd pentagonal subdivision scheme with hotkey

  Revision 2.9  2003/08/24 20:27:45  vinod
  Added variables for new multiConnect method.

  Revision 2.8  2003/07/07 16:12:16  vinod
  Added mfh_use_max_offsets option for multi-connect

  Revision 2.7  2003/04/25 15:45:24  vinod
  Added scale factor and extrude distance variables
  for multiConnectFaces method

  Revision 2.6  2003/04/14 16:50:24  vinod
  Cleaned up multi-face handle creation

  Revision 2.5  2003/04/14 15:47:41  vinod
  Added option for convex hull testing. Increased no. of faces to 6 for multiconnect

  Revision 2.4  2003/04/04 06:28:41  vinod
  Added edgeCleanup option

  Revision 2.3  2003/03/27 08:20:42  vinod
  Changed to use new multiConnectFaces with same hotkey

  Revision 2.2  2003/03/10 08:36:16  vinod
  Added CollapseEdge mode. Rearranged case statements alphabetically

  Revision 2.1  2003/01/30 07:04:48  vinod
  Added print feature for basic selection. Added MultiSelectFace operation for 3 faces

  Revision 2.0  2003/01/25 08:48:52  vinod
  *** empty log message ***

  Revision 1.20  2003/01/25 08:08:19  vinod
  Added MultiSelect modes

  Revision 1.19  2003/01/20 21:43:36  vinod
  Name change from skeleton modeling to wireframe modeling

  Revision 1.18  2002/12/30 17:30:04  vinod
  Added splitValence2Vertices method and variables.
  Resorted case statements alphabetically

  Revision 1.17  2002/12/30 04:09:48  vinod
  Added hotkey for skeleton modeling

  Revision 1.16  2002/12/18 16:23:21  vinod
  Added face subdivision and tile texturing stuff

  Revision 1.15  2002/12/17 03:53:40  vinod
  Fixed comment logs

  Revision 1.14  2002/12/17 03:18:37  vinod
  See comments for previous revision

  Revision 1.13  2002/12/17 02:55:03  vinod
  Additions by Paul
  Handling of ExtrudeDualFace and StellateFace

  Revision 1.12  2002/12/17 02:44:59  vinod
  DLFLWindow.hh

  Revision 1.11  2002/11/07 08:19:54  vinod
  Added edge check flag for dooSabin subdivision

  Revision 1.10  2002/10/31 06:07:47  vinod
  Disabled undo for hole punching in crust modeling

  Revision 1.9  2002/10/22 21:25:34  vinod
  Added undo feature

  Revision 1.8  2002/10/17 02:22:39  vinod
  Fixed typo in subDivideDooSabin call

  Revision 1.7  2002/10/16 14:17:25  vinod
  Changed to use methods defined in class

  Revision 1.6  2002/10/09 15:05:34  vinod
  Fixed handling operations requiring two selections. Changed order of if stmnts.

  Revision 1.5  2002/10/09 14:58:11  vinod
  Made changes to properly handle operations requiring two selections

  Revision 1.4  2002/10/09 07:28:53  vinod
  Bug fix: fixed array indices for selection lists for operations

  Revision 1.3  2002/10/08 16:23:51  vinod
  Changed references to active viewport to local member from DLFLViewport class member

  Revision 1.2  2002/10/02 17:03:29  vinod
  Fixed typos and minor bugs

  Revision 1.1  2002/10/02 16:41:24  vinod
  Window class to display object. Handles keyboard events and selection

*/
