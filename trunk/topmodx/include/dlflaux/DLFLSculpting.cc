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

#include "DLFLSculpting.hh"
#include "DLFLConnect.hh"

namespace DLFL {

	void createConvexHull( DLFLObjectPtr obj ){
		Vector3dArray varray;
		varray.resize(obj->num_vertices());
		DLFLVertexPtrList::const_iterator first = obj->beginVertex(), last = obj->endVertex();
		int i = 0;
		while ( first != last ) {
			varray[i++] = (*first)->getCoords();
			++first;
		}
		DLFLConvexHull chull;
		chull.createHull(varray);
		obj->reset();
		obj->splice(chull);
	}

	void createDualConvexHull( DLFLObjectPtr obj ){

		DLFLVertexPtrArray verts;
		Vector3dArray ovarray;
		obj->getVertices(verts);
		for(int i = 0; i < (int)verts.size(); i++)
			ovarray.push_back(verts.at(i)->getCoords());

		//find min and max points
		float fmax=9999999, fmin = -9999999;
		Vector3d cvmin(fmax,fmax,fmax),cvmax(fmin,fmin,fmin);
		for(int i = 0; i < (int)ovarray.size(); i++) {
			Vector3d v = ovarray.at(i);
			for(int j=0;j<3;j++) {
				if (v[j]<cvmin[j]) cvmin[j] = v[j];
				if (v[j]>cvmax[j]) cvmax[j] = v[j];
			}
		}	

		//slice object uniformly
		int SLICENUM[3] = {1,1,1};
		int DUAL = 1;
		float EPS = 0.001;

		obj->reset();
		Vector3d step = (cvmax - cvmin + Vector3d(2*EPS,2*EPS,2*EPS));
		step[0]/=SLICENUM[0];
		step[1]/=SLICENUM[1];
		step[2]/=SLICENUM[2]; 

		/*step[0]-=1;
		step[1]-=1;
		step[2]-=1; 
		*/

		float x=cvmin[0]-EPS;
		for(int ix = 0; ix<SLICENUM[0]; ix++){
			float y=cvmin[1]-EPS;
			for(int iy = 0; iy<SLICENUM[1]; iy++){	
				float z=cvmin[2]-EPS;
				for(int iz = 0; iz<SLICENUM[2]; iz++){
					Vector3dArray coords;

					for(int i=0;i<ovarray.size();i++)
						if ( (ovarray.at(i)[0]  > x)&&(ovarray.at(i)[0]  < x+step[0]+EPS) && 
						(ovarray.at(i)[1]  > y)&&(ovarray.at(i)[1]  < y+step[1]+EPS) &&
						(ovarray.at(i)[2]  > z)&&(ovarray.at(i)[2]  < z+step[2]+EPS) ) 
						coords.push_back(ovarray.at(i));
					if (coords.size()>3){
						if (!DUAL){
							DLFLConvexHull chull;
							chull.createHull(coords);
							obj->splice(chull);
						} else {
							DLFLObjectPtr object = createDualConvexHull(obj,coords);
							obj->splice(*object);
						}
					}
					z+=step[2];
				}
				y+=step[1];
			}
			x+=step[0];
		}	 
	}

	DLFLObjectPtr createDualConvexHull( DLFLObjectPtr obj, const Vector3dArray &ovarray){


		DLFLVertexPtrArray cverts;

		if (convexhull==NULL) convexhull=new DLFLConvexHull();

		convexhull->createHull(ovarray);
		convexhull->getVertices(cverts);
		float fmax=9999999, fmin = -9999999;
		Vector3d cvmin(fmax,fmax,fmax),cvmax(fmin,fmin,fmin);
		for(int i=0;i<ovarray.size();i++){
			Vector3d v = ovarray.at(i);
			for(int j=0;j<3;j++){
				if (v[j]<cvmin[j]) cvmin[j] = v[j];
				if (v[j]>cvmax[j]) cvmax[j] = v[j];
			}
		}

		DLFLObjectPtr cube= new DLFLObject();
		Vector3dArray verts;
		Vector3d p;
		double x = 0.5;

		verts.resize(4);

		// Create top face
		p.set(cvmax[0],cvmax[1],cvmax[2]); verts[0] = p;
		p.set(cvmax[0],cvmax[1],cvmin[2]); verts[1] = p;
		p.set(cvmin[0],cvmax[1],cvmin[2]); verts[2] = p;
		p.set(cvmin[0],cvmax[1],cvmax[2]); verts[3] = p;
		cube->createFace(verts);
		DLFLFacePtr fp1 = cube->lastFace();

		// Create bottom face
		p.set(cvmax[0],cvmin[1],cvmax[2]); verts[0] = p;
		p.set(cvmin[0],cvmin[1],cvmax[2]); verts[1] = p;
		p.set(cvmin[0],cvmin[1],cvmin[2]); verts[2] = p;
		p.set(cvmax[0],cvmin[1],cvmin[2]); verts[3] = p;
		cube->createFace(verts);
		DLFLFacePtr fp2 = cube->lastFace();

		// fp1 and fp2 will be inward facing
		// Connect them using the first corners in each
		DLFLFaceVertexPtr fvp1, fvp2;
		fvp1 = fp1->firstVertex(); fvp2 = fp2->firstVertex();
		connectFaces(cube,fvp1,fvp2);

		for(int j=0;j<cverts.size();j++){
			//printf("processing %d of %d\n",j+1,cverts.size());
			// (j==cverts.size()) return;	
			DLFLVertexPtr vp= cverts.at(j);
			Vector3d normal = vp->computeNormal();
			peelByPlane(cube,normal,vp->getCoords());		
		}

		return cube;
	}

	void peelByPlane( DLFLObjectPtr obj, Vector3d normal,Vector3d P0) {   

		DLFLVertexPtr vp1, vp2, vnew;
		normal = normalized(normal);

		DLFLEdgePtrArray edgestodel,edges;
		DLFLEdgePtr ep;
		DLFLFaceVertexPtrArray corner_list, newcorners;

		Vector3d p1, p2,pnew;
		float d1, d2;
		int c=0;

		edgestodel.clear(); edges.clear();

		obj->getEdges(edges);
		float EPS = 0.000001;

		//check interssection of all edges w slicing plane	
		for(int i=0; i<edges.size();i++){
			ep = edges.at(i);	
			ep->getVertexPointers(vp1,vp2);
			p1 = vp1->getCoords();
			p2 = vp2->getCoords();
			float d1 = normal*(p1 - P0);
			float d2 = normal*(p2 - P0);

			//if both vertices of the edge are on (+) side
			if ((d1>0)&&(d2>0))
				edgestodel.push_back(ep);

			//if one end is - one end is +
			if ( ((d1*d2)<0) || ((d1>0)&&(d2==0)) || ((d1==0)&&(d2>0)) ){
				vnew = subdivideEdge(obj,ep);
				if (d1>0)
					edgestodel.push_back(vnew->getEdgeTo(vp1));
				else
					edgestodel.push_back(vnew->getEdgeTo(vp2));

	//make planar
				pnew = vnew->getCoords();
				Vector3d ne = normalized(p2-p1);
				float t = ( normal * ( P0 - pnew) ) / (normal * ne);
				vnew->setCoords(pnew + ne*t);		

				vnew->getFaceVertices(corner_list);	
				newcorners.push_back(corner_list.at(0));
				newcorners.push_back(corner_list.at(1));
			}
		}
		DLFLFaceVertexPtr cp1,cp2;

		for(int i =0; i<newcorners.size();i++){
			cp1 = newcorners.at(i);
			for(int j =0; j<newcorners.size();j++){
				if (i==j) continue;
				cp2 = newcorners.at(j);
				if (cp1->getFacePtr()==cp2->getFacePtr()){
					insertEdge(obj,cp1,cp2);
					continue;
				}
			}
		}

		for(int i=0;i<edgestodel.size();i++)
			deleteEdge(obj,edgestodel.at(i));

		edgestodel.clear();
	}

	void visitVertex(DLFLObjectPtr obj, DLFLVertexPtr vp,DLFLEdgePtr from,Vector3d normal,Vector3d P0){

		if (vp->isvisited) return;
		vp->isvisited = 1;
		Vector3d p1 = vp->getCoords();
		float d1 = normal*(p1 - P0);
		//if (d1<0) return;
		DLFLEdgePtrArray vedges;
		if (vp){
			vp->getEdges(vedges);
			int size = vedges.size();

		 	//check interssection of all edges w slicing plane	
		  for(int i=0; i<size;i++){
				DLFLEdgePtr ep = vedges.at(i);
				if (ep==from) continue;
				if (ep->isvisited) continue;
				ep->isvisited = 1;
				DLFLVertexPtr evp1,evp2,vp2;
			 	ep->getVertexPointers(evp1,evp2);
				vp2 = (vp==evp1)?evp2:evp1;
				Vector3d p2 = vp2->getCoords();
				float d2 = normal*(p2 - P0);

				//if both vertices of the edge are on (+) side
				if ((d1>0)&&(d2>0)){
					//ep->istodel = 1;
					edges2del[e2dsize++] = ep;
					visitVertex(obj, vp2, ep, normal, P0);
				}

				//if (ep->istodel) continue;
				//if one end is - one end is +
				if ( ((d1*d2)<0) || ((d1>0)&&(d2==0)) || ((d1==0)&&(d2>0)) ){
					int id = getCutIndex(ep);
					DLFLVertexPtr vnew = subdivideEdge(obj, ep);
					if (d1>0){
						//vnew->getEdgeTo(vp)->istodel = 1;
						edges2del[e2dsize++] = vnew->getEdgeTo(vp);
						if (id!=-1){
							sedges[id] = vnew->getEdgeTo(vp2);
							sverts[id] = vnew;
						}

					}else{
						//fillet
						//vnew->getEdgeTo(vp2)->istodel = 1;
						//edges2del[e2dsize++] = vnew->getEdgeTo(vp2);
					}
					//make planar
					Vector3d pnew = vnew->getCoords();
					Vector3d ne = normalized(p2-p1);
					float t = ( normal * ( P0 - pnew) ) / (normal * ne);
					vnew->setCoords(pnew + ne*t);
					DLFLFaceVertexPtrArray fvlist;
					vnew->getFaceVertices(fvlist);	
					newcorners[ncsize++] = fvlist.at(0);
					newcorners[ncsize++] = fvlist.at(1);
				}
		  }
		}
	}
	
	void localCut(DLFLObjectPtr obj, DLFLVertexPtr vp,Vector3d normal,Vector3d P0){

		if(!vp) return;
		DLFLVertexPtrArray verts;
		DLFLEdgePtrArray edges;
		obj->getVertices(verts);
		obj->getEdges(edges);

		for(int i=0;i<verts.size();i++)
			verts.at(i)->isvisited=0;
		
		for(int i=0;i<edges.size();i++)
			edges.at(i)->isvisited=0;

		int size = obj->num_edges();
		newcorners = new DLFLFaceVertexPtr[size];
		edges2del  = new DLFLEdgePtr[size];
		e2dsize=0;
		ncsize=0;
		visitVertex(obj, vp,0,normal,P0);

	//connect new corners
		DLFLFaceVertexPtr cp1,cp2;
		for(int i =0; i<ncsize;i++){
			cp1 = newcorners[i];
			for(int j =0; j<ncsize;j++){
				if (i==j) continue;
				cp2 = newcorners[j];
				if ( cp1->getFacePtr() == cp2->getFacePtr() ){
					insertEdge(obj, cp1,cp2);
					continue;
				}
			}
		}

	//delete edges
		for(int i=0;i<e2dsize;i++){
			// printf("edge %d: %x\n",i,edges2del[i]);
			edges2del[i]->isdummy=0;
		}

		for(int i=0;i<e2dsize;i++){
			DLFLEdgePtr ep = edges2del[i];

			for(int j=i+1;j<e2dsize;j++){
				if (ep==edges2del[j]) {
					// printf("ayni egde %d = %d = %x\n",i,j,ep);
					ep->isdummy=1;
				}
			}
		}

		for(int i=0;i<e2dsize;i++){
			deleteEdge(obj, edges2del[i]);
		}

	}
	// void localCut( DLFLObjectPtr obj, DLFLVertexPtr vp,Vector3d normal,Vector3d P0 ){
	// 
	// 	DLFLVertexPtrArray verts;
	// 	
	// 	// for(int i=0;i<verts.size();i++)
	// 	// 	verts.at(i)->isvisited=0;
	// 	
	// 	int size = obj->num_edges();
	//  		newcorners = new DLFLFaceVertexPtr[size];
	// 	edges2del  = new DLFLEdgePtr[size];
	// 	e2dsize=0;
	// 	ncsize=0;
	// 	obj->visitVertex(vp,0,normal,P0);
	// 	
	// 	//connect new corners
	// 	DLFLFaceVertexPtr cp1,cp2;
	// 	for(int i =0; i<ncsize;i++){
	// 		cp1 = newcorners[i];
	// 		for(int j =0; j<ncsize;j++){
	// 			if (i==j) continue;
	// 			cp2 = newcorners[j];
	// 			if ( cp1->getFacePtr() == cp2->getFacePtr() ){
	// 				insertEdge(cp1,cp2);
	// 				continue;
	// 			}
	// 		}
	// 	}
	// 	
	// 	//delete edges
	// 	for(int i=0;i<e2dsize;i++)
	// 		deleteEdge(edges2del[i]);
	// 	
	// 
	// }

	void performCutting( DLFLObjectPtr obj, int type,float offsetE,float offsetV,bool global,bool selected) {

		// if (type==204){
		// 	// truncateEdges(offsetE);
		// 	return;
		// }
		// global = false;

		DLFLEdgePtrArray edges;
		DLFLVertexPtrArray verts;
		DLFLFacePtrArray faces;

		cutcount = 0;
		int vcount = 0;
		int num=0;

		Vector3d * locs, * norms;
		obj->getEdges(edges);
		obj->getVertices(verts);
		obj->getFaces(faces);
		int esize 	= edges.size();
		int vsize		= verts.size();
		int fsize 	= faces.size();
		int totalsize 	= esize+vsize+fsize;
		locs   		= new Vector3d[totalsize];
		norms  		= new Vector3d[totalsize];
		sverts 		= new DLFLVertexPtr[totalsize];
		sedges 		= new DLFLEdgePtr[esize];

		//this loops through all edges??? why?
		for(int i=0;i<esize;i++){
			sverts[cutcount]=0;

			DLFLEdgePtr e = edges.at(i);
			if ( ((selected)&&(!e->ismarked)) || (type==201) ) continue;

			DLFLVertexPtr v1,v2,v;
			e->getVertexPointers(v1,v2);

			int vnum=0;
			Vector3d * vlocs = new Vector3d[v1->valence() + v2->valence()];

			for(int j=0;j<2;j++){
				v =(j)?v2:v1;
				DLFLEdgePtrArray vedges;
				v->getEdges(vedges);
				for(int k=0;k<vedges.size();k++){
					DLFLEdgePtr ve = vedges.at(k);
					if (ve==e) continue;
					DLFLVertexPtr ev1,ev2;
					ve->getVertexPointers(ev1,ev2);
					if (ev2==v){
						ev2 = ev1;
						ev1 = v;  
					}
					vlocs[vnum++] = (ev2->getCoords() - ev1->getCoords())*offsetE + ev1->getCoords();
				}
			}

			//calculate the normal and position 
			Vector3d vecp,vec,n(0,0,0),nprev,norm(0,0,0),mid(0,0,0);
			for(int j=0;j<vnum;j++)
				mid = mid + vlocs[j];
			mid = mid / ((float)vnum);

			for(int j=0;j<vnum;j++){
				vec = (mid - vlocs[j]);
				if (j==0){
					vecp = vec;
					continue;
				}
				n = n + vecp % vec;
			}

			DLFLFacePtr f1,f2;
			e->getFacePointers(f1,f2);
			norms[cutcount] = normalized(f1->computeNormal()+f2->computeNormal());
			locs[cutcount] = mid;
			sverts[cutcount] = v1;
			sedges[cutcount] = e;
			cutcount++;
		}

		//loop through all vertices
		for(int i = 0; i<vsize;i++){
			sverts[cutcount]=0;

			DLFLVertexPtr vp = verts.at(i);
			if ( ((selected)&&(!vp->ismarked))||(type==200) ) continue;

			DLFLEdgePtrArray vedges;
			vp->getEdges(vedges);

			int vnum=0;
			Vector3d * vlocs = new Vector3d[vp->valence()];

			for(int k=0;k<vedges.size();k++){

				DLFLEdgePtr ve = vedges.at(k);
				DLFLVertexPtr ev1,ev2;
				ve->getVertexPointers(ev1,ev2);
				if (ev2==vp){
					ev2 = ev1;
					ev1 = vp;  
				}
				vlocs[vnum++] = (ev2->getCoords() - ev1->getCoords())*offsetV + ev1->getCoords();
			}

			//calculate the normal and position for bevelling
			Vector3d vecp,vec,n(0,0,0),nprev,norm(0,0,0),mid(0,0,0);
			for(int j=0;j<vnum;j++)
				mid = mid + vlocs[j];
			mid = mid / ((float)vnum);

			norms[cutcount] = vp->computeNormal();
			locs[cutcount] = mid;
			sverts[cutcount] = vp;
			cutcount++;
		}

		//cut by face mode
		if (type==203) {
			//loop through all faces
			for(int i = 0; i<fsize;i++){
				sverts[cutcount]=0;

				DLFLFacePtr fp = faces.at(i);
				if ( ((selected)&&(!fp->ismarked))) continue;

				DLFLVertexPtrArray fverts;
				DLFLFaceVertexPtrArray fcorners;
				DLFLVertexPtr v;
				int vnum=0;
				fp->getCorners(fcorners);

				for(int j=0;j<fcorners.size();j++){
					DLFLFaceVertexPtr fv = fcorners.at(j);
					fverts.push_back(fv->getVertexPtr());
				}

				Vector3d * vlocs = new Vector3d[fverts.size()*10];

				for(int j=0;j<fverts.size();j++){
					v = fverts.at(j);
					DLFLEdgePtrArray vedges;
					v->getEdges(vedges);

					for(int k=0;k<vedges.size();k++){
						DLFLEdgePtr ve = vedges.at(k);
						DLFLFacePtr ef1,ef2;
						ve->getFacePointers(ef1,ef2);
						if ((ef1==fp )||(ef2==fp)) continue;
						DLFLVertexPtr ev1,ev2;
						ve->getVertexPointers(ev1,ev2);
						if (ev2==v){
							ev2 = ev1;
							ev1 = v;  
						}
						vlocs[vnum++] = (ev2->getCoords() - ev1->getCoords())*offsetV + ev1->getCoords();
					}
				}


			//calculate the normal and position for beveling
				Vector3d vecp,vec,n(0,0,0),nprev,norm(0,0,0),mid(0,0,0);
				for(int j=0;j<vnum;j++)
					mid = mid + vlocs[j];
				mid = mid / ((float)vnum);

				norms[cutcount] = fp->computeNormal();
				locs[cutcount] = mid;
				float dmax = -999;
				DLFLVertexPtr vp,svp;
				for(int j=0;j<fverts.size();j++){
					DLFLVertexPtr vp = fverts.at(j);
					float d = norms[cutcount]*(vp->getCoords() - mid);
				// printf("d:%f\n",d);
					if (d>dmax){
						dmax = d;
						svp = vp;
					}
				}
				sverts[cutcount] = svp;
				cutcount++;
			}
			}//end if type == 203

			for(int i = 0; i<cutcount;i++){
				if (global)
					peelByPlane(obj,norms[i],locs[i]);	
				else	localCut(obj, sverts[i],norms[i],locs[i]);
			}

			delete [] norms;
			delete [] locs;
			delete [] sverts;
			delete [] sedges;
	}//end performCutting Function

	void cutSelectedFaces( DLFLObjectPtr obj, float offsetE,float offsetV, bool global,bool selected) {

				// bool global = false;

				vector<DLFLVertexPtr>::iterator vit;
				vector<DLFLEdgePtr>::iterator eit,eit2;
				vector<DLFLFacePtr>::iterator fit,fit2;
				DLFLFacePtrArray sfptrarr = obj->sel_fptr_array;

				DLFLEdgePtrArray edges;
				DLFLVertexPtrArray verts;
				DLFLFacePtrArray faces;

				cutcount = 0;
				int vcount = 0;
				int num=0;

				Vector3d * locs, * norms;
				obj->getEdges(edges);
				obj->getVertices(verts);
				obj->getFaces(faces);
				int esize 	= edges.size();
				int vsize		= verts.size();
				int fsize 	= faces.size();
				int totalsize 	= esize+vsize+fsize;
				locs   		= new Vector3d[totalsize];
				norms  		= new Vector3d[totalsize];
				sverts 		= new DLFLVertexPtr[totalsize];
				sedges 		= new DLFLEdgePtr[esize];


			//loop through all faces
				for(fit = sfptrarr.begin(); fit != sfptrarr.end(); fit++, cutcount++){
			// std::cout << "in side selected face loop!\n";
					sverts[cutcount]=0;

					DLFLVertexPtrArray fverts;
					DLFLFaceVertexPtrArray fcorners;
					DLFLVertexPtr v;
					int vnum=0;
					(*fit)->getCorners(fcorners);

					for(int j=0;j<fcorners.size();j++){
						DLFLFaceVertexPtr fv = fcorners.at(j);
						fverts.push_back(fv->getVertexPtr());
					}

					Vector3d * vlocs = new Vector3d[fverts.size()*10];

					for(int j=0;j<fverts.size();j++){
						v = fverts.at(j);
						DLFLEdgePtrArray vedges;
						v->getEdges(vedges);

						for(int k=0;k<vedges.size();k++){
							DLFLEdgePtr ve = vedges.at(k);
							DLFLFacePtr ef1,ef2;
							ve->getFacePointers(ef1,ef2);
							if ((ef1==(*fit) )||(ef2==(*fit))) continue;
							DLFLVertexPtr ev1,ev2;
							ve->getVertexPointers(ev1,ev2);
							if (ev2==v){
								ev2 = ev1;
								ev1 = v;  
							}
							vlocs[vnum++] = (ev2->getCoords() - ev1->getCoords())*offsetV + ev1->getCoords();
						}
					}

			//calculate the normal and position for beveling
					Vector3d vecp,vec,n(0,0,0),nprev,norm(0,0,0),mid(0,0,0);
					for(int j=0;j<vnum;j++)
						mid = mid + vlocs[j];
					mid = mid / ((float)vnum);

					norms[cutcount] = (*fit)->computeNormal();
					locs[cutcount] = mid;
					float dmax = -999;
					DLFLVertexPtr vp,svp;
					for(int j=0;j<fverts.size();j++){
						DLFLVertexPtr vp = fverts.at(j);
						float d = norms[cutcount]*(vp->getCoords() - mid);
				// printf("d:%f\n",d);
						if (d>dmax){
							dmax = d;
							svp = vp;
						}
					}
					sverts[cutcount] = svp;
				}

				for(int i = 0; i<cutcount;i++){
					if (global)
						peelByPlane(obj,norms[i],locs[i]);	
					else	localCut(obj, sverts[i],norms[i],locs[i]);
				}

				delete [] norms;
				delete [] locs;
				delete [] sverts;
				delete [] sedges;
	}//end cutselectedFaces Function

	void cutSelectedEdges( DLFLObjectPtr obj, float offsetE,float offsetV, bool global,bool selected) {

		// bool global = false;
		vector<DLFLVertexPtr>::iterator vit;
		vector<DLFLEdgePtr>::iterator eit,eit2;
		DLFLEdgePtrArray septrarr = obj->sel_eptr_array;

		DLFLEdgePtrArray edges;
		DLFLVertexPtrArray verts;
		DLFLFacePtrArray faces;

		cutcount = 0;
		int vcount = 0;
		int num=0;

		Vector3d * locs, * norms;
		obj->getEdges(edges);
		obj->getVertices(verts);
		obj->getFaces(faces);
		int esize 	= edges.size();
		int vsize		= verts.size();
		int fsize 	= faces.size();
		int totalsize 	= esize+vsize+fsize;
		locs   		= new Vector3d[totalsize];
		norms  		= new Vector3d[totalsize];
		sverts 		= new DLFLVertexPtr[totalsize];
		sedges 		= new DLFLEdgePtr[esize];

		//this loops through all edges??? why?
		for(eit = septrarr.begin(); eit != septrarr.end(); eit++, cutcount++){
		sverts[cutcount]=0;

		DLFLVertexPtr vp1,vp2,v;
		(*eit)->getVertexPointers(vp1,vp2);

		int vnum=0;
		Vector3d * vlocs = new Vector3d[vp1->valence() + vp2->valence()];

		for(int j=0;j<2;j++){
		v =(j)?vp2:vp1;
		DLFLEdgePtrArray vedges;
		v->getEdges(vedges);

		for(eit2 = vedges.begin(); eit2 != vedges.end(); eit2++){
			if ( (*eit2) == (*eit) ) continue;
			DLFLVertexPtr ev1,ev2;
			(*eit2)->getVertexPointers(ev1,ev2);
			if (ev2==v){
				ev2 = ev1;
				ev1 = v;  
			}
			vlocs[vnum++] = (ev2->getCoords() - ev1->getCoords())*offsetE + ev1->getCoords();
		}
		}

		//calculate the normal and position 
		Vector3d vecp,vec,n(0,0,0),nprev,norm(0,0,0),mid(0,0,0);
		for(int j=0;j<vnum;j++)
		mid += vlocs[j];
		mid /= ((float)vnum);

		for(int j=0;j<vnum;j++){
		vec = (mid - vlocs[j]);
		if (j==0){
			vecp = vec;
			continue;
		}
		n = n + vecp % vec;
		}

		DLFLFacePtr f1,f2;
		(*eit)->getFacePointers(f1,f2);
		norms[cutcount] = normalized(f1->computeNormal()+f2->computeNormal());
		locs[cutcount] = mid;
		sverts[cutcount] = vp1;
		sedges[cutcount] = (*eit);
		// cutcount++;
		}


		for(int i = 0; i<cutcount;i++){
		if (global)
		peelByPlane(obj,norms[i],locs[i]);	
		else	localCut(obj, sverts[i],norms[i],locs[i]);
		}

		delete [] norms;
		delete [] locs;
		delete [] sverts;
		delete [] sedges;
	}//end performCutting Function

	void cutSelectedVertices( DLFLObjectPtr obj, float offsetE,float offsetV, bool global,bool selected) {

		// bool global = false;
		vector<DLFLVertexPtr>::iterator vit;
		vector<DLFLEdgePtr>::iterator eit;
		DLFLVertexPtrArray svptrarr = obj->sel_vptr_array;

		DLFLEdgePtrArray edges;
		DLFLVertexPtrArray verts;
		DLFLFacePtrArray faces;

		cutcount = 0;
		int vcount = 0;
		int num=0;

		Vector3d * locs, * norms;
		obj->getEdges(edges);
		obj->getVertices(verts);
		obj->getFaces(faces);
		int esize 	= edges.size();
		int vsize		= verts.size();
		int fsize 	= faces.size();
		int totalsize 	= esize+vsize+fsize;
		locs   		= new Vector3d[totalsize];
		norms  		= new Vector3d[totalsize];
		sverts 		= new DLFLVertexPtr[totalsize];
		sedges 		= new DLFLEdgePtr[esize];

		//loop through all selected vertices
		cutcount = 0;
		for(vit = svptrarr.begin(); vit != svptrarr.end(); vit++, cutcount++){
			sverts[cutcount]=0;

			DLFLEdgePtrArray vedges;
			(*vit)->getEdges(vedges);

			int vnum=0;
			Vector3d * vlocs = new Vector3d[(*vit)->valence()];

		//loop through the edges for the current vertex
			for(eit = vedges.begin(); eit != vedges.end(); eit++){
		// DLFLEdgePtr ep = vedges.at(k);
				DLFLVertexPtr vp1,vp2;
				(*eit)->getVertexPointers(vp1,vp2);
		//swap the vertex pointers if necessary
				if (vp2== (*vit)){
					vp2 = vp1;
					vp1 = (*vit);  
				}
				vlocs[vnum++] = (vp2->getCoords() - vp1->getCoords())*offsetV + vp1->getCoords();
			}

		//calculate the normal and position for bevelling
			Vector3d vecp,vec,n(0,0,0),nprev,norm(0,0,0),mid(0,0,0);
			for(int j=0;j<vnum;j++)
				mid += vlocs[j];
			mid /= ((float)vnum);

			norms[cutcount] = (*vit)->computeNormal();
			locs[cutcount] = mid;
			sverts[cutcount] = (*vit);
		}

		for(int i = 0; i<cutcount;i++){
			if (global)
				peelByPlane(obj,norms[i],locs[i]);	
			else	
				localCut(obj, sverts[i],norms[i],locs[i]);
		}

		delete [] norms;
		delete [] locs;
		delete [] sverts;
		delete [] sedges;
	}//end cutSelectedVertices Function

	int isMarked(DLFLVertexPtr vp){
		DLFLEdgePtrArray vedges;
		DLFLEdgePtr ep;
		vp->getEdges(vedges);
		for(int j=0;j<vedges.size();j++){
			ep = vedges.at(j);
			if (ep->isdummy) return 1;
		}
		return 0;
	}

	void autoMarkEdges(DLFLObjectPtr obj){
		DLFLEdgePtrArray edges;
		DLFLVertexPtr vp,vp1,vp2;
		DLFLEdgePtrArray vedges;
		DLFLVertexPtrArray verts;
		DLFLEdgePtr ep;
		int E = 1;
		if(E){
			obj->getEdges(edges);
			//sort edge list from big to small
			DLFLEdgePtr emax,e,etmp;
			int size = edges.size();
			DLFLEdgePtr sorted[size];

			for(int i=0;i<size;i++) 
				sorted[i] = edges.at(i);
     	for(int j = 1; j < size; j++){
        e = sorted[j];
				int i;
	      for(i = j - 1; (i >= 0) && (sorted[i]->length() < e->length()); i--) 
					sorted[i+1] = sorted[i];
				sorted[i+1] = e;
			}
			//sort edges
			for(int i=0;i<size;i++){
				ep = sorted[i];
				if (ep->isdummy) continue;
				ep->getVertexPointers(vp1,vp2);
				if ((isMarked(vp1))||(isMarked(vp2))) continue;
				ep->isdummy = 1;
			}
		}
		else {
			obj->getVertices(verts);
			for(int i=0;i<verts.size();i++){
				vp = verts.at(i);
				vp->getEdges(vedges);
				for(int j=0;j<vedges.size();j++){
					ep = vedges.at(j);
					if (ep->isdummy) continue;
					ep->getVertexPointers(vp1,vp2);
					if ((isMarked(vp1))||(isMarked(vp2))) continue;
					ep->isdummy = 1;
				}
			}
		}
	}

	int getCutIndex(DLFLVertexPtr vp){
		for(int i=0;i<cutcount;i++)
			if (sverts[i]==vp) return i;
		return -1;
	}

	int getCutIndex(DLFLEdgePtr ep){
		for(int i=0;i<cutcount;i++)
			if (sedges[i]==ep) return i;
		return -1;
	}
} // end namespace
