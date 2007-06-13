#include "GeometryRenderer.hh"
#include "DLFLRenderer.hh"

void GeometryRenderer::render( DLFLObjectPtr obj ) const {
  DLFLMaterialPtrList::iterator mp_it;
  DLFLFacePtrList::iterator fp_it;
  glPushMatrix( ); {
    obj->transform( );
    for( mp_it = obj->beginMaterial(); mp_it != obj->endMaterial(); mp_it++ ) {
      DLFLMaterialPtr mat = *mp_it;
      for( fp_it = mat->faces.begin(); fp_it != mat->faces.end(); fp_it++ ) {
	renderFace( *fp_it );
      }
    }
  } glPopMatrix( );
}

void GeometryRenderer::renderFace( DLFLFacePtr df, bool useAttrs ) const {
  if( useAttrs ) {
    glBeginFace( df->size(), useOutline );
  } else {
    glBeginFace( df->size(), false );
  }
  DLFLFaceVertexPtr head, curr;
  curr = head = df->front();
  renderFaceVertex( curr, useAttrs );
  curr = curr->next();
  while( curr != head ) {
    renderFaceVertex( curr, useAttrs );
    curr = curr->next();
  }
  glEnd( );

  if( drawFaceCentroid || drawFaceNormals )
    df->updateCentroid( );

  if( drawFaceCentroid ) {
    // Draw a dot in the center of the face
    glPushAttrib( GL_CURRENT_BIT );
    glColor3d(1.0,0.0,0.0);
    glBegin( GL_POINT ); {
      glVertex3dv( df->centroid.getCArray() );
    } glEnd( );
    glPopAttrib( );
  }

  if( drawFaceNormals ) {
    glPushAttrib( GL_CURRENT_BIT );
    glColor3d(1.0,0.0,0.0);
    glBegin( GL_LINES ); {
      Vector3d norm = df->centroid + (df->getAuxNormal() * 0.2);
      glVertex3dv( df->centroid.getCArray() );
      glVertex3dv( norm.getCArray() );
    } glEnd( );
    glPopAttrib( );
  } 
}
 
void GeometryRenderer::renderFaceVertex( DLFLFaceVertexPtr dfv, bool useAttrs ) const {
  if( dfv->vertex ) {
    if( useAttrs ) {
      if( useNormal ) { glNormal3dv( dfv->normal.getCArray() ); }
      if( useColor && useMaterial ) {
	// Lighting and solid color material
	RGBColor rgb(renderColor[0], renderColor[1], renderColor[2] );
	rgb = product(rgb, dfv->color);
	glColor4f( rgb.r, rgb.g, rgb. b, renderColor[3]);
      } else {
	if( useColor ) { glColor( dfv->color ); } // Actually means use lighting
	if( useMaterial ) { glColor4dv(renderColor); }
      }
      if( useTexture ) { 
	if( isReversed )
	  glTexCoord2d(1.0-dfv->texcoord[0],1.0-dfv->texcoord[1]);
	else
	  glTexCoord2d(1.0-dfv->texcoord[0],1.0-dfv->texcoord[1]);
      }
    }
    DLFLVertexPtr vp = dfv->vertex;
    glVertex3dv( vp->coords.getCArray() );
  }
}

void GeometryRenderer::renderEdge( DLFLEdgePtr dep ) const {
  DLFLFaceVertexPtr fvptr1, fvptr2;
  dep->getFaceVertexPointers( fvptr1, fvptr2 );
  glVertex3dv((fvptr1->vertex->coords).getCArray());
  glVertex3dv((fvptr2->vertex->coords).getCArray());
}

void GeometryRenderer::renderVertex( DLFLVertexPtr dvp ) const { 
  glBegin(GL_POINTS); { 
    glVertex3dv( dvp->coords.getCArray() ); 
  } glEnd();
}

void GeometryRenderer::renderVertices( DLFLObjectPtr obj, double size ) const {
  DLFLEdgePtrList::iterator it;
  // Just render all the vertices with specified point size
  glPushMatrix(); {
    obj->transform();
    glPointSize( size );
    glBegin(GL_POINTS);
    for( it = obj->beginEdge(); it != obj->endEdge(); it++ ) {
      renderEdge( *it );
    }
    glEnd();
    glPointSize(1.0);
  } glPopMatrix();
}

void GeometryRenderer::renderEdges( DLFLObjectPtr obj, double width ) const {
  DLFLEdgePtrList::iterator it;
  // Just render all the edges with specified line width
  glPushMatrix(); {
    obj->transform();
    glLineWidth( width );
    glBegin(GL_LINES);
    for( it = obj->beginEdge(); it != obj->endEdge(); it++ ) {
      renderEdge( *it );
    }
    glEnd();
    glLineWidth(1.0);
    glPopMatrix();
  }
};

// Call glBegin with the appropriate macro depending on no of vertices
// This is for filled polygons.
void GeometryRenderer::glBeginFace( int num, bool outline ) {
  if( outline ) {
    switch ( num ) {
    case 0 :
      return;
    case 1 :
      glBegin(GL_POINTS); break;
    case 2 :
      glBegin(GL_LINES); break;
    default :
      glBegin(GL_LINE_LOOP);
    }
  } else {
    switch ( num ) {
    case 0 :
      return;
    case 1 :
      glBegin(GL_POINTS); break;
    case 2 :
      glBegin(GL_LINES); break;
    case 3 :
      glBegin(GL_TRIANGLES); break;
    case 4 :
      glBegin(GL_QUADS); break;
    default :
      glBegin(GL_POLYGON);
    }
  }
}

// Singleton Stuff
GeometryRenderer* GeometryRenderer::mInstance = 0;
/*GeometryRenderer& GeometryRenderer::instance( ) {
  // ensures only 1 instance (singleton)
  if ( !mInstance )
    mInstance = new GeometryRenderer;
  return *mInstance;
  }*/

GeometryRenderer* GeometryRenderer::instance( ) {
  // ensures only 1 instance (singleton)
  if ( !mInstance )
    mInstance = new GeometryRenderer;
  return mInstance;
}
