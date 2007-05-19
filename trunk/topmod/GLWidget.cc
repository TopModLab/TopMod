#include "GLWidget.hh"


// //==============================================================================
// // Make the linker happy for the framework check (see link below for more info)
// // http://developer.apple.com/documentation/MacOSX/Conceptual/BPFrameworks/Concepts/WeakLinking.html
// 
// extern OSErr InstallConnexionHandlers() __attribute__((weak_import));
// 
// OSErr	error = InstallConnexionHandlers(GLWidget::spaceNavEvent, 0L, 0L);;
// UInt16 fConnexionClientID = RegisterConnexionClient('MCTt', (UInt8*)"\pTopMod", kConnexionClientModeTakeOver, kConnexionMaskAll);

// Constructor
GLWidget::GLWidget(int x, int y, int w, int h, QWidget *parent) : QGLWidget(parent),
	viewport(w,h), object(NULL), renderer(NULL), /*grid(ZX,20.0,10),*/
	showgrid(false), showaxes(false){

}

// Constructor
GLWidget::GLWidget(int x, int y, int w, int h, DLFLRendererPtr rp, DLFLObjectPtr op, QWidget *parent ) : QGLWidget(parent),
	viewport(w,h), object(op), renderer(rp),  /*grid(ZX,20.0,10),*/	showgrid(false), showaxes(false) {
		
	// setupViewport(w,h);
	// paintGL();

	// QGLContext *cx;
	// 		QGLFormat f;
	// 		f.setStereo( TRUE );
	// 		QGLFormat::setDefaultFormat( f );
	// 		cx->setFormat( f );
	// 		if ( !cx->create() )
	// 		    // printf("FALSE \n");
	// 			QMessageBox::about(this, tr("false"),tr("false"));
	// 		if ( !cx->format().stereo() )

	// // 			QMessageBox::about(this, tr("false"),tr("false"));
	// 		QGLFormat f;
	// 		f.setStereo(true);
	// 		QGLContext *cx = new QGLContext(f);
	// 		 // cx->setFormat(f);
	// 		 if (!cx->create())
	// 		     exit(0); // no OpenGL support, or cannot render on the specified paintdevice
	// 		 if (!cx->format().stereo())
	// 		     exit(0); // could not create stereo context
}

GLWidget::GLWidget(int w, int h, DLFLRendererPtr rp, DLFLObjectPtr op, const QGLFormat & format, QWidget * parent ) : QGLWidget(format, parent, NULL),
 										viewport(w,h), object(op), renderer(rp), /*grid(ZX,20.0,10),*/ showgrid(false), showaxes(false) {
	
}

GLWidget::~GLWidget(){
	// UnregisterConnexionClient(fConnexionClientID); 
	// CleanupConnexionHandlers(); 
}

void GLWidget::redraw() {
  updateGL();
}

void GLWidget::initializeGL( ) {
  setAutoBufferSwap( true );
  glClearColor(0.29,0.33,0.37,1); // Modo's viewport color
  viewport.resize(this->size().width(),this->size().height());
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);					// Set Line Antialiasing
  glEnable(GL_BLEND);							// Enable Blending
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// Type Of Blending To Use

  mShowVertexIDs = false;
  mShowEdgeIDs = false;
  mShowFaceIDs = false;
}

void GLWidget::paintGL( ) {

	viewport.resize(this->size().width(),this->size().height());	
	renderer->initialize();	
	
	glClearColor(0.29,0.33,0.37,1); // Modo's viewport color
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);

  glMatrixMode(GL_MODELVIEW);

  glPushMatrix(); 
  {
    // Multiply by the transformation matrix.
    viewport.apply_transform();
    
    // Draw the axes if required. Use thick lines
    if ( showaxes ) {
      glLineWidth(3.0);
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(2,0xAAAA);
      glBegin(GL_LINES); {
				glColor3f(0.5,0.25,0.25);                     // X Axis - Red
				glVertex3f(0,0,0); glVertex3f(-10,0,0);
				glColor3f(0.25,0.5,0.25);                     // Z Axis - Green
				glVertex3f(0,0,0); glVertex3f(0,0,-10);
				glColor3f(0.25,0.25,0.5);                     // Y Axis - Blue
				glVertex3f(0,0,0); glVertex3f(0,-10,0);
      }
      glEnd();
      glDisable(GL_LINE_STIPPLE);
      glBegin(GL_LINES); 
      {
				glColor3f(0.6,0.25,0.25);                     // X Axis - Red
				glVertex3f(0,0,0); glVertex3f(10,0,0);
				glColor3f(0.25,0.6,0.25);                     // Z Axis - Green
				glVertex3f(0,0,0); glVertex3f(0,0,10);
				glColor3f(0.25,0.25,0.6);                     // Y Axis - Blue
				glVertex3f(0,0,0); glVertex3f(0,10,0);
      }
      glEnd();
      glLineWidth(3.0);
    }
    // Draw the actual object using the renderer.
    // Assumes that render flags have already been set for the renderer
    // Default color is slightly bluish
    glColor3f(0.2,0.5,0.85);

    if ( renderer ) renderer->render(object);
    
    // Show any selected items.
    // Adjust the depthrange so selected items are shown clearly
    glDepthRange(0,1-0.0005-0.0005);
    GLWidget::drawSelected();    
    drawIDs( ); // draw vertex ids
    glDepthRange(0,1);

  } 
  glPopMatrix();

  // glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // glFrustum(-1.0, +1.0, -1.0, 1.0, 5.0, 60.0);
  gluPerspective(60.0f,(GLfloat)width()/(GLfloat)height(),0.1f,100.0f);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslated(0.0, 0.0, -50.0);
}

void GLWidget::resizeGL( int width, int height ){
	
  setupViewport(width,height);
	viewport.resize(this->size().width(),this->size().height());
  // drawText(width,height);	
}


void GLWidget::paintOverlayGL() {
	// 
	//   QMessageBox::about(this, tr("false"),tr("paintOverlayGL = %1").arg(width()));
	//   
	// QPainter painter;
	// painter.begin(this);
	// 
	// painter.setRenderHint(QPainter::Antialiasing);
	// drawText(width(),height());	
	//   
	// painter.drawImage((width() - image.width())/2, 0, image);
	// painter.end();
	
}

void GLWidget::resizeOverlayGL( int width, int height ){
	
  // setupViewport(width,height);
  drawText(width,height);	
}

void GLWidget::initializeOverlayGL( ){
	
  // setupViewport(width,height);
  // drawText(width,height);	


}

// void GLWidget::showEvent(QShowEvent *event){
// 	
// 	

void GLWidget::setupViewport(int width, int height){
  if (height==0)								// Prevent A Divide By Zero By
    {
      height=1;							// Making Height Equal One
    }
  
  glViewport(0, 0, width, height);
  
  glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
  glLoadIdentity();							// Reset The Projection Matrix
  // glFrustum(-1.0, +1.0, -1.0, 1.0, 5.0, 60.0);
  // Calculate The Aspect Ratio Of The Window
  gluPerspective(60.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
  
  glMatrixMode(GL_MODELVIEW);						// Select The Modelview Matrix
  glLoadIdentity();
  glTranslated(0.0, 0.0, -50.0);
}

void GLWidget::drawText(int width, int height ){
  // QString text = tr("Click and drag with the left mouse button "
  //                   "to rotate the Qt logo.");
  // QFontMetrics metrics = QFontMetrics(font());
  // int border = qMax(4, metrics.leading());
  // 
  // QRect rect = metrics.boundingRect(0, 0, width - 2*border, int(height*0.125),
  // 				    Qt::AlignCenter | Qt::TextWordWrap, text);
  // image = QImage(width, rect.height() + 2*border, QImage::Format_ARGB32_Premultiplied);
  // image.fill(qRgba(0, 0, 0, 127));
  // 
  // QPainter painter;
  // painter.begin(&image);
  // painter.setRenderHint(QPainter::TextAntialiasing);
  // painter.setPen(Qt::white);
  // painter.drawText((width - rect.width())/2, border,
  //                  rect.width(), rect.height(),
  //                  Qt::AlignCenter | Qt::TextWordWrap, text);
  // painter.end();
}

void GLWidget::drawIDs( ) {
  glDisable(GL_DEPTH_TEST);
 
  QFont font10("arial"); 
  font10.setPointSize(10);
  font10.setBold(true);
	font10.setStyleStrategy(QFont::NoAntialias);

  /* Draw Vertex IDs */
  if( mShowVertexIDs ) {
    DLFLVertexPtrArray vparray;
    object->getVertices(vparray);
    DLFLVertexPtrArray::iterator it;
    for( it = vparray.begin(); it != vparray.end(); it++) {
      QString id = QString::number( (*it)->getID() );
      double x,y,z; (*it)->coords.get(x,y,z);
      glColor3f(0.886,0.565,0.02);
      renderText( x, y, z, id, font10 );
    }
  }

  /* Draw Edge IDs */
  if( mShowEdgeIDs ) {
    DLFLEdgePtrArray eparray;
    object->getEdges(eparray);
    DLFLEdgePtrArray::iterator it;
    for( it = eparray.begin(); it != eparray.end(); it++) {
      QString id = QString::number( (*it)->getID() );
      double x,y,z; (*it)->getMidPoint().get(x,y,z);
      glColor3f(0.671,0.886,0.02);
      renderText( x, y, z, id, font10 );
    }
  }

  /* Draw Face IDs */
  if( mShowFaceIDs ) {
    DLFLFacePtrArray fparray;
    object->getFaces(fparray);
    DLFLFacePtrArray::iterator it;
    for( it = fparray.begin(); it != fparray.end(); it++) {
      QString id = QString::number( (*it)->getID() );
      double x,y,z; (*it)->geomCentroid().get(x,y,z);
      glColor3f(0.886,0.02,0.875);
      renderText( x, y, z, id, font10 );
    }
  }
  glEnable(GL_DEPTH_TEST);
}

void GLWidget::toggleFullScreen( ) {
  if( !mIsFullScreen ) {    
    move(0,0);
    resize( QApplication::desktop()->size() );
    resizeGL( QApplication::desktop()->size().width(), QApplication::desktop()->size().height() );
    //setFocusPolicy( Qt::StrongFocus );
    //setWindowFlags( Qt::FramelessWindowHint );
    show();
  } else {
    //setWindowFlags( Qt::Widget );
    resize( 700, 800 );
    show();
  }
  mIsFullScreen != mIsFullScreen;
}

//-- Define static members --//

DLFLVertexPtrArray GLWidget::sel_vptr_array;
DLFLEdgePtrArray GLWidget::sel_eptr_array;
DLFLFacePtrArray GLWidget::sel_fptr_array;
DLFLFaceVertexPtrArray GLWidget::sel_fvptr_array;

// Subroutine for selecting a Vertex
DLFLVertexPtr GLWidget::selectVertex(int mx, int my)
{
  GLuint selectBuf[1024];
  uint closest;
  GLuint dist;
  long hits, index;
  DLFLVertexPtr sel(NULL);
  glSelectBuffer(1024,selectBuf);
  glRenderMode(GL_SELECT);
  glInitNames(); glPushName(0);
  GLint vp[4];
  glGetIntegerv(GL_VIEWPORT, vp);
  viewport.camera.enterSelectionMode(mx,my,20,20,vp); // Reduced sensitivity for picking points
  // Make sure earlier matrices are preserved, since multiple windows
  // seem to be sharing the same matrix stacks
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  
  // viewport.reshape();
  viewport.apply_transform();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  object->renderVerticesForSelect();
  // glFlush();
  
  glPopMatrix();
  
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  
  viewport.camera.leaveSelectionMode();
  
  hits = glRenderMode(GL_RENDER);
  if ( hits > 0 ){
    closest = 0; dist = 0xffffffff;
    while ( hits ){
      index = (hits-1)*4;
      if ( selectBuf[index+1] < dist ){
	dist = selectBuf[index+1];
	closest = selectBuf[index+3];
      }
      hits--;
    }
    // closest now has the id of the selected vertex
    sel = DLFLObject::vparray[closest];
  }
  return sel;
}

// Subroutine for selecting an Edge
DLFLEdgePtr GLWidget::selectEdge(int mx, int my)
{
  GLuint selectBuf[1024];
  uint closest;
  GLuint dist;
  long hits, index;
  DLFLEdgePtr sel(NULL);
         
  glSelectBuffer(1024,selectBuf);
  glRenderMode(GL_SELECT);

  glInitNames(); glPushName(0);

  GLint vp[4];
  glGetIntegerv(GL_VIEWPORT, vp);
  viewport.camera.enterSelectionMode(mx,my,10,10,vp);

  // Make sure earlier matrices are preserved, since multiple windows
  // seem to be sharing the same matrix stacks
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  viewport.reshape();
  viewport.apply_transform();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  object->renderEdgesForSelect();
  // glFlush();

  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  viewport.camera.leaveSelectionMode();

  hits = glRenderMode(GL_RENDER);
  if ( hits > 0 ){
    closest = 0; dist = 0xffffffff;
    while ( hits ){
      index = (hits-1)*4;
      if ( selectBuf[index+1] < dist ){
	dist = selectBuf[index+1];
	closest = selectBuf[index+3];
      }
      hits--;
    }
    // closest now has the id of the selected edge
    sel = DLFLObject::eparray[closest];
  }
  return sel;
}

// Subroutine for selecting a Face
DLFLFacePtr GLWidget::selectFace(int mx, int my)
{
  GLuint selectBuf[1024];
  uint closest;
  GLuint dist;
  long hits, index;
  DLFLFacePtr sel(NULL);
         
  glSelectBuffer(1024,selectBuf);
  glRenderMode(GL_SELECT);

  glInitNames(); glPushName(0);

  GLint vp[4];
  glGetIntegerv(GL_VIEWPORT, vp);
  viewport.camera.enterSelectionMode(mx,my,2.5,2.5,vp);

  // Make sure earlier matrices are preserved, since multiple windows
  // seem to be sharing the same matrix stacks
  // glLoadIdentity();
	
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  viewport.reshape();
  viewport.apply_transform();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  object->renderFacesForSelect();
  glFlush();

  glPopMatrix();
  
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  viewport.camera.leaveSelectionMode();

  hits = glRenderMode(GL_RENDER);
  if ( hits > 0 ){
    closest = 0; dist = 0xffffffff;
    while ( hits ){
      index = (hits-1)*4;
      if ( selectBuf[index+1] < dist ){
				dist = selectBuf[index+1];
				closest = selectBuf[index+3];
      }
      hits--;
    }
    // closest now has the id of the selected face
    sel = DLFLObject::fparray[closest];
  }	
  return sel;
}

// Subroutine for selecting a FaceVertex (Corner) within a Face
DLFLFaceVertexPtr GLWidget::selectFaceVertex(DLFLFacePtr fp, int mx, int my)
{
  GLuint selectBuf[1024];
  uint closest;
  GLuint dist;
  long hits, index;
  DLFLFaceVertexPtr sel(NULL);
         
  glSelectBuffer(1024,selectBuf);
  glRenderMode(GL_SELECT);

  glInitNames(); glPushName(0);

  GLint vp[4];
  glGetIntegerv(GL_VIEWPORT, vp);
  viewport.camera.enterSelectionMode(mx,my,40,40,vp);
  // Make sure earlier matrices are preserved, since multiple windows
  // seem to be sharing the same matrix stacks
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  viewport.reshape();
  viewport.apply_transform();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  object->renderFaceVerticesForSelect(fp);
  // glFlush();

  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  viewport.camera.leaveSelectionMode();

  hits = glRenderMode(GL_RENDER);
  if ( hits > 0 ){
    closest = 0; dist = 0xffffffff;
    while ( hits ){
      index = (hits-1)*4;
      if ( selectBuf[index+1] < dist ){
	dist = selectBuf[index+1];
	closest = selectBuf[index+3];
      }
      hits--;
    }
    // closest now has the id of the selected face vertex
    sel = DLFLObject::fvparray[closest];
  }
  return sel;
}

// Draw the selected items
void GLWidget::drawSelected(void)
{
  if ( !sel_vptr_array.empty() ){
    // Selected vertices are shown in red color
    glPointSize(5.0);
    glBegin(GL_POINTS);
    glColor3f(1,0,0);
    DLFLVertexPtrArray::iterator first, last;
    first = sel_vptr_array.begin(); last = sel_vptr_array.end();
    while ( first != last ){
      (*first)->render(); ++first;
    }
    glEnd();
    glPointSize(1.0);
  }

  if ( !sel_eptr_array.empty() ){
    // Selected edges are shown in blue color
    glLineWidth(3.0);
    glColor3f(0,0,1);
    DLFLEdgePtrArray::iterator first, last;
    first = sel_eptr_array.begin(); last = sel_eptr_array.end();
    while ( first != last ){
      (*first)->render(); ++first;
    }
    glLineWidth(3.0);
  }

  if ( !sel_fptr_array.empty() ){
    // Selected faces are shown in cyan color
    glLineWidth(2.0);
    glColor3f(0,1,1);
    DLFLFacePtrArray::iterator first, last;
    first = sel_fptr_array.begin(); last = sel_fptr_array.end();
    while ( first != last )	{
      (*first)->render_FVN(); ++first;
    }
    glLineWidth(3.0);
  }

  if ( !sel_fvptr_array.empty() ) {
    // Selected corners (vertices) are shown in green color
    glPointSize(6.0);
    glColor3f(0,1,0);
    glBegin(GL_POINTS);
    DLFLFaceVertexPtrArray::iterator first, last;
    first = sel_fvptr_array.begin(); last = sel_fvptr_array.end();
    while ( first != last ){
      (*first)->render(); ++first;
    }
    glEnd();
    glPointSize(1.5);
  }
}

void GLWidget::wheelEvent(QWheelEvent *event) {
  
  int x = event->x(), y = event->y();
  int numDegrees = event->delta() / 8;
  int numSteps = numDegrees / 15;
  double z;
  
  if ( viewport.current() == VPRotate) {
    viewport.handle_rotate(VPRelease,x,y); // Stop rotating
    //viewport.handle_zoom(VPPush,x,y); // Start zooming
    //QCursor::setShape(Qt::SizeAllCursor);
  } 
  else  if ( 	!(QApplication::keyboardModifiers() == Qt::AltModifier) && 
  		!(QApplication::keyboardModifiers() == Qt::ShiftModifier) &&
  		!(QApplication::keyboardModifiers() == (Qt::ShiftModifier | Qt::AltModifier) ) ){
    // QMessageBox::about(this, tr("false"),tr("numDegrees = %1").arg(numDegrees));
    viewport.handle_zoom(VPPush,x,y); // Start zooming
    viewport.send_to_current(VPPush,x,y);
    // updateGL();
  
    //QMessageBox::about(this, tr("false"),tr("z = %1").arg(z));
    viewport.handle_zoom(VPPush,x+numDegrees*3,y); // Start zooming
    //QMessageBox::about(this, tr("false"),tr("z = %1").arg(z));
    viewport.send_to_current(VPPush,x,y);		
    viewport.handle_zoom(VPRelease,x+numDegrees,y); // Start zooming
    viewport.send_to_current(VPRelease,x,y);		
    updateGL();
  
    // 	//QCursor::setShape(Qt::SizeAllCursor);
  }
  else  if ( QApplication::keyboardModifiers() == Qt::ShiftModifier ){
  		
    viewport.handle_pan(VPPush,x,y); 
    viewport.send_to_current(VPPush,x,y);
  		
    viewport.handle_pan(VPPush,x+numDegrees*2,y);
    viewport.send_to_current(VPPush,x,y);		
  		
    viewport.handle_pan(VPRelease,x+numDegrees*2,y);
    viewport.send_to_current(VPRelease,x,y);
  				
    updateGL();
  }
  else  if ( QApplication::keyboardModifiers() == Qt::AltModifier ){
  		
    viewport.handle_pan(VPPush,x,y); // Start panning up and down
    viewport.send_to_current(VPPush,x,y);
  		
    viewport.handle_pan(VPPush,x,y+numDegrees*2); // Start panning up and down		
    viewport.send_to_current(VPPush,x,y);		
  		
    viewport.handle_pan(VPRelease,x,y+numDegrees*2); // Start panning up and down
    viewport.send_to_current(VPRelease,x,y);
  				
    updateGL();
  }
  else  if ( QApplication::keyboardModifiers() == (Qt::AltModifier | Qt::ShiftModifier) ){
  		
    viewport.handle_rotate(VPPush,x,y); 
    viewport.send_to_current(VPPush,x,y);
  		
    viewport.handle_rotate(VPPush,x+numDegrees*2,y); 
    viewport.send_to_current(VPPush,x,y);		
  		
    viewport.handle_rotate(VPRelease,x+numDegrees*2,y); 
    viewport.send_to_current(VPRelease,x,y);
  				
    updateGL();
  }
  else  if ( QApplication::keyboardModifiers() == (Qt::ShiftModifier | Qt::MetaModifier) ){
  		
    viewport.handle_rotate(VPPush,x,y); 
    viewport.send_to_current(VPPush,x,y);
  		
    viewport.handle_rotate(VPPush,x,y+numDegrees*2); 
    viewport.send_to_current(VPPush,x,y);		
  		
    viewport.handle_rotate(VPRelease,x,y+numDegrees*2); 
    viewport.send_to_current(VPRelease,x,y);
  				
    updateGL();
  }
}
void GLWidget::mousePressEvent(QMouseEvent *event) {
  // updateGL();
  // QMessageBox::about(this, "HEY", tr("QApplication::KeyboardModifiers() = %1").arg(QApplication::keyboardModifiers()));		
  if ( QApplication::keyboardModifiers() == Qt::AltModifier ) {
    int x = event->x(), y = event->y();

    if ( event->buttons() == Qt::LeftButton ) {
      if ( viewport.current() == VPPan) {
	viewport.handle_pan(VPRelease,x,y); // Stop panning
	//viewport.handle_rotate(VPPush,x,y); // Start rotating
	setCursor(Qt::ArrowCursor);
      }
      else {
	viewport.handle_rotate(VPPush,x,y); // Start rotating
	// 	set the qt cursor - make a custom cursor later
	setCursor(Qt::CrossCursor);
      }
      // 	viewport.send_to_current(GLWidget::translateEvent(event),x,y);
      // 	this->updateGL();
			
    } 
    else if ( event->buttons() == Qt::RightButton ) {
      if ( viewport.current() == VPRotate) {
	viewport.handle_rotate(VPRelease,x,y); // Stop rotating
	//viewport.handle_zoom(VPPush,x,y); // Start zooming
	setCursor(Qt::ArrowCursor);
      } 
      else {
	viewport.handle_zoom(VPPush,x,y); // Start zooming
	setCursor(Qt::SizeAllCursor);
      }
    }
    else if ( event->buttons() == Qt::MidButton ) {
      if ( viewport.current() == VPRotate ) {
	viewport.handle_rotate(VPRelease,x,y); // Stop rotating
	//viewport.handle_pan(VPPush,x,y);
	setCursor(Qt::ArrowCursor);
      } 
      else {
	viewport.handle_pan(VPPush,x,y); // Start panning
	setCursor(Qt::CrossCursor);
      }
    }
  }
  else if ( (event->buttons() == Qt::LeftButton && QApplication::keyboardModifiers() == (Qt::ShiftModifier | Qt::AltModifier)) ){
    int x = event->x(), y = event->y();

    if ( viewport.current() == VPRotate ) {
      viewport.handle_rotate(VPRelease,x,y); // Stop rotating
      //viewport.handle_pan(VPPush,x,y);
      setCursor(Qt::ArrowCursor);
    } 
    else {
      viewport.handle_pan(VPPush,x,y); // Start panning
      setCursor(Qt::CrossCursor);
    }
  }
  else if ( event->buttons() == Qt::RightButton ){
    event->ignore();
  }
  else { event->ignore(); }
}

// this has been modified for the QT interface, so it handles focus automatically
void GLWidget::mouseMoveEvent(QMouseEvent *event) {	
  int x = event->x(), y = event->y();
	
  if ( QApplication::keyboardModifiers() == Qt::AltModifier ) {
    if ( viewport.send_to_current(GLWidget::translateEvent(event),x,y) ) {
      updateGL();
    }
  } 
  else if ( QApplication::keyboardModifiers() == (Qt::AltModifier | Qt::ShiftModifier) ) {
    if ( viewport.send_to_current(GLWidget::translateEvent(event),x,y) ) {
      updateGL();
    }
  }
  else if ( event->buttons() == Qt::RightButton ){
    event->ignore();
  }
  else {
    event->ignore();
  }
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
  setCursor(Qt::ArrowCursor);
	
  int x = event->x(), y = event->y();
  //QMessageBox::about(this, tr("About TopMod"),tr("%1").arg(QApplication::keyboardModifiers()));
  if ( QApplication::keyboardModifiers() == Qt::AltModifier ) {
    if ( viewport.send_to_current(GLWidget::translateEvent(event),x,y) ) {
      updateGL();
    }
  } 
  else if ( QApplication::keyboardModifiers() == (Qt::AltModifier | Qt::ShiftModifier) ) {
    if ( viewport.send_to_current(GLWidget::translateEvent(event),x,y) ) {
      updateGL();
    }
  }
  else if ( event->buttons() == Qt::RightButton ){
    event->ignore();
  }
  else { event->ignore(); }
  //	else {}//QMessageBox::about(this, tr("About TopMod"),tr("alt is not pressed: %1").arg(QApplication::keyboardModifiers()));
}


// void GLWidget::spaceNavEvent(io_connect_t connection, natural_t messageType, void *messageArgument){
// 	 
//  // ConnexionDeviceState  *state; 
//   
// 	QMessageBox::about(0, tr("About TopMod"),tr("you clicked the spacenav's buttons: %1").arg(messageType));
// 	ConnexionDeviceStatePtr msg = (ConnexionDeviceStatePtr)messageArgument;
// 	static UInt16 lastBtnPressed = 0;
//     
//     switch(messageType)
// 	{
// 		case kConnexionMsgDeviceState:
// 			/* Device state messages are broadcast to all clients.  It is up to
// 			 * the client to figure out if the message is meant for them. This
// 			 * is done by comparing the "client" id sent in the message to our
//              * assigned id when the connection to the driver was established.
// 			 * 
// 			 * There is a special mode wherein all events are sent to this 
// 			 * client regardless if it was meant for it or not.  This mode is 
// 			 * determined by the showClientEventOnly flag.
// 			 */
//             if (!gDevWrapperInfo.showClientEventsOnly || msg->client == gDevWrapperInfo.clientID)
// 			{
// 				switch (msg->command)
// 				{
//                     case kConnexionCmdHandleAxis:
//                     {
//                         static SInt16 zerodata[] = {0, 0, 0, 0, 0, 0};
//                         static Boolean isZero = FALSE, wasZero;
//                         
//                         wasZero = isZero;
//                         if (isZero = (memcmp(msg->axis, zerodata, 
//                                              sizeof(zerodata)) == 0))
//                         {
//                             if (wasZero == FALSE)
//                                 TdxComputeEventZero();
//                         }
//                         else
//                         {
//                             TdxComputeAxes(msg->axis);
//                         }
//                         break;
//                     }
//                         
//                     case kConnexionCmdHandleButtons:
//                     {
//                         SInt16 buttonState;
// 
//                         if (msg->value == 0)
//                         {
//                             buttonState = 0;
//                         }
//                         else
//                         {
//                             lastBtnPressed = msg->buttons;
//                             buttonState = 1;
//                         }
//                         
//                         TdxComputeButtons(lastBtnPressed, buttonState);
// 						break;
//                     }
//                         
//                     default:
//                         break;
// 
// 				} /* switch */
//             }
// 
// 			break;
// 
// 		default:
// 			/* other messageTypes can happen and should be ignored */
// 			break;
// 	}
// } 
