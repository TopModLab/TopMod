#include "DLFLObject.hh"
#include "DLFLAux.hh"
#include "DLFLConvexHull.hh"

static int s=0;
static DLFLConvexHull * convexhull = NULL;

//new changes;these needs to be declared outside the function for local-cut
DLFLVertexPtr * sverts;
DLFLEdgePtr * sedges;
int cutcount;

void DLFLObject::createConvexHull(){
	Vector3dArray varray;
	varray.resize(vertex_list.size());
         	DLFLVertexPtrList :: const_iterator first = vertex_list.begin(), last = vertex_list.end();
	int i=0;
         	while ( first != last )
         	   {
         	     varray[i++] = (*first)->getCoords();
         	     ++first;
         	   }
	DLFLConvexHull chull;
	chull.createHull(varray);
	reset();
	splice(chull);
}


void DLFLObject::createDualConvexHull(){
	
	DLFLVertexPtrArray verts;
	Vector3dArray ovarray;
	getVertices(verts);
	for(int i=0;i<verts.size();i++)
		ovarray.push_back(verts.at(i)->getCoords());

	//find min and max points
	float fmax=9999999, fmin = -9999999;
	Vector3d cvmin(fmax,fmax,fmax),cvmax(fmin,fmin,fmin);
	for(int i=0;i<ovarray.size();i++){
		Vector3d v = ovarray.at(i);
		for(int j=0;j<3;j++){
			if (v[j]<cvmin[j]) cvmin[j] = v[j];
			if (v[j]>cvmax[j]) cvmax[j] = v[j];
		}
	}	

	//slice object uniformly
	int SLICENUM[3] = {1,1,1};
	int DUAL = 1;
	float EPS = 0.001;
	
	reset();
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
						splice(chull);
					}else {
						DLFLObjectPtr obj = createDualConvexHull(coords);
						splice(*obj);
					}
				}
				z+=step[2];
			}
			y+=step[1];
		}
		x+=step[0];
	}

}


DLFLObjectPtr DLFLObject::createDualConvexHull(const Vector3dArray &ovarray){

	
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
	cube->connectFaces(fvp1,fvp2);

	for(int j=0;j<cverts.size();j++){
		//printf("processing %d of %d\n",j+1,cverts.size());
		// (j==cverts.size()) return;	
		DLFLVertexPtr vp= cverts.at(j);
		Vector3d normal = vp->computeNormal();
		cube->peelByPlane(normal,vp->getCoords());		
 	}
	
	return cube;
}

void DLFLObject::peelByPlane(Vector3d normal,Vector3d P0)
{   
  
  DLFLVertexPtr vp1, vp2, vnew;
  normal = normalized(normal);
  
  DLFLEdgePtrArray edgestodel,edges;
  DLFLEdgePtr ep;
  DLFLFaceVertexPtrArray corner_list, newcorners;

  Vector3d p1, p2,pnew;
  float d1, d2;
  int c=0;

  edgestodel.clear(); edges.clear();
 
  getEdges(edges);
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
		vnew = subDivideEdge(ep);
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
				insertEdge(cp1,cp2);
				continue;
			}
		}
	}

	for(int i=0;i<edgestodel.size();i++)
		deleteEdge(edgestodel.at(i));
	
	edgestodel.clear();
}

void DLFLObject :: performCutting(int type,float offsetE,float offsetV,bool global,bool selected)
{

	if (type==204){
		//truncateEdges(offsetE);
		return;
	}

	DLFLEdgePtrArray edges;
	DLFLVertexPtrArray verts;
	DLFLFacePtrArray faces;

	cutcount = 0;
	int vcount = 0;
	int num=0;

	Vector3d * locs, * norms;
	getEdges(edges);
	getVertices(verts);
	getFaces(faces);
	int esize 	= edges.size();
	int vsize		= verts.size();
	int fsize 	= faces.size();
	int totalsize 	= esize+vsize+fsize;
	locs   		= new Vector3d[totalsize];
	norms  		= new Vector3d[totalsize];
	sverts 		= new DLFLVertexPtr[totalsize];
	sedges 		= new DLFLEdgePtr[esize];


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
	
	if (type==203)
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
			printf("d:%f\n",d);
			if (d>dmax){
				dmax = d;
				svp = vp;
			}
		}
		sverts[cutcount] = svp;
		cutcount++;
	}

	
	for(int i = 0; i<cutcount;i++)
		if (global)
			peelByPlane(norms[i],locs[i]);	
		//else	localCut(sverts[i],norms[i],locs[i]);
	
	delete [] norms;
	delete [] locs;
	delete [] sverts;
	delete [] sedges;

}