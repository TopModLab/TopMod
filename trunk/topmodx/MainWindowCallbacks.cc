/* $Id: MainWindowCallbacks.cc,v 4.13 2004/01/20 22:18:36 vinod Exp $ */

// Definitions for callback functions in the MainWindow class
// All these are static methods

#include "MainWindow.hh"

void MainWindow::load_texture() {
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open File..."),
		"$HOME",
		tr("All Supported Files (*.jpg *.png);;JPEG Files (*.jpg);;PNG Files (*.png);;All Files (*)"));
	if (!fileName.isEmpty()){
		// undoPush();
		QByteArray ba = fileName.toLatin1();
		const char *filename = ba.data();

		textured->setTexture(filename);
		texturedlit->setTexture(filename);
		getActive()->isValid(); // To ensure new texture is loaded into OpenGL
		redraw();

		// readObject(filename);
		// recomputePatches();
		// recomputeNormals();
		// //loadFile(fileName);
		// active->redraw();
	}
}

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
	recomputeNormals();
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

void MainWindow::select_multiple_faces() {
	setMode(MainWindow::MultiSelectFace);
	getActive()->showBrush();
}

void MainWindow::select_similar_faces() {
	setMode(MainWindow::SelectSimilarFaces);
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
	setMode(MainWindow::SelectFaceVertex);
}

void MainWindow::selectionMaskVertices() {
	setSelectionMask(MainWindow::MaskVertices);
}

void MainWindow::selectionMaskFaces() {
	setSelectionMask(MainWindow::MaskFaces);
}

void MainWindow::selectionMaskEdges() {
	setSelectionMask(MainWindow::MaskEdges);
}

void MainWindow::selectionMaskFaceVertices() {
	setSelectionMask(MainWindow::MaskFaceVertices);
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
		case MaskFaceVertices:
		active->selectAllFaceVertices();
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
		case MaskFaceVertices:
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

void MainWindow::changeNumSubDivs(int value) {
	MainWindow::num_e_subdivs = value;
}

void MainWindow::changeTileTexNum(int value) {
	MainWindow::tile_tex_n = value;
}

// Extrusions.
void MainWindow::changeExtrudeLength(double value) {
	MainWindow::extrude_dist = value;
}

void MainWindow::changeExtrudeRotation(int value) {
	MainWindow::extrude_rot = value;
}

void MainWindow::changeExtrudeScale(double value) {
	MainWindow::extrude_scale = value;
}

void MainWindow::changeNumExtrusions(int value) {
	MainWindow::num_extrusions = value;
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
void MainWindow::changeMaxSegments(int value) {
	MainWindow::max_segments = value;
}

void MainWindow::changeNumSegments(int value) {
	MainWindow::num_segments = value;
}

void MainWindow::changeNumSegments2(int value){

}

void MainWindow::changeMaxSegments2(int value){

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

void MainWindow::changeExtraTwists(int value) {
	MainWindow::num_extra_twists = value;
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

//column modeling and wireframe modeling
void MainWindow::changeWireframeThickness(double value) {
	MainWindow::wireframe_thickness = value;
}

void MainWindow::changeColumnThickness(double value) {
	MainWindow::column_thickness = value;
}

void MainWindow::changeColumnSegments(int value) {
	MainWindow::column_segments = value;
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
	undoPush();
	setModified(true);
	// void performCutting( DLFLObjectPtr obj, int type,float offsetE,float offsetV,bool global,bool selected) {
	DLFL::performCutting(&object, MainWindow::mode,MainWindow::cutOffsetE_factor,MainWindow::cutOffsetV_factor,MainWindow::global_cut,MainWindow::selected_cut);
	// recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::createConvexHull() {
	undoPush();
	setModified(true);
	DLFL::createConvexHull(&object);
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::createDualConvexHull() {
	undoPush();
	setModified(true);
	DLFL::createDualConvexHull(&object);
	recomputeNormals();
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
	object.position[0] = x;
	redraw();
}

void MainWindow::translatey(double y) {
	object.position[1] = y;
	redraw();
}

void MainWindow::translatez(double z) {
	object.position[2] = z;
	redraw();
}

void MainWindow::scalex(double x) {
	object.scale_factor[0] = x;
	redraw();
}

void MainWindow::scaley(double y) {
	object.scale_factor[1] = y;
	redraw();
}

void MainWindow::scalez(double z) {
	object.scale_factor[2] = z;
	redraw();
}

void MainWindow::freezeTransforms() {
	object.freezeTransformations();
	object.position.reset();
	object.scale_factor.set(1,1,1);
	object.rotation.reset();
	redraw();
}

// Global operations (don't require selection)
void MainWindow::recomputeNormals(void)     // Recompute normals and lighting
{
	object.computeNormals();
	computeLighting( &object, patchObject, &plight);
}

void MainWindow::recomputeLighting(void)                // Recompute lighting
{
	computeLighting( &object, patchObject, &plight);
}

void MainWindow::recomputePatches(void) // Recompute the patches for patch rendering
{
	if(patchObject)
		patchObject->updatePatches(&object);
}

void MainWindow::subdivideAllEdges(void)              // Sub-divide all edges
{
	undoPush();
	DLFL::subdivideAllEdges(&object,MainWindow::num_e_subdivs);
	MainWindow::clearSelected();
}

void MainWindow::subdivideSelectedFaces(void) // Subdivide all selected faces
{
	undoPush();
	DLFLFacePtrArray fparray;
	fparray.resize(GLWidget::numSelectedFaces());
	for (int i=0; i < GLWidget::numSelectedFaces(); ++i)	{
		fparray[i] = GLWidget::getSelectedFace(i);
	}
	DLFL::subdivideFaces(&object,fparray,use_quads);
	MainWindow::clearSelected();
}

void MainWindow::subdivideAllFaces(void) // Subdivide all the faces
{
	undoPush();
	DLFL::subdivideAllFaces(&object,use_quads);
	MainWindow::clearSelected();
}

void MainWindow::createMultiFaceHandle(void) // Create multi-face handle between selected faes
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
	switch ( MainWindow::mfh_algo )	{
		case ConvexHull :
		DLFL::multiConnectFaces(&object,sel_faces,MainWindow::mfh_scale_factor,
			MainWindow::mfh_extrude_dist,
			MainWindow::mfh_use_max_offsets);
		break;
		case ClosestEdge :
		DLFL::multiConnectFaces(&object,sel_faces);
		break;
		default :
		break;
	}
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::multiConnectMidpoints(void)
{
	// Multi-connect midpoints after simplest-subdivision without edge deletion
	undoPush();
	DLFL::multiConnectMidpoints(&object);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::multiConnectCrust(void)
{
	// Multi-connect after creating crust
	undoPush();
	DLFL::multiConnectCrust(&object);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::modifiedMultiConnectCrust(void)
{
	// Modified multi-connect after creating crust
	undoPush();
	DLFL::modifiedMultiConnectCrust(&object);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::createSponge(void)
{
	undoPush();
	DLFL::createSponge(&object,MainWindow::sponge_thickness,
		MainWindow::sponge_collapse_threshold);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::planarizeFaces(void)                  // Planarize all faces
{
	undoPush();
	DLFL::planarize(&object);
	recomputePatches();
	recomputeNormals();
}

void MainWindow::spheralizeObject(void)         // Spheralize object vertices
{
	undoPush();
	DLFL::spheralize(&object);
	recomputePatches();
	recomputeNormals();
}

void MainWindow::smoothMesh(void)                          // Smooth the mesh
{
	undoPush();
	DLFL::meshsmooth(&object);
	recomputePatches();
	recomputeNormals();
}

void MainWindow::subdivideCatmullClark(void)     // Catmull-Clark subdivision
{
	undoPush();
	DLFL::catmullClarkSubdivide(&object);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::subdivideDooSabin(void)             // Doo-Sabin subdivision
{
	undoPush();
	DLFL::dooSabinSubdivide(&object,doo_sabin_check);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::subdivideHoneycomb(void)            // Honeycomb subdivision
{
	undoPush();
	DLFL::honeycombSubdivide(&object);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::subdivideRoot4(void)                   // Root-4 subdivision
{
	undoPush();
	DLFL::root4Subdivide( &object, MainWindow::weight_factor,MainWindow::twist_factor);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::subdivideCornerCutting(void)   // Corner-cutting subdivision
{
	undoPush();
	DLFL::cornerCuttingSubdivide(&object);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::subdivideLinearVertexInsertion(void) // Bi-linear Vertex-insertion remeshing
{
	undoPush();
	DLFL::subdivideAllFaces(&object,true);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::subdivideSimplest(void)        // Corner-cutting subdivision
{
	undoPush();
	DLFL::simplestSubdivide(&object);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::subdivideVertexCutting(void)   // Vertex-cutting subdivision
{
	undoPush();
	DLFL::vertexCuttingSubdivide(&object,MainWindow::vertex_cutting_offset);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::subdividePentagonal(void)          // Pentagonal subdivision
{
	undoPush();
	DLFL::pentagonalSubdivide(&object,MainWindow::pentagonal_offset);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::subdivideCubicPentagonal(void) // Cubic Pentagonal remeshing scheme
{
// Implemented as Pentagonal + Dual + Dual
	undoPush();
	DLFL::pentagonalSubdivide(&object,MainWindow::pentagonal_offset);
	DLFL::createDual(&object,true); // Use accurate method
	DLFL::createDual(&object,true); // Use accurate method
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::subdivideDualPentagonal(void) // Dual-Pentagonal subdivision
{
// Implemented as Dual + Pentagonal + Dual
	undoPush();
	DLFL::createDual(&object,true); // Use accurate method
	DLFL::pentagonalSubdivide(&object,MainWindow::pentagonal_offset);
	DLFL::createDual(&object,true); // Use accurate method
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::subdividePentagonPreserving(void) // Pentagon preserving remeshing
{
	undoPush();
	DLFL::pentagonalSubdivide2(&object,MainWindow::pentagonal_scale);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::subdivideDualPentagonPreserving(void) // Dual Pentagon preserving remeshing
{
// Implemented as Dual + Pentagonal + Dual
	undoPush();
	DLFL::createDual(&object,true); // Use accurate method
	DLFL::pentagonalSubdivide2(&object,MainWindow::pentagonal_scale);
	DLFL::createDual(&object,true); // Use accurate method
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::subdivideDualHexagonPreserving(void) // Dual Hexagon Preserving remeshing
{
// Implemented as Dual + Root4 + Dual
	undoPush();
	DLFL::createDual(&object,true); // Use accurate method
	DLFL::root4Subdivide(&object,MainWindow::weight_factor,MainWindow::twist_factor);
	DLFL::createDual(&object,true); // Use accurate method
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::subdivideRoot3(void)                     // Root-3 remeshing
{
// Implemented as Dual + Honeycomb + Dual
	undoPush();
	DLFL::createDual(&object,true); // Use accurate method
	DLFL::honeycombSubdivide(&object);
	DLFL::createDual(&object,true); // Use accurate method
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::subdivideLoop(void)                      // Loop subdivision
{
	undoPush();
	DLFL::loopSubdivide(&object);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::subdivideDualLoop(void)          // Dual of Loop subdivision
{
// Implemented as Dual + Loop + Dual
	undoPush();
	DLFL::createDual(&object,true); // Use accurate method
	DLFL::loopSubdivide(&object);
	DLFL::createDual(&object,true); // Use accurate method
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::subdivide1264(void)                      // 12-6-4 remeshing
{
// Implemented as Dual + Dual 12-6-4 + Dual
	undoPush();
	DLFL::createDual(&object,true); // Use accurate method
	DLFL::dual1264Subdivide(&object,MainWindow::dual1264_scale_factor);
	DLFL::createDual(&object,true); // Use accurate method
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::subdivideDual1264(void) // Dual of 12-6-4 remeshing - Bei & Cansin
{
	undoPush();
	DLFL::dual1264Subdivide(&object,MainWindow::dual1264_scale_factor);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::subdivideCheckerBoard(void)       // Checker board remeshing
{
	undoPush();
	DLFL::checkerBoardRemeshing(&object,MainWindow::checkerboard_thickness);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::subdivideDualCheckerBoard(void) // Dual Checker board remeshing
{
// Implemented as Dual + Checker board + Dual
	undoPush();
	setModified(true);
	DLFL::createDual(&object,true); // Use accurate method
	DLFL::checkerBoardRemeshing(&object,MainWindow::checkerboard_thickness);
	DLFL::createDual(&object,true); // Use accurate method
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::subdivideStar(void)               // Star subdivision - Doug
{
	undoPush();
	setModified(true);
	DLFL::starSubdivide(&object,MainWindow::star_offset);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::subdivideSqrt3(void)           // sqrt(3) subdivision - Doug
{
	undoPush();
	setModified(true);
	DLFL::sqrt3Subdivide(&object);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::subdivideFractal(void)                     // fractal - Doug
{
	undoPush();
	setModified(true);
	DLFL::fractalSubdivide(&object,MainWindow::fractal_offset);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::subStellate1(void)            // stellate subdivision - Eric
{
	undoPush();
	setModified(true);
	DLFL::stellateSubdivide(&object);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::subStellate2(void)            // stellate subdivision - Eric
{
	undoPush();
	setModified(true);
	DLFL::twostellateSubdivide(&object,MainWindow::substellate_height, MainWindow::substellate_curve);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::subdivideDome(void)                          // Bei & Cansin
{
	undoPush();
	setModified(true);
	DLFL::domeSubdivide(&object,MainWindow::domeLength_factor, MainWindow::domeScale_factor);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::subdivideDooSabinBC(void) // Doo-Sabin(BC) subdivision - Bei & Cansin
{
	undoPush();
	setModified(true);
	DLFL::dooSabinSubdivideBC(&object,MainWindow::doo_sabin_check);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::subdivideDooSabinBCNew(void) // Doo-Sabin(BCNew) Bei & Cansin
{
	undoPush();
	setModified(true);
	DLFL::dooSabinSubdivideBCNew(&object,MainWindow::dooSabinBCnewScale_factor,
		MainWindow::dooSabinBCnewLength_factor);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::subdivideLoopStyle(void)     // Loop-style subdivision - Bei
{
	undoPush();
	setModified(true);
	DLFL::loopStyleSubdivide(&object,MainWindow::loopLength_factor);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::globalStellate(void)
{
	// Does not use length parameter for now. Uses subdivideFace method with triangles
	undoPush();
	setModified(true);
	DLFL::subdivideAllFaces(&object,false);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::splitValence2Vertices(void)      // Split Valence 2 vertices
{
	undoPush();
	setModified(true);
	DLFL::splitValence2Vertices(&object,MainWindow::vertex_split_offset);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::cleanupWingedVertices(void)     // Remove valence 2 vertices
{
	undoPush();
	setModified(true);
	DLFL::cleanupWingedVertices(&object);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::createDual(void)                       // Create dual object
{
	undoPush();
	setModified(true);
	DLFL::createDual(&object,MainWindow::accurate_dual);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::createCrust(bool use_scaling)        // Create a crust
{
	undoPush();
	setModified(true);
	if ( use_scaling ) 
		DLFL::createCrustWithScaling(&object,MainWindow::crust_scale_factor);
	else 
		DLFL::createCrust(&object,MainWindow::crust_thickness);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}


//this function will tag all currently selected faces with the FTHole type
//when the user presses the rind modeling button, and it will punch out all those faces
void MainWindow::createCrust2(bool use_scaling) {
	// DLFLFacePtrArray::iterator first, last;
	vector<DLFLFacePtr>::iterator it;

	undoPush();
	setModified(true);
	if ( use_scaling ) 
		DLFL::createCrustWithScaling(&object,MainWindow::crust_scale_factor);
	else 
		DLFL::createCrust(&object,MainWindow::crust_thickness);
	recomputePatches();
	recomputeNormals();
	if ( GLWidget::numSelectedFaces() >= 1 ) {
		DLFLFacePtrArray sfptrarr = GLWidget::getSelectedFaces();
		if ( sfptrarr[0] ) {
			for(it = sfptrarr.begin(); it != sfptrarr.end(); it++) 
				(*it)->setType(FTHole);
			DLFL::punchHoles(&object);
		}
	}
	recomputePatches();
	recomputeNormals();
	GLWidget::clearSelectedFaces();
	redraw();	
}

void MainWindow::makeWireframe(void)                    // Create a wireframe
{
	undoPush();
	setModified(true);
	DLFL::makeWireframe(&object,MainWindow::wireframe_thickness);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::makeWireframe2() {// Create a wireframe // dave {
	vector<DLFLFacePtr>::iterator it;
	undoPush();
	setModified(true);
	DLFL::makeWireframe(&object,MainWindow::wireframe_thickness);
	// recomputePatches();
	// recomputeNormals();
	// if ( GLWidget::numSelectedFaces() >= 1 ) {
	// 	DLFLFacePtrArray sfptrarr = GLWidget::getSelectedFaces();
	// 	if ( sfptrarr[0] ) {
	// 		//mark all the faces for hole punching
	// 		for(it = sfptrarr.begin(); it != sfptrarr.end(); it++)
	// 			(*it)->setType(FTHole);
	// 		// object.punchHoles();
	// 		object.makeWireframe2(MainWindow::wireframe_thickness);
	// 	}
	// }
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::makeWireframeWithColumns(void) // Create a wireframe using columns
{
	undoPush();
	setModified(true);
	DLFL::makeWireframeWithColumns(&object,MainWindow::column_thickness, MainWindow::column_segments);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
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
	recomputePatches();
	recomputeNormals();
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
	recomputePatches();
	recomputeNormals();
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
	recomputePatches();
	recomputeNormals();
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
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
}
*/
void MainWindow::randomAssignTexCoords(void) // Randomly assign texture coordinates
{
	object.randomAssignTexCoords();
}

void MainWindow::assignTileTexCoords(void) // Assign texture coordinates for tile texturing
{
	object.assignTileTexCoords(MainWindow::tile_tex_n);
}

void MainWindow::edgeCleanup(void) // Cleanup redundant edges
{
	undoPush();
	setModified(true);
	DLFL::edgeCleanup(&object);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
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
