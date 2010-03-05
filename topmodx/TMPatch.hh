/*
*
* ***** BEGIN GPL LICENSE BLOCK *****
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software  Foundation,
* Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*
* The Original Code is Copyright (C) 2005 by xxxxxxxxxxxxxx
* All rights reserved.
*
* The Original Code is: all of this file.
*
* Contributor(s): none yet.
*
* ***** END GPL LICENSE BLOCK *****
*/

#ifndef _TM_PATCH_HH_
#define _TM_PATCH_HH_

/* Class for a TopMod Bezier Patch */

#include <DLFLCore.hh>
#include <DLFLCoreExt.hh>
#include <Light.hh>
#include <map>

using namespace DLFL;

class TMPatch;
typedef vector<TMPatch> TMPatchArray;
typedef TMPatch* TMPatchPtr;
typedef vector<TMPatchPtr> TMPatchPtrArray;

// This stuff is to map the patches to the face vertices by index
struct compare { 
  bool operator()( DLFLFaceVertexPtr a, DLFLFaceVertexPtr b ) const { 
		return ( a->getID() < b->getID() );
	}
};
  
typedef	std::map<DLFLFaceVertexPtr, TMPatchPtr, compare> TMPatchMap;

static void setPatchPtr( TMPatchMap &map, TMPatchPtr p, DLFLFaceVertexPtr fvp ) {
  map[fvp] = p;
};

static TMPatchPtr getPatchPtr( TMPatchMap &map, DLFLFaceVertexPtr fvp )  {
  std::map<DLFLFaceVertexPtr, TMPatchPtr, compare>::iterator it;
  it = map.find(fvp);
  return (*it).second;
};

static bool destroyPatchMap( TMPatchMap &map ) { 
 map.clear(); 
 return map.empty();
};

class TMPatch {
protected :

  int patchsize;			   // Size of the control grid
  Vector3dGrid ctrlpts;		 // The grid of control points
  Vector3dGrid ctrlptnormals;      // Normal for each control point
  Vector4dGrid ctrlptcolors; // Color for each control point (RGBA)
  GLdouble * glctrlpts;		  // Control points for OpenGL
  GLdouble * glctrlptcolors;     // Control point colors for OpenGL

  void allocateGLArray() {
    // Allocate memory for the GLdouble array to be sent to OpenGL
    // Uses patchsize to determine size of grid
    // Releases existing memory
    delete [] glctrlpts; glctrlpts = NULL;
    delete [] glctrlptcolors; glctrlptcolors = NULL;
    if ( patchsize > 0 ) {
			glctrlpts = new GLdouble[patchsize*patchsize*3];
			glctrlptcolors = new GLdouble[patchsize*patchsize*4];
		}
  }

  void populateGLArray()
  {
    // Copy values from the Vector3dGrid to the GLdouble array
    // Assumes that the GLdouble array has enough space
    // Assumes that the Vector3dGrid is a square grid of size patchsize
    if ( glctrlpts != NULL ) {
			// The order of traversal may have to be changed
			int index, colorindex;
			for (int i=0; i < patchsize; ++i)
				for (int j=0; j < patchsize; ++j) {
					index = (i*patchsize+j)*3;
					glctrlpts[index+0] = ctrlpts[j][i][0]; 
					glctrlpts[index+1] = ctrlpts[j][i][1]; 
					glctrlpts[index+2] = ctrlpts[j][i][2]; 
					
					colorindex = (i*patchsize+j)*4;
					glctrlptcolors[colorindex+0] = ctrlptcolors[j][i][0];
					glctrlptcolors[colorindex+1] = ctrlptcolors[j][i][1];
					glctrlptcolors[colorindex+2] = ctrlptcolors[j][i][2];
					glctrlptcolors[colorindex+3] = ctrlptcolors[j][i][3];
				}
		}
  }
     
public :

  // Default constructor
  TMPatch()
    : patchsize(0), ctrlpts(), ctrlptnormals(), ctrlptcolors(), glctrlpts(NULL), glctrlptcolors(NULL)
  {
    resizePatch(4);
  }

  // Copy constructor
  TMPatch(const TMPatch& patch)
    : patchsize(patch.patchsize), ctrlpts(patch.ctrlpts), ctrlptnormals(patch.ctrlptnormals),
      ctrlptcolors(patch.ctrlptcolors),
      glctrlpts(NULL), glctrlptcolors(NULL)
  {
    allocateGLArray();
    populateGLArray();
  }

  // Destructor
  ~TMPatch()
  {
    delete [] glctrlpts;
    delete [] glctrlptcolors;
  }
     
  // Assignment operator
  TMPatch& operator = (const TMPatch& patch)
  {
    patchsize = patch.patchsize;
    ctrlpts = patch.ctrlpts; ctrlptnormals = patch.ctrlptnormals;
    ctrlptcolors = patch.ctrlptcolors;
    allocateGLArray(); populateGLArray();
    return (*this);
  }

public :

  // Resize the patch of control points
  void resizePatch(int size) {
    if ( size != patchsize ) {
			patchsize = size;
			ctrlpts.resize(patchsize); ctrlptnormals.resize(patchsize); ctrlptcolors.resize(patchsize);
			for (int i=0; i < ctrlpts.size(); ++i) {
					ctrlpts[i].resize(patchsize);
					ctrlptnormals[i].resize(patchsize);
					ctrlptcolors[i].resize(patchsize);
			}
			allocateGLArray(); // Has to be reallocated if size changes
		}
  }

  const Vector3d& getControlPoint(int i, int j) const
  {
    // Return the control point at the specified location
    // Does not check for index out of bounds errors
    // Flips the i and j since it is flipped when control points are stored (see calculatePatchPoints method)
    return ctrlpts[j][i];
  }

  void setControlPoint(int i, int j, const Vector3d& p)
  {
    // Set the control point at the specified location
    // Does not check for index out of bounds errors
    // Flips the i and j since it is flipped when control points are stored (see calculatePatchPoints method)
    ctrlpts[j][i] = p;
  }
         
  // Calculate the points of the patch given the base grid points and base grid normals
  void calculatePatchPoints(const Vector3d (&cp)[4][4], const Vector3d (&cn)[4][4])
  {
    // Simply assign the points from the control grid
    // Base grid normals are ignored for now

    // resize the patch to be 4x4
    resizePatch(4);
    int index;
    for (int i=0; i < patchsize; ++i)
      for (int j=0; j < patchsize; ++j)
	{
	  ctrlpts[j][i] = cp[i][j];
	  ctrlptnormals[j][i] = cn[i][j];
	  index = (i*patchsize+j)*3;
	  glctrlpts[index+0] = ctrlpts[j][i][0]; 
	  glctrlpts[index+1] = ctrlpts[j][i][1]; 
	  glctrlpts[index+2] = ctrlpts[j][i][2]; 
	}
  }

  void updateGLPointArray(void)
  {
		if( glctrlpts == NULL )
			return;
    int index;
    for (int i=0; i < patchsize; ++i)
      for (int j=0; j < patchsize; ++j) {
				index = (i*patchsize+j)*3;
				glctrlpts[index+0] = ctrlpts[j][i][0]; 
				glctrlpts[index+1] = ctrlpts[j][i][1]; 
				glctrlpts[index+2] = ctrlpts[j][i][2]; 
			}
  }
     
  void computeLighting(const RGBColor& basecolor, double Ka, double Kd, double Ks, LightPtr lightptr)
  {
    // Calculate lighting at each control point and update the color
    int colorindex;
    RGBColor color;
    for (int i=0; i < patchsize; ++i)
      for (int j=0; j < patchsize; ++j)
	{
	  color = Kd * lightptr->illuminate(ctrlpts[j][i],ctrlptnormals[j][i]);
	  color += (1.0 - Kd) * basecolor;
	  colorindex = (i*patchsize+j)*4;
	  glctrlptcolors[colorindex+0] = color.r;
	  glctrlptcolors[colorindex+1] = color.g;
	  glctrlptcolors[colorindex+2] = color.b;
	  glctrlptcolors[colorindex+3] = 1.0;
	}
  }

  // Calculate the points of the patch given 4 corners and normals at the 4 corners
  // Points are assumed to be in counter-clockwise direction w.r.t. the patch
  void calculatePatchPoints(const Vector3d& p1, const Vector3d& p2, const Vector3d& p3, const Vector3d& p4,
			    const Vector3d& n1, const Vector3d& n2, const Vector3d& n3, const Vector3d& n4,
			    double w1, double w2, double w3, double w4)
  {
    int maxindex = patchsize-1;
    double u,v;

    //       cout << p1 << " " << p2 << " " << p3 << " " << p4 << endl;
    //       cout << norm(n1) << " " << norm(n2) << " " << norm(n3) << " " << norm(n4) << endl;
       
    ctrlpts[0][0] = p1;
    ctrlpts[0][maxindex] = p2;
    ctrlpts[maxindex][maxindex] = p3;
    ctrlpts[maxindex][0] = p4;
	 
    for (int i=0; i < patchsize; ++i)
      {
	for (int j=0; j < patchsize; ++j)
	  {
	    // Exclude corner points
	    if ( !((i == 0 && j == 0) ||
		   (i == 0 && j == maxindex) ||
		   (i == maxindex && j == maxindex) ||
		   (i == maxindex && j == 0)) )
	      {
		u = double(j) / double(maxindex); v = double(i) / double(maxindex);
		      
		ctrlpts[i][j] = (1.0-v) * ((1.0-u)*ctrlpts[0][0] + u*ctrlpts[0][maxindex]) +
		  v * ((1.0-u)*ctrlpts[maxindex][0] + u*ctrlpts[maxindex][maxindex]);
		      
		// Adjust the points for curvature
		if ( i < patchsize/2 )
		  {
		    if ( j < patchsize/2 )
		      {
			ctrlpts[i][j] -= ((ctrlpts[i][j] - ctrlpts[0][0])*n1)*n1;
		      }
		    else
		      {
			ctrlpts[i][j] -= ((ctrlpts[i][j] - ctrlpts[0][maxindex])*n2)*n2;
		      }
		  }
		else
		  {
		    if ( j < patchsize/2 )
		      {
			ctrlpts[i][j] -= ((ctrlpts[i][j] - ctrlpts[maxindex][0])*n4)*n4;
		      }
		    else
		      {
			ctrlpts[i][j] -= ((ctrlpts[i][j] - ctrlpts[maxindex][maxindex])*n3)*n3;
		      }
		  }
	      }
	  }
      }
    populateGLArray();
  }

  // Render this patch using filled polygons
  void render(void)
  {
    glMap2d(GL_MAP2_VERTEX_3,0,1,3,patchsize,0,1,patchsize*3,patchsize,glctrlpts);
    glMap2d(GL_MAP2_COLOR_4,0,1,4,patchsize,0,1,patchsize*4,patchsize,glctrlptcolors);
    glEnable(GL_MAP2_VERTEX_3);
    glEnable(GL_MAP2_COLOR_4);
    glMapGrid2d(12,0.0,1.0,12,0.0,1.0);
    glEvalMesh2(GL_FILL,0,12,0,12);
    glDisable(GL_MAP2_COLOR_4);
    glDisable(GL_MAP2_VERTEX_3);
  }

  // Render this patch using outlined polygons
  void outline(void)
  {
    glMap2d(GL_MAP2_VERTEX_3,0,1,3,patchsize,0,1,patchsize*3,patchsize,glctrlpts);
    glEnable(GL_MAP2_VERTEX_3);
    glMapGrid2d(6,0.0,1.0,6,0.0,1.0);
    glEvalMesh2(GL_LINE,0,6,0,6);
    glDisable(GL_MAP2_VERTEX_3);
  }

  // Render the patch boundary
  void patch_boundary(void)
  {
    glMap1d(GL_MAP1_VERTEX_3,0,1,3,patchsize,glctrlpts);
    glEnable(GL_MAP1_VERTEX_3);
    glMapGrid1d(12,0.0,1.0);
    glEvalMesh1(GL_LINE,0,12);
    glDisable(GL_MAP1_VERTEX_3);

    glMap1d(GL_MAP1_VERTEX_3,0,1,patchsize*3,patchsize,glctrlpts+patchsize*3-3);
    glEnable(GL_MAP1_VERTEX_3);
    glMapGrid1d(12,0.0,1.0);
    glEvalMesh1(GL_LINE,0,12);
    glDisable(GL_MAP1_VERTEX_3);
  }

  // Render the patch boundary
  void face_boundary(void)
  {
    glMap1d(GL_MAP1_VERTEX_3,0,1,3,patchsize,glctrlpts);
    glEnable(GL_MAP1_VERTEX_3);
    glMapGrid1d(12,0.0,1.0);
    glEvalMesh1(GL_LINE,0,12);
    glDisable(GL_MAP1_VERTEX_3);
  }

  // Render the control patch as a grid of lines
  void controlgrid(void)
  {
    for (int i=0; i < patchsize; ++i)
      {
	glBegin(GL_LINE_STRIP);
	for (int j=0; j < patchsize; ++j)
	  glVertex3dv( (ctrlpts[i][j]).getCArray() );
	glEnd();

	glBegin(GL_LINE_STRIP);
	for (int j=0; j < patchsize; ++j) 
	  glVertex3dv( (ctrlpts[j][i]).getCArray() );
	glEnd();
      }
  }

  // Show the control points alone
  void controlpoints(void)
  {
    glBegin(GL_POINTS);
    for (int i=0; i < patchsize; ++i)
      for (int j=0; j < patchsize; ++j)
	glVertex3dv( (ctrlpts[i][j]).getCArray() );
    glEnd();
  }

  void renderNormals(void)
  {
    glBegin(GL_LINES);
    for (int i=0; i < patchsize; ++i)
      for (int j=0; j < patchsize; ++j) {
	  glVertex3dv( (ctrlpts[i][j]).getCArray() );
	  Vector3d vec = ctrlpts[i][j] + 0.25*ctrlptnormals[i][j];
	  glVertex3dv( vec.getCArray() );
	}
  }
  /* stuart - bezier export */
  void printControlPoints( ostream &stream ) {
    for( int i = 0; i < patchsize; i++ ) {
      for( int j = 0; j < patchsize; j++ ) {
	stream << "v "
	       << ctrlpts[i][j][0] << " "
	       << ctrlpts[i][j][1] << " "
	       << ctrlpts[i][j][2] 
	       << std::endl;
      }
    }
  }
};

#endif /* #ifndef _TM_PATCH_HH_ */

