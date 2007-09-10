#ifndef _GL_WIDGET_H
#define _GL_WIDGET_H

// #include "Viewport.hh"
//#include "include/Graphics/Grid.hh"
#include <QtOpenGL>
#include <QGLContext>
#include <QCursor>

#include <QBrush>
#include <QImage>
#include <QPen>

#include <QApplication>
#include <QKeyEvent>
#include <QFont>
#include <QPushButton>

#include <DLFLObject.hh>
#include "DLFLRenderer.hh"
#include "TMPatchObject.hh"

#include "DLFLLighting.hh"
#include <Light.hh>
#include <SpotLight.hh>
#include <PointLight.hh>
#include <AmbientLight.hh>

#include "Camera3.hh"
#include "CgData.hh"

#ifdef GPU_OK
using namespace Cg;
#endif // GPU_OK

using namespace DLFL;

#ifdef WITH_VERSE
#include "TKE_verse.h"
#include "TIF_verse.h"
#endif

class QCursor;
class QImage;

#include "DLFLLocator.hh"     // brianb

//extern DLFLVertexPtrArray DLFLObject::sel_vptr_array; // List of selected DLFLVertex pointers
//extern DLFLEdgePtrArray DLFLObject::sel_eptr_array; // List of selected DLFLEdge pointers
//extern DLFLFacePtrArray DLFLObject::sel_fptr_array; // List of selected DLFLFace pointers
//extern DLFLFaceVertexPtrArray DLFLObject::sel_fvptr_array; // List of selected DLFLFaceVertex pointers

class GLWidget : public QGLWidget {
	Q_OBJECT        // must include this if you use Qt signals/slots

public :
	
	GLWidget(	int w, int h , DLFLRendererPtr rp, QColor color, QColor vcolor, DLFLObjectPtr op, const QGLFormat & format, QWidget * parent = 0 );

	~GLWidget( );

	public slots :

	// void toggleFullScreen( ); //not needed anymore... moved to main window
	void update() { repaint(); };
	// void switchTo(VPView view);

		//set lighting colors... warm and cool
	void setWarmLightColor(QColor c);
	void setCoolLightColor(QColor c);
	void setLightIntensity(double i);

	//setters for properties inside DLFLRenderer
	void setRenderColor(QColor c);
	
	void setViewportColor(QColor c){ mViewportColor = c; redraw(); };
	void setWireframeColor(QColor c){ if(renderer) renderer->setWireframeColor(c); redraw(); };
	void setSilhouetteColor(QColor c){ if( renderer ) renderer->setSilhouetteColor(c); redraw(); };
	void setWireframeThickness(double t){	if( renderer ) renderer->setWireframeThickness(t); redraw(); };
	void setVertexThickness(double t){ if( renderer ) renderer->setVertexThickness(t); redraw(); };
	void setSilhouetteThickness(double t){ if( renderer ) renderer->setSilhouetteThickness(t); redraw(); };
	void setNormalColor(QColor c){ if( renderer ) renderer->setNormalColor(c); redraw(); };
	void setFaceCentroidColor(QColor c){ if( renderer ) renderer->setFaceCentroidColor(c); redraw(); };
	void setFaceCentroidThickness(double t){ if( renderer ) renderer->setFaceCentroidThickness(t); redraw(); };
	void setNormalThickness(double t){ if( renderer ) renderer->setNormalThickness(t); redraw(); };
	void setNormalLength(double l){ if( renderer ) renderer->setNormalLength(l); redraw(); };

	//setters for properties in glwidget... always call redraw()
	void setSelectedVertexThickness(double t){ mSelectedVertexThickness = t; redraw(); };
	void setSelectedEdgeThickness(double t){ mSelectedEdgeThickness = t; redraw(); };
	void setSelectedEdgeColor(QColor c){ mSelectedEdgeColor = c; redraw(); };
	void setSelectedFaceColor(QColor c){ mSelectedFaceColor = c; redraw(); };
	void setSelectedVertexColor(QColor c){ mSelectedVertexColor = c; redraw(); };
	void setVertexIDBgColor(QColor c){ mVertexIDBgColor = c; redraw(); };
	void setFaceIDBgColor(QColor c){ mFaceIDBgColor = c; redraw(); };
	void setEdgeIDBgColor(QColor c){ mEdgeIDBgColor = c; redraw(); };	

	inline void setLightPosition(Vector3d p){ plight.position = p; };

	void setNearPlane(double n) { mCamera->setNearPlane(n); redraw(); };
	void setFarPlane(double f){ mCamera->setFarPlane(f); redraw(); };
	void setFOV(double fov){ mCamera->setFOV(fov); redraw(); };
	
		//compute lighting and normals functions now moved here from MainWindow
	void recomputeNormals();
	void recomputeLighting();
	void recomputePatches();	

	void toggleHUD() {
		mShowHUD = !mShowHUD;
		this->repaint();
	}

	#ifdef GPU_OK
	void toggleGPU(){
		if (renderer)
			renderer->toggleGPU();
		mUseGPU = !mUseGPU;
		recomputeLighting();
		repaint();
	}
	#endif

	void toggleAntialiasing(){
		// std::cout<< mAntialiasing << "\n";
		mAntialiasing = !mAntialiasing;
		// std::cout << mAntialiasing << "\n";
		renderer->toggleAntialiasing();
		repaint();
	}

			// Toggle wireframe
	void toggleWireframe(void) {
		if ( renderer ) renderer->toggleWireframe();
		this->repaint();
	}

			// Toggle silhouette
	void toggleSilhouette(void) {
		if ( renderer ) renderer->toggleSilhouette();
		this->repaint();

	}

			// Toggle vertices
	void toggleVertices(void) {
		if ( renderer ) renderer->toggleVertices();
		this->repaint();
	}

	void toggleFaceIDs( ) {
		mShowFaceIDs = !mShowFaceIDs;
		this->repaint();
	}

	void toggleEdgeIDs( ) {
		mShowEdgeIDs = !mShowEdgeIDs;
		this->repaint();
	}

	void toggleVertexIDs( ) {
		mShowVertexIDs = !mShowVertexIDs;
		this->repaint();
	}

	void toggleSelectedIDs( ) {
		mShowSelectedIDs = !mShowSelectedIDs;
		this->repaint();
	}

	void toggleBrush( ) {
		mShowBrush = !mShowBrush;
		this->repaint();
	}

	void toggleSelectionWindow( ) {
		mShowSelectionWindow = !mShowSelectionWindow;
		this->repaint();
	}

	void showSelectionWindow( ) {
		mShowSelectionWindow = true;
		this->repaint();
	}

	void hideSelectionWindow( ) {
		mShowSelectionWindow = false;
		this->repaint();
	}
	
	void setSelectionWindowStartX(double x){
		mSelectionWindowStartX = x;
	}

	void setSelectionWindowStartY(double y){
		mSelectionWindowStartY = y;
	}

	void showBrush( ) {
		mShowBrush = true;
		setCursor(Qt::BlankCursor);
		this->repaint();
	}

	void hideBrush( ) {
		mShowBrush = false;
		setCursor(Qt::ArrowCursor);
		this->repaint();
	}

	bool isBrushVisible(){ return mShowBrush; }

			// Toggle object orientation
	void toggleObjectOrientation(void) {
		if ( renderer ) renderer->toggleObjectOrientation();
		this->repaint();

	}

	void toggleNormals(void) {
		if ( renderer ) renderer->toggleFaceNormals();
		this->repaint();
	}

	void toggleFaceCentroids(void) {
		if ( renderer ) renderer->toggleFaceCentroids();
		this->repaint();
	}

	void selectAllFaces(){
		clearSelectedFaces();
		object->getFaces(object->sel_fptr_array);
		repaint();
	}

	void selectAllEdges(){
		clearSelectedEdges();
		object->getEdges(object->sel_eptr_array);
		repaint();
	}

	void selectAllVertices(){
		clearSelectedVertices();
		object->getVertices(object->sel_vptr_array);
		repaint();
	}

	void selectAllFaceVertices(){
		clearSelectedFaceVertices();
		// object->getVertices(object->sel_vptr_array);
		repaint();
	}
	
	void selectInverseFaces(){
		DLFLFacePtrArray fparray;
		vector<DLFLFacePtr>::iterator it;
		object->getFaces(fparray);
		for (it = fparray.begin(); it != fparray.end(); it++){
			if (!isSelected(*it)){
				addToSelection(*it);
			}
			else clearSelectedFace(*it);
		}
		repaint();
	}

	void selectInverseEdges(){
		DLFLEdgePtrArray eparray;
		vector<DLFLEdgePtr>::iterator it;
		object->getEdges(eparray);
		for (it = eparray.begin(); it != eparray.end(); it++){
			if (!isSelected(*it)){
				addToSelection(*it);
			}
			else clearSelectedEdge(*it);
		}
		repaint();
	}

	void selectInverseVertices(){
		DLFLVertexPtrArray vparray;
		vector<DLFLVertexPtr>::iterator it;
		object->getVertices(vparray);
		for (it = vparray.begin(); it != vparray.end(); it++){
			if (!isSelected(*it)){
				addToSelection(*it);
			}
			else clearSelectedVertex(*it);
		}
		repaint();
	}
		//maybe this isn't necessary? ??
	void selectInverseFaceVertices(){
		// DLFLFaceVertexPtrArray fvparray;
		// vector<DLFLFaceVertexPtr>::iterator it;
		// object->getFaceVertices(fvparray);
		// for (it = fvparray.begin(); it != fvparray.end(); it++){
		// 	if (!isSelected(*it)){
		// 		addToSelection(*it);
		// 	}
		// 	else clearSelectedFaceVertex(*it);
		// }
		// repaint();
	}
	
		// Toggle grid display on/off
	void toggleGrid(void) {
			// if ( showgrid == true ) showgrid = false;
	// 	    else showgrid = true;
	// 			this->repaint();
	}

		// Toggle axis display on/off
	void toggleAxes(void) {
		if ( showaxes == true ) showaxes = false;
		else showaxes = true;
		this->repaint();
	}

	protected :

	void initializeGL( );
	void paintEvent(QPaintEvent *event);

	void drawText( int width, int height );
	void drawIDs( QPainter *painter, const GLdouble *model, const GLdouble *proj, const GLint	*view);
	void drawSelectedIDs( QPainter *painter, const GLdouble *model, const GLdouble *proj, const GLint	*view);
	void drawHUD(QPainter *painter);
	void drawBrush(QPainter *painter);
	void drawSelectionWindow(QPainter *painter);
	void resizeGL( int width, int height );

	void setupViewport(int width, int height);

	#ifdef GPU_OK
		void enableGLLights(); //gl lighting for use in cg shaders
	  CgData *cg;
	  void initCg( );
	#endif // GPU_OK

	// void showEvent(QShowEvent *event);

	// void paintOverlayGL();
	// void initializeOverlayGL();
	// void resizeOverlayGL(int width, int height);

	bool mIsFullScreen;
	bool mShowFaceIDs;
	bool mShowVertexIDs;
	bool mShowEdgeIDs;
	bool mShowSelectedIDs;
	bool mShowFaceVertexIDs;
	bool mShowHUD;
	bool mShowBrush;
	bool mShowSelectionWindow;
	bool mUseGPU;
	bool mAntialiasing;
	int mBrushStartX;
	
	//temporarily disable object rendering
	bool renderObject;

		// Selection lists - these are shared by all viewports
	static DLFLLocatorPtrArray sel_lptr_array; // List of selected DLFLLocator pointers  // brianb  
  //static DLFLVertexPtrArray sel_vptr_array; // List of selected DLFLVertex pointers
  //static DLFLEdgePtrArray sel_eptr_array; // List of selected DLFLEdge pointers
  //static DLFLFacePtrArray sel_fptr_array; // List of selected DLFLFace pointers
  //static DLFLFaceVertexPtrArray sel_fvptr_array; // List of selected DLFLFaceVertex pointers

	// Viewport viewport;                     // Viewport for the window
	PerspCamera *mCamera;											//new camera originally from Don House and Chris Root, modified by Stuart and then Dave

		// Each viewport will have its own object to display. But since only
		// the pointer is stored, different viewports can share the same object.
	DLFLObjectPtr object;	                 // Pointer to DLFLObjec
	TMPatchObjectPtr patchObject; // The Patch Object associated with object

		// Each viewport will have its own renderer. But since only the pointer
		// is stored different viewports can share the same renderer object.
	DLFLRendererPtr renderer;
	
		//light is now a member of Glwidget instead of MainWindow
	PointLight plight;

		// Each viewport will have its own grid
	// Grid grid;                                        // Display grid

	// Default Locator object // brianb
	DLFLLocatorPtr locatorPtr;

		// Boolean flags - these are viewport specific
	bool showgrid;                           // Should grid be shown?
	bool showaxes;                           // Should axes be shown?

	QImage image;

	public :

	inline TMPatchObjectPtr patchobject( ) { return patchObject; };

	QCursor *cursor;
	void redraw();
	
	double getBrushSize(){ return mBrushSize;}
	void setBrushSize(double s){ mBrushSize = max(2.0,min(200.0,s)); repaint();	}

	inline void resetCamera(){ mCamera->Reset(); };
	// Viewport* getViewport() // brianb
	// {
	// 	return &viewport;
	// }
	
	void setRenderingEnabled(bool b){ renderObject = b; };
	
	void setModeString(QString s){
		mModeString = s;
	}

	void setExtrusionModeString(QString s){
		mExtrusionModeString = s;
	}
	
	void setRemeshingSchemeString(QString s){
		mRemeshingSchemeString = s;
	}

	void setSelectionMaskString(QString s){
		mSelectionMaskString = s;
	}
	QString getSelectionMaskString() { return mSelectionMaskString; }
	
	void renderLocatorsForSelect() // brianb
	{
		locatorPtr->setRenderSelection(true);
		locatorPtr->render();
		locatorPtr->setRenderSelection(false);
	}

	void resetLocator() // brianb
	{
		locatorPtr->setActiveVertex(NULL);
	}

		//--- Initialize the selection lists ---//
	void initializeSelectionLists(int num) {
				// Reserves memory for the arrays to avoid reallocation
				// It is not mandatory to call this function, but recommended
		object->sel_vptr_array.reserve(num);
		object->sel_eptr_array.reserve(num);
		object->sel_fptr_array.reserve(num);
		object->sel_fvptr_array.reserve(num);
		sel_lptr_array.reserve(num); // brianb
	}

		//--- Add items to the selection lists - check for NULL pointers ---//
	void addToSelection(DLFLLocatorPtr lp) // brianb
	{
		if ( lp ) sel_lptr_array.push_back(lp);
	}

	void addToSelection(DLFLVertexPtr vp) {
		if ( vp ) object->sel_vptr_array.push_back(vp);
	}

	void addToSelection(DLFLEdgePtr ep) {
		if ( ep ) object->sel_eptr_array.push_back(ep);
	}

	void addToSelection(DLFLFacePtr fp) {
		if ( fp ) object->sel_fptr_array.push_back(fp);
	}

	void addToSelection(DLFLFaceVertexPtr fvp) {
		if ( fvp ) object->sel_fvptr_array.push_back(fvp);
	}

		//--- Check if given item is there in the selection list ---//
			//--- Check if given item is there in the selection list ---//
	static bool isSelected(DLFLLocatorPtr vp)
	{
		bool found = false;
		if ( vp )
		{
			for (int i=0; i < sel_lptr_array.size(); ++i)
				if ( sel_lptr_array[i] == vp )
			{
				found = true; break;
			}
		}
		return found;
	}

	bool isSelected(DLFLVertexPtr vp) {
		bool found = false;
		if ( vp )
		{
			for (uint i=0; i < object->sel_vptr_array.size(); ++i)
				if ( object->sel_vptr_array[i] == vp )
			{
				found = true; break;
			}
		}
		return found;
	}

	bool isSelected(DLFLEdgePtr ep) {
		bool found = false;
		if ( ep )
		{
			for (uint i=0; i < object->sel_eptr_array.size(); ++i)
				if ( object->sel_eptr_array[i] == ep )
			{
				found = true; break;
			}
		}
		return found;
	}

	bool isSelected(DLFLFacePtr fp) {
		bool found = false;
		if ( fp )
		{
			for (uint i=0; i < object->sel_fptr_array.size(); ++i)
				if ( object->sel_fptr_array[i] == fp )
			{
				found = true; break;
			}
		}
		return found;
	}

	bool isSelected(DLFLFaceVertexPtr fvp) {
		bool found = false;
		if ( fvp )
		{
			for (uint i=0; i < object->sel_fvptr_array.size(); ++i)
				if ( object->sel_fvptr_array[i] == fvp )
			{
				found = true; break;
			}
		}
		return found;
	}

		//--- Set the selected item at given index ---//
		//--- If size of array is smaller than index item will be added to end of array ---//
	void setSelectedLocator(int index, DLFLLocatorPtr vp) // brianb
	{
		if ( vp && index >= 0 )
		{
			if ( index < sel_lptr_array.size() ) sel_lptr_array[index] = vp;
			else sel_lptr_array.push_back(vp);
		}
	}

	void setSelectedVertex(int index, DLFLVertexPtr vp) {
		if ( vp && index >= 0 )
		{
			if ( (uint) index < object->sel_vptr_array.size() ) object->sel_vptr_array[index] = vp;
			else object->sel_vptr_array.push_back(vp);
		}
	}

	void setSelectedEdge(int index, DLFLEdgePtr ep) {
		if ( ep && index >= 0 )
		{
			if ( (uint) index < object->sel_eptr_array.size() ) object->sel_eptr_array[index] = ep;
			else object->sel_eptr_array.push_back(ep);
		}
	}

	void setSelectedFace(int index, DLFLFacePtr fp) {
		if ( fp && index >= 0 )
		{
			if ( (uint) index < object->sel_fptr_array.size() ) object->sel_fptr_array[index] = fp;
			else object->sel_fptr_array.push_back(fp);
		}
	}

	void setSelectedFaceVertex(int index, DLFLFaceVertexPtr fvp) {
		if ( fvp && index >= 0 )
		{
			if ( (uint) index < object->sel_fvptr_array.size() ) object->sel_fvptr_array[index] = fvp;
			else object->sel_fvptr_array.push_back(fvp);
		}
	}

	void setSelectedVertex(DLFLVertexPtr vp) {
		if ( vp )
		{
			if (!isSelected(vp)){
				// if ( (uint) index < object->sel_vptr_array.size() ) object->sel_vptr_array[index] = vp;
				object->sel_vptr_array.push_back(vp);
			}
		}
	}

	void setSelectedEdge(DLFLEdgePtr ep) {
		if ( ep )
		{
			if (!isSelected(ep)){
				// if ( (uint) index < object->sel_eptr_array.size() ) object->sel_eptr_array[index] = ep;
				 object->sel_eptr_array.push_back(ep);
			}
		}
	}

	void setSelectedFace(DLFLFacePtr fp) {
		if ( fp )
		{
			if (!isSelected(fp)){
				// if ( (uint) index < object->sel_fptr_array.size() ) object->sel_fptr_array[index] = fp;
				object->sel_fptr_array.push_back(fp);
			}
		}
	}

	void setSelectedFaceVertex(DLFLFaceVertexPtr fvp) {
		if ( fvp )
		{
			if (!isSelected(fvp)){
				// if ( (uint) index < object->sel_fvptr_array.size() ) object->sel_fvptr_array[index] = fvp;
				object->sel_fvptr_array.push_back(fvp);
			}
		}
	}

		//--- Return the selected items at given index ---//
	DLFLLocatorPtr getLocatorPtr() const  // brianb
	{
		return locatorPtr;
	}

	static DLFLLocatorPtr getSelectedLocator(int index) // brianb
	{
		if ( index < sel_lptr_array.size() ) return sel_lptr_array[index];
		return NULL;
	}

	DLFLVertexPtr getSelectedVertex(int index) {
		if ( (uint) index < object->sel_vptr_array.size() ) return object->sel_vptr_array[index];
		return NULL;
	}

	DLFLEdgePtr getSelectedEdge(int index) {
		if ( (uint) index < object->sel_eptr_array.size() ) return object->sel_eptr_array[index];
		return NULL;
	}

	DLFLFacePtr getSelectedFace(int index) {
		if ( (uint) index < object->sel_fptr_array.size() ) return object->sel_fptr_array[index];
		return NULL;
	}

	DLFLFacePtrArray getSelectedFaces() {
		return object->sel_fptr_array;
	}

	DLFLVertexPtrArray getSelectedVertices() {
		return object->sel_vptr_array;
	}

	DLFLEdgePtrArray getSelectedEdges() {
		return object->sel_eptr_array;
	}

	DLFLFaceVertexPtrArray getSelectedCorners() {
		return object->sel_fvptr_array;
	}

	DLFLFaceVertexPtr getSelectedFaceVertex(int index) {
		if ( (uint) index < object->sel_fvptr_array.size() ) return object->sel_fvptr_array[index];
		return NULL;
	}

		//--- Find the number of items in the selection lists ---//
	static int numSelectedLocators(void) { // brianb
		return sel_lptr_array.size();
}

int numSelectedVertices(void) {
	return object->sel_vptr_array.size();
}

int numSelectedEdges(void) {
	return object->sel_eptr_array.size();
}

int numSelectedFaces(void) {
	return object->sel_fptr_array.size();
}

int numSelectedCorners(void) {
	return object->sel_fvptr_array.size();
}

		//--- Clear individual selection lists ---//
static void clearSelectedLocators(void) {
	sel_lptr_array.clear();
}

void clearSelectedVertices(void) {
	object->sel_vptr_array.clear();
}

void clearSelectedEdges(void) {
	object->sel_eptr_array.clear();
}

void clearSelectedFaces(void) {
	object->sel_fptr_array.clear();
}

void clearSelectedCorners(void) {
	object->sel_fvptr_array.clear();
}

void clearSelectedFace(DLFLFacePtr fp){
	vector<DLFLFacePtr>::iterator fiterator;
	for(fiterator = object->sel_fptr_array.begin(); fiterator != object->sel_fptr_array.end(); fiterator++) {
		if (fp == *fiterator){
			// delete fp;
			object->sel_fptr_array.erase(fiterator);
			return;
		}
	}
}

void clearSelectedEdge(DLFLEdgePtr ep){
	vector<DLFLEdgePtr>::iterator it;
	for(it = object->sel_eptr_array.begin(); it != object->sel_eptr_array.end(); it++) {
		if (ep == *it){
			object->sel_eptr_array.erase(it);
			return;
		}
	}
}

void clearSelectedVertex(DLFLVertexPtr vp){
	vector<DLFLVertexPtr>::iterator it;
	for(it = object->sel_vptr_array.begin(); it != object->sel_vptr_array.end(); it++) {
		if (vp == *it){
			object->sel_vptr_array.erase(it);
			return;
		}
	}
}

void clearSelectedFaceVertex(DLFLFaceVertexPtr fvp){
	vector<DLFLFaceVertexPtr>::iterator it;
	for(it = object->sel_fvptr_array.begin(); it != object->sel_fvptr_array.end(); it++) {
		if (fvp == *it){
			object->sel_fvptr_array.erase(it);
			return;
		}
	}
}

void clearSelectedFaceVertices(void) {
	object->sel_fvptr_array.clear();
}

		// Reset all selection lists
void clearSelected(void) {
	sel_lptr_array.clear(); // brianb
	object->sel_vptr_array.clear();
	object->sel_eptr_array.clear();
	object->sel_fptr_array.clear();
	object->sel_fvptr_array.clear();
}

		// Draw the selected items
void drawSelected(void);

		// Subroutine to translate FLTK events to Viewport events
// static VPMouseEvent translateEvent(QMouseEvent *event) {
// 			// QMessageBox::about(NULL, tr("About TopMod"),tr("%1").arg(event->type()));
// 	return ( (event->type() == 2/*QEvent::mouseButtonPress*/) ? VPPush :
// 	( (event->type() == 3/*QEvent::mouseButtonRelease*/) ? VPRelease :
// 	( (event->type() == 5/*QEvent::mouseMove*/) ? VPDrag : VPUnknown ) ) );
// }

		// Set the object which should be shown in this viewport
	void createPatchObject( ) {//TMPatchObject* po ) {
		//patchObject = po;
		if( patchObject != NULL ) { 
			delete patchObject; patchObject = 0; 
		}
		patchObject = new TMPatchObject( object->getID() );
		if( patchObject )
			patchObject->updatePatches( object );
	};

		// Set the renderer for this viewport
void setRenderer(DLFLRendererPtr rp) {
	renderer = rp;
	renderer->setState();
	this->repaint();
}

DLFLRendererPtr getRenderer(){
	return renderer;
}

		// Set the render flags
void setRenderFlags(int rflags) {
	if ( renderer ) renderer->setRenderFlags(rflags);
	this->repaint();
}

		// Get the render flags
int getRenderFlags(void) const {
	if ( renderer )
		return renderer->getRenderFlags();
	return -1;
}

	// Subroutines for selecting Vertices, Edges, Faces and FaceVertices (Corners)
DLFLLocatorPtr selectLocator(int mx, int my, int w=10, int h=10);  // brianb
DLFLVertexPtr selectVertex(int mx, int my, int w=30, int h=30);
DLFLVertexPtrArray selectVertices(int mx, int my, int w=30, int h=30);
DLFLEdgePtr selectEdge(int mx, int my, int w=10, int h=10);
DLFLEdgePtrArray selectEdges(int mx, int my, int w=10, int h=10);
DLFLFacePtr selectFace(int mx, int my, int w=2.5, int h=2.5);
DLFLFacePtrArray selectFaces(int mx, int my, int w=2.5, int h=2.5);
DLFLFacePtr deselectFaces(int mx, int my, int w=2.5, int h=2.5);
// DLFLFacePtrArray selectFaces(int mx, int my);
// DLFLFacePtrArray deselectFaces(int mx, int my);
DLFLFaceVertexPtr selectFaceVertex(DLFLFacePtr fp, int mx, int my, int w=40, int h=40);
DLFLFaceVertexPtrArray selectFaceVertices(DLFLFacePtr fp, int mx, int my, int w=40, int h=40);

	// Event handler for the viewport. handles only mouse events when glwidget has focus
	// which relate to navigational controls (ALT + push/drag/release)
void mouseMoveEvent(QMouseEvent *event);
void mousePressEvent(QMouseEvent *event);
void mouseReleaseEvent(QMouseEvent *event);
void wheelEvent(QWheelEvent *event);

// void keyPressEvent(QKeyEvent *event);
// void keyReleaseEvent(QKeyEvent *event);

private :
friend class QGLFormat;
QColor mGlobalAmbient;
QColor mRenderColor;
QColor mViewportColor;

QColor mWireframeColor;
QColor mSilhouetteColor;
double mWireframeThickness;
double mSilhouetteThickness;
double mVertexThickness;

QColor mVertexIDColor;
QColor mEdgeIDColor;
QColor mFaceIDColor;
QColor mVertexIDBgColor;
QColor mEdgeIDBgColor;
QColor mFaceIDBgColor;

QColor mSelectedVertexColor;
double mSelectedVertexThickness;
QColor mSelectedFaceColor;
QColor mSelectedEdgeColor;
double mSelectedEdgeThickness;

double mBrushSize;
double mStartDragX;
double mStartDragY;
double mSelectionWindowStartX;
double mSelectionWindowStartY;

QColor mXAxisColor;
QColor mYAxisColor;
QColor mZAxisColor;

QWidget *mParent;
QString mModeString, mRemeshingSchemeString, mSelectionMaskString, mExtrusionModeString;

};

#endif 
