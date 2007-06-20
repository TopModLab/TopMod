#ifndef _GEOMETRY_RENDERER_H_
#define _GEOMETRY_RENDERER_H_

#include <DLFLObject.hh>

using namespace DLFL;

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

  bool useMaterial;
  bool useColor;
  bool useNormal;
  bool useTexture;
  bool useOutline;
  bool drawFaceCentroid;
  bool drawFaceNormals;
  bool isReversed;
	bool useGPU;

  bool drawPatchWireframe;
  bool drawPatchBoundaries;
  bool drawCVs;
  bool drawPatchNormals;

  GLdouble* renderColor;

private :
  //DLFLObjectPtr mObj;

  static GeometryRenderer *mInstance;
  GeometryRenderer( bool gpu = true) : useMaterial(false), useColor(false), 
			useNormal(false), useTexture(false), 
			useOutline(false), drawFaceCentroid(false),
			drawFaceNormals(false), isReversed(false), useGPU(gpu) { 
    renderColor = new GLdouble[4];
  };
};

#endif // _GEOMETRY_RENDERER_H_
