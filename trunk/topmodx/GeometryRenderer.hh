#ifndef _GEOMETRY_RENDERER_H_
#define _GEOMETRY_RENDERER_H_

#include <DLFLObject.hh>
#include "CgData.hh"


#ifdef GPU_OK
using namespace Cg;
#endif // GPU_OK

using namespace DLFL;

/*!
	\file GeometryRenderer.hh
	\brief Definition of the GeometryRenderer class
	
	\see GeometryRenderer
*/

class GeometryRenderer {
public :
  /** Singleton Methods **/
  //static GeometryRenderer& instance( );
  static GeometryRenderer* instance( );
  ~GeometryRenderer( ) { delete renderColor; renderColor = 0; };

  //void setObject( DLFLObjectPtr obj ) { mObj = obj; };
  static void glBeginFace( int num, bool outline = false );

  void render( DLFLObjectPtr obj ) const;
  void renderFace( DLFLFacePtr dfp, bool useAttrs = true ) const;
  void renderFaceVertex( DLFLFaceVertexPtr dfvp, bool useAttrs = true ) const;
  void renderEdge( DLFLEdgePtr dep ) const;
  void renderVertex( DLFLVertexPtr dvp ) const;

  void renderVertices( DLFLObjectPtr obj, double size = 5.0 ) const;
  void renderEdges( DLFLObjectPtr obj, double width = 1.0 ) const;
	void renderFaceNormals( DLFLObjectPtr obj, double width, double length ) const;
	void renderFaceCentroids( DLFLObjectPtr obj, double size ) const;


  bool useMaterial;
  bool useLighting;
  bool useNormal;
  bool useTexture;
  bool useOutline;
	bool drawWireframe;
	bool drawSilhouette;
	bool drawVertices;
  bool drawFaceCentroids;
  bool drawFaceNormals;
  bool isReversed;
  bool useGPU;
	bool antialiasing;

  bool drawPatchWireframe;
  bool drawPatchBoundaries;
  bool drawCVs;
  bool drawPatchNormals;

  GLdouble* renderColor;

private :
  //DLFLObjectPtr mObj;

  static GeometryRenderer *mInstance;
  GeometryRenderer( bool gpu = false) : useMaterial(false), useLighting(false), 
				       useNormal(false), useTexture(false), 
							useOutline(false), drawFaceCentroids(false), drawVertices(false),
							drawSilhouette(false),drawWireframe(true),
				       drawFaceNormals(false), isReversed(false), useGPU(gpu) {
    renderColor = new GLdouble[4];
  };
};

#endif // _GEOMETRY_RENDERER_H_
