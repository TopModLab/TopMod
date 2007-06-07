// Source code for DLFLWindow class

#include "DLFLWindow.hh"
#include "DLFLLighting.hh"

// DLFLAux Library includes
#include <DLFLCast.hh>
#include <DLFLConnect.hh>
#include <DLFLConvexHull.hh>
#include <DLFLCrust.hh>
#include <DLFLDual.hh>
#include <DLFLExtrude.hh>
#include <DLFLMeshSmooth.hh>
#include <DLFLMultiConnect.hh>
#include <DLFLSculpting.hh>
#include <DLFLSubdiv.hh>

//-- Parameters used in various operations on the DLFL object --//
//-- See header file for explanations --//

				// Edge deletion
bool DLFLWindow::delete_edge_cleanup = true;

				// Handles
int DLFLWindow::num_segments = 4;
int DLFLWindow::max_segments = -1;
bool DLFLWindow::symmetric_weights = true;
double DLFLWindow::nwt1 = 5.0;
double DLFLWindow::nwt2 = 5.0;
int DLFLWindow::num_extra_twists = 0;

				// Extrusions
double DLFLWindow::extrude_dist = 2.0;
double DLFLWindow::extrude_rot = 0.0;
double DLFLWindow::extrude_scale = 1.0;
int DLFLWindow::num_extrusions = 1;
double DLFLWindow::ds_ex_twist = 0.0;
bool DLFLWindow::dual_mesh_edges_check = false;
bool DLFLWindow::hexagonalize_dodeca_extrude = false;
double DLFLWindow ::extrude_bendT = 0;
double DLFLWindow ::extrude_bendB = 0;

				// Edge subdivision
int DLFLWindow::num_e_subdivs = 2;

				// Split valence 2 vertices
double DLFLWindow::vertex_split_offset=-0.1;

				// Crust modeling
double DLFLWindow::crust_thickness = 0.5;
double DLFLWindow::crust_scale_factor = 0.9;
bool DLFLWindow::crust_cleanup = true;

				// Wireframe modeling
double DLFLWindow::wireframe_thickness = 0.25;

				// Column modeling
double DLFLWindow::column_thickness = 0.25;
int DLFLWindow::column_segments = 4;

				// Subdivision
double DLFLWindow::twist_factor = 0.0;
double DLFLWindow::weight_factor = 0.0;
bool DLFLWindow::doo_sabin_check = true;
double DLFLWindow::vertex_cutting_offset = 0.25;
double DLFLWindow::pentagonal_offset = 0.0;
double DLFLWindow::pentagonal_scale = 0.75;
bool DLFLWindow::accurate_dual = true;
double DLFLWindow::checkerboard_thickness = 0.33;

			// Added by Doug
double DLFLWindow::star_offset = 0.0;
double DLFLWindow::fractal_offset = 1.0;

			// Added by Eric
double DLFLWindow::substellate_height = 0.0;
double DLFLWindow::substellate_curve = 0.0;

			// Added by bei&cansin dec 09
double DLFLWindow::domeLength_factor = 1.0 ;
double DLFLWindow::domeScale_factor = 1.0  ;
double DLFLWindow::dual1264_scale_factor = 0.7 ;

			// Added by bei dec 11
double DLFLWindow::dooSabinBCnewScale_factor = 1.0;
double DLFLWindow::dooSabinBCnewLength_factor = 1.0;

			// Added by bei 12-11-03
double DLFLWindow::loopLength_factor = 1.0;

			// Face subdivision
bool DLFLWindow::use_quads = true;

				// Tile texturing
int DLFLWindow::tile_tex_n = 2;

				// Multi-face handles
DLFLWindow::MFHAlgo DLFLWindow::mfh_algo = DLFLWindow::ConvexHull;
double DLFLWindow::mfh_scale_factor = 1.0;
double DLFLWindow::mfh_extrude_dist = 0.5;
bool DLFLWindow::mfh_use_max_offsets = false;
double DLFLWindow::mfh_min_adj_factor = 0.01;
bool DLFLWindow::mfh_make_connections = true;

				// Menger Sponge creation
double DLFLWindow::sponge_thickness = 0.67; // Thickness for Menger Sponge
double DLFLWindow::sponge_collapse_threshold = 0.0; // Threshold factor for edge collapse
bool DLFLWindow::sponge_fractional_thickness = false; // Thickness is fractional

				//conical sculpting initialization from ozgur
double DLFLWindow ::  cutOffsetE_factor = 0.25;
double DLFLWindow ::  cutOffsetV_factor = 0.25;
double DLFLWindow ::  pnormalBendS_factor = 0;
double DLFLWindow ::  pnormalBendT_factor = 0;
bool   DLFLWindow ::  global_cut = false;
int    DLFLWindow ::  numberofPeels = 90;
bool   DLFLWindow :: selected_cut=true;

//-- Selection count, used as index into selection lists --//
int DLFLWindow::num_sel_verts = 0;
int DLFLWindow::num_sel_edges = 0;
int DLFLWindow::num_sel_faces = 0;
int DLFLWindow::num_sel_faceverts = 0;

				// Initialize the viewports, etc.
void DLFLWindow::initialize(int x, int y, int w, int h, DLFLRendererPtr rp){

	// Create the 4 viewports. They will automatically be added
	// as tabs in this QTabWidget
	int sw=w, sh=h;

	layout = new QVBoxLayout;
	layout->setMargin(0);

	patchObject = new TMPatchObject( object.getID() );

	QGLFormat fmt;
	// fmt.setOverlay(true);
	// QColor v(1.0,1.0,1.0,1.0);
	// QColor c(1.0,1.0,1.0,1.0);
	persp = new GLWidget(sw,sh, VPPersp, rp, QColor(255,255,255,255),QColor(255,255,255,255) , &object, patchObject, fmt, this);
	persp->switchTo(VPPersp);
	//this->addTab(persp, "Perspective View");
	persp->setMinimumSize(400,400);
	persp->resize(1000, 800);
	persp->setFocusPolicy(Qt::StrongFocus);

	layout->addWidget(persp);

	this->setContentsMargins(0,0,0,0);

	setLayout(layout);
	// if (!persp->format().hasOverlay()){
	// 	QMessageBox::about(this, tr("About TopMod"),tr("hey it has an overlay context!") );
	// 	
	// }


	// // 
	// top = new GLWidget(0,0,sw,sh,rp,QColor(0.2,0.5,0.85,0.8),QColor(0.29,0.33,0.37,1),&object);
	// top->switchTo(VPTop);
	// // // this->addTab(top, "Top View");    
	// // // 
	// front = new GLWidget(0,0,sw,sh,rp,QColor(0.2,0.5,0.85,0.8),QColor(0.29,0.33,0.37,1),&object);
	// front->switchTo(VPFront);
	// // // this->addTab(front, "Front View");
	// // // 
	// right = new GLWidget(0,0,sw,sh,rp,QColor(0.2,0.5,0.85,0.8),QColor(0.29,0.33,0.37,1),&object);
	// right->switchTo(VPRight);
	// // // this->addTab(right, "Right View");	

	active = persp;
	//initialize light color
	plight.position.set(50,25,0);
	plight.warmcolor.set(1,1,0.6);
	plight.coolcolor.set(0.2,0.2,0.4);
	// plight.coolcolor.set(0.2,0.2,0.2);
	plight.intensity = 2.0;
	// persp->switchTo(VPPersp);


}

void DLFLWindow::setWarmLightColor(QColor c){
	plight.warmcolor.set(c.redF(),c.greenF(),c.blueF());
	recomputeLighting();
	redraw();
}

void DLFLWindow::setCoolLightColor(QColor c){
	plight.coolcolor.set(c.redF(),c.greenF(),c.blueF());
	recomputeLighting();
	redraw();
}

void DLFLWindow::setLightIntensity(double i){
	plight.intensity = i;
	recomputeLighting();
	redraw();
}

void DLFLWindow::setUndoLimit(int limit) {
	undolimit = limit;
}

void DLFLWindow::toggleUndo(void) {
	if ( useUndo ) useUndo = false;
	else useUndo = true;
}


	// Do selection of various entities depending on current mode
void DLFLWindow::doSelection(int x, int y)
{
	DLFLVertexPtr svptr = NULL;
	DLFLEdgePtr septr = NULL;
	DLFLFacePtr sfptr = NULL;
	DLFLFaceVertexPtr sfvptr = NULL;
	DLFLFacePtrArray sfptrarr;
	DLFLFacePtrArray::iterator first, last;

	switch ( mode ) {

		case SelectVertex:
		case MarkVertex:
		case CutVertex://ozgur
		svptr = active->selectVertex(x,y);
		GLWidget::setSelectedVertex(num_sel_verts,svptr);
		break;
		case MultiSelectVertex :
		svptr = active->selectVertex(x,y);
		if ( !GLWidget::isSelected(svptr) )
			GLWidget::setSelectedVertex(num_sel_verts,svptr);
		break;
		case DeleteEdge :
		case SubdivideEdge :
		case CollapseEdge :
		case SelectEdge :
		case CutEdge :
		case TruncateEdge :
		case MarkEdge ://ozgur
		septr = active->selectEdge(x,y);
		GLWidget::setSelectedEdge(num_sel_edges,septr);
		break;
		case CutEdgeandVertex://ozgur
		septr = active->selectEdge(x,y);
		svptr = active->selectVertex(x,y);
		GLWidget::setSelectedEdge(num_sel_edges,septr);
		GLWidget::setSelectedVertex(num_sel_verts,svptr);
		break;
		case MultiSelectEdge :
		septr = active->selectEdge(x,y);
		if ( !GLWidget::isSelected(septr) )
			GLWidget::setSelectedEdge(num_sel_edges,septr);
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
		case CutFace://ozgur
		sfptr = active->selectFace(x,y);
		GLWidget::setSelectedFace(num_sel_faces,sfptr);
		break;
		case ExtrudeMultipleFaces :
		case MultiSelectFace :
		case SubdivideFace :
									// No duplicates allowed
		// sfptr = active->selectFace(x,y);
		// if ( !GLWidget::isSelected(sfptr) ){
		// 	GLWidget::setSelectedFace(num_sel_faces,sfptr);
		// 	num_sel_faces++;
		// }
		// else if ( GLWidget::isSelected(sfptr) && QApplication::keyboardModifiers() == Qt::ControlModifier) {
		// 	GLWidget::clearSelectedFace(sfptr);
		// 	num_sel_faces--;
		// }
		if ( QApplication::keyboardModifiers() == Qt::ControlModifier) {
			sfptrarr = active->deselectFaces(x,y);
			first = sfptrarr.begin(); last = sfptrarr.end();
			while ( first != last ){
				GLWidget::clearSelectedFace(*first);
				++first;
				num_sel_faces--;
			}
			active->redraw();
			sfptrarr.clear();
		}
		else {
			sfptrarr = active->selectFaces(x,y);
			first = sfptrarr.begin(); last = sfptrarr.end();
			while ( first != last ){
				GLWidget::setSelectedFace(num_sel_faces,*first);
				++first;
				num_sel_faces++;
			}
			active->redraw();
			sfptrarr.clear();
		}

		break;
		case SelectCheckerboard :
		//get one selected face
		sfptr = active->selectFace(x,y);
		if (!GLWidget::isSelected(sfptr) && sfptr){
			GLWidget::setSelectedFace(num_sel_faces,sfptr);
			num_sel_faces++;
			getCheckerboardSelection(sfptr);
			// sfptrarr.push_back(sfptr);
			// DLFLFacePtrArray fparray;
			// sfptr->getNeighboringFaces(fparray);
			// first = fparray.begin(); last = fparray.end();
			// while ( first != last ){
			// 	if (sfptr->sharesOneVertex((*first)) && !GLWidget::isSelected(*first)){
			// 		GLWidget::setSelectedFace(num_sel_faces,(*first));
			// 		num_sel_faces++;
			// 	}
			// 	first++;
			// }//end while first
		}		
		active->redraw();
		sfptrarr.clear();

		break;

		case SelectFaceVertex :
		case ReorderFace :
		case InsertEdge :
		case SpliceCorners :
		case ConnectFaceVertices :
		case BezierConnectFaces :
		case HermiteConnectFaces :
		sfptr = active->selectFace(x,y);
		GLWidget::setSelectedFace(num_sel_faces,sfptr);
		if ( sfptr )
		{
			sfvptr = active->selectFaceVertex(sfptr,x,y);
			GLWidget::setSelectedFaceVertex(num_sel_faceverts,sfvptr);
		}
		break;

		case MultiSelectFaceVertex :
		sfptr = active->selectFace(x,y);
		GLWidget::setSelectedFace(num_sel_faces,sfptr);
		if ( sfptr )
		{
			sfvptr = active->selectFaceVertex(sfptr,x,y);
			if ( !GLWidget::isSelected(sfvptr) )
				GLWidget::setSelectedFaceVertex(num_sel_faceverts,sfvptr);
		}
		break;

		case ConnectEdges :
		sfptr = active->selectFace(x,y);
		GLWidget::setSelectedFace(num_sel_faces,sfptr);
		if ( sfptr )
		{
			septr = active->selectEdge(x,y);
			GLWidget::setSelectedEdge(num_sel_edges,septr);
		}
		break;
	};	
	if ( svptr != NULL || septr != NULL || sfptr != NULL ) redraw();
}

	// Handle keyboard and mouse events
void DLFLWindow::mousePressEvent(QMouseEvent *event) {

	if ( event->buttons() == Qt::LeftButton && mode != NormalMode ){
		doSelection(event->x(),this->size().height()-event->y());
	}
	// else if ( event->buttons() == Qt::RightButton && QApplication::keyboardModifiers() == Qt::ShiftModifier){
	// 	// event->ignore();
	// 	if (!active->isBrushVisible()) active->showBrush();
	// 	mBrushStartX = event->x();
	// 	// mBrushStartY = event->y();
	// }
	else event->ignore();
}


void DLFLWindow::mouseMoveEvent(QMouseEvent *event) {
	// if (active->isBrushVisible()) active->redraw();
	if ( mode != NormalMode )
		doSelection(event->x(),this->size().height()-event->y() );
	// else if ( event->buttons() == Qt::RightButton && QApplication::keyboardModifiers() == Qt::ShiftModifier){
	// 	// event->ignore();
	// 	setBrushSize(mBrushSize+event->x()-mBrushStartX);
	// }
	else event->ignore();
}
void DLFLWindow::mouseReleaseEvent(QMouseEvent *event) 
{
					// The mouse was dragged or released
					// Send this event to the subroutine handling the current event, if any
					// If we are not currently doing anything, nothing will happen
					// If ALT key is pressed we are mainpulating the view which is handled by the viewport
	if ( mode != NormalMode )
	{
		switch ( mode )
		{
			case SelectVertex :
			if ( GLWidget::numSelectedVertices() >= 1 )
			{
				DLFLVertexPtr vp = GLWidget::getSelectedVertex(0);
				vp->print();
				GLWidget::clearSelectedVertices();
				num_sel_verts = 0;
				redraw();
			}
			break;
			case SelectEdge :
			if ( GLWidget::numSelectedEdges() >= 1 ) {
				DLFLEdgePtr septr = GLWidget::getSelectedEdge(0);
				if (septr) {
																						//septr->print();
																						//num_sel_edges = 0;
				}
				GLWidget::clearSelectedEdges();
				redraw();
			}
			break;
			case SelectFace :
			if ( GLWidget::numSelectedFaces() >= 1 )
			{
				DLFLFacePtr fp = GLWidget::getSelectedFace(0);
				fp->print();
				GLWidget::clearSelectedFaces();
				num_sel_faces = 0;
				redraw();
			}
			break;
			case SelectFaceVertex :
			if ( GLWidget::numSelectedFaceVertices() >= 1 )
			{
				DLFLFaceVertexPtr fvp = GLWidget::getSelectedFaceVertex(0);
				fvp->print();
				GLWidget::clearSelectedFaceVertices();
				GLWidget::clearSelectedFaces();
				num_sel_faceverts = 0; num_sel_faces = 0;
				redraw();
			}
			break;
			case InsertEdge :
			if ( GLWidget::numSelectedFaceVertices() >= 2 )
			{
				DLFLFaceVertexPtr sfvptr1, sfvptr2;
				sfvptr1 = GLWidget::getSelectedFaceVertex(0);
				sfvptr2 = GLWidget::getSelectedFaceVertex(1);
				if ( sfvptr1 && sfvptr2 )
				{
					DLFLMaterialPtr mptr = sfvptr1->getFacePtr()->material();
					undoPush();
					setModified(true);
					//object.insertEdge(sfvptr1,sfvptr2,false,mptr);
					DLFL::insertEdge(&object,sfvptr1,sfvptr2,false,mptr);
					GLWidget::clearSelectedFaces();
					GLWidget::clearSelectedFaceVertices();
					num_sel_faceverts = 0; num_sel_faces = 0;
					recomputePatches();
					recomputeNormals();
					redraw();   
				}
			}
			else if ( GLWidget::numSelectedFaceVertices() == 1 )
			{
				num_sel_faceverts=1; num_sel_faces=1;
			}
			break;
			case DeleteEdge :
			if ( GLWidget::numSelectedEdges() >= 1 )
			{
				DLFLEdgePtr septr = GLWidget::getSelectedEdge(0);
				if ( septr )
				{
					undoPush();
					setModified(true);
					//object.deleteEdge(septr,DLFLWindow::delete_edge_cleanup);
					DLFL::deleteEdge( &object, septr, DLFLWindow::delete_edge_cleanup);
					recomputePatches();
					recomputeNormals();
				}
				GLWidget::clearSelectedEdges();
				redraw();
			}
			break;
			case SubdivideEdge :
			if ( GLWidget::numSelectedEdges() >= 1 )
			{
				DLFLEdgePtr septr = GLWidget::getSelectedEdge(0);
				if ( septr )
				{
					undoPush();
					setModified(true);
					//object.subdivideEdge(num_e_subdivs,septr);
					DLFL::subdivideEdge(&object, num_e_subdivs,septr);
					recomputePatches();
					recomputeNormals();
				}
				GLWidget::clearSelectedEdges();
				redraw();
			}
			break;
			case CollapseEdge :
			if ( GLWidget::numSelectedEdges() >= 1 )
			{
				DLFLEdgePtr septr = GLWidget::getSelectedEdge(0);
				if ( septr )
				{
					undoPush();
					setModified(true);
					//object.collapseEdge(septr);
					DLFL::collapseEdge(&object,septr);
					recomputePatches();
					recomputeNormals();
				}
				GLWidget::clearSelectedEdges();
				redraw();
			}
			break;
			case SpliceCorners :
			if ( GLWidget::numSelectedFaceVertices() >= 2 )
			{
				DLFLFaceVertexPtr sfvptr1, sfvptr2;
				sfvptr1 = GLWidget::getSelectedFaceVertex(0);
				sfvptr2 = GLWidget::getSelectedFaceVertex(1);
				if ( sfvptr1 && sfvptr2 )
				{
					DLFLMaterialPtr mptr = sfvptr1->getFacePtr()->material();
					undoPush();
					setModified(true);
					//object.spliceCorners(sfvptr1,sfvptr2);
					DLFL::spliceCorners(&object,sfvptr1,sfvptr2);
					GLWidget::clearSelectedFaces();
					GLWidget::clearSelectedFaceVertices();
					num_sel_faceverts = 0; num_sel_faces = 0;
					recomputePatches();
					recomputeNormals();
					redraw();   
				}
			}
			else if ( GLWidget::numSelectedFaceVertices() == 1 )
			{
				num_sel_faceverts=1; num_sel_faces=1;
			}
			break;
			case ConnectFaces :
			if ( GLWidget::numSelectedFaces() >= 2 )
			{
				DLFLFacePtr sfptr1, sfptr2;
				sfptr1 = GLWidget::getSelectedFace(0);
				sfptr2 = GLWidget::getSelectedFace(1);
				if ( sfptr1 && sfptr2 )
				{
					undoPush();
					setModified(true);
					DLFL::connectFaces(&object,sfptr1,sfptr2,num_segments);
					recomputePatches();
					recomputeNormals();
					GLWidget::clearSelectedFaces();
					redraw();   
				}
			}
			else if ( GLWidget::numSelectedFaces() == 1 )
			{
				num_sel_faces = 1;
			}
			break;
			case ConnectFaceVertices :
			if ( GLWidget::numSelectedFaceVertices() >= 2 )
			{
				DLFLFaceVertexPtr sfvptr1, sfvptr2;
				sfvptr1 = GLWidget::getSelectedFaceVertex(0);
				sfvptr2 = GLWidget::getSelectedFaceVertex(1);
				if ( sfvptr1 && sfvptr2 )
				{
					undoPush();
					setModified(true);
					DLFL::connectFaces(&object,sfvptr1,sfvptr2,num_segments, max_segments);
					GLWidget::clearSelectedFaces();
					GLWidget::clearSelectedFaceVertices();
					num_sel_faceverts = 0; num_sel_faces = 0;
					recomputePatches();
					recomputeNormals();
					redraw();   
				}
			}
			else if ( GLWidget::numSelectedFaceVertices() == 1 )
			{
				num_sel_faceverts = 1; num_sel_faces = 1;
			}
			break;
			case ConnectEdges :
			if ( GLWidget::numSelectedEdges() >= 2 )
			{
				DLFLEdgePtr septr1, septr2;
				DLFLFacePtr sfptr1, sfptr2;
				septr1 = GLWidget::getSelectedEdge(0);
				septr2 = GLWidget::getSelectedEdge(1);
				sfptr1 = GLWidget::getSelectedFace(0);
				sfptr2 = GLWidget::getSelectedFace(1);
				if ( septr1 && septr2 )
				{
					undoPush();
					setModified(true);
					DLFL::connectEdges(&object,septr1,sfptr1,septr2,sfptr2);
					GLWidget::clearSelectedEdges();
					GLWidget::clearSelectedFaces();
					num_sel_edges = 0; num_sel_faces = 0;
					redraw();   
				}
			}
			else if ( GLWidget::numSelectedEdges() == 1 )
			{
				num_sel_edges = 1; num_sel_faces = 1;
			}
			break;
			case ExtrudeFace :
			if ( GLWidget::numSelectedFaces() >= 1 )
			{
				DLFLFacePtr sfptr = GLWidget::getSelectedFace(0);
				if ( sfptr )
				{
					undoPush();
					setModified(true);
					DLFL::extrudeFace(&object,sfptr,extrude_dist,num_extrusions,extrude_rot,extrude_scale);
					recomputePatches();
					recomputeNormals();
				}
				GLWidget::clearSelectedFaces();
				redraw();
			}
			break;
			case ExtrudeMultipleFaces :
			if ( GLWidget::numSelectedFaces() >= 1 )
			{
				DLFLFacePtrArray sfptrarr = GLWidget::getSelectedFaces();
				if ( sfptrarr[0] )
				{
					undoPush();
					setModified(true);
					vector<DLFLFacePtr>::iterator it;
					for(it = sfptrarr.begin(); it != sfptrarr.end(); it++) {
						DLFL::extrudeFace(&object,*it,extrude_dist,num_extrusions,extrude_rot,extrude_scale);
						recomputePatches();
						recomputeNormals();						
					}
				}
				GLWidget::clearSelectedFaces();
				redraw();
			}
			break;
			case ExtrudeFaceDS :
			if ( GLWidget::numSelectedFaces() >= 1 )
			{
				DLFLFacePtr sfptr = GLWidget::getSelectedFace(0);
				if ( sfptr )
				{
					undoPush();
					setModified(true);
					DLFL::extrudeFaceDS(&object,sfptr,extrude_dist,num_extrusions,
						ds_ex_twist,extrude_scale);
					recomputePatches();
					recomputeNormals();
				}
				GLWidget::clearSelectedFaces();
				redraw();
			}
			break;
			case ExtrudeDualFace :
			if ( GLWidget::numSelectedFaces() >= 1 )
			{
				DLFLFacePtr sfptr = GLWidget::getSelectedFace(0);
				if ( sfptr )
				{
					undoPush();
					setModified(true);
					DLFL::extrudeDualFace(&object,sfptr,extrude_dist,num_extrusions,
						extrude_rot,extrude_scale,
						dual_mesh_edges_check);
					recomputePatches();
					recomputeNormals();
				}
				GLWidget::clearSelectedFaces();
				redraw();
			}
			break;
			case ExtrudeFaceDodeca :
			if ( GLWidget::numSelectedFaces() >= 1 )
			{
				DLFLFacePtr sfptr = GLWidget::getSelectedFace(0);
				if ( sfptr )
				{
					undoPush();
					setModified(true);
					DLFL::extrudeFaceDodeca(&object,sfptr,extrude_dist,num_extrusions,
						ds_ex_twist,extrude_scale,
						hexagonalize_dodeca_extrude);
					recomputePatches();
					recomputeNormals();
				}
				GLWidget::clearSelectedFaces();
				redraw();
			}
			break;
			case ExtrudeFaceIcosa :
			if ( GLWidget::numSelectedFaces() >= 1 )
			{
				DLFLFacePtr sfptr = GLWidget::getSelectedFace(0);
				if ( sfptr )
				{
					undoPush();
					setModified(true);
					DLFL::extrudeFaceIcosa(&object,sfptr,extrude_dist,num_extrusions,
						ds_ex_twist,extrude_scale);
					recomputePatches();
					recomputeNormals();
				}
				GLWidget::clearSelectedFaces();
				redraw();
			}
			break;
			case StellateFace :
			if ( GLWidget::numSelectedFaces() >= 1 )
			{
				DLFLFacePtr sfptr = GLWidget::getSelectedFace(0);
				if ( sfptr )
				{
					undoPush();
					setModified(true);
					DLFL::stellateFace(&object,sfptr,extrude_dist);
					recomputePatches();
					recomputeNormals();
				}
				GLWidget::clearSelectedFaces();
				redraw();
			}
			break;
			case DoubleStellateFace :
			if ( GLWidget::numSelectedFaces() >= 1 )
			{
				DLFLFacePtr sfptr = GLWidget::getSelectedFace(0);
				if ( sfptr )
				{
					undoPush();
					setModified(true);
					DLFL::doubleStellateFace(&object,sfptr,extrude_dist);
					recomputePatches();
					recomputeNormals();
				}
				GLWidget::clearSelectedFaces();
				redraw();
			}
			break;
			case CrustModeling :
			if ( GLWidget::numSelectedFaces() >= 1 )
			{
				DLFLFacePtr sfptr = GLWidget::getSelectedFace(0);
				if ( sfptr )
				{
																										// No undo for hole punching in crust modeling mode
																										// because the ids in the recreated object
																										// will be different
					if ( QApplication::keyboardModifiers() == Qt::ShiftModifier )
					{
						DLFL::tagMatchingFaces(&object,sfptr);
						DLFL::punchHoles(&object);
						recomputePatches();
						recomputeNormals();
					}
					else
						DLFL::cmMakeHole(&object,sfptr,crust_cleanup);
//                                                  recomputeNormals();
				}
				GLWidget::clearSelectedFaces();
				redraw();
			}
			break;
			case BezierConnectFaces :
			if ( GLWidget::numSelectedFaceVertices() >= 2 )
			{
				DLFLFaceVertexPtr sfvptr1, sfvptr2;
				sfvptr1 = GLWidget::getSelectedFaceVertex(0);
				sfvptr2 = GLWidget::getSelectedFaceVertex(1);
				if ( sfvptr1 && sfvptr2 )
				{
					undoPush();
					setModified(true);
					DLFL::bezierConnectFaces(&object,sfvptr1,sfvptr2,
						num_segments,nwt1,nwt2);
					GLWidget::clearSelectedFaces();
					GLWidget::clearSelectedFaceVertices();
					num_sel_faceverts = 0; num_sel_faces = 0;
					recomputePatches();
					recomputeNormals();
					redraw();   
				}
			}
			else if ( GLWidget::numSelectedFaceVertices() == 1 )
			{
				num_sel_faceverts = 1; num_sel_faces = 1;
			}
			break;
			case HermiteConnectFaces :
			if ( GLWidget::numSelectedFaceVertices() >= 2 )
			{
				DLFLFaceVertexPtr sfvptr1, sfvptr2;
				sfvptr1 = GLWidget::getSelectedFaceVertex(0);
				sfvptr2 = GLWidget::getSelectedFaceVertex(1);
				if ( sfvptr1 && sfvptr2 )
				{
					undoPush();
					setModified(true);
					if ( symmetric_weights )
						DLFL::hermiteConnectFaces(&object,sfvptr1,sfvptr2,
						num_segments,nwt1,nwt1,
						max_segments,num_extra_twists);
					else
						DLFL::hermiteConnectFaces(&object,sfvptr1,sfvptr2,
						num_segments,nwt1,nwt2,
						max_segments,num_extra_twists);
					GLWidget::clearSelectedFaces();
					GLWidget::clearSelectedFaceVertices();
					num_sel_faceverts = 0; num_sel_faces = 0;
					recomputePatches();
					recomputeNormals();
					redraw();
				}
			}
			else if ( GLWidget::numSelectedFaceVertices() == 1 )
			{
				num_sel_faceverts = 1; num_sel_faces = 1;
			}
			break;
			case ReorderFace :
			if ( GLWidget::numSelectedFaceVertices() >= 1 )
			{
				DLFLFaceVertexPtr sfvptr = GLWidget::getSelectedFaceVertex(0);
				if ( sfvptr ) sfvptr->getFacePtr()->reorder(sfvptr);
				redraw();
			}
			break;
			case SubdivideFace :
			case MultiSelectFace :
			// num_sel_faces++;
			break;
//from ozgur
			case CutEdge :
			if ( GLWidget::numSelectedEdges() >= 1 )
			{
				DLFLEdgePtr septr = GLWidget::getSelectedEdge(0);
				if ( septr )
				{
					undoPush();
					septr->ismarked = 1- septr->ismarked;
																		// DLFL::cutEdge( &object, septr, peelDistance_factor,pnormalBendS_factor,pnormalBendT_factor, peel_all_edges);
				}
				GLWidget::clearSelectedEdges();
				redraw();
			}
			break;
			case TruncateEdge :
			if ( GLWidget::numSelectedEdges() >= 1 )
			{
				DLFLEdgePtr septr = GLWidget::getSelectedEdge(0);
				if ( septr )
				{
					undoPush();
					septr->ismarked = 1- septr->ismarked;
																		// DLFL::cutEdge( &object, septr, peelDistance_factor,pnormalBendS_factor,pnormalBendT_factor, peel_all_edges);
				}
				GLWidget::clearSelectedEdges();
				redraw();
			}
			break;
			case CutVertex :
			if ( GLWidget::numSelectedVertices() >= 1 )
			{
				DLFLVertexPtr svptr = GLWidget::getSelectedVertex(0);
				if ( svptr )
				{
					undoPush();
					svptr->ismarked = 1 - svptr->ismarked;
				}
				// GLWidget::clearSelectedVertices();
				redraw();
			}
			break;

			case CutEdgeandVertex :
			if ( GLWidget::numSelectedEdges() >= 1 )
			{	
				if ( GLWidget::numSelectedEdges() >= 1 ) {
					DLFLVertexPtr svptr = GLWidget::getSelectedVertex(0);
					DLFLEdgePtr septr = GLWidget::getSelectedEdge(0);
					if ( septr ) {
						if (svptr) {
							undoPush();
							septr->ismarked = 1- septr->ismarked;
						// DLFL::cutEdge( &object, septr, peelDistance_factor,pnormalBendS_factor,pnormalBendT_factor, peel_all_edges);
						}
					}
					GLWidget::clearSelectedEdges();
					GLWidget::clearSelectedVertices();
					redraw();
				}
			}
			break;
			case MarkEdge:
			if ( GLWidget::numSelectedEdges() >= 1 )
			{
				DLFLEdgePtr septr = GLWidget::getSelectedEdge(0);
				if ( septr )
				{
					undoPush();
					septr->ismarked = 1- septr->ismarked;
				}
				GLWidget::clearSelectedEdges();
				redraw();
			}
			break;

			case MarkVertex :
			if ( GLWidget::numSelectedVertices() >= 1 )
			{
				DLFLVertexPtr svptr = GLWidget::getSelectedVertex(0);
				if ( svptr )
				{
					undoPush();
					svptr->ismarked = 1 - svptr->ismarked;
				}
				GLWidget::clearSelectedVertices();
				redraw();
			}
			break;

			case CutFace :
			if ( GLWidget::numSelectedFaces() >= 1 )
			{
				DLFLFacePtr sfptr = GLWidget::getSelectedFace(0);
				if ( sfptr )
				{
					undoPush();
					sfptr->ismarked = 1 - sfptr->ismarked;
				}
				GLWidget::clearSelectedFaces();
				redraw();
			}
			break;
		}//end switch (mode)

	}//end if (mode != NormalMode)
	// else if ( event->buttons() == Qt::RightButton && QApplication::keyboardModifiers() == Qt::ShiftModifier){
	// 	// event->ignore();
	// 	mBrushStartX = 0;
	// 	// mBrushStartY = 0;
	// }
	else event->ignore();
}//end function mousereleaseevent


void DLFLWindow::testConvexHull(void)
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

void DLFLWindow::performRemeshing(void)
{
	switch ( remeshingscheme )
	{
		case Dual :
		createDual();
		break;
		case Root3 :
		subdivideRoot3();
		break;
		case DualVertexTrunc :
		subdivideSqrt3();
		break;
		case GlobalStellate :
		globalStellate();
		break;
		case Star :
		subdivideStar();
		break;
		case Generic1264 :
		subdivide1264();
		break;
		case Honeycomb :
		subdivideHoneycomb();
		break;
		case VertexTrunc :
		subdivideVertexCutting();
		break;
		case DualGeneric1264 :
		subdivideDual1264();
		break;
		case LinearVertexInsertion :
		subdivideLinearVertexInsertion();
		break;
		case CatmullClark :
		subdivideCatmullClark();
		break;
		case ModifiedStellate :
		subStellate1();
		break;
		case DooSabin :
		subdivideDooSabin();
		break;
		case CornerCutting :
		subdivideCornerCutting();
		break;
		case Simplest :
		subdivideSimplest();
		break;
		case Pentagonal :
		subdividePentagonal();
		break;
		case CubicPentagonal :
		subdivideCubicPentagonal();
		break;
		case DualPentagonal :
		subdivideDualPentagonal();
		break;
		case LoopStyle :
		subdivideLoopStyle();
		break;
		case Loop :
		subdivideLoop();
		break;
		case Root4 :
		subdivideRoot4();
		break;
		case DualLoop :
		subdivideDualLoop();
		break;
		case GlobalCubicExtrude :
		cout << "NOT YET IMPLEMENTED!!!" << endl;
		break;
		case CheckerBoard :
		subdivideCheckerBoard();
		break;
		case DualGlobalCubicExtrude :
		cout << "NOT YET IMPLEMENTED!!!" << endl;
		break;
		case DualCheckerBoard :
		subdivideDualCheckerBoard();
		break;
		case PentagonPreserving :
		subdividePentagonPreserving();
		break;
		case DualPentagonPreserving :
		subdivideDualPentagonPreserving();
		break;
		case HexagonPreserving :
		subdivideRoot4();
		break;
		case DualHexagonPreserving :
		subdivideDualHexagonPreserving();
		break;
		case Fractal :
		subdivideFractal();
		break;
		case ModifiedDoubleStellate :
		subStellate2();
		break;
		case Dome :
		subdivideDome();
		break;
		case DooSabinBC :
		subdivideDooSabinBC();
		break;
		case DooSabinBCNew :
		subdivideDooSabinBCNew();
		break;
	}
	// setMode(NormalMode);
	setMode(mode);
	redraw();
}

			// Change the renderer for all viewports
void DLFLWindow::setRenderer(DLFLRendererPtr rp)
{
	// top->setRenderer(rp);
	persp->setRenderer(rp);
	// front->setRenderer(rp);
	// right->setRenderer(rp);
}

// DLFLRendererPtr getRenderer(){
// 	return active->getRenderer();
// }

// void DLFLWindow::setPatchRenderer()
// {
// 	this->setRenderer(patch);
// }


// Do appropriate selection depending on current mode
// Mouse position is passed. Selected entities will be added
// to the selection lists in the GLWidget class, which are assumed
// to have been cleared before calling this function

// void doSelection(int x, int y);

// Override show() method to show subwindows also
void DLFLWindow::show(void) {
	active->show();
}

// Return pointer to the active GLWidget
GLWidget *DLFLWindow::getActive() {
	return active;
}

// Override redraw() method to mark subwindows also for redraw
void DLFLWindow::redraw() {
	active->redraw();
}

// Override resize() method to properly resize subwindows
void DLFLWindow::resize(int x, int y, int w, int h) { }

//--- Methods to perform various operations ---//

// Display overlays
void DLFLWindow::toggleWireframe(void)         // Toggle display of wireframe
{
	active->toggleWireframe();
}

void DLFLWindow::toggleSilhouette(void)       // Toggle display of silhouette
{
	active->toggleSilhouette();
}

void DLFLWindow::toggleVertices(void)             // Toggle display of points
{
	active->toggleVertices();
}

void DLFLWindow::toggleFaceIDs( ) {
	active->toggleFaceIDs( );
}

void DLFLWindow::toggleVertexIDs( ) {
	active->toggleVertexIDs( );
}

void DLFLWindow::toggleEdgeIDs( ) {
	active->toggleEdgeIDs( );
}

void DLFLWindow::turnOffOverlays(void)               // Turn off all overlays
{
	active->setRenderFlags(0);
}

void DLFLWindow::toggleObjectOrientation(void)    // Reverse displayed object
{
	active->toggleObjectOrientation();
}

void DLFLWindow::toggleNormals(void)    // Reverse displayed object
{
	active->toggleNormals();
}

// Switch to specified operating mode
void DLFLWindow::setMode(Mode m)
{
	mode = m;
	switch ( mode )	{
		case SelectVertex :
		DLFLWindow::num_sel_verts = 0;
		break;
		case SelectEdge :
		DLFLWindow::num_sel_edges = 0;
		break;
		case SelectFace :
		DLFLWindow::num_sel_faces = 0;
		break;
		case SelectFaceVertex :
		DLFLWindow::num_sel_faceverts = 0;
		break;
		case NormalMode:
		default :
									// Nothing to be done for other modes except clearing selection lists
		DLFLWindow::clearSelected();
		break;
	}
	if (mode != MultiSelectFace)
		active->hideBrush();
	else active->showBrush();
}

void DLFLWindow::setRemeshingScheme(RemeshingScheme scheme)
{
	remeshingscheme = scheme;
}

// Geometric transformations
void DLFLWindow::translatex(double x)
{
	object.position[0] = x;
}

void DLFLWindow::translatey(double y)
{
	object.position[1] = y;
}

void DLFLWindow::translatez(double z)
{
	object.position[2] = z;
}

void DLFLWindow::scalex(double x)
{
	object.scale_factor[0] = x;
}

void DLFLWindow::scaley(double y)
{
	object.scale_factor[1] = y;
}

void DLFLWindow::scalez(double z)
{
	object.scale_factor[2] = z;
}

void DLFLWindow::freezeTransforms(void)
{
	object.freezeTransformations();
	object.position.reset();
	object.scale_factor.set(1,1,1);
	object.rotation.reset();
}

// Global operations (don't require selection)
void DLFLWindow::recomputeNormals(void)     // Recompute normals and lighting
{
  object.computeNormals();
  computeLighting( &object, patchObject, &plight);
}

void DLFLWindow::recomputeLighting(void)                // Recompute lighting
{
  computeLighting( &object, patchObject, &plight);
}

void DLFLWindow::recomputePatches(void) // Recompute the patches for patch rendering
{
  if(patchObject)
    patchObject->updatePatches(&object);
}

void DLFLWindow::subdivideAllEdges(void)              // Sub-divide all edges
{
	undoPush();
	DLFL::subdivideAllEdges(&object,DLFLWindow::num_e_subdivs);
	DLFLWindow::clearSelected();
}

void DLFLWindow::subdivideSelectedFaces(void) // Subdivide all selected faces
{
	undoPush();
	DLFLFacePtrArray fparray;
	fparray.resize(GLWidget::numSelectedFaces());
	for (int i=0; i < GLWidget::numSelectedFaces(); ++i)	{
		fparray[i] = GLWidget::getSelectedFace(i);
	}
	DLFL::subdivideFaces(&object,fparray,use_quads);
	DLFLWindow::clearSelected();
}

void DLFLWindow::subdivideAllFaces(void) // Subdivide all the faces
{
	undoPush();
	DLFL::subdivideAllFaces(&object,use_quads);
	DLFLWindow::clearSelected();
}

void DLFLWindow::createMultiFaceHandle(void) // Create multi-face handle between selected faes
{
	int numsel = GLWidget::numSelectedFaces();
	if ( numsel < 3 ) return;
	undoPush();
	DLFLFacePtrArray sel_faces;
	DLFLFacePtr sfptr;
	for (int i=0; i < numsel; ++i)	{
		sfptr = GLWidget::getSelectedFace(i);
		if ( sfptr ) sel_faces.push_back(sfptr);
	}
	switch ( DLFLWindow::mfh_algo )	{
		case ConvexHull :
		DLFL::multiConnectFaces(&object,sel_faces,DLFLWindow::mfh_scale_factor,
			DLFLWindow::mfh_extrude_dist,
			DLFLWindow::mfh_use_max_offsets);
		break;
		case ClosestEdge :
		DLFL::multiConnectFaces(&object,sel_faces);
		break;
		default :
		break;
	}
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::multiConnectMidpoints(void)
{
	// Multi-connect midpoints after simplest-subdivision without edge deletion
	undoPush();
	DLFL::multiConnectMidpoints(&object);
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::multiConnectCrust(void)
{
	// Multi-connect after creating crust
	undoPush();
	DLFL::multiConnectCrust(&object);
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::modifiedMultiConnectCrust(void)
{
	// Modified multi-connect after creating crust
	undoPush();
	DLFL::modifiedMultiConnectCrust(&object);
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::createSponge(void)
{
	undoPush();
	DLFL::createSponge(&object,DLFLWindow::sponge_thickness,
		DLFLWindow::sponge_collapse_threshold);
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::planarizeFaces(void)                  // Planarize all faces
{
	undoPush();
	DLFL::planarize(&object);
	recomputePatches();
	recomputeNormals();
}

void DLFLWindow::spheralizeObject(void)         // Spheralize object vertices
{
	undoPush();
	DLFL::spheralize(&object);
	recomputePatches();
	recomputeNormals();
}

void DLFLWindow::smoothMesh(void)                          // Smooth the mesh
{
	undoPush();
	DLFL::meshsmooth(&object);
	recomputePatches();
	recomputeNormals();
}

void DLFLWindow::subdivideCatmullClark(void)     // Catmull-Clark subdivision
{
	undoPush();
	DLFL::catmullClarkSubdivide(&object);
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::subdivideDooSabin(void)             // Doo-Sabin subdivision
{
	undoPush();
	DLFL::dooSabinSubdivide(&object,doo_sabin_check);
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::subdivideHoneycomb(void)            // Honeycomb subdivision
{
	undoPush();
	DLFL::honeycombSubdivide(&object);
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::subdivideRoot4(void)                   // Root-4 subdivision
{
	undoPush();
	DLFL::root4Subdivide( &object, DLFLWindow::weight_factor,DLFLWindow::twist_factor);
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::subdivideCornerCutting(void)   // Corner-cutting subdivision
{
	undoPush();
	DLFL::cornerCuttingSubdivide(&object);
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::subdivideLinearVertexInsertion(void) // Bi-linear Vertex-insertion remeshing
{
	undoPush();
	DLFL::subdivideAllFaces(&object,true);
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::subdivideSimplest(void)        // Corner-cutting subdivision
{
	undoPush();
	DLFL::simplestSubdivide(&object);
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::subdivideVertexCutting(void)   // Vertex-cutting subdivision
{
	undoPush();
	DLFL::vertexCuttingSubdivide(&object,DLFLWindow::vertex_cutting_offset);
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::subdividePentagonal(void)          // Pentagonal subdivision
{
	undoPush();
	DLFL::pentagonalSubdivide(&object,DLFLWindow::pentagonal_offset);
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::subdivideCubicPentagonal(void) // Cubic Pentagonal remeshing scheme
{
// Implemented as Pentagonal + Dual + Dual
	undoPush();
	DLFL::pentagonalSubdivide(&object,DLFLWindow::pentagonal_offset);
	DLFL::createDual(&object,true); // Use accurate method
	DLFL::createDual(&object,true); // Use accurate method
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::subdivideDualPentagonal(void) // Dual-Pentagonal subdivision
{
// Implemented as Dual + Pentagonal + Dual
	undoPush();
	DLFL::createDual(&object,true); // Use accurate method
	DLFL::pentagonalSubdivide(&object,DLFLWindow::pentagonal_offset);
	DLFL::createDual(&object,true); // Use accurate method
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::subdividePentagonPreserving(void) // Pentagon preserving remeshing
{
	undoPush();
	DLFL::pentagonalSubdivide2(&object,DLFLWindow::pentagonal_scale);
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::subdivideDualPentagonPreserving(void) // Dual Pentagon preserving remeshing
{
// Implemented as Dual + Pentagonal + Dual
	undoPush();
	DLFL::createDual(&object,true); // Use accurate method
	DLFL::pentagonalSubdivide2(&object,DLFLWindow::pentagonal_scale);
	DLFL::createDual(&object,true); // Use accurate method
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::subdivideDualHexagonPreserving(void) // Dual Hexagon Preserving remeshing
{
// Implemented as Dual + Root4 + Dual
	undoPush();
	DLFL::createDual(&object,true); // Use accurate method
	DLFL::root4Subdivide(&object,DLFLWindow::weight_factor,DLFLWindow::twist_factor);
	DLFL::createDual(&object,true); // Use accurate method
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::subdivideRoot3(void)                     // Root-3 remeshing
{
// Implemented as Dual + Honeycomb + Dual
	undoPush();
	DLFL::createDual(&object,true); // Use accurate method
	DLFL::honeycombSubdivide(&object);
	DLFL::createDual(&object,true); // Use accurate method
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::subdivideLoop(void)                      // Loop subdivision
{
	undoPush();
	DLFL::loopSubdivide(&object);
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::subdivideDualLoop(void)          // Dual of Loop subdivision
{
// Implemented as Dual + Loop + Dual
	undoPush();
	DLFL::createDual(&object,true); // Use accurate method
	DLFL::loopSubdivide(&object);
	DLFL::createDual(&object,true); // Use accurate method
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::subdivide1264(void)                      // 12-6-4 remeshing
{
// Implemented as Dual + Dual 12-6-4 + Dual
	undoPush();
	DLFL::createDual(&object,true); // Use accurate method
	DLFL::dual1264Subdivide(&object,DLFLWindow::dual1264_scale_factor);
	DLFL::createDual(&object,true); // Use accurate method
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::subdivideDual1264(void) // Dual of 12-6-4 remeshing - Bei & Cansin
{
	undoPush();
	DLFL::dual1264Subdivide(&object,DLFLWindow::dual1264_scale_factor);
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::subdivideCheckerBoard(void)       // Checker board remeshing
{
	undoPush();
	DLFL::checkerBoardRemeshing(&object,DLFLWindow::checkerboard_thickness);
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::subdivideDualCheckerBoard(void) // Dual Checker board remeshing
{
// Implemented as Dual + Checker board + Dual
	undoPush();
	setModified(true);
	DLFL::createDual(&object,true); // Use accurate method
	DLFL::checkerBoardRemeshing(&object,DLFLWindow::checkerboard_thickness);
	DLFL::createDual(&object,true); // Use accurate method
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::subdivideStar(void)               // Star subdivision - Doug
{
	undoPush();
	setModified(true);
	DLFL::starSubdivide(&object,DLFLWindow::star_offset);
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::subdivideSqrt3(void)           // sqrt(3) subdivision - Doug
{
	undoPush();
	setModified(true);
	DLFL::sqrt3Subdivide(&object);
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::subdivideFractal(void)                     // fractal - Doug
{
	undoPush();
	setModified(true);
	DLFL::fractalSubdivide(&object,DLFLWindow::fractal_offset);
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::subStellate1(void)            // stellate subdivision - Eric
{
	undoPush();
	setModified(true);
	DLFL::stellateSubdivide(&object);
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::subStellate2(void)            // stellate subdivision - Eric
{
	undoPush();
	setModified(true);
	DLFL::twostellateSubdivide(&object,DLFLWindow::substellate_height, DLFLWindow::substellate_curve);
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::subdivideDome(void)                          // Bei & Cansin
{
	undoPush();
	setModified(true);
	DLFL::domeSubdivide(&object,DLFLWindow::domeLength_factor, DLFLWindow::domeScale_factor);
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::subdivideDooSabinBC(void) // Doo-Sabin(BC) subdivision - Bei & Cansin
{
	undoPush();
	setModified(true);
	DLFL::dooSabinSubdivideBC(&object,DLFLWindow::doo_sabin_check);
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::subdivideDooSabinBCNew(void) // Doo-Sabin(BCNew) Bei & Cansin
{
	undoPush();
	setModified(true);
	DLFL::dooSabinSubdivideBCNew(&object,DLFLWindow::dooSabinBCnewScale_factor,
		DLFLWindow::dooSabinBCnewLength_factor);
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::subdivideLoopStyle(void)     // Loop-style subdivision - Bei
{
	undoPush();
	setModified(true);
	DLFL::loopStyleSubdivide(&object,DLFLWindow::loopLength_factor);
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::globalStellate(void)
{
	// Does not use length parameter for now. Uses subdivideFace method with triangles
	undoPush();
	setModified(true);
	DLFL::subdivideAllFaces(&object,false);
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::splitValence2Vertices(void)      // Split Valence 2 vertices
{
	undoPush();
	setModified(true);
	DLFL::splitValence2Vertices(&object,DLFLWindow::vertex_split_offset);
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::cleanupWingedVertices(void)     // Remove valence 2 vertices
{
	undoPush();
	setModified(true);
	DLFL::cleanupWingedVertices(&object);
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::createDual(void)                       // Create dual object
{
	undoPush();
	setModified(true);
	DLFL::createDual(&object,DLFLWindow::accurate_dual);
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::createCrust(bool use_scaling)        // Create a crust
{
	undoPush();
	setModified(true);
	if ( use_scaling ) 
	  DLFL::createCrustWithScaling(&object,DLFLWindow::crust_scale_factor);
	else 
	  DLFL::createCrust(&object,DLFLWindow::crust_thickness);
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::createCrust2(bool use_scaling) {
	// DLFLFacePtrArray::iterator first, last;
	vector<DLFLFacePtr>::iterator it;
			
	undoPush();
	setModified(true);
	if ( use_scaling ) 
	  DLFL::createCrustWithScaling(&object,DLFLWindow::crust_scale_factor);
	else 
	  DLFL::createCrust(&object,DLFLWindow::crust_thickness);
	recomputePatches();
	recomputeNormals();
	if ( GLWidget::numSelectedFaces() >= 1 ) {
		DLFLFacePtrArray sfptrarr = GLWidget::getSelectedFaces();
		if ( sfptrarr[0] ) {
			for(it = sfptrarr.begin(); it != sfptrarr.end(); it++) {
				DLFL::cmMakeHole(&object,*it,crust_cleanup);
			}
		}
	}
	recomputePatches();
	recomputeNormals();
	GLWidget::clearSelectedFaces();
	setMode(DLFLWindow::MultiSelectFace);
	redraw();	
}



void DLFLWindow::makeWireframe(void)                    // Create a wireframe
{
	undoPush();
	setModified(true);
	DLFL::makeWireframe(&object,DLFLWindow::wireframe_thickness);
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::makeWireframeWithColumns(void) // Create a wireframe using columns
{
	undoPush();
	setModified(true);
	DLFL::makeWireframeWithColumns(&object,DLFLWindow::column_thickness, DLFLWindow::column_segments);
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}
/*
void DLFLWindow::makeUnitCube(double edgelength)
{
	undoPush();
	setModified(true);
	DLFLObjectPtr unitcube = DLFLObject::makeUnitCube(edgelength);
	object.reset();
	object.splice(*unitcube);
	delete unitcube;
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::makeUnitTetrahedron(double edgelength)
{
	undoPush();
	setModified(true);
	DLFLObjectPtr unittetra = DLFLObject::makeUnitTetrahedron();
	object.reset();
	object.splice(*unittetra);
	delete unittetra;
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::makeMengerSponge(int level)
{
	undoPush();
	setModified(true);
	DLFLObjectPtr mengersponge = DLFLObject::makeMengerSponge(level);
	object.reset();
	object.splice(*mengersponge);
	delete mengersponge;
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::makeSierpinskiTetrahedron(int level)
{
	undoPush();
	setModified(true);
	DLFLObjectPtr stetra = DLFLObject::makeSierpinskiTetrahedron(level);
	object.reset();
	object.splice(*stetra);
	delete stetra;
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}
*/
void DLFLWindow::randomAssignTexCoords(void) // Randomly assign texture coordinates
{
	object.randomAssignTexCoords();
}

void DLFLWindow::assignTileTexCoords(void) // Assign texture coordinates for tile texturing
{
	object.assignTileTexCoords(DLFLWindow::tile_tex_n);
}

void DLFLWindow::edgeCleanup(void) // Cleanup redundant edges
{
	undoPush();
	setModified(true);
	DLFL::edgeCleanup(&object);
	recomputePatches();
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::printSummary(void)              // Print summary information
{
	object.print();
}

void DLFLWindow::printVertexList(void)                   // Print vertex list
{
	object.printVertexList();
}

void DLFLWindow::printEdgeList(void)                       // Print edge list
{
	object.printEdgeList();
}

void DLFLWindow::printCVList(void)                       // Print CV list
{
  ///object.printPatchCVList();
}

void DLFLWindow::printFaceList(void)                       // Print face list
{
	object.printFaces();
}

// Axes and grid
void DLFLWindow::toggleAxes(void)        // Toggle display of coordinate axes
{
	active->toggleAxes();
}

void DLFLWindow::toggleGrid(void)                   // Toggle display of grid
{
	active->toggleGrid();
}

	// Read the DLFL object from a file
void DLFLWindow::readObject(const char * filename) {
	active->clearSelected();
	ifstream file;
	file.open(filename);
	if ( strstr(filename,".dlfl") || strstr(filename,".DLFL") )
		object.readDLFL(file);
	else if ( strstr(filename,".obj") || strstr(filename,".OBJ") )
		object.readObject(file);
	file.close();
}

	// Read the DLFL object from a file
void DLFLWindow::readObjectQFile(QString filename) {
	active->clearSelected();
	QFile file(filename);
	file.open(QIODevice::ReadOnly);

	QByteArray ba = file.readAll();
	const char *filecontents = ba.data();
	string str(filecontents);
	istringstream filestring(str);

	if ( filename.indexOf(".dlfl") == filename.length()-4 || filename.indexOf(".dlfl") == filename.length()-4 )
		object.readDLFL(filestring);
	else if ( filename.indexOf(".OBJ") == filename.length()-4 || filename.indexOf(".obj") == filename.length()-4 )
		object.readObject(filestring);
	file.close();

#ifdef WITH_PYTHON
  DLFLObjectPtr obj = &object;
  if( obj )
    emit loadedObject(obj,filename);
#endif
}

	// Read the DLFL object from a file - use alternate OBJ reader for OBJ files
void DLFLWindow::readObjectAlt(const char * filename) {
	active->clearSelected();
	ifstream file;
	file.open(filename);
	if ( strstr(filename,".dlfl") || strstr(filename,".DLFL") )
		object.readDLFL(file);
	else if ( strstr(filename,".obj") || strstr(filename,".OBJ") )
		object.readObjectAlt(file);
	file.close();
}

void DLFLWindow::readObjectOBJ(const char * filename) {
	active->clearSelected();
	ifstream file;
	file.open(filename);
	object.readObject(file);
	file.close();
}

void DLFLWindow::readObjectDLFL(const char * filename) {
	active->clearSelected();
	ifstream file;
	file.open(filename);
	object.readDLFL(file);
	file.close();
}

	// Write the DLFL object to a file
void DLFLWindow::writeObject(const char * filename, bool with_normals, bool with_tex_coords) {
	ofstream file;
	file.open(filename);
	if ( strstr(filename,".dlfl") || strstr(filename,".DLFL") )
		object.writeDLFL(file);
	else if ( strstr(filename,".obj") || strstr(filename,".OBJ") )
		object.writeObject(file,with_normals,with_tex_coords);
	file.close();
}

void DLFLWindow::writeObjectOBJ(const char * filename, bool with_normals, bool with_tex_coords) {
	ofstream file;
	file.open(filename);
	object.writeObject(file,with_normals,with_tex_coords);
	file.close();
}

/* stuart - bezier export */
void DLFLWindow::writePatchOBJ( const char *filename ) {
	ofstream file;
	file.open(filename);
	//object.objPatchWrite(file);
	file.close();
}

void DLFLWindow::writeObjectDLFL(const char * filename) {
	ofstream file;
	file.open(filename);
	object.writeDLFL(file);
	file.close();
}

// File handling
void DLFLWindow::openFile(void) {
  QString fileName = QFileDialog::getOpenFileName(this,
						  tr("Open File..."),
						  "$HOME",
						  tr("All Supported Files (*.obj *.dlfl);;Wavefront Files (*.obj);;DLFL Files (*.dlfl);;All Files (*)"));
  if (!fileName.isEmpty()){
    if (!curFile.isEmpty()){
      undoPush();
      setModified(false);
    }
    QByteArray ba = fileName.toLatin1();
    const char *filename = ba.data();
    mWasPrimitive = false;
    mIsPrimitive = false;
    readObject(filename);
#ifdef WITH_PYTHON
    // Emit and send to python script editor
    DLFLObjectPtr obj = &object;
    if( obj )
      emit loadedObject(obj,fileName);
#endif
    recomputePatches();
    recomputeNormals();
    setCurrentFile(fileName);
    active->redraw();
  }
}

void DLFLWindow::openFile(QString fileName){
	QFile file(fileName);
	file.open(QIODevice::ReadOnly);
	QFileInfo info(file);
	QByteArray ba = info.absoluteFilePath().toLatin1();
	const char *filename = ba.data();
	mWasPrimitive = false;
	mIsPrimitive = false;
	if (!curFile.isEmpty()){
		undoPush();
		setModified(false);
	}

	readObject(filename);
#ifdef WITH_PYTHON
	DLFLObjectPtr obj = &object;
	if( obj )
	  emit loadedObject(obj,fileName);
#endif
	recomputePatches();
	recomputeNormals();
	setCurrentFile(fileName);
	active->redraw();
}

bool DLFLWindow::saveFile(bool with_normals, bool with_tex_coords) {
	if (!curFile.isEmpty() && !mIsPrimitive){
		QByteArray ba = curFile.toLatin1();
		const char *filename = ba.data();
		writeObject(filename,with_normals,with_tex_coords);
		return true;
	}
	else {
		QString fileName = QFileDialog::getSaveFileName(this,
			tr("Save File As..."),
			curFile,
			tr("All Supported Files (*.obj *.dlfl);;Wavefront Files (*.obj);;DLFL Files (*.dlfl);;All Files (*)"));
		if (!fileName.isEmpty()){
			if (!fileName.indexOf(".obj") || !fileName.indexOf(".dlfl") || !fileName.indexOf(".OBJ") || !fileName.indexOf(".DLFL"))
				fileName.append(".dlfl");
			setCurrentFile(fileName);	
			QByteArray ba = fileName.toLatin1();
			const char *filename = ba.data();
			writeObject(filename,with_normals,with_tex_coords);
			return true;
		}
	}
	return false;
}

bool DLFLWindow::saveFileAs(bool with_normals, bool with_tex_coords) {
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save File As..."),
		curFile,
		tr("All Supported Files (*.obj *.dlfl);;Wavefront Files (*.obj);;DLFL Files (*.dlfl);;All Files (*)"));
	if (!fileName.isEmpty()){
		if (!fileName.indexOf(".obj") || !fileName.indexOf(".dlfl") || !fileName.indexOf(".OBJ") || !fileName.indexOf(".DLFL"))
			fileName.append(".dlfl");
		setCurrentFile(fileName);				
		QByteArray ba = fileName.toLatin1();
		const char *filename = ba.data();
		writeObject(filename,with_normals,with_tex_coords);
		return true;
	}
	return false;
}

void DLFLWindow::openFileOBJ(void) {	
	// QString fileName = QFileDialog::getOpenFileName(this,
	// 																								tr("Open File..."),
	// 																								"$HOME",
	// 																								tr("Wavefront Files (*.obj);;All Files (*)"));
	// if (!fileName.isEmpty()) {
	// 	
	// 	setCurrentFile(fileName);
	// 	undoPush();
	// 	setModified(true);
	// 	QByteArray ba = fileName.toLatin1();
	// 	const char *filename = ba.data();
	// 	readObjectOBJ(filename);
	// 	recomputePatches();
	// 	recomputeNormals();
	// }
}

void DLFLWindow::saveFileOBJ(bool with_normals, bool with_tex_coords) {
	// QString fileName = QFileDialog::getSaveFileName(this,
	// 																								tr("Save File As..."),
	// 																								curFile,
	// 																								tr("Wavefront Files (*.obj);;All Files (*)"));
	// if (!fileName.isEmpty()){
	// 	QByteArray ba = fileName.toLatin1();
	// 	const char *filename = ba.data();
	// 	writeObjectOBJ(filename,with_normals,with_tex_coords);
	// }
}

/* stuart - bezier export */
bool DLFLWindow::saveFileBezierOBJ( ) {
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save Bezier Patch (OBJ)..."),
		curFile,
		tr("Wavefront Files (*.obj);;All Files (*)"));
	if (!fileName.isEmpty()){
		QByteArray ba = fileName.toLatin1();
		const char *filename = ba.data();
		writePatchOBJ(filename);
		return true;
	}
	return false;
}

void DLFLWindow::openFileDLFL(void) {
	// QString fileName = QFileDialog::getOpenFileName(this,
	// 																								tr("Open File..."),
	// 																								"$HOME",
	// 																								tr("DLFL Files (*.dlfl);;All Files (*)"));
	// if (!fileName.isEmpty()){
	// 	setCurrentFile(fileName);
	// 	undoPush();
	// 	setModified(true);
	// 	QByteArray ba = fileName.toLatin1();
	// 	const char *filename = ba.data();
	// 	readObjectDLFL(filename);
	// 	recomputePatches();
	// 	recomputeNormals();
	// }
}

void DLFLWindow::saveFileDLFL(void) {

	// QString fileName = QFileDialog::getSaveFileName(this,
	// 																								tr("Save File As..."),
	// 																								curFile,
	// 																								tr("DLFL Files (*.dlfl);;All Files (*)"));
	// if (!fileName.isEmpty()){
	// 	QByteArray ba = fileName.toLatin1();
	// 	const char *filename = ba.data();
	// 	writeObjectDLFL(filename);
	// }
}

void DLFLWindow::setCurrentFile(QString fileName) {

	curFile = fileName;
	QString shownName;
	if (curFile.isEmpty())
		shownName = "untitled.obj";
	else
		shownName = QFileInfo(curFile).fileName();

	((MainWindow*)mParent)->setWindowTitle( tr("%1[*] - %2").arg(shownName).arg(tr("TopMod")));
	((MainWindow*)mParent)->setWindowModified(false);
}

void DLFLWindow::cleanupForExit(void) // Do memory cleanup if any before exit
{
// Nothing to be done
}

//conical stuff from ozgur
void DLFLWindow::performCutting(void) {
	undoPush();
	setModified(true);
	DLFL::performCutting(&object,DLFLWindow::mode,DLFLWindow::cutOffsetE_factor,DLFLWindow::cutOffsetV_factor,DLFLWindow::global_cut,DLFLWindow::selected_cut);
	// recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::createConvexHull(void) {
	undoPush();
	setModified(true);
	DLFL::createConvexHull(&object);
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::createDualConvexHull(void) {
	undoPush();
	setModified(true);
	DLFL::createDualConvexHull(&object);
	recomputeNormals();
	DLFLWindow::clearSelected();
}

void DLFLWindow::loadCube(){	
	if (isModified())
		undoPush();
	setModified(false);
	mIsPrimitive = true;
	mWasPrimitive = true;
	setCurrentFile(tr("cube.obj"));
	readObjectQFile(":/cube.obj");
	recomputePatches();
	recomputeNormals();
	active->redraw();
}

void DLFLWindow::loadOctahedron(){	
	if (isModified())
		undoPush();
	setModified(false);
	mIsPrimitive = true;
	mWasPrimitive = true;
	setCurrentFile(tr("octahedron.obj"));
	readObjectQFile(":/octahedron.obj");
	recomputePatches();
	recomputeNormals();
	active->redraw();
}

void DLFLWindow::loadTetrahedron(){	
	if (isModified())
		undoPush();
	setModified(false);
	mIsPrimitive = true;
	mWasPrimitive = true;
	setCurrentFile(tr("tetrahedron.obj"));
	readObjectQFile(":/tetrahedron.obj");
	recomputePatches();
	recomputeNormals();
	active->redraw();
}

void DLFLWindow::loadDodecahedron(){	
	if (isModified())
		undoPush();
	setModified(false);
	mIsPrimitive = true;
	mWasPrimitive = true;
	setCurrentFile(tr("dodecahedron.obj"));
	readObjectQFile(":/dodecahedron.obj");
	recomputePatches();
	recomputeNormals();
	active->redraw();
}

void DLFLWindow::loadIcosahedron(){	
	if (isModified())
		undoPush();
	setModified(false);	
	mIsPrimitive = true;
	mWasPrimitive = true;
	setCurrentFile(tr("icosahedron.obj"));
	readObjectQFile(":/icosahedron.obj");
	recomputePatches();
	recomputeNormals();
	active->redraw();
}

void DLFLWindow::loadSoccerball(){	
	if (isModified())
		undoPush();
	setModified(false);
	mIsPrimitive = true;
	mWasPrimitive = true;
	setCurrentFile(tr("soccerball.obj"));
	readObjectQFile(":/soccerball.obj");
	recomputePatches();
	recomputeNormals();
	active->redraw();
}

void DLFLWindow::loadGeodesic(){	
	if (isModified())
		undoPush();
	setModified(false);
	mIsPrimitive = true;
	mWasPrimitive = true;
	setCurrentFile(tr("geodesic.obj"));
	readObjectQFile(":/geodesic.obj");
	recomputePatches();
	recomputeNormals();
	active->redraw();
}

bool DLFLWindow::isModified(){
	return mIsModified;
}

void DLFLWindow::setModified(bool isModified){
	mIsModified = isModified;
	((DLFLWindow*)mParent)->setWindowModified(mIsModified);	
}

void DLFLWindow::switchPerspView(){
	active->switchTo(VPPersp);
	active->redraw();
}

void DLFLWindow::switchTopView(){
	active->switchTo(VPTop);
	active->redraw();
}

void DLFLWindow::switchBottomView(){
	active->switchTo(VPBottom);
	active->redraw();
}

void DLFLWindow::switchRightView(){
	active->switchTo(VPRight);
	active->redraw();
}

void DLFLWindow::switchLeftView(){
	active->switchTo(VPLeft);
	active->redraw();
}

void DLFLWindow::switchFrontView(){
	active->switchTo(VPFront);
	active->redraw();
}

void DLFLWindow::switchBackView(){
	active->switchTo(VPBack);
	active->redraw();
}

//recurse through selected faces to get a list of checkerboard selection for the entire object
void DLFLWindow::getCheckerboardSelection(DLFLFacePtr fptr) {
	// fparray.clear();
	// sfptr = active->selectFace(x,y);
	if (fptr){
		int numShared = 0;
		DLFLFacePtrArray fparray;
		vector<DLFLFacePtr>::iterator it;
		fptr->getNeighboringFaces(fparray);
		for ( it = fparray.begin(); it != fparray.end(); it++){
			if (fptr->sharesOneVertex((*it)) && !GLWidget::isSelected(*it)){
				numShared++;
				GLWidget::setSelectedFace(num_sel_faces,(*it));
				num_sel_faces++;
				getCheckerboardSelection((*it));
			}
		}//end for loop
		if (numShared == 0) return; //break out of recursive loop is there are no one vertex sharing faces
	}		
	
	// active->redraw();
}
