#include "GeometryRenderer.hh"
#include "DLFLRenderer.hh"

/*!
\ingroup gui
@{

	\class GeometryRenderer
		\brief functions for rendering edges, faces, vertices of DLFL Objects

		\note 

		\see GeometryRenderer
*/

void GeometryRenderer::render( DLFLObjectPtr obj ) const {
	// std::cout << "usegpu  = " << useGPU << "\n";
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
	if (useGPU){
		double Ka = df->material()->Ka;
		double Kd = df->material()->Kd;
		double Ks = df->material()->Ks;
		RGBColor basecolor = df->material()->color;

		cgSetParameter3f(CgData::instance()->basecolor, basecolor.r, basecolor.g, basecolor.b);
		cgSetParameter3f(CgData::instance()->Ka, Ka, Ka, Ka);
	  cgSetParameter3f(CgData::instance()->Kd, Kd, Kd, Kd);
	  cgSetParameter3f(CgData::instance()->Ks, Ks, Ks, Ks);
	  cgSetParameter1f(CgData::instance()->shininess, 50);
	}
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
}

void GeometryRenderer::renderFaceVertex( DLFLFaceVertexPtr dfv, bool useAttrs ) const {
	if( dfv->vertex ) {
		if( useAttrs ) {
			if( useNormal ) { glNormal3dv( (dfv->normal).getCArray() ); }
			if( useLighting && useMaterial ) {
				// Lighting and solid color material
				RGBColor rgb(renderColor[0], renderColor[1], renderColor[2] );
				// RGBColor rgb( 0.1, 0.1, 0.1 );
				if (!useGPU){
					rgb = product(rgb, dfv->color); // this multiplies corresponding elements together (e.g. RGBColor(r1*r2, g1*g2, b1*b2)  ) 
				}
				glColor4f( rgb.r, rgb.g, rgb.b, renderColor[3]);
			} else {
				if( useLighting ) { glColor( dfv->color ); } // Actually means use lighting
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
		glVertex3dv( (vp->coords).getCArray() );
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
}

// if( drawFaceCentroids || drawFaceNormals )
// 
// if( drawFaceCentroids ) {
// }
// if( drawFaceNormals ) {
// 	// std::cout << "hey this is a normal! \n";
// 	glPushAttrib( GL_CURRENT_BIT );
// 	glLineWidth(3.0);
// 	glColor4f(1.0,0.0,0.0,1.0);
// 	glBegin( GL_LINES ); {
// 		Vector3d norm = df->centroid + (df->getAuxNormal() * 0.2);
// 		glVertex3dv( (df->centroid).getCArray() );
// 		glVertex3dv( norm.getCArray() );
// 	} glEnd( );
// 	glPopAttrib( );
// } 
// 
void GeometryRenderer::renderFaceCentroids( DLFLObjectPtr obj, double size ) const {
	DLFLFacePtrList::iterator it;
	glPushMatrix(); {
		obj->transform();
		glPointSize( size );
		glBegin(GL_POINTS);
		for( it = obj->beginFace(); it != obj->endFace(); it++ ) {
			(*it)->updateCentroid( );
			// renderVertex( *it );
			glVertex3dv( (*it)->centroid.getCArray() ); 
		}
		glEnd();
		glPointSize(1.0);
	}	glPopMatrix();
}

void GeometryRenderer::renderFaceNormals( DLFLObjectPtr obj, double width, double length ) const {
	DLFLFacePtrList::iterator it;
	glPushMatrix(); {
		obj->transform();
		glLineWidth( width );
		glBegin(GL_LINES);
		for( it = obj->beginFace(); it != obj->endFace(); it++ ) {
			// renderEdge( *it );
			(*it)->updateCentroid( );
			// (*it)->updateNormal();
			Vector3d norm = (*it)->centroid + ((*it)->getNormal() * length);
			glVertex3dv( (*it)->centroid.getCArray() );
			glVertex3dv( norm.getCArray() );			
		}
		glEnd();
		glLineWidth(1.0);
	}	glPopMatrix();
	// glPushAttrib( GL_CURRENT_BIT );
	// glLineWidth(3.0);
	// glColor4f(1.0,0.0,0.0,1.0);
	// glBegin( GL_LINES ); {
	// 	Vector3d norm = df->centroid + (df->getAuxNormal() * 0.2);
	// 	glVertex3dv( (df->centroid).getCArray() );
	// 	glVertex3dv( norm.getCArray() );
	// } glEnd( );
	// glPopAttrib( );
}

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

GeometryRenderer* GeometryRenderer::instance( ) {
	// ensures only 1 instance (singleton)
	if ( !mInstance )
		mInstance = new GeometryRenderer;
	return mInstance;
}
