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
*
* Short description of this file
*
* name of .hh file containing function prototypes
*
*/


/*
  Crust modeling routines for DLFL
*/
#include "DLFLCrust.hh"

#include <DLFLCore.hh>
#include <DLFLCoreExt.hh>
#include "DLFLConvexHull.hh"
#include "DLFLConnect.hh"
#include "DLFLSubdiv.hh"

namespace DLFL {

  /*
    Create a crust for this object.
    Creates an inner (outer if thickness is negative) surface
    duplicating the existing surface and moving every vertex along
    the average normal at the vertex for the given distance.
    Boolean flag indicates if crust should be of uniform thickness
    which means thickness at vertices will be adjusted to account
    for normal averaging.
  */

  void createCrust(DLFLObjectPtr obj, double thickness, bool uniform) {
    if ( !isNonZero(thickness) ) return;

    // Clear the arrays used to store crust modeling information
    crustfp1.clear(); crustfp2.clear();

    // Resize the arrays to appropriate size
    crust_num_old_faces = obj->num_faces();
    crustfp1.resize(crust_num_old_faces,NULL);
    crustfp2.resize(crust_num_old_faces,NULL);
  
    StringStream dlflstream; // Stream used to duplicate the object
    StringStream mtlstream; // Stream used to duplicate the object

    // Write the object in DLFL format with the faces reversed
    obj->writeDLFL(dlflstream,mtlstream, true);

    int num_old_verts = obj->num_vertices();

    // Read back from the stream and append to existing object
    obj->readDLFL(dlflstream,mtlstream, false);

    // Fill the arrays storing information for crust modeling
    // Since we are traversing the faces, also compute and store
    // the normals at corners of each face for use later
    DLFLFacePtrList::iterator fl_first, fl_last;
    DLFLFacePtr fp;
    int num_faces = 0;
    fl_first = obj->beginFace(); fl_last = obj->endFace();
    while ( num_faces < crust_num_old_faces ) {
      fp = *fl_first;
      crustfp1[num_faces] = fp; fp->makeUnique(); fp->storeNormals();
      ++fl_first; ++num_faces;
    }
    num_faces = 0; 
    while ( fl_first != fl_last ) {
      fp = *fl_first;
      crustfp2[num_faces] = fp; fp->makeUnique(); fp->storeNormals();
      ++fl_first; ++num_faces;
    }

    // If thickness is negative move the old vertices outward
    // Otherwise move the new vertices inward
    DLFLVertexPtrList::iterator vl_first, vl_last;
    DLFLVertexPtr vp;
    Vector3dArray normals;
    Vector3d avenormal, newpos;
    double mod_thickness; // Modified thickness for uniform thickness shells
    int num_verts = 0;
		//negative thickness
    if ( thickness < 0.0 ) {
      vl_first = obj->beginVertex();
      while ( num_verts < num_old_verts ) {
				vp = (*vl_first); ++vl_first; ++num_verts;

				// If uniform thickness is required, adjust thickness appropriately
				if ( uniform ) {
				  avenormal = vp->getNormals(normals);
				  mod_thickness = 0.0;
				  for (int i=0; i < normals.size(); ++i)
				    mod_thickness += thickness/(avenormal*normals[i]);
				  mod_thickness /= normals.size();
				} else {
				  avenormal = vp->averageNormal();
				  mod_thickness = thickness;
				}
				// Negative sign because thickness is negative and normal is outward
				// Use the modified thickness
				newpos = vp->coords - mod_thickness*avenormal;
				vp->coords = newpos;
      }
    }
		//thickness > 0
		else {
      vl_first = obj->beginVertex(); vl_last = obj->endVertex();
      advance(vl_first,num_old_verts);
      while ( vl_first != vl_last ) {
				vp = (*vl_first); ++vl_first;
				// If uniform thickness is required, adjust thickness appropriately
				if ( uniform ) {
				  avenormal = vp->getNormals(normals);
				  mod_thickness = 0.0;
				  for (int i=0; i < normals.size(); ++i)
				    mod_thickness += thickness/(avenormal*normals[i]);
				  mod_thickness /= normals.size();
				}
				else {
				  avenormal = vp->averageNormal();
				  mod_thickness = thickness;
				}
				// Positive sign because thickness is positive and normal is inward
				// Use the modified thickness
				newpos = vp->coords + mod_thickness*avenormal; 
				vp->coords = newpos;
      }
    }
    // Find and store the min. id for the face list
    crust_min_face_id = (obj->firstFace())->getID();
  }

  /*
    Create a crust for this object.
    Creates an inner (outer if scale_factor is negative) surface
    duplicating the existing surface and scaling the inner or outer surface
    w.r.t centroid of object.
  */

  void createCrustWithScaling(DLFLObjectPtr obj, double scale_factor) {
    if ( !isNonZero(scale_factor) ) return;
  
    // Clear the arrays used to store crust modeling information
    crustfp1.clear(); crustfp2.clear();

    // Resize the arrays to appropriate size
    crust_num_old_faces = obj->num_faces();
    crustfp1.resize(crust_num_old_faces,NULL);
    crustfp2.resize(crust_num_old_faces,NULL);
  
    StringStream dlflstream; // Stream used to duplicate the object
    StringStream mtlstream; // Stream used to duplicate the object

    // Write the object in DLFL format with the faces reversed
    obj->writeDLFL(dlflstream, mtlstream, true);

    int num_old_verts = 0;
    Vector3d objcen;
    // We need to find the centroid of the object
    // We can find num of vertices at the same time
    DLFLVertexPtrList::iterator first, last;
    first = obj->beginVertex(); last = obj->endVertex();
    objcen.reset(); num_old_verts = 0;
    while ( first != last ) {
      objcen += (*first)->coords; ++first; ++num_old_verts;
    }
    objcen /= num_old_verts;

    // Read back from the stream and append to existing object
    obj->readDLFL(dlflstream, mtlstream, false);

    // Fill the arrays storing information for crust modeling
    DLFLFacePtrList::iterator fl_first, fl_last;
    DLFLFacePtr fp;
    int num_faces = 0;
    fl_first = obj->beginFace(); fl_last = obj->endFace();
    while ( num_faces < crust_num_old_faces ) {
      fp = *fl_first;
      crustfp1[num_faces] = fp; fp->makeUnique();
      ++fl_first; ++num_faces;
    }
    num_faces = 0; 
    while ( fl_first != fl_last ) {
      fp = *fl_first;
      crustfp2[num_faces] = fp; fp->makeUnique();
      ++fl_first; ++num_faces;
    }

    // Clamp the scale factor to lie between -1 and 1. If negative use inverse of scale factor
    // to get scale factor > 1
    if ( scale_factor < -1.0 ) scale_factor = -1.0;
    else if ( scale_factor > 1.0 ) scale_factor = 1.0;
  
    // If scale_factor is negative scale the old vertices using scale_factor > 1
    // Otherwise scale the new vertices using scale_factor < 1
    DLFLVertexPtrList::iterator vl_first, vl_last;
    DLFLVertexPtr vp;
    Vector3d normal, newpos;
    int num_verts = 0;
    if ( scale_factor < 0.0 ) {
      scale_factor = -1.0/scale_factor;
      vl_first = obj->beginVertex();
      while ( num_verts < num_old_verts ) {
				vp = (*vl_first); ++vl_first; ++num_verts;
				newpos = (vp->coords - objcen)*scale_factor + objcen;
				vp->coords = newpos;
      }
    }
    else {
      vl_first = obj->beginVertex(); vl_last = obj->endVertex();
      advance(vl_first,num_old_verts);
      while ( vl_first != vl_last ) {
				vp = (*vl_first); ++vl_first;
				newpos = (vp->coords - objcen)*scale_factor + objcen;
				vp->coords = newpos;
      }
    }

    // Find and store the min. id for the face list
    crust_min_face_id = (obj->firstFace())->getID();
  }

  void cmMakeHole(DLFLObjectPtr obj, DLFLFacePtr fp, bool cleanup) {
    int index = fp->getID() - crust_min_face_id;
    DLFLFacePtr fp1,fp2;
    DLFLFaceVertexPtr fvp1,fvp2;
    DLFLEdgePtrArray eparray1, eparray2;
  
    if ( index > crust_num_old_faces ) index -= crust_num_old_faces;
    if ( index >= crustfp1.size() ) {
      cout << "Index out of range " << index << endl;
      return; // This refers to one of the newly created faces
    }
    fp1 = crustfp1[index]; fp2 = crustfp2[index];
    if ( fp1 != NULL && fp2 != NULL ) {
      fvp1 = fp1->firstVertex(); fvp2 = fp2->firstVertex();
      //fvp2 = fp2->findClosest(fvp1->vertex->coords);
      if ( cleanup ) {
				// Find edges in the 2 faces and store them in the arrays. These
				// will be used to cleanup the edges afterwards.
				fp1->getEdges(eparray1); fp2->getEdges(eparray2);
      }
      connectFaces(obj,fvp1,fvp2);
      crustfp1[index] = crustfp2[index] = NULL; // These face pointers are no longer valid

      // Do the cleanup if required
      if ( cleanup ) {
				// Traverse eparray1 in forward direction, eparray2 in reverse direction
				int num_edges = eparray1.size();
				DLFLEdgePtr ep1, ep2;
				DLFLFacePtr fp11,fp12,fp21,fp22;
				for (int i=0; i < num_edges; ++i) {
				  ep1 = eparray1[i]; ep2 = eparray2[num_edges-i-1];
				  ep1->getFacePointers(fp11,fp12); ep2->getFacePointers(fp21,fp22);
				  if ( ( (fp11 == fp21) && (fp12 == fp22) ) ||
				       ( (fp11 == fp22) && (fp12 == fp21) ) ) {
				    // Both edges are adjacent to the same 2 faces.
				    // We have an infinitesimally thin face which we want to remove
				    // We can use either of the 2 faces to do the deletion of the edges
				    DLFLEdgePtrArray eparray;
						
				    // Get all the edges in one of the faces
				    fp11->getEdges(eparray);

				    // Go through the edges obtained above and delete them
				    // Point-spheres will be cleaned up by default
				    for (int j=0; j < eparray.size(); ++j)
				      deleteEdge(obj,eparray[j]);
				  }
				}
			}
		}
		else cout << "NULL pointers found" << endl;
		
	}

  void createCrustForWireframe(DLFLObjectPtr obj, double thickness) {
	
		bool uniform = true;
		
    if ( !isNonZero(thickness) ) return;

    // Clear the arrays used to store crust modeling information
    crustfp1.clear(); crustfp2.clear();

    // Resize the arrays to appropriate size
    crust_num_old_faces = obj->num_faces();
    crustfp1.resize(crust_num_old_faces,NULL);
    crustfp2.resize(crust_num_old_faces,NULL);
  
    StringStream dlflstream; // Stream used to duplicate the object
    StringStream mtlstream; // Stream used to duplicate the object

    // Write the object in DLFL format with the faces reversed
    obj->writeDLFL(dlflstream,mtlstream, true);

    int num_old_verts = obj->num_vertices();

    // Read back from the stream and append to existing object
    obj->readDLFL(dlflstream, mtlstream, false);

    // Fill the arrays storing information for crust modeling
    // Since we are traversing the faces, also compute and store
    // the normals at corners of each face for use later
    DLFLFacePtrList::iterator fl_first, fl_last;
    DLFLFacePtr fp;
    int num_faces = 0;
    fl_first = obj->beginFace(); fl_last = obj->endFace();
    while ( num_faces < crust_num_old_faces ) {
      fp = *fl_first;
      crustfp1[num_faces] = fp; fp->makeUnique(); fp->storeNormals();
      ++fl_first; ++num_faces;
    }
    num_faces = 0; 
    while ( fl_first != fl_last ) {
      fp = *fl_first;
      crustfp2[num_faces] = fp; fp->makeUnique(); fp->storeNormals();
      ++fl_first; ++num_faces;
    }

    // If thickness is negative move the old vertices outward
    // Otherwise move the new vertices inward

    DLFLVertexPtrList::iterator vl_first, vl_first_old, vl_last;
    DLFLVertexPtr vp,vpNew;
    Vector3d normal, newpos;
	  Vector3dArray normals;
	  Vector3d avenormal;
	  double mod_thickness; // Modified thickness for uniform thickness shells
	  int num_verts = 0;

    if ( thickness < 0.0 ) {
	    vl_first = obj->beginVertex();
	    while ( num_verts < num_old_verts ) {
					vp = (*vl_first); ++vl_first; ++num_verts;

					// If uniform thickness is required, adjust thickness appropriately
					if ( uniform ) {
					  avenormal = vp->getNormals(normals);
					  mod_thickness = 0.0;
					  for (int i=0; i < normals.size(); ++i)
					    mod_thickness += thickness/(avenormal*normals[i]);
					  mod_thickness /= normals.size();
					} else {
					  avenormal = vp->averageNormal();
					  mod_thickness = thickness;
					}
					// Negative sign because thickness is negative and normal is outward
					// Use the modified thickness
					newpos = vp->coords - mod_thickness*avenormal;
					vp->coords = newpos;
	    }
    }
    if ( thickness > 0.0 ) {
      vl_first = obj->beginVertex(); vl_first_old= obj->beginVertex();
      for (int i = 0; i < num_old_verts; i++) 
				++vl_first_old;
				
				      while (  num_verts < num_old_verts) {
				vpNew = (*vl_first_old); vp = (*vl_first); ++vl_first; ++num_verts; ++vl_first_old;
				
				DLFLFaceVertexPtrList fvpList;
				DLFLFaceVertexPtr fvp1,fvp2,fvp0, fvp3, fvp4, fvptemp, fvptemp1, fvptemp2;
				DLFLFacePtr fphole, fp1, fp2, fp;
				DLFLEdgePtr ep1, ep0;
				Vector3d v0, v1, v2, v3, v4, n1, n2, n3 ,n4, ntemp;
				DLFLFaceVertexPtrList fvplist;
				fvplist=vp->getFaceVertexList();
				
				// get the face that has the current vertex as one of its vertices
				// and is marked for making a hole
				DLFLFaceVertexPtrList::iterator first = fvplist.begin(), last = fvplist.end();
				while ( first != last ) {
					fvptemp = (*first);
					fp = fvptemp->getFacePtr();		
				
				 	if ( fp->getType() == FTHole ) {
				   	fphole = fp; break;
				 	}
				 	++first;
				}
				fp = fphole;
				
				fvp0 = vp->getFaceVertexInFace(fp);
				fvp1 = fvp0->next();
				fvp2 = fvp0->prev();
				
				// get vertex pointers
				// vp1 = fvp1->getVertexPtr(); vp2 = fvp2->getVertexPtr();
				
				// Get the two edges originating from the present vertex
				ep0 = fvp0->getEdgePtr(); ep1 = fvp2->getEdgePtr(); 
				
				// For the edge starting at v0 to wards v1 
				ep0->getFacePointers(fp1,fp2);
				
				// get the face to NOT be holed
				if ( fp1->getType() == FTHole ) fp = fp2;
				else fp = fp1;
					    
				// get the three face vertices
				fvptemp = vp->getFaceVertexInFace(fp);
				fvptemp1 = fvptemp->next();
				
				// get the vertices of the other two vertices in the face
				//vptemp1 = fvptemp1->getVertexPtr();
				
				fvp3 = fvptemp1;
				
				// For the edge starting at v1 towards v0
				// Same as before after this
				ep1->getFacePointers(fp1, fp2);
				if ( fp1->getType() == FTHole ) fp = fp2;
				else fp = fp1;
				
				fvptemp = vp->getFaceVertexInFace(fp);
				fvptemp2 = fvptemp->prev();
				
				//vptemp2 = fvptemp2->getVertexPtr();
				fvp4 = fvptemp2;
				
				// Get the vertex coordinates of the five face vertices found
				v0 = fvp0->getVertexCoords();
				v1 = fvp1->getVertexCoords();
				v2 = fvp2->getVertexCoords();
				v3 = fvp3->getVertexCoords();
				v4 = fvp4->getVertexCoords();
						
				n1 = normalized( (v1-v0) % (v3-v0) );
				n2 = normalized( n1 % (v1-v0) );
				n3 = normalized( (v2-v0)% ( v4-v0) );
				n4 = normalized( (v2-v0) % n3 );
				normal = normalized(n2 % n4); // normal is the direction in which the new_pos will lie.
				
				newpos = vpNew->coords - thickness*normal;
				vpNew->coords = newpos;
				      }
				// vl_first = obj->beginVertex(); vl_last = obj->endVertex();
				// 		    advance(vl_first,num_old_verts);
				// 		    while ( vl_first != vl_last ) {
				// 		vp = (*vl_first); ++vl_first;
				// 		// If uniform thickness is required, adjust thickness appropriately
				// 		if ( uniform ) {
				// 		  avenormal = vp->getNormals(normals);
				// 		  mod_thickness = 0.0;
				// 		  for (int i=0; i < normals.size(); ++i)
				// 		    mod_thickness += thickness/(avenormal*normals[i]);
				// 		  mod_thickness /= normals.size();
				// 		}
				// 		else {
				// 		  avenormal = vp->averageNormal();
				// 		  mod_thickness = thickness;
				// 		}
				// 		// Positive sign because thickness is positive and normal is inward
				// 		// Use the modified thickness
				// 		newpos = vp->coords + mod_thickness*avenormal; 
				// 		vp->coords = newpos;
				// 		    }
		    
    }

    // Find and store the min. id for the face list
    crust_min_face_id = (obj->firstFace())->getID();
  }

  void createCrustForWireframe2(DLFLObjectPtr obj, double scale_factor) {
    if ( !isNonZero(scale_factor) ) return;

    // Clear the arrays used to store crust modeling information
    crustfp1.clear(); crustfp2.clear();

    // Resize the arrays to appropriate size
    crust_num_old_faces = obj->num_faces();
    crustfp1.resize(crust_num_old_faces,NULL);
    crustfp2.resize(crust_num_old_faces,NULL);
  
    StringStream dlflstream; // Stream used to duplicate the object
    StringStream mtlstream; // Stream used to duplicate the object

    // Write the object in DLFL format with the faces reversed
    obj->writeDLFL(dlflstream,mtlstream, true);

    int num_old_verts = obj->num_vertices();

    // Read back from the stream and append to existing object
    obj->readDLFL(dlflstream,mtlstream, false);

    // Fill the arrays storing information for crust modeling
    // Since we are traversing the faces, also compute and store
    // the normals at corners of each face for use later
    DLFLFacePtrList::iterator fl_first, fl_last;
    DLFLFacePtr fp;
    int num_faces = 0;
    fl_first = obj->beginFace(); fl_last = obj->endFace();
    while ( num_faces < crust_num_old_faces ) {
      fp = *fl_first;
      crustfp1[num_faces] = fp; fp->makeUnique(); fp->storeNormals();
      ++fl_first; ++num_faces;
    }
    num_faces = 0; 
    while ( fl_first != fl_last ) {
      fp = *fl_first;
      crustfp2[num_faces] = fp; fp->makeUnique(); fp->storeNormals();
      ++fl_first; ++num_faces;
    }

    // If scale_factor is negative move the old vertices outward
    // Otherwise move the new vertices inward

    DLFLVertexPtrList::iterator vl_first, vl_first_old;
    DLFLVertexPtr vp,vpNew;
    Vector3d normal, newpos;
    int num_verts = 0;

    if ( scale_factor < 0.0 ) {
      cout<<"ERROR: scale_factor should be positive"<<endl;
      return;
    }
    if ( scale_factor > 0.0 ) {
      vl_first = obj->beginVertex(); vl_first_old= obj->beginVertex();
      for (int i = 0; i < num_old_verts; i++) 
				++vl_first_old;

      while (  num_verts < num_old_verts) {
				vpNew = (*vl_first_old); vp = (*vl_first); ++vl_first; ++num_verts; ++vl_first_old;

				DLFLFaceVertexPtrList fvpList;
				DLFLFaceVertexPtr fvp1,fvp2,fvp0, fvp3, fvp4, fvptemp, fvptemp1, fvptemp2;
				DLFLFacePtr fphole, fp1, fp2, fp;
				DLFLEdgePtr ep1, ep0;
				Vector3d v0, v1, v2, v3, v4, n1, n2, n3 ,n4, ntemp;
				DLFLFaceVertexPtrList fvplist;
				fvplist=vp->getFaceVertexList();

				// get the face that has the current vertex as one of its vertices
				// and is marked for making a hole
				DLFLFaceVertexPtrList::iterator first = fvplist.begin(), last = fvplist.end();
				while ( first != last ) {
					fvptemp = (*first);
					fp = fvptemp->getFacePtr();		

				 	if ( fp->getType() == FTHole ) {
				   	fphole = fp; break;
				 	}
				 	++first;
				}
				fp = fphole;

				fvp0 = vp->getFaceVertexInFace(fp);
				fvp1 = fvp0->next();
				fvp2 = fvp0->prev();

				// get vertex pointers
				//vp1 = fvp1->getVertexPtr(); vp2 = fvp2->getVertexPtr();

				// Get the two edges originating from the present vertex
				ep0 = fvp0->getEdgePtr(); ep1 = fvp2->getEdgePtr(); 

				// For the edge starting at v0 to wards v1 
				ep0->getFacePointers(fp1,fp2);

				// get the face to NOT be holed
				if ( fp1->getType() == FTHole ) fp = fp2;
				else fp = fp1;
	    
				// get the three face vertices
				fvptemp = vp->getFaceVertexInFace(fp);
				fvptemp1 = fvptemp->next();

				// get the vertices of the other two vertices in the face
				//vptemp1 = fvptemp1->getVertexPtr();

				fvp3 = fvptemp1;

				// For the edge starting at v1 towards v0
				// Same as before after this
				ep1->getFacePointers(fp1, fp2);
				if ( fp1->getType() == FTHole ) fp = fp2;
				else fp = fp1;

				fvptemp = vp->getFaceVertexInFace(fp);
				fvptemp2 = fvptemp->prev();

				//vptemp2 = fvptemp2->getVertexPtr();
				fvp4 = fvptemp2;

				// Get the vertex coordinates of the five face vertices found
				v0 = fvp0->getVertexCoords();
				v1 = fvp1->getVertexCoords();
				v2 = fvp2->getVertexCoords();
				v3 = fvp3->getVertexCoords();
				v4 = fvp4->getVertexCoords();
		
				n1 = normalized( (v1-v0) % (v3-v0) );
				n2 = normalized( n1 % (v1-v0) );
				n3 = normalized( (v2-v0)% ( v4-v0) );
				n4 = normalized( (v2-v0) % n3 );
				normal = normalized(n2 % n4); // normal is the direction in which the new_pos will lie.

				//****by dave - experimental - find the centroid of the faces that contain the current vertex
				Vector3d vpcen; vpcen.reset();
		    // We need to find the centroid of the object
		    // We can find num of vertices at the same time
				DLFLEdgePtrArray eparr;
				vpNew->getEdges(eparr);
		    DLFLEdgePtrArray::iterator it;
				for (it = eparr.begin(); it != eparr.end(); it++){
					// vpcen += (*it)->getMidPoint(true);
					vpcen += (*it)->getOtherVertexPointer(vpNew)->coords;
				}
				vpcen /= eparr.size();

				newpos = (vpNew->coords - vpcen)*scale_factor*2 + vpcen;
				// newpos = vpNew->coords - scale*normal;
				vpNew->coords = newpos;
      }
    }

    // Find and store the min. id for the face list
    crust_min_face_id = (obj->firstFace())->getID();
  }

  void createWireframeWithSegments(DLFLObjectPtr obj, double thickness, int numSides) {
    DLFLFacePtrArray edge_connect_fparray, temp_face_array;
    DLFLEdgePtrArray edge_array, temp_edge_ptr_array;
    DLFLFaceVertexPtrArray fvparray;

    DLFLEdgePtrList::iterator el_first, el_last;
    DLFLFacePtrList::iterator fl_first, fl_last;
    DLFLVertexPtrList::iterator vl_first, vl_last, vl_tempLast;

    DLFLEdgePtr eptr = NULL,eptr_temp1,eptr_temp2, ep_temp;
    DLFLVertexPtr  vp,newvptr,temp_vptr,VListPtr, vp1, vp2;
    DLFLFacePtr tmpfptr1, fp1_temp, fp2_temp,tmpfptr2,valOneFace;
    DLFLFaceVertexPtr tmpfvptr, tmp_ch_fvp, dirFvp1,dirFvp2;

    Vector3d edgeVector, xyz, dir, edgeVec1, edgeVec2, axis,start_point,faceNormal, P0,P1,P2, normal;
    Vector3dArray vertlist, edge_connect_normals,temp_vertlist, temp_chface_vertlist;

    int count = 0,numFvps=0, edgeID, min_edge_id, index;
    float angle,cosT1,cosT2,T1,T2,L,L1,L2;
    int numVerts, num_original_edges;
    Vector3d zeroVec(0,0,0), Xaxis(1,0,0), Yaxis(0,1,0), vec,PonAxs,vec1, Ndir1, Ndir2,VListVec;
    double A[3], B[3],C[3], D[3];

    bool match,jointCreated,vertListSizeMatch,allFacesMatched;
    float dist1, dist, tolerence = 0.0001;
    StringStream dlflstream;
    StringStream mtlstream;

    obj->writeDLFL(dlflstream,mtlstream,false);

    // Read back from the stream and append to existing object
    obj->readDLFL(dlflstream,mtlstream, true);

    num_original_edges = obj->num_edges();
    edge_connect_normals.reserve(2*num_original_edges);
    edge_connect_fparray.reserve(2*num_original_edges);
    min_edge_id = (obj->firstEdge())->getID();

    //initialize to zeroVec
    for (int i = 0 ; i < 2*num_original_edges; i++) {
      edge_connect_normals.push_back(zeroVec);
      edge_connect_fparray.push_back(NULL);
    }

    el_first = obj->beginEdge(); el_last = obj->endEdge();
    numVerts = obj->num_vertices();
    vl_first = obj->beginVertex(); vl_last = obj->endVertex();
    int num =0;

    while ( num<numVerts ) {
      vp =(*vl_first);
    
      ++vl_first; num++;
      fvparray.clear();

      vp->getFaceVertices(fvparray);
      float length;
      if ( fvparray.size() != 1 ) {
	tmpfvptr = fvparray[0];
	numFvps = fvparray.size();
	fvparray.clear();
	fvparray.push_back(tmpfvptr);
	for (int i=1; i < numFvps; i++) {
	  tmpfvptr = tmpfvptr->vnext();
	  fvparray.push_back(tmpfvptr);
	}

	vp->getEdges(edge_array);
	length = -99999;	// the largest length for the vertex

	//FIND THE LARGEST DISTANCE FORM THE VERTEX FOR THE NEW FACE TO BE PUT if 
	for (int  i=0; i<fvparray.size(); i++) {  
	  eptr_temp1 = (fvparray[i])->getEdgePtr();
	  for (int  ii=0; ii<fvparray.size(); ii++) {
	    eptr_temp2 = (fvparray[ii])->getEdgePtr();
	    if (eptr_temp1->getID() != eptr_temp2->getID()) {	 
	      edgeVec1 = (eptr_temp1->getOtherVertexPointer(vp))->getCoords() - vp->getCoords();
	      edgeVec2 = (eptr_temp2->getOtherVertexPointer(vp))->getCoords() - vp->getCoords();
	      edgeVec1 =normalized(edgeVec1);
	      edgeVec2 =normalized(edgeVec2);
	      cosT1 = edgeVec1*edgeVec2; //axis;
	      T1 = acos(cosT1);
	      L= thickness/tan(T1/2);
	      if (L > length)  length = L + fabs(cosT1*thickness)+ 0.001*thickness; 
	    }//if
	  }//for
	}//for largest distance
	
      }// valence 1
      else length = 0;

      for (int i=0; i < fvparray.size(); i++) {
	double projected_length;
	eptr = (fvparray[i])->getEdgePtr();
	edgeID = eptr->getID();
	index = 2*(edgeID -  min_edge_id);

	if ( eptr->getType() != ETChull) {/*cout<<"ETChull"<<endl;*/}
	else index += 1;

	eptr->setType(ETChull);
	
	eptr->getFacePointers(tmpfptr1,tmpfptr2);
	edgeVector = normalized((eptr->getOtherVertexPointer(vp))->getCoords() - vp->getCoords());
	axis = edgeVector;
	if ( fvparray.size() == 1 ) {
	  vec = Xaxis;
	  if ( fabs(axis*vec) == 1 ) vec = Yaxis;
	  dir = normalized(edgeVector % vec);
	}
	else if ( fvparray.size() == 2 )
	  dir = (((tmpfptr1)->computeNormal() +(tmpfptr2)->computeNormal())/2);	 
	else {
	  dirFvp1 = vp->getFaceVertexInFace(tmpfptr1);
	  dirFvp2 = vp->getFaceVertexInFace(tmpfptr2);
	  Ndir1 = ((dirFvp1->next())->getVertexCoords() - vp->getCoords()) %
	    ((dirFvp1->prev())->getVertexCoords() - vp->getCoords());
	  Ndir2 = ((dirFvp2->next())->getVertexCoords() - vp->getCoords()) %
	    ((dirFvp2->prev())->getVertexCoords() - vp->getCoords());				
	  dir = (normalized(Ndir1) + normalized(Ndir2))/2;				
	}

	//start point with axis translated to origin (+vertex - vertex)
	length += 0.0001;
	start_point = length*axis+ normalized(dir) * thickness + vp->getCoords();
	projected_length = (start_point - vp->getCoords())*axis ;
	start_point = start_point + (( length)-projected_length) * axis;
	PonAxs = vp->getCoords() + (length) * axis;
	dir = normalized(start_point -PonAxs );
	start_point = PonAxs +  dir*thickness;
	start_point = start_point -vp->getCoords() ;
	for (int j=0; j < numSides; j++) {
	  //calculate new point
	  angle = ((2*3.15)/numSides)*j;
	  Quaternion rot;
	  rot.setAxisAndAngle(axis, angle);
	  normalize(rot);
	  Quaternion rotp;
	  rotp = rot * start_point * conjugate(rot);
	  xyz.set(rotp[0],rotp[1],rotp[2]);
	   
	  //move back from origin
	  xyz = xyz + vp->getCoords();
	  vertlist.push_back(xyz);       //vertlist for vertex convex hull
	  temp_vertlist.push_back(xyz);  //vertlist for each edge face
	}
	if ( fvparray.size() == 1 ) {
	  obj->createFace(temp_vertlist);
	  fl_last = obj->endFace();
	  --fl_last;--fl_last;
	  valOneFace = *fl_last;	 
	}		
	normal= axis;
	edge_connect_normals[index] = normalized(normal);
	temp_vertlist.clear();
      }//for 

      //join the corresponding vertices of the new faces created for this vertex

      DLFLConvexHull convexhull;
      DLFLFacePtrArray chfparray;
      DLFLVertexPtrArray vparray, vparrayOrdered;
      DLFLFacePtr chfp; 
      Vector3d sfpn, chfpn;
      DLFLFaceVertexPtrArray corners;
      IntArray matched_face_indx;
      int temp_index;
      int num_ch_faces = 0;
      int num_ch_verts = 0;

      vertListSizeMatch = false;
      jointCreated = false;

      num_ch_verts = fvparray.size()*numSides;
      for(int i = 0; i < num_ch_verts ; i++) {
	vparrayOrdered.push_back(NULL);      
      }
      if ( fvparray.size() == 1 ) { 
	num_ch_faces = 1;
	valOneFace->getCorners(corners);
	for (int j = 0; j < corners.size(); j++) {
	  temp_vptr = corners[j]->getVertexPtr();
	  temp_vptr->CHullIndex = j;
	  temp_index = temp_vptr->CHullIndex;
	  vparrayOrdered[temp_index] = temp_vptr;
	}
	jointCreated = true;
      } else {   
	if ( !convexhull.createHull(vertlist) ) jointCreated = false;
	else jointCreated = true;
	if ( jointCreated ) {
	  //convexhull.edgeCleanup();
	  edgeCleanup( &convexhull );
	  convexhull.getFaces(chfparray); num_ch_faces = chfparray.size();

	  for(int i =0 ; i < num_ch_faces ; i++) {
	    chfp = chfparray[i];
	    chfp->getCorners(corners);
	    for (int j = 0; j < corners.size(); j++) {
	      temp_vptr = corners[j]->getVertexPtr();
	      temp_index = temp_vptr->CHullIndex;
	      vparrayOrdered[temp_index] = temp_vptr;
	    }
	  }
	}
      }

      //FACE MATCHING/////////////////////////////////////////
      int num_matches=0;
      double theta = 0;
      if ( jointCreated ) {
	for (int j=0; j < fvparray.size(); ++j) {
	  eptr = (fvparray[j])->getEdgePtr();
	  edgeID = eptr->getID();
	  index = 2*(edgeID -  min_edge_id);

	  if ( edge_connect_normals[index+1] == zeroVec )
	    index = index;	
	  else index += 1; 

	  sfpn = edge_connect_normals[index];
	  num_matches=0;
	  for (int i=0; i < num_ch_faces; ++i) {
	    if ( fvparray.size() == 1 ) chfp = valOneFace;
	    else chfp = chfparray[i];
	    match = false;
	    if ( chfp->size() == numSides ) {
	      if (vparrayOrdered.size() == fvparray.size()*numSides) {
		chfp->getCorners(corners);
		int ii;
		ii = j;
		VListPtr = vparrayOrdered[ii*numSides];  //first vertex of the face
		for(int jj=0; jj < corners.size(); jj++) {
		  if ( VListPtr->getID() == corners[jj]->getVertexID() ) {  
		    //match rest of the ch face verts with the vertlist
		    if ( vparrayOrdered[ii*numSides + 1]->getID() ==
			 (corners[jj]->next())->getVertexID() ) {
		      //go in forward direction
		      tmp_ch_fvp = (corners[jj]->next())->next();
		      for (int kk=2; kk < numSides ; kk++) { // check rest
			if ( vparrayOrdered[ii*numSides + kk]->getID() ==
			     tmp_ch_fvp->getVertexID() )
			  match = true;
			tmp_ch_fvp = tmp_ch_fvp->next();
		      }
		    }
		    else if ( vparrayOrdered[ii*numSides + 1]->getID() ==
			      (corners[jj]->prev())->getVertexID() ) {
		      //go in reverse direction
		      tmp_ch_fvp = (corners[jj]->prev())->prev();
		      for(int kk=2; kk < numSides ; kk++) { //check rest
			if ( vparrayOrdered[ii*numSides + kk]->getID() ==
			     tmp_ch_fvp->getVertexID() )
			  match = true;
			tmp_ch_fvp = tmp_ch_fvp->prev();
		      }				      
		    }
		    else match = false;	//no match
		  }
		}//for
		corners.clear();
		vertListSizeMatch = true;
	      } else {
		cout<<"vertlist size dosent match" << endl;
		vertListSizeMatch = false;
	      }

	      if (match) {
		edge_connect_fparray[index] = chfp;
		temp_face_array.push_back(chfp);
		matched_face_indx.push_back(j);
		num_matches++;
	      }//if

	    }//if
	    temp_chface_vertlist.clear();
	  }//for
	}//for
      }
      else
	cout << "joint not created for vertex: " << vp->getID() << endl;

      IntArray not_matched_array;
      DLFLEdgePtrArray temp_edge_del_array;
      int ID , otherID;
      allFacesMatched = false;
      if ( vertListSizeMatch && jointCreated ) {
	if ( matched_face_indx.size() != fvparray.size() )
	  allFacesMatched = false;
	else 
	  allFacesMatched = true;
	  
      }
      else
	cout << "vertListSizeMatch not matched for vertex " << vp->getID() << endl;

      if ( allFacesMatched && vertListSizeMatch && jointCreated ) {
	// all faces shoud be matched now delete the edes that are not required
	for (int i= 0; i < temp_face_array.size(); i++) {
	  temp_face_array[i]->getEdges(temp_edge_ptr_array);
	  if ( temp_edge_ptr_array.size() == numSides ) {
	    for (int j=0 ; j< numSides ; j++)
	      temp_edge_ptr_array[j]->setType(ETdoNotDelete);
	  }
	  else {
	    cout << "face dosent have right number of edges" << endl;
	    return;
	  }
	}
      }
      else
	cout << "something not matched for vertex" << vp->getID() << endl;

      if ( allFacesMatched && vertListSizeMatch && jointCreated) {
	//go thru the vertices and get edges and delete the extraneous ones
	if ( fvparray.size() != 1 ) {
	  DoubleArray edegLen;
	  double eLen = 999999;
	  int eInd, Cindex1, Cindex2;
	  bool doSimilar;
	  DLFLEdgePtr eMinPtr;

	  eLen = 999999;
	  for (int i = 0; i < vparrayOrdered.size(); i++) {
	    vparrayOrdered[i]->getEdges(temp_edge_ptr_array);
	    for (int jj =0; jj < temp_edge_ptr_array.size(); jj++) {
	      temp_edge_ptr_array[jj]->getVertexPointers(vp1, vp2);

	      while(vp1->CHullIndex >= numSides)
		vp1->CHullIndex -= numSides;
	      while(vp2->CHullIndex >= numSides)
		vp2->CHullIndex -= numSides;
	      Cindex1 = vp1->CHullIndex;
	      Cindex2 = vp2->CHullIndex;

	      if ( temp_edge_ptr_array[jj]->getType() != ETdoNotDelete ) {
		if ( temp_edge_ptr_array[jj]->length() < eLen ) {
		  eMinPtr = temp_edge_ptr_array[jj]; 
		  eLen = temp_edge_ptr_array[jj]->length();
		}
	      }
	    }
	  }
	  eMinPtr->getVertexPointers(vp1, vp2);
	  Cindex1 = vp1->CHullIndex;
	  Cindex2 = vp2->CHullIndex;
	  if (((Cindex1 % 2 == 0) && (Cindex2 % 2 == 0)) || ((Cindex1 % 2 != 0) && (Cindex2 % 2 != 0)))
	    doSimilar = true;
	  if (((Cindex1 % 2 == 0) && (Cindex2 % 2 != 0)) || ((Cindex1 % 2 != 0) && (Cindex2 % 2 == 0)))
	    doSimilar = false;	
	  for (int i = 0; i < vparrayOrdered.size(); i++) {
	    vparrayOrdered[i]->getEdges(temp_edge_ptr_array);

	    for (int j =0; j<temp_edge_ptr_array.size(); j++) {
	      if (temp_edge_ptr_array[j]->getType() != ETdoNotDelete) {
		temp_edge_ptr_array[j]->getVertexPointers(vp1, vp2);
		if ( doSimilar ) {
		  if (((vp1->CHullIndex % 2 == 0) && (vp2->CHullIndex % 2 != 0)) ||
		      ((vp1->CHullIndex % 2 != 0) && (vp2->CHullIndex % 2 == 0))) {
		    temp_edge_ptr_array[j]->setType(ETdoDelete);
		  }
		}
		else {
		  if (((vp1->CHullIndex % 2 == 0) && (vp2->CHullIndex % 2 == 0)) ||
		      ((vp1->CHullIndex % 2 != 0) && (vp2->CHullIndex % 2 != 0))) {
		    temp_edge_ptr_array[j]->setType(ETdoDelete);
		  }
		}
	      }//if
	    }//for      
	  }//for
	}
      }
      else
	cout << "something not matched for vertex " << vp->getID() << endl;//if 0/1

      vparrayOrdered.clear();
      matched_face_indx.clear();
      temp_edge_ptr_array.clear();
      temp_face_array.clear();

      if ( fvparray.size() !=1 && jointCreated ) obj->splice(convexhull);
	 
      //}//end if jointCreated	
      fvparray.clear();
      vertlist.clear();
    }//while
  
    el_first = obj->beginEdge(); el_last = obj->endEdge();
  
    Vector3d N1, N2;
    count =0;
    while ( el_first != el_last ) {
      eptr = (*el_first);
      ++el_first; 
      if ( eptr->getType() == ETChull ) {
	if ( edge_connect_fparray[count] != NULL && edge_connect_fparray[count+1] != NULL ) {
	  connectFaces(obj,edge_connect_fparray[count],edge_connect_fparray[count+1],1); // makes the connections
	  ++count;++count;
	} else
	  cout << "pipe not created for edge: " << eptr->getID() << endl;
	deleteEdge(obj,eptr);
      }
      if ( eptr->getType() == ETdoDelete) deleteEdge(obj,eptr);
    } 
    edge_array.clear();
    edge_connect_normals.clear();
    edge_connect_fparray.clear();
  }

  void tagMatchingFaces(DLFLObjectPtr obj, DLFLFacePtr fptr) {
    DLFLFacePtrList::iterator fl_first=obj->beginFace(), fl_last = obj->endFace();
    DLFLFacePtr fp;
    int facevalence = fptr->size();
    int count=0;
    int index;

    // Look at only the outer crust
    while ( fl_first != fl_last ) {
      fp = (*fl_first); ++fl_first; ++count;
      index = fp->getID() - crust_min_face_id;
      if ( index < crustfp1.size() ) {
	if ( fp->size() == facevalence ) fp->setType(FTHole);
      }
      else {
	// We are done with outer surface
	break;
      }
    }
  }

	//selects all faces with same number of vertices as the face passed to the function
	//initializes them in fparray by reference
	void selectMatchingFaces(DLFLObjectPtr obj, DLFLFacePtr fptr, DLFLFacePtrArray &fparray) {
		fparray.clear();
		DLFLFacePtrList::iterator fl_first=obj->beginFace(), fl_last = obj->endFace();

		while ( fl_first != fl_last )	{
			if ( (*fl_first)->size() == fptr->size() ) fparray.push_back(*fl_first);
			fl_first++;
		}
	}

	void selectMatchingEdges(DLFLObjectPtr obj, DLFLEdgePtr eptr, DLFLEdgePtrArray &eparray) {
		eparray.clear();
		DLFLEdgePtrList::iterator el_first=obj->beginEdge(), el_last = obj->endEdge();
		DLFLVertexPtr vp1,vp2,v2,v1;
		eptr->getVertexPointers(v2,v1);
		while ( el_first != el_last )	{
			(*el_first)->getVertexPointers(vp1,vp2);
			
			if ( (vp1->numEdges() == v1->numEdges() && vp2->numEdges() == v2->numEdges()) ||
			 		 (vp1->numEdges() == v2->numEdges() && vp2->numEdges() == v1->numEdges())   ) {
				eparray.push_back(*el_first);
			}
			el_first++;
		}
	}

	void selectMatchingVertices(DLFLObjectPtr obj, DLFLVertexPtr vptr, DLFLVertexPtrArray &vparray) {
		vparray.clear();
		DLFLVertexPtrList::iterator vl_first=obj->beginVertex(), vl_last = obj->endVertex();

		while ( vl_first != vl_last )	{
			if ( (*vl_first)->numEdges() == vptr->numEdges() ) vparray.push_back(*vl_first);
			vl_first++;
		}
	}

	void selectFacesByArea(DLFLObjectPtr obj, DLFLFacePtr fptr, DLFLFacePtrArray &fparray, float delta ){
		fparray.clear();
		DLFLFacePtrList::iterator fl_first=obj->beginFace(), fl_last = obj->endFace();
		float area = fptr->getArea();
		while ( fl_first != fl_last )	{
			if ( (*fl_first)->getArea() <= (area+delta) && (*fl_first)->getArea() >= (area-delta) ) fparray.push_back(*fl_first);
			fl_first++;
		}
	}

	void selectFacesByColor(DLFLObjectPtr obj, DLFLFacePtr fptr, DLFLFacePtrArray &fparray, float delta ){
		//use delta later...
		fparray.clear();
		DLFLFacePtrList::iterator fl_first=obj->beginFace(), fl_last = obj->endFace();
		DLFLMaterialPtr matl = fptr->material();
		while ( fl_first != fl_last )	{
			if ( (*fl_first)->material() == matl ) fparray.push_back(*fl_first);
			fl_first++;
		}
	}

  void punchHoles( DLFLObjectPtr obj ) {
    // Go through list of faces and punch holes through faces that have type
    // flag set to FTHole
    // Assumes that the crust has already been created.
    DLFLFacePtrList::iterator fl_first=obj->beginFace(), fl_last=obj->endFace();
    DLFLFacePtrList hole_faces;
    DLFLFacePtr fp;
    while ( fl_first != fl_last ) {
      fp = (*fl_first);
      if ( fp->getType() == FTHole ) {
	hole_faces.push_back(fp);
      }
      ++fl_first;
    }
    fl_first = hole_faces.begin(); fl_last = hole_faces.end();
    while ( fl_first != fl_last ) {
      fp = (*fl_first); ++fl_first;
      fp->resetType();
      cmMakeHole(obj, fp,true);
    }
  }

  void makeWireframe(DLFLObjectPtr obj, double crust_thickness, bool split) {
    // Make a wireframe from the given model.

    // First do a modified corner-cutting subdivision
    modifiedCornerCuttingSubdivide(obj,crust_thickness);

    // Split Valence 2 vertices if 'split' flag is true
    if ( split ) splitValence2Vertices(obj,-1.0);
  
    // Create a crust with specified thickness
    createCrustForWireframe(obj,crust_thickness);

    // Punch holes to get the wireframe
    punchHoles(obj);
  }

  void makeWireframe2(DLFLObjectPtr obj, double crust_thickness, double crust_width, bool split) {
    // Make a wireframe from the given model.
		// std::cout << crust_thickness << "\t" << crust_width << "\n";
    // First do a modified corner-cutting subdivision
    modifiedCornerCuttingSubdivide(obj,crust_width);

    // Split Valence 2 vertices if 'split' flag is true
    if ( split ) splitValence2Vertices(obj,-1.0);
  
    // Create a crust with specified thickness
    createCrustForWireframe(obj,crust_thickness);

    // Punch holes to get the wireframe
    punchHoles(obj);
  }

  void makeWireframeWithColumns(DLFLObjectPtr obj, double wireframe_thickness, int wireframe_segments) {
    createWireframeWithSegments(obj,wireframe_thickness,wireframe_segments);
  }

} // end namespace
