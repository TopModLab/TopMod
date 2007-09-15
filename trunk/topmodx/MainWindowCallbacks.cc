/* $Id: MainWindowCallbacks.cc,v 4.13 2004/01/20 22:18:36 vinod Exp $ */

// Definitions for callback functions in the MainWindow class
// All these are static methods

#include "MainWindow.hh"

void MainWindow::load_texture() {
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open File..."),
		"$HOME",
		tr("All Supported Files (*.jpg *.png);;JPEG Files (*.jpg);;PNG Files (*.png);;All Files (*)"),
		0, QFileDialog::DontUseSheet);
		
	if (!fileName.isEmpty()){
		// undoPush();
		QByteArray ba = fileName.toLatin1();
		const char *texfile = ba.data();
		if (texfile != NULL){
			textured->setTexture(texfile);
			texturedlit->setTexture(texfile);
			// getActive()->valid(0); // To ensure new texture is loaded into OpenGL
			redraw();
		}
		// readObject(filename);
		// active->recomputePatches();
 		// active->recomputeNormals();
		// //loadFile(fileName);
		// active->redraw();
	}
}

// void DLFLAppWindow :: load_texture(Fl_Menu_*, void*)
// {
//   char * texfile = fl_file_chooser("Load Texture","*.{jpg,png}","");
//   if ( texfile != NULL )
//      {
//        textured->setTexture(texfile);
//        texturedlit->setTexture(texfile);
//        mainWindowPtr->getActive()->valid(0); // To ensure new texture is loaded into OpenGL
//        mainWindowPtr->redraw();
//        Fl :: focus(mainWindowPtr);
//      }
// }

//use a specific renderer...
void MainWindow::useWireframeRenderer() {
	setRenderer(wired);
	redraw();
}

void MainWindow::useNormalRenderer() {
	setRenderer(normal);
	redraw();
}

void MainWindow::useLightedRenderer() {
	active->recomputeNormals();
	setRenderer(lit);
	redraw();
}

void MainWindow::useTexturedRenderer() {
	setRenderer(textured);
	redraw();
}

void MainWindow::useTexturedLitRenderer() {
	setRenderer(texturedlit);
	redraw();
}

void MainWindow::usePatchRenderer() {
	setRenderer(patch);
	redraw();
}

//preference functions for topmodpreferences - dave
void MainWindow::setAutoSave(int value){
	//start the timer if it's ON
	//stop timer if it's OFF
	mAutoSave = bool(value);
	if (mAutoSave)
		mAutoSaveTimer->start();
	else 
		mAutoSaveTimer->stop();
}

//this value is sent in minutes... so multiply it by 6000!
void MainWindow::setAutoSaveDelay(double value){
	mAutoSaveDelay = value;
	mAutoSaveTimer->setInterval(mAutoSaveDelay*60000);
	// mAutoSaveTimer->setInterval(5000);
}

//incremental save will just save a new file each time...
void MainWindow::setIncrementalSave(int value){
	mIncrementalSave = (bool)value;
}

void MainWindow::setIncrementalSaveMax(double value){
	mIncrementalSaveMax = (int)value;
}

void MainWindow::setSaveDirectory(QString s){
	mSaveDirectory = s;
}

void MainWindow::checkSaveDirectory(){
	QDir dir(mSaveDirectory);
	if (dir.exists()){
		// std::cout<<"good job\n";
	}
	else {
		QString s = QString("Directory\n\n%1\n\ndoes not exist, please type a different directory name.\n\nCheck to make sure there are no unnecessary spaces at the end.").arg(mSaveDirectory);
		QMessageBox::about(this, tr("TopMod Error"), s);
	}
}

void MainWindow::setCommandCompleterIndexToggle(int value){
	//stuff here...?
	std::cout << "toggle command completer thing\n";
	mCommandCompleterIndexToggle = (bool)value;
}

// Selection Menu.
void MainWindow::select_vertex() {
	setMode(MainWindow::SelectVertex);
}

void MainWindow::edit_vertex() {
	setMode(MainWindow::EditVertex);
}

void MainWindow::select_face() {
	setMode(MainWindow::SelectFace);
}

void MainWindow::select_face_loop() {
	setMode(MainWindow::SelectFaceLoop);
}

void MainWindow::select_edge_ring() {
	setMode(MainWindow::SelectEdgeRing);
}

void MainWindow::select_multiple_faces() {
	setMode(MainWindow::MultiSelectFace);
}

void MainWindow::select_multiple() {
	switch (selectionmask){
		case MaskVertices:
		setMode(MainWindow::MultiSelectVertex);
		break;
		case MaskEdges: 
		setMode(MainWindow::MultiSelectEdge);
		break;
		case MaskFaces://face stuff
		setMode(MainWindow::MultiSelectFace);
		break;
		case MaskCorners:
		break;
		default:
		break;
	};
}

void MainWindow::select_multiple_edges() {
	setMode(MainWindow::MultiSelectEdge);
}

void MainWindow::select_multiple_vertices() {
	setMode(MainWindow::MultiSelectVertex);
}

void MainWindow::select_similar_faces() {
	// setMode(MainWindow::SelectSimilarFaces);
}

void MainWindow::select_similar() {
	// switch(selectionmask){
	// 	case MaskFaces:
	setMode(MainWindow::SelectSimilar);
	// 	break;
	// 	case MaskEdges:
	// 	setMode(MainWindow::SelectSimilarEdges);
	// 	break;
	// 	case MaskVertices:
	// 	setMode(MainWindow::SelectSimilarVertices);
	// 	break;
	// 	default:
	// 	break;
	// };
}

void MainWindow::select_faces_by_area() {
	setMode(MainWindow::SelectFacesByArea);
}

void MainWindow::selection_window() {
	setMode(MainWindow::SelectionWindow);
}

void MainWindow::select_checkerboard_faces() {
	setMode(MainWindow::SelectCheckerboard);
}

void MainWindow::select_edge() {
	setMode(MainWindow::SelectEdge);
}

void MainWindow::select_edge_loop() {
	setMode(MainWindow::SelectEdgeLoop);
}

void MainWindow::select_corner() {
	setMode(MainWindow::SelectCorner);
}

void MainWindow::selectionMaskVertices() {
	// setSelectionMask(MainWindow::MaskVertices);
	setMode(MainWindow::SelectVertex);
}

void MainWindow::selectionMaskFaces() {
	// setSelectionMask(MainWindow::MaskFaces);
	setMode(MainWindow::SelectFace);
}

void MainWindow::selectionMaskEdges() {
	// setSelectionMask(MainWindow::MaskEdges);
	setMode(MainWindow::SelectEdge);
	
}

void MainWindow::selectionMaskCorners() {
	// setSelectionMask(MainWindow::MaskCorners);
	setMode(MainWindow::SelectCorner);
	
}

void MainWindow::selectAll(){
	switch (selectionmask){
		case MaskVertices:
		active->selectAllVertices();
		break;
		case MaskEdges:
		active->selectAllEdges();
		break;
		case MaskFaces:
		active->selectAllFaces();
		break;
		case MaskCorners:
		// active->selectAllCorners();
		case MaskObject:
		break;
		default:
		// active->selectAllVertices();
		break;
	};
}

void MainWindow::selectInverse(){
	switch (selectionmask){
		case MaskVertices:
		active->selectInverseVertices();
		break;
		case MaskEdges:
		active->selectInverseEdges();
		break;
		case MaskFaces:
		active->selectInverseFaces();
		break;
		case MaskCorners:
		active->selectInverseFaceVertices();
		case MaskObject:
		break;
		default:
		// active->selectInverseVertices();
		break;
	};
}
void MainWindow::exit_selection_mode() {
	setMode(MainWindow::NormalMode);
	redraw();
}

void MainWindow::clear_selected(){
	MainWindow::clearSelected();
	redraw();
}

// Basics.
void MainWindow::toggleDeleteEdgeCleanupFlag(int state) {
	MainWindow::delete_edge_cleanup = bool(state);
}

void MainWindow::changeNumSubDivs(double value) {
	MainWindow::num_e_subdivs = (int)value;
}

void MainWindow::changeFaceAreaTolerance(double value) {
	MainWindow::face_area_tolerance = value;
}

void MainWindow::changeTileTexNum(double value) {
	MainWindow::tile_tex_n = (int)value;
}

// Extrusions.
void MainWindow::changeExtrudeLength(double value) {
	MainWindow::extrude_dist = value;
}

void MainWindow::changeExtrudeRotation(double value) {
	MainWindow::extrude_rot = (int)value;
}

void MainWindow::changeExtrudeScale(double value) {
	MainWindow::extrude_scale = value;
}

void MainWindow::changeExtrudeLength1(double value) {
	MainWindow::extrude_length1 = value;
}

void MainWindow::changeExtrudeLength2(double value) {
	MainWindow::extrude_length2 = value;
}

void MainWindow::changeExtrudeLength3(double value) {
	MainWindow::extrude_length3 = value;
}

void MainWindow::changeExtrudeAngle(double value){
	MainWindow::extrude_angle = value;
}

void MainWindow::changeExtrudeLength1Icosa(double value) {
	MainWindow::extrude_length1_icosa = value;
}

void MainWindow::changeExtrudeLength2Icosa(double value) {
	MainWindow::extrude_length2_icosa = value;
}

void MainWindow::changeExtrudeLength3Icosa(double value) {
	MainWindow::extrude_length3_icosa = value;
}

void MainWindow::changeExtrudeAngleIcosa(double value){
	MainWindow::extrude_angle_icosa = value;
}

void MainWindow::changeNumExtrusions(double value) {
	MainWindow::num_extrusions = (int)value;
}

void MainWindow::toggleDualMeshEdgesFlag(int state) {
	MainWindow::dual_mesh_edges_check = bool(state);
}

void MainWindow::toggleHexagonalizeDodecaExtrudeFlag(int state) {
	MainWindow::hexagonalize_dodeca_extrude = bool(state);
}

// Stellation
void MainWindow::changeStellateLength(double value) {
	MainWindow::extrude_dist = value;
}

// Holes and Handles.
void MainWindow::changeMaxSegments(double value) {
	MainWindow::max_segments = (int)value;
}

void MainWindow::changeNumSegments(double value) {
	MainWindow::num_segments = (int)value;
}

void MainWindow::changeNumSegments2(double value){

}

void MainWindow::changeMaxSegments2(double value){

}

void MainWindow::changeWeight1(double value) {
	MainWindow::nwt1 = value;
}

void MainWindow::toggleSymmetricWeightsFlag(int state) {
	MainWindow::symmetric_weights = bool(state);
}

void MainWindow::changeWeight2(double value) {
	MainWindow::nwt2 = value;
}

void MainWindow::changeExtraTwists(double value) {
	MainWindow::num_extra_twists = (int)value;
}

// Crust Modeling.
void MainWindow::changeCrustScaleFactor(double value) {
	MainWindow::crust_scale_factor = value;
}

void MainWindow::changeCrustThickness(double value) {
	MainWindow::crust_thickness = value;
}

void MainWindow::toggleCrustCleanupFlag(int state) {
	MainWindow::crust_cleanup = bool(state);
}

void MainWindow::crustModeling1() {
	setMode(MainWindow::CrustModeling);
	createCrust(true);
	redraw();
}

void MainWindow::crustModeling2() {
	setMode(MainWindow::CrustModeling);
	createCrust(false);
	redraw();
}

void MainWindow::crustModeling3() {
// setMode(MainWindow::CrustModelingPainting);
	createCrust2(false);
	redraw();
}

void MainWindow::crustModeling4() {
// setMode(MainWindow::CrustModelingPainting);
	createCrust2(true);
	redraw();
}

//for modified corner cutting scheme... it's the same as the first step in the wireframe process
void MainWindow::changeModifiedCornerCuttingThickness(double value) {
	MainWindow::modified_corner_cutting_thickness = value;
}

//column modeling and wireframe modeling
void MainWindow::changeWireframeThickness(double value) {
	MainWindow::wireframe_thickness = value;
}

void MainWindow::changeWireframe2Thickness(double value) {
	MainWindow::wireframe2_thickness = value;
}

void MainWindow::changeWireframe2Width(double value) {
	MainWindow::wireframe2_width = value;
}

void MainWindow::changeColumnThickness(double value) {
	MainWindow::column_thickness = value;
}

void MainWindow::changeColumnSegments(double value) {
	MainWindow::column_segments = (int)value;
}

// Multi-face handle
void MainWindow::mfh_use_closest_edge_algo() {
	MainWindow::mfh_algo = MainWindow::ClosestEdge;
}

void MainWindow::mfh_use_convex_hull_algo() {
	MainWindow::mfh_algo = MainWindow::ConvexHull;
}

void MainWindow::changeMultiFaceHandleScaleFactor(double value) {
	MainWindow::mfh_scale_factor = value;
}

void MainWindow::changeMultiFaceHandleExtrudeDist(double value) {
	MainWindow::mfh_extrude_dist = value;
}

void MainWindow::toggleMultiFaceHandleUseMaxOffsetFlag(int state) {
	MainWindow::mfh_use_max_offsets = bool(state);
}

//menger sponge operations
void MainWindow::changeMengerSpongeThickness(double value) {
	MainWindow::sponge_thickness = value;
}

void MainWindow::changeMengerSpongeCollapseThreshold(double value) {
	MainWindow::sponge_collapse_threshold = value;
}

void MainWindow::toggleMengerSpongeFractionalThicknessFlag(int state) {
	MainWindow::sponge_fractional_thickness = bool(state);
}

/* begin ozgur */
//sculpting operations from ozgur
//conical stuff from ozgur
void MainWindow::performCutting() {
	// DLFLEdgePtrArray eparray;
	// DLFLVertexPtrArray vparray;
	// DLFLEdgePtrArray fparray;

	undoPush();
	setModified(true);
	// void performCutting( DLFLObjectPtr obj, int type,float offsetE,float offsetV,bool global,bool selected) {
	// DLFL::performCutting(&object, MainWindow::mode,MainWindow::cutOffsetE_factor,MainWindow::cutOffsetV_factor,MainWindow::global_cut,MainWindow::selected_cut);
	switch (selectionmask){
		case MaskEdges:
		// std::cout << "cut by edge!\n";
		// active->getSelectedEdges(eparray);
		DLFL::cutSelectedEdges(&object, MainWindow::cutOffsetE_factor,MainWindow::cutOffsetV_factor);
		break;
		case MaskVertices:
		// std::cout << "cut by vertex!\n";
		// active->getSelectedVertices(vparray);
		DLFL::cutSelectedVertices(&object, MainWindow::cutOffsetE_factor,MainWindow::cutOffsetV_factor);
		break;
		case MaskFaces:
		// std::cout << "cut by face!\n";
		// active->getSelectedFaces(fparray);
		DLFL::cutSelectedFaces(&object, MainWindow::cutOffsetE_factor,MainWindow::cutOffsetV_factor);
		break;
		default:
		break;
	};
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::createConvexHull() {
	undoPush();
	setModified(true);
	DLFL::createConvexHull(&object);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::createDualConvexHull() {
	undoPush();
	setModified(true);
	DLFL::createDualConvexHull(&object);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::changeCutOffsetE(double value) {
	MainWindow::cutOffsetE_factor= value;
}

void MainWindow::changeCutOffsetV(double value) {
	MainWindow::cutOffsetV_factor= value;
}

void MainWindow::changePNormalBendS(double value) {
	MainWindow:: pnormalBendS_factor= value;
}

void MainWindow::changePNormalBendT(double value) {
	MainWindow:: pnormalBendT_factor= value;
}

void MainWindow::toggleGlobalCut(int state){    //ozgur
	MainWindow::global_cut = bool(state);	
}
void MainWindow::toggleSelectedCut(int state){  //ozgur
	MainWindow::selected_cut = bool(state);
}

void MainWindow::changeTiltPlane1(double value){

}
void MainWindow::changeTiltPlane2(double value){

}
/* end ozgur */

// Geometric transformations
void MainWindow::translatex(double x) {
	Vector3d oldtr = object.position;
	object.position[0] = x;
	object.tr.transform[0][3] = x;
	redraw();
}

void MainWindow::translatey(double y) {
	object.position[1] = y;
	object.tr.transform[1][3] = y;
	redraw();
}

void MainWindow::translatez(double z) {
	object.position[2] = z;
	object.tr.transform[2][3] = z;
	redraw();
}

void MainWindow::scalex(double x) {
	object.scale_factor[0] = x;
	object.tr.transform[0][0] = x;
	redraw();
}

void MainWindow::scaley(double y) {
	object.scale_factor[1] = y;
	object.tr.transform[1][1] = y;
	redraw();
}

void MainWindow::scalez(double z) {
	object.scale_factor[2] = z;
	object.tr.transform[2][2] = z;
	redraw();
}

void MainWindow::freezeTransforms() {
	object.freezeTransformations();
	object.position.reset();
	object.scale_factor.set(1,1,1);
	object.rotation.reset();
	object.tr.reset();
	active->recomputePatches();
	active->recomputeLighting();
	redraw();
}

// // Global operations (don't require selection)
// void MainWindow::active->recomputeNormals(void)     // Recompute normals and lighting
// {
// 	object.computeNormals();
// 	computeLighting( &object, patchObject, &plight);
// }
// 
// void MainWindow::recomputeLighting(void)                // Recompute lighting
// {
// 	computeLighting( &object, patchObject, &plight);
// }
// 
// void MainWindow::active->recomputePatches(void) // Recompute the patches for patch rendering
// {
//   if(patchObject)
//     patchObject->updatePatches(&object);
// }
// 
void MainWindow::subdivideAllEdges(void)              // Sub-divide all edges
{
	undoPush();
	DLFL::subdivideAllEdges(&object,MainWindow::num_e_subdivs);
	MainWindow::clearSelected();
	active->recomputePatches();
	active->recomputeNormals();
	redraw();
}

void MainWindow::subdivideSelectedEdges(void) // Subdivide all selected edges
{
	undoPush();
	// DLFLEdgePtrArray eparray;
	// eparray.resize(active->numSelectedEdges());
	for (int i=0; i < active->numSelectedEdges(); ++i)	{
		subdivideEdge(&object,MainWindow::num_e_subdivs, active->getSelectedEdge(i));
	}  
	// DLFL::subdivideEdges(&object,fparray,use_quads);
	MainWindow::clearSelected();
	active->recomputePatches();
	active->recomputeNormals();
	redraw();
}

void MainWindow::subdivideSelectedFaces(void) // Subdivide all selected faces
{
	undoPush();
	DLFLFacePtrArray fparray;
	fparray.resize(active->numSelectedFaces());
	for (int i=0; i < active->numSelectedFaces(); ++i)	{
		fparray[i] = active->getSelectedFace(i);
	}
	DLFL::subdivideFaces(&object,fparray,use_quads);
	MainWindow::clearSelected();
	active->recomputePatches();
	active->recomputeNormals();
	redraw();
}

void MainWindow::subdivideAllFaces(void) // Subdivide all the faces
{
	undoPush();
	DLFL::subdivideAllFaces(&object,use_quads);
	MainWindow::clearSelected();
	active->recomputePatches();
	active->recomputeNormals();
	redraw();
}

void MainWindow::createMultiFaceHandle(void) // Create multi-face handle between selected faces
{
	int numsel = active->numSelectedFaces();
	if ( numsel < 3 ) return;
	undoPush();
	DLFLFacePtrArray sel_faces;
	DLFLFacePtr sfptr;
	for (int i=0; i < numsel; ++i)	{
		sfptr = active->getSelectedFace(i);
		if ( sfptr ) sel_faces.push_back(sfptr);
	}
	switch ( MainWindow::mfh_algo )	{
		case ConvexHull :
		DLFL::multiConnectFaces(&object,sel_faces,MainWindow::mfh_scale_factor,MainWindow::mfh_extrude_dist,MainWindow::mfh_use_max_offsets);
		break;
		case ClosestEdge :
		DLFL::multiConnectFaces(&object,sel_faces);
		break;
		default :
		break;
	}
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::multiConnectMidpoints(void)
{
	// Multi-connect midpoints after simplest-subdivision without edge deletion
	undoPush();
	DLFL::multiConnectMidpoints(&object);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::multiConnectCrust(void)
{
	// Multi-connect after creating crust
	undoPush();
	DLFL::multiConnectCrust(&object);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::modifiedMultiConnectCrust(void)
{
	// Modified multi-connect after creating crust
	undoPush();
	DLFL::modifiedMultiConnectCrust(&object);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::createSponge(void)
{
	undoPush();
	DLFL::createSponge(&object,MainWindow::sponge_thickness,
		MainWindow::sponge_collapse_threshold);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::planarizeFaces(void)                  // Planarize all faces
{
	undoPush();
	DLFL::planarize(&object);
	active->recomputePatches();
	active->recomputeNormals();
	redraw();
}

void MainWindow::spheralizeObject(void)         // Spheralize object vertices
{
	undoPush();
	DLFL::spheralize(&object);
	active->recomputePatches();
	active->recomputeNormals();
	redraw();
}

void MainWindow::smoothMesh(void)                          // Smooth the mesh
{
	undoPush();
	DLFL::meshsmooth(&object);
	active->recomputePatches();
	active->recomputeNormals();
	redraw();
}

void MainWindow::subdivideCatmullClark(void)     // Catmull-Clark subdivision
{
	undoPush();
	DLFL::catmullClarkSubdivide(&object);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
	QString cmd( "subdivide(\"catmull-clark\")");
	emit echoCommand( cmd );
	// redraw();
}

void MainWindow::subdivideDooSabin(void)             // Doo-Sabin subdivision
{
	undoPush();
	
	// QProgressDialog *progress = new QProgressDialog("Performing Doo Sabin Remeshing...", "Cancel", 0, 1);
	// progress->setMinimumDuration(2000);
	// progress->setWindowModality(Qt::WindowModal);
	
	// DLFLObjectPtr t = object;
	// active->setRenderingEnabled(false);
	
	// if (!DLFL::dooSabinSubdivide(&object, doo_sabin_check/* , progress*/) ){
		// active->recomputePatches();
		// active->recomputeNormals();
		// undo();
		// MainWindow::clearSelected();
		// active->setRenderingEnabled(true);
	// }
	// else {
		// object = t;
		// delete t;
		// active->setRenderingEnabled(true);
		DLFL::dooSabinSubdivide(&object, doo_sabin_check/* , progress*/);
		active->recomputePatches();
		active->recomputeNormals();
		MainWindow::clearSelected();
		QString cmd( "subdivide(\"doo-sabin\",");
		QString check("False");
		if( doo_sabin_check )
			check = QString("True");
		cmd += check + QString(")");
		emit echoCommand( cmd );
	// }
	
}

void MainWindow::subdivideHoneycomb(void)            // Honeycomb subdivision
{
	undoPush();
	DLFL::honeycombSubdivide(&object);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();

	QString cmd("subdivide(\"honeycomb\")");
	emit echoCommand( cmd );
}

void MainWindow::subdivideRoot4(void)                   // Root-4 subdivision
{
	undoPush();
	DLFL::root4Subdivide( &object, MainWindow::weight_factor,MainWindow::twist_factor);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
	QString cmd( "subdivide(\"root4\",");
	cmd += QString().setNum(MainWindow::weight_factor) + QString(",");
	cmd += QString().setNum(MainWindow::twist_factor) + QString(")");
	emit echoCommand( cmd );
}

void MainWindow::subdivideCornerCutting(void)   // Corner-cutting subdivision
{
	undoPush();
	DLFL::cornerCuttingSubdivide(&object);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
	QString cmd("subdivide(\"corner-cut\")");
	emit echoCommand( cmd );
}

void MainWindow::subdivideModifiedCornerCutting(void)   // Modified Corner-cutting subdivision
{
	undoPush();
	DLFL::modifiedCornerCuttingSubdivide(&object,MainWindow::modified_corner_cutting_thickness);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
	// QString cmd("subdivide(\"modified-corner-cut\")");
	// emit echoCommand( cmd );
}


void MainWindow::subdivideLinearVertexInsertion(void) // Bi-linear Vertex-insertion remeshing
{
	undoPush();
	DLFL::subdivideAllFaces(&object,true);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
	QString cmd("subdivide(\"linear-vertex\",True)");
	emit echoCommand( cmd );
}

void MainWindow::subdivideSimplest(void)        // Corner-cutting subdivision
{
	undoPush();
	DLFL::simplestSubdivide(&object);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
	QString cmd("subdivide(\"simplest\")");
	emit echoCommand( cmd );
}

void MainWindow::subdivideVertexCutting(void)   // Vertex-cutting subdivision
{
	undoPush();
	DLFL::vertexCuttingSubdivide(&object,MainWindow::vertex_cutting_offset);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();

	QString cmd = QString("subdivide(\"vertex-cut\"," )
		+ QString().setNum(MainWindow::vertex_cutting_offset)
		+ QString(")");
	emit echoCommand( cmd );
}

void MainWindow::subdividePentagonal(void)          // Pentagonal subdivision
{
	undoPush();
	DLFL::pentagonalSubdivide(&object,MainWindow::pentagonal_offset);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
	QString cmd( "subdivide(\"pentagon\",");
	cmd += QString().setNum(MainWindow::pentagonal_offset) + QString(")");
	emit echoCommand( cmd );
}

void MainWindow::subdivideCubicPentagonal(void) // Cubic Pentagonal remeshing scheme
{
// Implemented as Pentagonal + Dual + Dual
	undoPush();
	DLFL::pentagonalSubdivide(&object,MainWindow::pentagonal_offset);
	DLFL::createDual(&object,true); // Use accurate method
	DLFL::createDual(&object,true); // Use accurate method
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
	QString cmd( "subdivide(\"pentagon\",");
	cmd += QString().setNum(MainWindow::pentagonal_offset) + QString(")");
	cmd += QString("\ndual()\ndual()");
	emit echoCommand( cmd );
}

void MainWindow::subdivideDualPentagonal(void) // Dual-Pentagonal subdivision
{
// Implemented as Dual + Pentagonal + Dual
	undoPush();
	DLFL::createDual(&object,true); // Use accurate method
	DLFL::pentagonalSubdivide(&object,MainWindow::pentagonal_offset);
	DLFL::createDual(&object,true); // Use accurate method
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
	QString cmd( "dual()\nsubdivide(\"pentagon\",");
	cmd += QString().setNum(MainWindow::pentagonal_offset) + QString(")");
	cmd += QString("\ndual()");
	emit echoCommand( cmd );
}

void MainWindow::subdividePentagonPreserving(void) // Pentagon preserving remeshing
{
	undoPush();
	DLFL::pentagonalSubdivide2(&object,MainWindow::pentagonal_scale);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
	QString cmd( "subdivide(\"pentagon-preserve\",");
	cmd += QString().setNum(MainWindow::pentagonal_scale) + QString(")");
	emit echoCommand( cmd );
}

void MainWindow::subdivideDualPentagonPreserving(void) // Dual Pentagon preserving remeshing
{
// Implemented as Dual + Pentagonal + Dual
	undoPush();
	DLFL::createDual(&object,true); // Use accurate method
	DLFL::pentagonalSubdivide2(&object,MainWindow::pentagonal_scale);
	DLFL::createDual(&object,true); // Use accurate method
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
	QString cmd( "dual()\nsubdivide(\"pentagon-preserve\",");
	cmd += QString().setNum(MainWindow::pentagonal_scale) + QString(")");
	cmd += QString("\ndual()");
	emit echoCommand( cmd );
}

void MainWindow::subdivideDualHexagonPreserving(void) // Dual Hexagon Preserving remeshing
{
// Implemented as Dual + Root4 + Dual
	undoPush();
	DLFL::createDual(&object,true); // Use accurate method
	DLFL::root4Subdivide(&object,MainWindow::weight_factor,MainWindow::twist_factor);
	DLFL::createDual(&object,true); // Use accurate method
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
	QString cmd( "dual()\nsubdivide(\"root4\",");
	cmd += QString().setNum(MainWindow::weight_factor) + QString(",");
	cmd += QString().setNum(MainWindow::twist_factor) + QString(")");
	cmd += QString("\ndual()");
	emit echoCommand( cmd );
}

void MainWindow::subdivideRoot3(void)                     // Root-3 remeshing
{
// Implemented as Dual + Honeycomb + Dual
	undoPush();
	DLFL::createDual(&object,true); // Use accurate method
	DLFL::honeycombSubdivide(&object);
	DLFL::createDual(&object,true); // Use accurate method
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();

	QString cmd = QString("dual()\n");
	cmd += QString("subdivide(\"honeycomb\")\n");
	cmd += QString("dual()");
	emit echoCommand( cmd );
}

void MainWindow::subdivideLoop(void)                      // Loop subdivision
{
	undoPush();
	DLFL::loopSubdivide(&object);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();

	QString cmd("subdivide(\"loop\")\n");
	emit echoCommand( cmd );
}

void MainWindow::subdivideDualLoop(void)          // Dual of Loop subdivision
{
// Implemented as Dual + Loop + Dual
	undoPush();
	DLFL::createDual(&object,true); // Use accurate method
	DLFL::loopSubdivide(&object);
	DLFL::createDual(&object,true); // Use accurate method
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();

	QString cmd = QString("dual()\n");
	cmd += QString("subdivide(\"loop\")\n");
	cmd += QString("dual()");
	emit echoCommand( cmd );
}

void MainWindow::subdivide1264(void)                      // 12-6-4 remeshing
{
// Implemented as Dual + Dual 12-6-4 + Dual
	undoPush();
	DLFL::createDual(&object,true); // Use accurate method
	DLFL::dual1264Subdivide(&object,MainWindow::dual1264_scale_factor);
	DLFL::createDual(&object,true); // Use accurate method
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();

	QString cmd = QString("dual()\ndual()\n");
	cmd += QString("subdivide(\"dual-12.6.4\",")
		+ QString().setNum(MainWindow::dual1264_scale_factor) 
		+ QString(")\n");
	cmd += QString("dual()");
	emit echoCommand(cmd);
}

void MainWindow::subdivideDual1264(void) // Dual of 12-6-4 remeshing - Bei & Cansin
{
	undoPush();
	DLFL::dual1264Subdivide(&object,MainWindow::dual1264_scale_factor);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();

	QString cmd("subdivide(\"dual-12.6.4\",");
	cmd += QString().setNum(MainWindow::dual1264_scale_factor);
	cmd += QString(")");
	emit echoCommand(cmd);
}

void MainWindow::subdivideCheckerBoard(void)       // Checker board remeshing
{
	undoPush();
	DLFL::checkerBoardRemeshing(&object,MainWindow::checkerboard_thickness);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
	QString cmd("subdivide(\"checker\",");
	cmd += QString().setNum(MainWindow::checkerboard_thickness);
	cmd += QString(")");
	emit echoCommand(cmd);
}

void MainWindow::subdivideDualCheckerBoard(void) // Dual Checker board remeshing
{
// Implemented as Dual + Checker board + Dual
	undoPush();
	setModified(true);
	DLFL::createDual(&object,true); // Use accurate method
	DLFL::checkerBoardRemeshing(&object,MainWindow::checkerboard_thickness);
	DLFL::createDual(&object,true); // Use accurate method
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
	QString cmd("dual()\nsubdivide(\"checker\",");
	cmd += QString().setNum(MainWindow::checkerboard_thickness);
	cmd += QString(")\ndual()");
	emit echoCommand(cmd);
}

void MainWindow::subdivideStar(void)               // Star subdivision - Doug
{
	undoPush();
	setModified(true);
	DLFL::starSubdivide(&object,MainWindow::star_offset);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();

	QString cmd = QString("subdivide(\"star\")");
	emit echoCommand( cmd );
}

void MainWindow::subdivideSqrt3(void)           // sqrt(3) subdivision - Doug
{
	undoPush();
	setModified(true);
	DLFL::sqrt3Subdivide(&object);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();

	QString cmd = QString("subdivide(\"sqrt3\")");
	emit echoCommand( cmd );
}

void MainWindow::subdivideFractal(void)                     // fractal - Doug
{
	undoPush();
	setModified(true);
	DLFL::fractalSubdivide(&object,MainWindow::fractal_offset);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();

	QString cmd("subdivide(\"fractal\",");
	cmd += QString().setNum(MainWindow::fractal_offset);
	cmd += QString(")");
	emit echoCommand(cmd);
}

void MainWindow::subStellate1(void)            // stellate subdivision - Eric
{
	undoPush();
	setModified(true);
	DLFL::stellateSubdivide(&object);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();

	QString cmd = QString("subdivide(\"stellate\")");
	emit echoCommand( cmd );
}

void MainWindow::subStellate2(void)            // stellate subdivision - Eric
{
	undoPush();
	setModified(true);
	DLFL::twostellateSubdivide(&object,MainWindow::substellate_height, MainWindow::substellate_curve);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();

	QString cmd = QString("subdivide(\"double-stellate\",");
	cmd += QString().setNum(MainWindow::substellate_height) + QString(",");
	cmd += QString().setNum(MainWindow::substellate_curve) + QString(")");
	emit echoCommand( cmd );
}

void MainWindow::subdivideDome(void)                          // Bei & Cansin
{
	undoPush();
	setModified(true);
	DLFL::domeSubdivide(&object,MainWindow::domeLength_factor, MainWindow::domeScale_factor);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();

	QString cmd = QString("subdivide(\"dome\",");
	cmd += QString().setNum(MainWindow::substellate_height) + QString(",");
	cmd += QString().setNum(MainWindow::substellate_curve) + QString(")");
	emit echoCommand( cmd );
}

void MainWindow::subdivideDooSabinBC(void) // Doo-Sabin(BC) subdivision - Bei & Cansin
{
	undoPush();
	setModified(true);
	DLFL::dooSabinSubdivideBC(&object,MainWindow::doo_sabin_check);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
	QString check("False");
	if(MainWindow::doo_sabin_check)
		check = QString("True");
	QString cmd = QString("subdivide(\"doo-sabin-bc\",");
	cmd += check + QString(")");
	emit echoCommand( cmd );
}

void MainWindow::subdivideDooSabinBCNew(void) // Doo-Sabin(BCNew) Bei & Cansin
{
	undoPush();
	setModified(true);
	DLFL::dooSabinSubdivideBCNew(&object,MainWindow::dooSabinBCnewScale_factor,
		MainWindow::dooSabinBCnewLength_factor);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
	QString cmd = QString("subdivide(\"doo-sabin-bc-new\",");
	cmd += QString().setNum(MainWindow::dooSabinBCnewScale_factor) + QString(",");
	cmd += QString().setNum(MainWindow::dooSabinBCnewLength_factor) + QString(")");
	emit echoCommand( cmd );
}

void MainWindow::subdivideLoopStyle(void)     // Loop-style subdivision - Bei
{
	undoPush();
	setModified(true);
	DLFL::loopStyleSubdivide(&object,MainWindow::loopLength_factor);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
	QString cmd = QString("subdivide(\"loop-style\",");
	cmd += QString().setNum(MainWindow::loopLength_factor) + QString(")");
	emit echoCommand( cmd );
}

void MainWindow::globalStellate(void)
{
	// Does not use length parameter for now. Uses subdivideFace method with triangles
	undoPush();
	setModified(true);
	DLFL::subdivideAllFaces(&object,false);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();

	QString cmd = QString("subdivide(\"allfaces\",False)");
	emit echoCommand( cmd );
}

void MainWindow::triangulate(void)
{
	undoPush();
	setModified(true);
	DLFL::triangulateAllFaces(&object);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();

	// QString cmd = QString("subdivide(\"triangulate\",False)");
	// emit echoCommand( cmd );
}

void MainWindow::splitValence2Vertices(void)      // Split Valence 2 vertices
{
	undoPush();
	setModified(true);
	DLFL::splitValence2Vertices(&object,MainWindow::vertex_split_offset);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::cleanupWingedVertices(void)     // Remove valence 2 vertices
{
	undoPush();
	setModified(true);
	DLFL::cleanupWingedVertices(&object);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::createDual(void)                       // Create dual object
{
	undoPush();
	setModified(true);
	DLFL::createDual(&object,MainWindow::accurate_dual);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();

	QString cmd = QString("dual()");
	emit echoCommand( cmd );
}

void MainWindow::createCrust(bool use_scaling)        // Create a crust
{
	undoPush();
	setModified(true);
	if ( use_scaling ) 
		DLFL::createCrustWithScaling(&object,MainWindow::crust_scale_factor);
	else 
		DLFL::createCrust(&object,MainWindow::crust_thickness);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}


//this function will tag all currently selected faces with the FTHole type
//when the user presses the rind modeling button, and it will punch out all those faces
void MainWindow::createCrust2(bool use_scaling) {
	// DLFLFacePtrArray::iterator first, last;
	vector<DLFLFacePtr>::iterator it;

	QString facelist("[");

	undoPush();
	setModified(true);
	if ( use_scaling ) 
		DLFL::createCrustWithScaling(&object,MainWindow::crust_scale_factor);
	else 
		DLFL::createCrust(&object,MainWindow::crust_thickness);
	active->recomputePatches();
	active->recomputeNormals();
	if ( active->numSelectedFaces() >= 1 ) {
		DLFLFacePtrArray sfptrarr = active->getSelectedFaces();
		if ( sfptrarr[0] ) {
			for(it = sfptrarr.begin(); it != sfptrarr.end(); it++) {
				(*it)->setType(FTHole);
				facelist += QString().setNum((*it)->getID()) + QString(",");
			}
			facelist += QString("]");
			DLFL::punchHoles(&object);
		}
	}
	active->recomputePatches();
	active->recomputeNormals();
	active->clearSelectedFaces();
	redraw();

	QString cmd = QString("rind(") + 
		facelist + QString(",True,") + 
		MainWindow::crust_thickness +
		QString(")");
	emit echoCommand( cmd );
}

void MainWindow::makeWireframe(void)                    // Create a wireframe
{
	undoPush();
	setModified(true);
	DLFL::makeWireframe(&object,MainWindow::wireframe_thickness);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::makeWireframe2() {// Create a wireframe // dave {
	// vector<DLFLFacePtr>::iterator it;
	undoPush();
	setModified(true);
	DLFL::makeWireframe2(&object,MainWindow::wireframe2_thickness, MainWindow::wireframe2_width);
	// active->recomputePatches();
	// active->recomputeNormals();
	// if ( active->numSelectedFaces() >= 1 ) {
	// 	DLFLFacePtrArray sfptrarr = active->getSelectedFaces();
	// 	if ( sfptrarr[0] ) {
	// 		//mark all the faces for hole punching
	// 		for(it = sfptrarr.begin(); it != sfptrarr.end(); it++)
	// 			(*it)->setType(FTHole);
	// 		// object.punchHoles();
	// 		object.makeWireframe2(MainWindow::wireframe_thickness);
	// 	}
	// }
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::makeWireframeWithColumns(void) // Create a wireframe using columns
{
	undoPush();
	setModified(true);
	DLFL::makeWireframeWithColumns(&object,MainWindow::column_thickness, MainWindow::column_segments);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}
/*
void MainWindow::makeUnitCube(double edgelength)
{
	undoPush();
	setModified(true);
	DLFLObjectPtr unitcube = DLFLObject::makeUnitCube(edgelength);
	object.reset();
	object.splice(*unitcube);
	delete unitcube;
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::makeUnitTetrahedron(double edgelength)
{
	undoPush();
	setModified(true);
	DLFLObjectPtr unittetra = DLFLObject::makeUnitTetrahedron();
	object.reset();
	object.splice(*unittetra);
	delete unittetra;
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::makeMengerSponge(int level)
{
	undoPush();
	setModified(true);
	DLFLObjectPtr mengersponge = DLFLObject::makeMengerSponge(level);
	object.reset();
	object.splice(*mengersponge);
	delete mengersponge;
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::makeSierpinskiTetrahedron(int level)
{
	undoPush();
	setModified(true);
	DLFLObjectPtr stetra = DLFLObject::makeSierpinskiTetrahedron(level);
	object.reset();
	object.splice(*stetra);
	delete stetra;
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
}
*/
void MainWindow::randomAssignTexCoords(void) // Randomly assign texture coordinates
{
	object.randomAssignTexCoords();
	redraw();
}

void MainWindow::assignTileTexCoords(void) // Assign texture coordinates for tile texturing
{
	object.assignTileTexCoords(MainWindow::tile_tex_n);
	redraw();
}

void MainWindow::edgeCleanup(void) // Cleanup redundant edges
{
	undoPush();
	setModified(true);
	DLFL::edgeCleanup(&object);
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::printSummary(void)              // Print summary information
{
	object.print();
}

void MainWindow::printVertexList(void)                   // Print vertex list
{
	object.printVertexList();
}

void MainWindow::printEdgeList(void)                       // Print edge list
{
	object.printEdgeList();
}

void MainWindow::printCVList(void)                       // Print CV list
{
///object.printPatchCVList();
}

void MainWindow::printFaceList(void)                       // Print face list
{
	object.printFaces();
}

void MainWindow::getCommand(){
	int i = mCommandCompleter->exec();
	// QMessageBox::about(this, tr("%1").arg(text),tr("%1").arg(mCommandList.indexOf(text)));
	if (i > -1)
		mActionListWidget->actions().at(i)->activate(QAction::Trigger);
		
}

void MainWindow::initializeAnimatedHelp(){
	
	mAnimatedHelpWidget = new QWidget(this);
	// QSplashScreen *w = new QSplashScreen(this);
	mAnimatedHelpLayout = new QVBoxLayout;
	
	mAnimatedHelpMovie = new QMovie(this);
	mAnimatedHelpMovie->setCacheMode(QMovie::CacheAll);
	mAnimatedHelpMovie->setBackgroundColor(QColor(255,255,255,255));

	mAnimatedHelpLabel = new QLabel(tr("No movie loaded"));
	mAnimatedHelpLabel->setAlignment(Qt::AlignCenter);
	// mAnimatedHelpLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	// mAnimatedHelpLabel->setBackgroundRole(QPalette::Dark);
	// mAnimatedHelpLabel->setAutoFillBackground(true);
	
	mAnimatedHelpLayout->addWidget(mAnimatedHelpLabel);
	mAnimatedHelpLayout->addStretch(1);
	mAnimatedHelpLayout->setMargin(0);
	mAnimatedHelpWidget->setLayout(mAnimatedHelpLayout);
	
	mAnimatedHelpMovie->stop();
  mAnimatedHelpLabel->setMovie(mAnimatedHelpMovie);
  // mAnimatedHelp->setFileName("images/insert_edge.mng");
	// w->move(QCursor::pos());
	// w->show();
	
	// create the dockwidget, set it to the right side
	mAnimatedHelpDockWidget = new QDockWidget(tr("TopMod Animated Help"), this);
	mAnimatedHelpDockWidget->setAllowedAreas(Qt::NoDockWidgetArea);
	mAnimatedHelpDockWidget->setWidget(mAnimatedHelpWidget);
	// addDockWidget(Qt::RightDockWidgetArea, mAnimatedHelpDockWidget);
	mAnimatedHelpDockWidget->hide();	
	mAnimatedHelpDockWidget->setFloating(true);
	mAnimatedHelpDockWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable);
	// mAnimatedHelpDockWidget->setResizeEnabled(false);
	//sizing
	mAnimatedHelpDockWidget->setMinimumSize(200,200);
	mAnimatedHelpDockWidget->setMaximumSize(200,200);
	
	//fix positioning later
	mAnimatedHelpDockWidget->move(width() , 150+mToolOptionsDockWidget->height()+200);
}

void MainWindow::setAnimatedHelpImage(){
  mAnimatedHelpMovie->setFileName("images/insert_edge.mng");
  mAnimatedHelpMovie->start();
}

void MainWindow::deleteSelected(){
	DLFLEdgePtrArray septrarr;
	DLFLFacePtrArray sfptrarr;
	DLFLVertexPtrArray svptrarr;
	DLFLFaceVertexPtrArray sfvptrarr;
	
	vector<DLFLFaceVertexPtr>::iterator fvt;
	vector<DLFLVertexPtr>::iterator vit;
	vector<DLFLEdgePtr>::iterator eit;
	vector<DLFLFacePtr>::iterator fit;
	
	// undoPush();
	// setModified(true);
	switch (selectionmask){
		case MaskEdges:
			septrarr = active->getSelectedEdges();
			if ( septrarr[0] ) {
				undoPush();
				setModified(true);
				vector<DLFLEdgePtr>::iterator eit;
				for(eit = septrarr.begin(); eit != septrarr.end(); eit++){
					DLFL::deleteEdge( &object, *eit, true);
					// DLFL::deleteEdge( &object, septr, MainWindow::delete_edge_cleanup);					
					// active->recomputePatches();
					// active->recomputeNormals();						
				}
			}			
			active->clearSelectedEdges();
			active->recomputePatches();
			active->recomputeNormals();		
			redraw();
		break;
		case MaskVertices:
			svptrarr = active->getSelectedVertices();
			if ( svptrarr[0] ) {
				undoPush();
				setModified(true);
				for(vit = svptrarr.begin(); vit != svptrarr.end(); vit++){
					(*vit)->getEdges(septrarr);
					vector<DLFLEdgePtr>::iterator eit;
					for(eit = septrarr.begin(); eit != septrarr.end(); eit++)
						DLFL::deleteEdge( &object, *eit, true);
						// active->recomputePatches();
						// active->recomputeNormals();						
					}
			}			
			active->clearSelectedVertices();
			active->recomputePatches();
			active->recomputeNormals();		
			redraw();
		break;
		case MaskFaces:
		// sfptrarr = active->getSelectedFaces();
		// if ( sfptrarr[0] ) {
		// 	undoPush();
		// 	setModified(true);
		// 	for(fit = sfptrarr.begin(); fit != sfptrarr.end(); fit++){
		// 		//get edges, collapse them
		// 		(*fit)->getEdges(septrarr);
		// 		for(eit = septrarr.begin(); eit != septrarr.end(); eit++)
		// 			DLFL::collapseEdge( &object, *eit, MainWindow::delete_edge_cleanup);
		// 		// (*fit)->getCorners(sfvptrarr);
		// 		// for(fvt = sfvptrarr.begin(); fvt != sfvptrarr.end(); fvt++){
		// 		// 	if (*fvt){
		// 		// 		DLFLVertexPtr vptr = (*fvt)->getVertexPtr();
		// 		// 		if (vptr){
		// 		// 			vptr->getEdges(septrarr);
		// 		// 			for(eit = septrarr.begin(); eit != septrarr.end(); eit++)
		// 		// 				DLFL::deleteEdge( &object, *eit, MainWindow::delete_edge_cleanup);
		// 		// 		}
		// 		// 	}
		// 		// } // end loop through corners of current face
		// 	}//end loop through selected faces
		// 	active->recomputePatches();
		// 	active->recomputeNormals();						
		// }			
		// active->clearSelectedFaces();
		// redraw();		
		break;
		case MaskCorners:
		break;
		case MaskObject://someday...
		break;
		default:
		break;
	};
}

void MainWindow::collapseSelectedEdges(){
	DLFLEdgePtrArray septrarr;
	vector<DLFLEdgePtr>::iterator eit;
	septrarr = active->getSelectedEdges();
	if ( septrarr[0] ) {
		undoPush();
		setModified(true);
		vector<DLFLEdgePtr>::iterator eit;
		for(eit = septrarr.begin(); eit != septrarr.end(); eit++){
			if (*eit){
				DLFL::collapseEdge( &object, *eit, MainWindow::delete_edge_cleanup);
				active->recomputeNormals();						
			}
		}
		active->recomputePatches();
	}			
	active->clearSelectedEdges();
	redraw();
}

void MainWindow::selectEdgesFromFaces(){
	DLFLEdgePtrArray septrarr;
	DLFLFacePtrArray sfptrarr;
	vector<DLFLFacePtr>::iterator fit;
	vector<DLFLEdgePtr>::iterator eit;
	//loop through selected faces, get corresponding edges, select them
	sfptrarr = active->getSelectedFaces();
	if ( sfptrarr.size() > 0 && sfptrarr[0] ) {
		for(fit = sfptrarr.begin(); fit != sfptrarr.end(); fit++){
			(*fit)->getEdges(septrarr);
			vector<DLFLEdgePtr>::iterator eit;
			for(eit = septrarr.begin(); eit != septrarr.end(); eit++){
				if (!active->isSelected(*eit)){
					active->setSelectedEdge(num_sel_edges,*eit);
					num_sel_edges++;
				}
			}
		}
		active->recomputePatches();
		active->recomputeNormals();						
	}			
	setMode(MainWindow::SelectEdge);
	active->clearSelectedFaces();
	redraw();		
}

void MainWindow::selectEdgesFromVertices(){
	DLFLEdgePtrArray septrarr;
	DLFLVertexPtrArray svptrarr;
	vector<DLFLVertexPtr>::iterator vit;
	vector<DLFLEdgePtr>::iterator eit;
	//loop through selected vertices, get corresponding edges, select them
	svptrarr = active->getSelectedVertices();
	if ( svptrarr.size() > 0 && svptrarr[0] ) {
		for(vit = svptrarr.begin(); vit != svptrarr.end(); vit++){
			(*vit)->getEdges(septrarr);
			for(eit = septrarr.begin(); eit != septrarr.end(); eit++){
				if (!active->isSelected(*eit)){
					active->setSelectedEdge(num_sel_edges,*eit);
					num_sel_edges++;
				}
			}
		}
		active->recomputePatches();
		active->recomputeNormals();						
	}			
	setMode(MainWindow::SelectEdge);
	active->clearSelectedVertices();
	redraw();	
}

void MainWindow::selectFacesFromEdges(){
	DLFLEdgePtrArray septrarr;
	vector<DLFLEdgePtr>::iterator eit;
	DLFLFacePtr fptr1, fptr2;
	//loop through selected faces, get corresponding vertices, select them
	septrarr = active->getSelectedEdges();
	if ( septrarr.size() > 0 && septrarr[0] ) {
		for(eit = septrarr.begin(); eit != septrarr.end(); eit++){
			(*eit)->getFacePointers(fptr1,fptr2);
			if (!active->isSelected(fptr1)){
				active->setSelectedFace(num_sel_faces,fptr1);
				num_sel_faces++;
			}
			if (!active->isSelected(fptr2)){
				active->setSelectedFace(num_sel_faces,fptr2);
				num_sel_faces++;
			}
		}
		active->recomputePatches();
		active->recomputeNormals();						
	}			
	setMode(MainWindow::SelectFace);	
	active->clearSelectedEdges();
	redraw();	
}

void MainWindow::selectFacesFromVertices(){
	DLFLVertexPtrArray svptrarr;
	vector<DLFLVertexPtr>::iterator vit;
	DLFLFacePtrArray sfptrarr;
	vector<DLFLFacePtr>::iterator fit;
	//loop through selected faces, get corresponding edges, select them
	svptrarr = active->getSelectedVertices();
	if ( svptrarr.size() > 0 && svptrarr[0] ) {
		for(vit = svptrarr.begin(); vit != svptrarr.end(); vit++){
			(*vit)->getFaces(sfptrarr);
			for(fit = sfptrarr.begin(); fit != sfptrarr.end(); fit++){
				if (!active->isSelected(*fit)){
					active->setSelectedFace(num_sel_faces,*fit);
					num_sel_faces++;
				}
			}
		}
		active->recomputePatches();
		active->recomputeNormals();						
	}			
	setMode(MainWindow::SelectFace);
	active->clearSelectedVertices();
	redraw();			
}

void MainWindow::selectVerticesFromFaces(){
	DLFLFacePtrArray sfptrarr;
	vector<DLFLFacePtr>::iterator fit;
	DLFLFaceVertexPtrArray sfvptrarr;
	vector<DLFLFaceVertexPtr>::iterator fvit;
	DLFLVertexPtr vptr;
	//loop through selected faces, get corresponding vertices, select them
	sfptrarr = active->getSelectedFaces();
	if ( sfptrarr.size() > 0 && sfptrarr[0] ) {
		for(fit = sfptrarr.begin(); fit != sfptrarr.end(); fit++){
			(*fit)->getCorners(sfvptrarr);
			for(fvit = sfvptrarr.begin(); fvit != sfvptrarr.end(); fvit++){
				vptr = (*fvit)->getVertexPtr();
				if (!active->isSelected(vptr)){
					active->setSelectedVertex(num_sel_verts,vptr);
					num_sel_verts++;
				}
			}
		}
		active->recomputePatches();
		active->recomputeNormals();						
	}			
	setMode(MainWindow::SelectVertex);
	active->clearSelectedFaces();
	redraw();	
}

void MainWindow::selectVerticesFromEdges(){	
	DLFLEdgePtrArray septrarr;
	vector<DLFLEdgePtr>::iterator eit;
	DLFLVertexPtr vptr1,vptr2;
	//loop through selected faces, get corresponding vertices, select them
	septrarr = active->getSelectedEdges();
	// std::cout<< septrarr[0] << " \t\t" << septrarr.size() <<"\n";
	if ( septrarr.size() > 0 && septrarr[0]) {
		for(eit = septrarr.begin(); eit != septrarr.end(); eit++){
			(*eit)->getVertexPointers(vptr1,vptr2);
			if (!active->isSelected(vptr1)){
				active->setSelectedVertex(num_sel_verts,vptr1);
				num_sel_verts++;
			}
			if (!active->isSelected(vptr2)){
				active->setSelectedVertex(num_sel_verts,vptr2);
				num_sel_verts++;
			}
		}
		active->recomputePatches();
		active->recomputeNormals();						
	}			
	setMode(MainWindow::SelectVertex);
	active->clearSelectedEdges();
	redraw();	
}


void MainWindow::growSelection(){
	DLFLEdgePtrArray septrarr;
	DLFLVertexPtrArray svptrarr;
	DLFLFaceVertexPtrArray sfvptrarr;
	DLFLFacePtrArray sfptrarr;
	vector<DLFLVertexPtr>::iterator vit;
	vector<DLFLEdgePtr>::iterator eit;
	vector<DLFLFacePtr>::iterator fit;
	vector<DLFLFaceVertexPtr>::iterator fvit;
	
	DLFLVertexPtr vptr1,vptr2;
	DLFLFacePtr fptr1,fptr2;
	DLFLEdgePtrArray eptrarray;
	vector<DLFLEdgePtr>::iterator eit2;
	
	switch (selectionmask){
		case MaskVertices:
		//loop through selected vertices, get corresponding edges
		svptrarr = active->getSelectedVertices();
		if ( svptrarr.size() > 0 && svptrarr[0] ) {
			for(vit = svptrarr.begin(); vit != svptrarr.end(); vit++){
				(*vit)->getEdges(septrarr);
				//loop through edges, select all vertices connected to these edges
				for(eit = septrarr.begin(); eit != septrarr.end(); eit++){
					DLFLVertexPtr vp1, vp2;
					//get the two vertices for each edge, select them if they aren't already selected
					(*eit)->getVertexPointers(vp1,vp2);
					if (!active->isSelected(vp1)){
						active->setSelectedVertex(num_sel_verts,vp1);
						num_sel_verts++;
					}
					if (!active->isSelected(vp2)){
						active->setSelectedVertex(num_sel_verts,vp2);
						num_sel_verts++;
					}
				}
			}
		}			
		redraw();
		break;
		case MaskEdges:
		//loop through selected edges, get vertices, then get those edges
		septrarr = active->getSelectedEdges();
		if ( septrarr.size() > 0 && septrarr[0]) {
			for(eit = septrarr.begin(); eit != septrarr.end(); eit++){
				//get the 2 vertices for the current edge
				(*eit)->getVertexPointers(vptr1,vptr2);
				//get the edges for vertex 1
				vptr1->getEdges(eptrarray);
				for (eit2 = eptrarray.begin(); eit2 != eptrarray.end(); eit2++){
					if (!active->isSelected(*eit2)){
						active->setSelectedEdge(num_sel_edges,*eit2);
						num_sel_edges++;
					}					
				}
				//get edges for vertex 2
				vptr2->getEdges(eptrarray);
				for (eit2 = eptrarray.begin(); eit2 != eptrarray.end(); eit2++){
					if (!active->isSelected(*eit2)){
						active->setSelectedEdge(num_sel_edges,*eit2);
						num_sel_edges++;
					}					
				}
			}
		}			
		redraw();
		break;
		case MaskFaces:
		//loop through selected faces
		sfptrarr = active->getSelectedFaces();
		if ( sfptrarr.size() > 0 && sfptrarr[0] ) {
			for(fit = sfptrarr.begin(); fit != sfptrarr.end(); fit++){
				//get the edges for the current face
				(*fit)->getEdges(eptrarray);
				for(eit = eptrarray.begin(); eit != eptrarray.end(); eit++){
					//get the 2 faces for the current edge, select them if they aren't already selected
					(*eit)->getFacePointers(fptr1,fptr2);
					if (!active->isSelected(fptr1)){
						active->setSelectedFace(num_sel_faces,fptr1);
						num_sel_faces++;
					}
					if (!active->isSelected(fptr2)){
						active->setSelectedFace(num_sel_faces,fptr2);
						num_sel_faces++;
					}
				}
			}
		}
		redraw();
		break;
		case MaskCorners:

		break;
		case MaskObject:
		break;
		default:
		break;
	};
}

void MainWindow::shrinkSelection(){
	DLFLEdgePtrArray septrarr;
	DLFLVertexPtrArray svptrarr;
	DLFLFaceVertexPtrArray sfvptrarr;
	DLFLFacePtrArray sfptrarr;
	vector<DLFLVertexPtr>::iterator vit;
	vector<DLFLEdgePtr>::iterator eit;
	vector<DLFLFacePtr>::iterator fit;
	vector<DLFLFaceVertexPtr>::iterator fvit;
	int i = 0;
	
	DLFLVertexPtr vptr1,vptr2;
	DLFLFacePtr fptr1,fptr2;
	DLFLEdgePtrArray eptrarray;
	vector<DLFLEdgePtr>::iterator eit2;
	
	switch (selectionmask){
		case MaskVertices:
		//loop through selected vertices, get corresponding edges
		svptrarr = active->getSelectedVertices();
		if ( svptrarr.size() > 0 && svptrarr[0] ) {
			bool deselectvertices[svptrarr.size()];
			for(vit = svptrarr.begin(); vit != svptrarr.end(); vit++,i++){
				deselectvertices[i] = false;
				(*vit)->getEdges(septrarr);
				//loop through edges, select all vertices connected to these edges
				for(eit = septrarr.begin(); eit != septrarr.end(); eit++){
					DLFLVertexPtr vp1, vp2;
					//get the two vertices for each edge, select them if they aren't already selected
					(*eit)->getVertexPointers(vp1,vp2);
					if (!active->isSelected(vp1) || !active->isSelected(vp2))
						deselectvertices[i] = true;
				}
			}
			//loop back through verts, now deselect the flagged ones
			i = 0;
			for(vit = svptrarr.begin(); vit != svptrarr.end(); vit++, i++){
				//if it's flagged, deselect it
				if (deselectvertices[i])
					active->clearSelectedVertex(*vit);
			}
		}			
		redraw();
		break;
		case MaskEdges:
		//loop through selected edges, get vertices, then get those edges
		septrarr = active->getSelectedEdges();
		if ( septrarr.size() > 0 && septrarr[0]) {
			bool deselectedges[septrarr.size()];
			for(eit = septrarr.begin(); eit != septrarr.end(); eit++,i++){
				//get the 2 vertices for the current edge
				(*eit)->getVertexPointers(vptr1,vptr2);
				//get the edges for vertex 1
				vptr1->getEdges(eptrarray);
				deselectedges[i] = false;
				for (eit2 = eptrarray.begin(); eit2 != eptrarray.end(); eit2++){
					if (!active->isSelected(*eit2))
						deselectedges[i] = true;
				}
				//get edges for vertex 2
				vptr2->getEdges(eptrarray);
				for (eit2 = eptrarray.begin(); eit2 != eptrarray.end(); eit2++){
					if (!active->isSelected(*eit2))
						deselectedges[i] = true;
				}
			}
			//loop back through edges, now deselect the flagged ones
			i = 0;
			for(eit = septrarr.begin(); eit != septrarr.end(); eit++,i++){
				//if it's flagged, deselect it
				if (deselectedges[i])
					active->clearSelectedEdge(*eit);
			}
		}			
		redraw();		break;
		case MaskFaces:
		//loop through selected faces
		sfptrarr = active->getSelectedFaces();
		if ( sfptrarr.size() > 0 && sfptrarr[0] ) {
			bool deselectfaces[sfptrarr.size()];
			for(fit = sfptrarr.begin(); fit != sfptrarr.end(); fit++, i++){
				//get the edges for the current face
				(*fit)->getEdges(eptrarray);
				//by default don't deselect this face
				deselectfaces[i] = false;
				for(eit = eptrarray.begin(); eit != eptrarray.end(); eit++){
					//get the 2 faces for the current edge, check if both of them are selected, flag the current face
					(*eit)->getFacePointers(fptr1,fptr2);
					if (!active->isSelected(fptr1) || !active->isSelected(fptr2))
						deselectfaces[i] = true;
				}
			}
			//loop back through faces, now deselect the flagged ones
			i = 0;
			for(fit = sfptrarr.begin(); fit != sfptrarr.end(); fit++,i++){
				//if it's flagged, deselect it
				if (deselectfaces[i])
					active->clearSelectedFace(*fit);
			}
		}
		redraw();
		break;
		case MaskCorners:
		// active->selectAllCorners();
		case MaskObject:
		break;
		default:
		break;
	};
}
void MainWindow::reorderSelectedFaces(){
	// DLFLFaceVertexPtrArray sfvptrarr;
	// vector<DLFLFaceVertexPtr>::iterator fvit;
	// DLFLFacePtrArray sfptrarr;
	// vector<DLFLFacePtr>::iterator fit;
	// //loop through selected corners, get corresponding faces, reorder them to start from the selected fvptr's
	// svptrarr = active->getSelectedVertices();
	// if ( svptrarr[0] ) {
	// 	for(vit = svptrarr.begin(); vit != svptrarr.end(); vit++){
	// 		(*vit)->getFaces(sfptrarr);
	// 		for(fit = sfptrarr.begin(); fit != sfptrarr.end(); fit++){
	// 			if (!active->isSelected(*fit)){
	// 				active->setSelectedFace(num_sel_faces,*fit);
	// 				num_sel_faces++;
	// 			}
	// 		}
	// 	}
	// 	active->recomputePatches();
	// 	active->recomputeNormals();						
	// }			
	// active->clearSelectedVertices();
	// redraw();			
}

void MainWindow::toggleFullScreen(){
	// go back into normal screen mode
	if (windowState() == Qt::WindowFullScreen && !mToolsToolBar->isVisible()) {
		setWindowState(Qt::WindowNoState);
		mEditToolBar->setVisible(true);
		//mSelectionMaskToolBar->toggleViewAction();
		mPrimitivesToolBar->setVisible(true);
		mToolsToolBar->setVisible(true);
		mExtrusionToolBar->setVisible(true);
		//mConicalToolBar->toggleViewAction();
		mHighgenusToolBar->setVisible(true);
		mTexturingToolBar->setVisible(true);
		// mRemeshingToolBar->setVisible(true);		
		mStatusBar->show();
	}
	// go to full screen mode 2
	else if (windowState() == Qt::WindowFullScreen) {
		// setWindowState(windowState() ^ Qt::WindowFullScreen);	
		mStatusBar->hide();
		mEditToolBar->setVisible(false);
		mPrimitivesToolBar->setVisible(false);
		mToolsToolBar->setVisible(false);
		mExtrusionToolBar->setVisible(false);
		mHighgenusToolBar->setVisible(false);
		mTexturingToolBar->setVisible(false);
		mRemeshingToolBar->setVisible(false);		
	}
	//go into full screen mode 1
	else {
		setWindowState(windowState() ^ Qt::WindowFullScreen);		
	}
	
	
}