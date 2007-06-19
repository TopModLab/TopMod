#ifndef _GL_WIDGET_H
#define _GL_WIDGET_H

#include "Viewport.hh"
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

#include "Camera2.hh"
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
	
	GLWidget(	int w, int h , VPView v, DLFLRendererPtr rp, QColor color, QColor vcolor, DLFLObjectPtr op, TMPatchObjectPtr pop, const QGLFormat & format, QWidget * parent = 0 );

	~GLWidget( );

	public slots :

	void toggleFullScreen( );
	void setSilhouetteThickness(double t);
	void setWireframeThickness(double t);
	void setVertexThickness(double t);
	void update() { repaint(); };
	void setSelectedVertexThickness(double t);
	void setSelectedEdgeThickness(double t);
	void switchTo(VPView view);

	void toggleHUD() {
		mShowHUD = !mShowHUD;
		this->repaint();
	}

	void toggleGPU(){
		mUseGPU = !mUseGPU;
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
		if ( renderer ) renderer->toggleNormals();
		this->repaint();
	}

	void selectAllFaces(){
		clearSelectedFaces();
		object->getFaces(DLFLObject::sel_fptr_array);
		repaint();
	}

	void selectAllEdges(){
		clearSelectedEdges();
		object->getEdges(DLFLObject::sel_eptr_array);
		repaint();
	}

	void selectAllVertices(){
		clearSelectedVertices();
		object->getVertices(DLFLObject::sel_vptr_array);
		repaint();
	}

	void selectAllFaceVertices(){
		clearSelectedFaceVertices();
		// object->getVertices(DLFLObject::sel_vptr_array);
		repaint();
	}
	
	void selectInverseFaces(){
		DLFLFacePtrArray fparray;
		vector<DLFLFacePtr>::iterator it;
		object->getFaces(fparray);
		for (it = fparray.begin(); it != fparray.end(); it++){
			if (!GLWidget::isSelected(*it)){
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
			if (!GLWidget::isSelected(*it)){
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
			if (!GLWidget::isSelected(*it)){
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
		// 	if (!GLWidget::isSelected(*it)){
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
	// void paintGL( );

	void drawText( int width, int height );
	void drawIDs( QPainter *painter, const GLdouble *model, const GLdouble *proj, const GLint	*view);
	void drawSelectedIDs( QPainter *painter, const GLdouble *model, const GLdouble *proj, const GLint	*view);
	void drawHUD(QPainter *painter);
	void drawBrush(QPainter *painter);
	void resizeGL( int width, int height );

	void setupViewport(int width, int height);

	#ifdef GPU_OK
	  CgData cg;
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
	bool mUseGPU;
	int mBrushStartX;

		// Selection lists - these are shared by all viewports
	static DLFLLocatorPtrArray sel_lptr_array; // List of selected DLFLLocator pointers  // brianb  
  //static DLFLVertexPtrArray DLFLObject::sel_vptr_array; // List of selected DLFLVertex pointers
  //static DLFLEdgePtrArray sel_eptr_array; // List of selected DLFLEdge pointers
  //static DLFLFacePtrArray sel_fptr_array; // List of selected DLFLFace pointers
  //static DLFLFaceVertexPtrArray sel_fvptr_array; // List of selected DLFLFaceVertex pointers

	// Viewport viewport;                     // Viewport for the window
	Camera2 *mCamera;											//new camera originally from Don House and Chris Root, modified by Stuart and then Dave

		// Each viewport will have its own object to display. But since only
		// the pointer is stored, different viewports can share the same object.
	DLFLObjectPtr object;	                 // Pointer to DLFLObjec
	TMPatchObjectPtr patchObject; // The Patch Object associated with object

		// Each viewport will have its own renderer. But since only the pointer
		// is stored different viewports can share the same renderer object.
	DLFLRendererPtr renderer;

		// Each viewport will have its own grid
	// Grid grid;                                        // Display grid

	// Default Locator object // brianb
	DLFLLocatorPtr locatorPtr;

		// Boolean flags - these are viewport specific
	bool showgrid;                           // Should grid be shown?
	bool showaxes;                           // Should axes be shown?

	QImage image;

	public :

	QCursor *cursor;
	void redraw();

	void setViewportColor(QColor c);
	QColor getViewportColor();

	void setRenderColor(QColor c);
	QColor getRenderColor();

	void setWireframeColor(QColor c);
	QColor getWireframeColor();

	void setSilhouetteColor(QColor c);
	QColor getSilhouetteColor();

	void setSelectedVertexColor(QColor c);
	QColor getSelectedVertexColor();

	void setSelectedEdgeColor(QColor c);
	QColor getSelectedEdgeColor();

	void setSelectedFaceColor(QColor c);
	QColor getSelectedFaceColor();

	void setVertexIDBgColor(QColor c);
	QColor getVertexIDBgColor();

	void setFaceIDBgColor(QColor c);
	QColor getFaceIDBgColor();

	void setEdgeIDBgColor(QColor c);
	QColor getEdgeIDBgColor();

	double getWireframeThickness();
	double getVertexThickness();
	double getSilhouetteThickness();
	double getSelectedVertexThickness();
	double getSelectedEdgeThickness();	

	double getBrushSize(){ return mBrushSize;}
	void setBrushSize(double s){ mBrushSize = max(2.0,min(200.0,s)); repaint();	}

	inline void resetCamera(){ mCamera->Reset(); };
	// Viewport* getViewport() // brianb
	// {
	// 	return &viewport;
	// }

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
	static void initializeSelectionLists(int num) {
				// Reserves memory for the arrays to avoid reallocation
				// It is not mandatory to call this function, but recommended
		DLFLObject::sel_vptr_array.reserve(num);
		DLFLObject::sel_eptr_array.reserve(num);
		DLFLObject::sel_fptr_array.reserve(num);
		DLFLObject::sel_fvptr_array.reserve(num);
		sel_lptr_array.reserve(num); // brianb
	}

		//--- Add items to the selection lists - check for NULL pointers ---//
	static void addToSelection(DLFLLocatorPtr lp) // brianb
	{
		if ( lp ) sel_lptr_array.push_back(lp);
	}

	static void addToSelection(DLFLVertexPtr vp) {
		if ( vp ) DLFLObject::sel_vptr_array.push_back(vp);
	}

	static void addToSelection(DLFLEdgePtr ep) {
		if ( ep ) DLFLObject::sel_eptr_array.push_back(ep);
	}

	static void addToSelection(DLFLFacePtr fp) {
		if ( fp ) DLFLObject::sel_fptr_array.push_back(fp);
	}

	static void addToSelection(DLFLFaceVertexPtr fvp) {
		if ( fvp ) DLFLObject::sel_fvptr_array.push_back(fvp);
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

	static bool isSelected(DLFLVertexPtr vp) {
		bool found = false;
		if ( vp )
		{
			for (uint i=0; i < DLFLObject::sel_vptr_array.size(); ++i)
				if ( DLFLObject::sel_vptr_array[i] == vp )
			{
				found = true; break;
			}
		}
		return found;
	}

	static bool isSelected(DLFLEdgePtr ep) {
		bool found = false;
		if ( ep )
		{
			for (uint i=0; i < DLFLObject::sel_eptr_array.size(); ++i)
				if ( DLFLObject::sel_eptr_array[i] == ep )
			{
				found = true; break;
			}
		}
		return found;
	}

	static bool isSelected(DLFLFacePtr fp) {
		bool found = false;
		if ( fp )
		{
			for (uint i=0; i < DLFLObject::sel_fptr_array.size(); ++i)
				if ( DLFLObject::sel_fptr_array[i] == fp )
			{
				found = true; break;
			}
		}
		return found;
	}

	static bool isSelected(DLFLFaceVertexPtr fvp) {
		bool found = false;
		if ( fvp )
		{
			for (uint i=0; i < DLFLObject::sel_fvptr_array.size(); ++i)
				if ( DLFLObject::sel_fvptr_array[i] == fvp )
			{
				found = true; break;
			}
		}
		return found;
	}

		//--- Set the selected item at given index ---//
		//--- If size of array is smaller than index item will be added to end of array ---//
	static void setSelectedLocator(int index, DLFLLocatorPtr vp) // brianb
	{
		if ( vp && index >= 0 )
		{
			if ( index < sel_lptr_array.size() ) sel_lptr_array[index] = vp;
			else sel_lptr_array.push_back(vp);
		}
	}

	static void setSelectedVertex(int index, DLFLVertexPtr vp) {
		if ( vp && index >= 0 )
		{
			if ( (uint) index < DLFLObject::sel_vptr_array.size() ) DLFLObject::sel_vptr_array[index] = vp;
			else DLFLObject::sel_vptr_array.push_back(vp);
		}
	}

	static void setSelectedEdge(int index, DLFLEdgePtr ep) {
		if ( ep && index >= 0 )
		{
			if ( (uint) index < DLFLObject::sel_eptr_array.size() ) DLFLObject::sel_eptr_array[index] = ep;
			else DLFLObject::sel_eptr_array.push_back(ep);
		}
	}

	static void setSelectedFace(int index, DLFLFacePtr fp) {
		if ( fp && index >= 0 )
		{
			if ( (uint) index < DLFLObject::sel_fptr_array.size() ) DLFLObject::sel_fptr_array[index] = fp;
			else DLFLObject::sel_fptr_array.push_back(fp);
		}
	}

	static void setSelectedFaceVertex(int index, DLFLFaceVertexPtr fvp) {
		if ( fvp && index >= 0 )
		{
			if ( (uint) index < DLFLObject::sel_fvptr_array.size() ) DLFLObject::sel_fvptr_array[index] = fvp;
			else DLFLObject::sel_fvptr_array.push_back(fvp);
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

	static DLFLVertexPtr getSelectedVertex(int index) {
		if ( (uint) index < DLFLObject::sel_vptr_array.size() ) return DLFLObject::sel_vptr_array[index];
		return NULL;
	}

	static DLFLEdgePtr getSelectedEdge(int index) {
		if ( (uint) index < DLFLObject::sel_eptr_array.size() ) return DLFLObject::sel_eptr_array[index];
		return NULL;
	}

	static DLFLFacePtr getSelectedFace(int index) {
		if ( (uint) index < DLFLObject::sel_fptr_array.size() ) return DLFLObject::sel_fptr_array[index];
		return NULL;
	}

	static DLFLFacePtrArray getSelectedFaces() {
		return DLFLObject::sel_fptr_array;
	}

	static DLFLFaceVertexPtr getSelectedFaceVertex(int index) {
		if ( (uint) index < DLFLObject::sel_fvptr_array.size() ) return DLFLObject::sel_fvptr_array[index];
		return NULL;
	}

		//--- Find the number of items in the selection lists ---//
	static int numSelectedLocators(void) { // brianb
		return sel_lptr_array.size();
}

static int numSelectedVertices(void) {
	return DLFLObject::sel_vptr_array.size();
}

static int numSelectedEdges(void) {
	return DLFLObject::sel_eptr_array.size();
}

static int numSelectedFaces(void) {
	return DLFLObject::sel_fptr_array.size();
}

static int numSelectedFaceVertices(void) {
	return DLFLObject::sel_fvptr_array.size();
}

		//--- Clear individual selection lists ---//
static void clearSelectedLocators(void) {
	sel_lptr_array.clear();
}

static void clearSelectedVertices(void) {
	DLFLObject::sel_vptr_array.clear();
}

static void clearSelectedEdges(void) {
	DLFLObject::sel_eptr_array.clear();
}

static void clearSelectedFaces(void) {
	DLFLObject::sel_fptr_array.clear();
}


static void clearSelectedFace(DLFLFacePtr fp){
	vector<DLFLFacePtr>::iterator fiterator;
	for(fiterator = DLFLObject::sel_fptr_array.begin(); fiterator != DLFLObject::sel_fptr_array.end(); fiterator++) {
		if (fp == *fiterator){
			// delete fp;
			DLFLObject::sel_fptr_array.erase(fiterator);
			return;
		}
	}
}

static void clearSelectedEdge(DLFLEdgePtr ep){
	vector<DLFLEdgePtr>::iterator it;
	for(it = DLFLObject::sel_eptr_array.begin(); it != DLFLObject::sel_eptr_array.end(); it++) {
		if (ep == *it){
			DLFLObject::sel_eptr_array.erase(it);
			return;
		}
	}
}

static void clearSelectedVertex(DLFLVertexPtr vp){
	vector<DLFLVertexPtr>::iterator it;
	for(it = DLFLObject::sel_vptr_array.begin(); it != DLFLObject::sel_vptr_array.end(); it++) {
		if (vp == *it){
			DLFLObject::sel_vptr_array.erase(it);
			return;
		}
	}
}

static void clearSelectedFaceVertex(DLFLFaceVertexPtr fvp){
	vector<DLFLFaceVertexPtr>::iterator it;
	for(it = DLFLObject::sel_fvptr_array.begin(); it != DLFLObject::sel_fvptr_array.end(); it++) {
		if (fvp == *it){
			DLFLObject::sel_fvptr_array.erase(it);
			return;
		}
	}
}

static void clearSelectedFaceVertices(void) {
	DLFLObject::sel_fvptr_array.clear();
}

		// Reset all selection lists
static void clearSelected(void) {
	sel_lptr_array.clear(); // brianb
	DLFLObject::sel_vptr_array.clear();
	DLFLObject::sel_eptr_array.clear();
	DLFLObject::sel_fptr_array.clear();
	DLFLObject::sel_fvptr_array.clear();
}

		// Draw the selected items
void drawSelected(void);

		// Subroutine to translate FLTK events to Viewport events
static VPMouseEvent translateEvent(QMouseEvent *event) {
			// QMessageBox::about(NULL, tr("About TopMod"),tr("%1").arg(event->type()));
	return ( (event->type() == 2/*QEvent::mouseButtonPress*/) ? VPPush :
	( (event->type() == 3/*QEvent::mouseButtonRelease*/) ? VPRelease :
	( (event->type() == 5/*QEvent::mouseMove*/) ? VPDrag : VPUnknown ) ) );
}

		// Set the object which should be shown in this viewport
void setObject(DLFLObjectPtr op) {
	object = op;
	if( patchObject ) { delete patchObject; patchObject = 0; }
	//patchObject = new TMPatchObject( object->getID() );
}

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
DLFLLocatorPtr selectLocator(int mx, int my);  // brianb
DLFLVertexPtr selectVertex(int mx, int my);
DLFLEdgePtr selectEdge(int mx, int my);
DLFLFacePtr selectFace(int mx, int my);
DLFLFacePtrArray selectFaces(int mx, int my);
DLFLFacePtrArray deselectFaces(int mx, int my);
DLFLFaceVertexPtr selectFaceVertex(DLFLFacePtr fp, int mx, int my);

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

QColor mXAxisColor;
QColor mYAxisColor;
QColor mZAxisColor;

};

#endif 
