/* $Id: MainWindowCallbacks.cc,v 4.13 2004/01/20 22:18:36 vinod Exp $ */

// Definitions for callback functions in the MainWindow class

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
		active->isValid(); // To ensure new texture is loaded into OpenGL
		redraw();
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

void MainWindow::useShadedRenderer() {
  setRenderer(shaded);
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

void MainWindow::select_vertices() {
  setSelectionMask(MainWindow::MaskVertices);
}

void MainWindow::select_faces() {
  setSelectionMask(MainWindow::MaskFaces);
}

void MainWindow::select_edges() {
  setSelectionMask(MainWindow::MaskEdges);
}

void MainWindow::select_face_vertices() {
  setSelectionMask(MainWindow::MaskFaceVertices);
}

void MainWindow::exit_selection_mode() {
  setMode(MainWindow::NormalMode);
	redraw();
}

void MainWindow::clear_selected(){
	MainWindow::clearSelected();
redraw();	redraw();
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

void MainWindow::crust_modeling1() {
	setMode(MainWindow::CrustModeling);
  createCrust(true);
  redraw();
}

void MainWindow::crust_modeling2() {
  setMode(MainWindow::CrustModeling);
  createCrust(false);
  redraw();
}

void MainWindow::crust_modeling3() {
  // setMode(MainWindow::CrustModelingPainting);
  createCrust2(false);
  redraw();
}

void MainWindow::crust_modeling4() {
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
	object.performCutting(MainWindow::mode,MainWindow::cutOffsetE_factor,MainWindow::cutOffsetV_factor,MainWindow::global_cut,MainWindow::selected_cut);
	// recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::createConvexHull() {
	undoPush();
	setModified(true);
	object.createConvexHull();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::createDualConvexHull() {
	undoPush();
	setModified(true);
	object.createDualConvexHull();
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
void MainWindow::recomputeNormals() {    // Recompute normals and lighting {
	object.computeNormals();
	object.computeLighting(&plight);
	active->redraw();
}

void MainWindow::recomputeLighting() {               // Recompute lighting {
	object.computeLighting(&plight);
	active->redraw();
}

void MainWindow::recomputePatches() { // Recompute the patches for patch rendering {
	object.updatePatches();
	active->redraw();
}

void MainWindow::subDivideAllEdges()  {            // Sub-divide all edges {
	undoPush();
	object.subDivideAllEdges(MainWindow::num_e_subdivs);
	MainWindow::clearSelected();
redraw();	active->redraw();
}

void MainWindow::subDivideSelectedFaces() {// Subdivide all selected faces {
	undoPush();
	DLFLFacePtrArray fparray;
	fparray.resize(GLWidget::numSelectedFaces());
	for (int i=0; i < GLWidget::numSelectedFaces(); ++i)	{
		fparray[i] = GLWidget::getSelectedFace(i);
	}
	object.subDivideFaces(fparray,use_quads);
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::subDivideAllFaces() {// Subdivide all the faces {
	undoPush();
	object.subDivideAllFaces(use_quads);
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::createMultiFaceHandle() { // Create multi-face handle between selected faes {
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
		object.multiConnectFaces(sel_faces,MainWindow::mfh_scale_factor,
			MainWindow::mfh_extrude_dist,
			MainWindow::mfh_use_max_offsets);
		break;
		case ClosestEdge :
		object.multiConnectFaces(sel_faces);
		break;
		default :
		break;
	}
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::multiConnectMidpoints() {
	// Multi-connect midpoints after simplest-subdivision without edge deletion
	undoPush();
	object.multiConnectMidpoints();
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::multiConnectCrust() {
	// Multi-connect after creating crust
	undoPush();
	object.multiConnectCrust();
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::modifiedMultiConnectCrust() {
	// Modified multi-connect after creating crust
	undoPush();
	object.modifiedMultiConnectCrust();
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::createSponge() {
	undoPush();
	object.createSponge(MainWindow::sponge_thickness,
		MainWindow::sponge_collapse_threshold);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::planarizeFaces() {
// Planarize all faces {
	undoPush();
	object.planarize();
	recomputePatches();
	recomputeNormals();
}

void MainWindow::spheralizeObject() {
	// Spheralize object vertices {
	undoPush();
	object.spheralize();
	recomputePatches();
	recomputeNormals();
}

void MainWindow::smoothMesh() {
	// Smooth the mesh {
	undoPush();
	object.meshsmooth();
	recomputePatches();
	recomputeNormals();
}

void MainWindow::subDivideCatmullClark()  {   
	// Catmull-Clark subdivision {
	undoPush();
	object.catmullClarkSubDivide();
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::subDivideDooSabin() {
 	// Doo-Sabin subdivision {
	undoPush();
	object.dooSabinSubDivide(doo_sabin_check);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::subDivideHoneycomb() {
	// Honeycomb subdivision {
	undoPush();
	object.honeycombSubDivide();
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::subDivideRoot4() {
	// Root-4 subdivision {
	undoPush();
	object.root4SubDivide(MainWindow::weight_factor,MainWindow::twist_factor);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::subDivideCornerCutting() {
	// Corner-cutting subdivision {
	undoPush();
	object.cornerCuttingSubDivide();
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::subDivideLinearVertexInsertion() {
	// Bi-linear Vertex-insertion remeshing {
	undoPush();
	object.subDivideAllFaces(true);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::subDivideSimplest() {
	// Corner-cutting subdivision {
	undoPush();
	object.simplestSubDivide();
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::subDivideVertexCutting() {
	// Vertex-cutting subdivision {
	undoPush();
	object.vertexCuttingSubDivide(MainWindow::vertex_cutting_offset);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::subDividePentagonal() {
	// Pentagonal subdivision {
	undoPush();
	object.pentagonalSubDivide(MainWindow::pentagonal_offset);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::subDivideCubicPentagonal() {
	// Cubic Pentagonal remeshing scheme {
// Implemented as Pentagonal + Dual + Dual
	undoPush();
	object.pentagonalSubDivide(MainWindow::pentagonal_offset);
	object.createDual(true); // Use accurate method
	object.createDual(true); // Use accurate method
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::subDivideDualPentagonal() {
	// Dual-Pentagonal subdivision {
// Implemented as Dual + Pentagonal + Dual
	undoPush();
	object.createDual(true); // Use accurate method
	object.pentagonalSubDivide(MainWindow::pentagonal_offset);
	object.createDual(true); // Use accurate method
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::subDividePentagonPreserving() {
	// Pentagon preserving remeshing {
	undoPush();
	object.pentagonalSubDivide2(MainWindow::pentagonal_scale);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::subDivideDualPentagonPreserving() {
	// Dual Pentagon preserving remeshing {
// Implemented as Dual + Pentagonal + Dual
	undoPush();
	object.createDual(true); // Use accurate method
	object.pentagonalSubDivide2(MainWindow::pentagonal_scale);
	object.createDual(true); // Use accurate method
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::subDivideDualHexagonPreserving() {
	// Dual Hexagon Preserving remeshing {
// Implemented as Dual + Root4 + Dual
	undoPush();
	object.createDual(true); // Use accurate method
	object.root4SubDivide(MainWindow::weight_factor,MainWindow::twist_factor);
	object.createDual(true); // Use accurate method
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::subDivideRoot3() {
	// Root-3 remeshing {
// Implemented as Dual + Honeycomb + Dual
	undoPush();
	object.createDual(true); // Use accurate method
	object.honeycombSubDivide();
	object.createDual(true); // Use accurate method
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::subDivideLoop() {
	// Loop subdivision {
	undoPush();
	object.loopSubDivide();
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::subDivideDualLoop() {
	// Dual of Loop subdivision {
// Implemented as Dual + Loop + Dual
	undoPush();
	object.createDual(true); // Use accurate method
	object.loopSubDivide();
	object.createDual(true); // Use accurate method
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::subDivide1264() {
	// 12-6-4 remeshing {
// Implemented as Dual + Dual 12-6-4 + Dual
	undoPush();
	object.createDual(true); // Use accurate method
	object.dual1264SubDivide(MainWindow::dual1264_scale_factor);
	object.createDual(true); // Use accurate method
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::subDivideDual1264() {
	// Dual of 12-6-4 remeshing - Bei & Cansin {
	undoPush();
	object.dual1264SubDivide(MainWindow::dual1264_scale_factor);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::subDivideCheckerBoard() {
	// Checker board remeshing {
	undoPush();
	object.checkerBoardRemeshing(MainWindow::checkerboard_thickness);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::subDivideDualCheckerBoard() {
	// Dual Checker board remeshing {
// Implemented as Dual + Checker board + Dual
	undoPush();
	setModified(true);
	object.createDual(true); // Use accurate method
	object.checkerBoardRemeshing(MainWindow::checkerboard_thickness);
	object.createDual(true); // Use accurate method
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::subDivideStar() {
	// Star subdivision - Doug {
	undoPush();
	setModified(true);
	object.starSubDivide(MainWindow::star_offset);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::subDivideSqrt3() {
	// sqrt(3) subdivision - Doug {
	undoPush();
	setModified(true);
	object.sqrt3SubDivide();
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::subDivideFractal() {
	// fractal - Doug {
	undoPush();
	setModified(true);
	object.fractalSubDivide(MainWindow::fractal_offset);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::subStellate1() {
	// stellate subdivision - Eric {
	undoPush();
	setModified(true);
	object.stellateSubDivide();
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::subStellate2() {// stellate subdivision - Eric {
	
	undoPush();
	setModified(true);
	object.twostellateSubDivide(MainWindow::substellate_height, MainWindow::substellate_curve);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::subDivideDome() {
	// Bei & Cansin {
	undoPush();
	setModified(true);
	object.domeSubDivide(MainWindow::domeLength_factor, MainWindow::domeScale_factor);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::subDivideDooSabinBC() {
	// Doo-Sabin(BC) subdivision - Bei & Cansin {
	undoPush();
	setModified(true);
	object.dooSabinSubDivideBC(MainWindow::doo_sabin_check);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::subDivideDooSabinBCNew() {
	// Doo-Sabin(BCNew) Bei & Cansin {
	undoPush();
	setModified(true);
	object.dooSabinSubDivideBCNew(MainWindow::dooSabinBCnewScale_factor,
		MainWindow::dooSabinBCnewLength_factor);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::subDivideLoopStyle() {
	// Loop-style subdivision - Bei {
	undoPush();
	setModified(true);
	object.loopStyleSubDivide(MainWindow::loopLength_factor);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::globalStellate() {
	// Does not use length parameter for now. Uses subDivideFace method with triangles
	undoPush();
	setModified(true);
	object.subDivideAllFaces(false);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::splitValence2Vertices() {
	// Split Valence 2 vertices {
	undoPush();
	setModified(true);
	object.splitValence2Vertices(MainWindow::vertex_split_offset);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::cleanupWingedVertices() {
// Remove valence 2 vertices {
	undoPush();
	setModified(true);
	object.cleanupWingedVertices();
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::createDual() {
	// Create dual object {
	undoPush();
	setModified(true);
	object.createDual(MainWindow::accurate_dual);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::createCrust(bool use_scaling) {
	 // Create a crust {
	undoPush();
	setModified(true);
	if ( use_scaling ) object.createCrustWithScaling(MainWindow::crust_scale_factor);
	else object.createCrust(MainWindow::crust_thickness);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::createCrust2(bool use_scaling) {
	// DLFLFacePtrArray::iterator first, last;
	vector<DLFLFacePtr>::iterator it;

	undoPush();
	setModified(true);
	if ( use_scaling ) object.createCrustWithScaling(MainWindow::crust_scale_factor);
	else object.createCrust(MainWindow::crust_thickness);
	recomputePatches();
	recomputeNormals();
	if ( GLWidget::numSelectedFaces() >= 1 ) {
		DLFLFacePtrArray sfptrarr = GLWidget::getSelectedFaces();
		if ( sfptrarr[0] ) {
			for(it = sfptrarr.begin(); it != sfptrarr.end(); it++) {
				object.cmMakeHole(*it,crust_cleanup);
			}
		}
	}
	recomputePatches();
	recomputeNormals();
	GLWidget::clearSelectedFaces();
	setMode(MainWindow::MultiSelectFace);
	redraw();	
}

void MainWindow::makeWireframe() {
	// Create a wireframe {
	undoPush();
	setModified(true);
	object.makeWireframe(MainWindow::wireframe_thickness);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::makeWireframe2() {// Create a wireframe // dave {
	undoPush();
	setModified(true);
	object.makeWireframe2(MainWindow::wireframe_thickness);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::makeWireframeWithColumns() {// Create a wireframe using columns {
	undoPush();
	setModified(true);
	object.makeWireframeWithColumns(MainWindow::column_thickness, MainWindow::column_segments);
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::makeUnitCube(double edgelength) {
	undoPush();
	setModified(true);
	DLFLObjectPtr unitcube = DLFLObject::makeUnitCube(edgelength);
	object.reset();
	object.splice(*unitcube);
	delete unitcube;
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::makeUnitTetrahedron(double edgelength) {
	undoPush();
	setModified(true);
	DLFLObjectPtr unittetra = DLFLObject::makeUnitTetrahedron();
	object.reset();
	object.splice(*unittetra);
	delete unittetra;
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
redraw();
}

void MainWindow::makeMengerSponge(int level) {
	undoPush();
	setModified(true);
	DLFLObjectPtr mengersponge = DLFLObject::makeMengerSponge(level);
	object.reset();
	object.splice(*mengersponge);
	delete mengersponge;
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::makeSierpinskiTetrahedron(int level) {
	undoPush();
	setModified(true);
	DLFLObjectPtr stetra = DLFLObject::makeSierpinskiTetrahedron(level);
	object.reset();
	object.splice(*stetra);
	delete stetra;
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::randomAssignTexCoords() {
	// Randomly assign texture coordinates {
	object.randomAssignTexCoords();
}

void MainWindow::assignTileTexCoords() {
	// Assign texture coordinates for tile texturing {
	object.assignTileTexCoords(MainWindow::tile_tex_n);
}

void MainWindow::edgeCleanup() {
	// Cleanup redundant edges {
	undoPush();
	setModified(true);
	object.edgeCleanup();
	recomputePatches();
	recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::printSummary() {
	// Print summary information {
	object.print();
}

void MainWindow::printVertexList() {
	// Print vertex list {
	object.printVertexList();
}

void MainWindow::printEdgeList() {
	// Print edge list {
	object.printEdgeList();
}

void MainWindow::printCVList() {
	// Print CV list {
	object.printPatchCVList();
}

void MainWindow::printFaceList() {
	// Print face list {
	object.printFaces();
}
