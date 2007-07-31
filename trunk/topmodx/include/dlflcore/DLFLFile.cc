#include "DLFLObject.hh"
#include <cstdio>
#include <cstring>

namespace DLFL {

	typedef vector<Vector3d> Vector3dArray;
	typedef vector<Vector2d> Vector2dArray;

	// Temporary array to store the vertices and face vertices
	static DLFLVertexPtrArray vertex_array;
	static DLFLFaceVertexPtrArray face_vertex_array;
	static Vector3dArray normals;
	static Vector2dArray texcoords;

	void DLFLObject::readObject(istream& i) {
		if ( !i ) {
			cerr  << "Incomplete OBJ file." << endl;
			return;
		}

		// Clear the object first
		reset();

		DLFLVertexPtr newvptr;
		DLFLFaceVertexPtr newfvptr;
		DLFLFacePtr newfptr;
		DLFLMaterialPtr cur_mtl = matl_list.front();
		DLFLEdge * edges = NULL;
		RGBColor color;
		bool matl_added = false;
		char matl_name[10];
		Vector3d xyz;
		Vector2d uv;
		char c,c2;
		int num_edges=0;

		// Read each line and set the Vertex, Normal, Face, Color or Texture
		while ( i ) {
			removeWhiteSpace(i); i.get(c); i.get(c2);
			if ( c == 'c' && c2 == ' ' ) {
	// Read a color specification
				i >> color; cur_mtl = findMaterial(color);
				if ( cur_mtl == NULL ) {                     // No matching material found
				if (matl_added == false) {
			// No new materials have been added.
			// Set default material to be this color
					setColor(color); matl_added = true;
					cur_mtl = matl_list.front();
				} else {
			// Atleast 1 new material was added, but none of the
			// existing materials match this color. So create a new
			// material with this color and add it to the list
					sprintf(matl_name,"material%d",matl_list.size());
					matl_list.push_back(new DLFLMaterial(matl_name,color));
					cur_mtl = matl_list.back();
				}
			}
			} else if ( c == 'v' ) {
				if ( c2 == ' ' ) {

		// Read a vertex specification
					i >> xyz;

		// Create a new DLFLVertex
					newvptr = new DLFLVertex( xyz );

		// Add the pointer to the VertexPtr list. The list will free the memory
					addVertexPtr(newvptr);

		// Add the pointer to the local vertex_array
		// for easy access when creating the faces
					vertex_array.push_back(newvptr);
					} else if ( c2 == 'n' ) {
		// Read a normal specification
						i >> xyz; 
						normals.push_back(xyz);
						} else if ( c2 == 't' ) {
		// Read a texture coordinate
							i >> uv;
							texcoords.push_back(uv);
						}
						} else if ( c == 'f' && c2 == ' ' ) {
	// Create a new DLFLFace
							newfptr = new DLFLFace;

	// Read a face specification
							c = i.peek(); 
							while ( c != '\n' ) {
								int v,vt,vn;
								i >> v; vt = -1; vn = -1; c = i.peek();
								if ( c == '/' ) {
									i.get(c); c = i.peek();
									if ( c != '/' ) {
										i >> vt; c = i.peek();
									}
									if ( c == '/' ) {
										i.get(c); i >> vn; c = i.peek();
									}
								}

		// We have v,vt and vn now
		// Create a new DLFLFaceVertex
								newfvptr = new DLFLFaceVertex;

		// Set the Vertex ptr field for the new face vertex
								newfvptr->vertex = vertex_array[v-1];
								if ( vt > 0 ) newfvptr->texcoord = texcoords[vt-1];
								if ( vn > 0 ) newfvptr->normal = normals[vn-1];

		// Add this new DLFLFaceVertexPtr to the new DLFLFace
								newfptr->addVertexPtr(newfvptr);
							}

	// Set material for new face to be current material and
	// add pointer to new face to current material
							newfptr->setMaterial(cur_mtl);

	// Add new face to the face list
							addFacePtr(newfptr);

	// Get the edges from the new face
							num_edges = newfptr->getEdges(&edges);

	// Add the edges from the new face
							addEdges(edges,num_edges);

	// Delete the Edge array allocated by getEdges, since addEdges makes a copy
							delete [] edges; edges = NULL;
						}
						if ( c2 != '\n' ) readTillEOL(i);
					}

		// Clear the temporary vertex array
					vertex_array.clear();

		// Make all Vertexes, Edges and Faces unique
					makeUnique();

		// update all the EdgePtr fields for the Faces through the Edges
					updateEdgeList();

		// update all the FacePtr fields for the FaceVertexes through the Faces
					updateFaceList();
				}

				void DLFLObject::writeObject(ostream& o, bool with_normals, bool with_tex_coords) {
		// Write out the DLFL object as an OBJ file into the given output stream

		// First make the Position ID's unique for the VertexList so Vertex IDs will
		// be contiguous and monotonically increasing
		// Don't Do this, that way the IDs stay the same
					for_each(vertex_list.begin(),vertex_list.end(),makeVertexUnique);

		// Get the Position ID for the first Vertex in the list
		// -1 is because OBJ file indices start at 1 and not 0
					uint min_id = vertex_list.front()->getID() - 1;

		// Output the Vertex list
					DLFLVertexPtrList::const_iterator vf = vertex_list.begin(), vl = vertex_list.end();
					while ( vf != vl ) {
						o << *(*vf);
						++vf;
					}

					o << "# " << vertex_list.size() << " vertices" << endl << endl;

					DLFLFacePtrList::const_iterator ff, fl = face_list.end();

					if ( with_normals ) {
						ff = face_list.begin();
			// Output the normals for each FaceVertex in each Face
						while ( ff != fl ) {
							(*ff)->objWriteNormals(o);
							++ff;
						}
					}
					if ( with_tex_coords ) {
						ff = face_list.begin();
			// Output the texture coordinates for each FaceVertex in each Face
						while ( ff != fl ) {
							(*ff)->objWriteTexCoords(o);
							++ff;
						}
					}

					ff = face_list.begin();
					if ( with_normals ) {
						uint normal_id_start = 1;
						if ( with_tex_coords ) {
	// Output the Face list with normals and texture coords
							uint tex_id_start = 1;
							while ( ff != fl ) {
								(*ff)->objWriteWithNormalsAndTexCoords(o,min_id,normal_id_start,tex_id_start);
								++ff;
							}
						} else {
	// Output the Face list with normals only
							while ( ff != fl )
							{
								(*ff)->objWriteWithNormals(o,min_id,normal_id_start);
								++ff;
							}
						}
					}
					else if ( with_tex_coords ) {
			// Output the face list with texture coords but without normals
						uint tex_id_start = 1;
						while ( ff != fl ) {
							(*ff)->objWriteWithTexCoords(o,min_id,tex_id_start);
							++ff;
						}
					} else {
			// Output the Face list without normals or texture coordinates
						while ( ff != fl ) {
							(*ff)->objWrite(o,min_id);
							++ff;
						}
					}

					o << "# " << face_list.size() << " faces" << endl << endl;
				}

				void DLFLObject::readDLFL(istream& i, bool clearold) {
		// Read the object from an input stream in DLFL format

					if ( !i ) {
						cerr  << "Incomplete DLFL file." << endl;
						return;
					}

		// Clear the object first if flag is set
		// Otherwise new vertices,faces and edges will be appended to the existing lists
					if ( clearold ) reset();

					DLFLVertexPtr newvptr;
					DLFLFaceVertexPtr newfvptr, fvptr;
					DLFLEdgePtr neweptr;
					DLFLFacePtr newfptr;
					DLFLMaterialPtr cur_mtl = matl_list.front();
					Vector3d xyz;
					char c,c2;
					uint vindex, fvindex, fvindex1, fvindex2;

		// First line of the file should be "DLFL"
					const char dlfl[] = "DLFL";
					char compare[4];
					i.read(compare,4);
					if( strncmp(dlfl,compare,4) != 0 ) {
						cerr << "File not in DLFL format" << endl;
						return;
					}
					readTillEOL(i); c = ' ';

		// Read the vertices first. Stop when we get to a '#' sign at the beginning of a line
					while ( i && c != '#' ) {
						i.get(c); i.get(c2);
						if ( c == 'v' && c2 == ' ') {

	// Read a vertex specification
							i >> xyz; readTillEOL(i);

	// Create a new DLFLVertex
							newvptr = new DLFLVertex(xyz);

	// Add the pointer to the VertexPtr list. The list will free the memory
							addVertexPtr(newvptr);

	// Add the pointer to the local vertex_array
	// for easy access when creating the face vertices
							vertex_array.push_back(newvptr);
						}
					}

					if ( !i ) {
						cerr << "Incomplete DLFL file" << endl;
						reset();
						return;
					}

		// Read the face vertices next. Stop when we get to a '#' sign at the beginning of a line
					if ( c2 != '\n' ) readTillEOL(i);
					c = ' ';
					while ( i && c != '#') {
						i.get(c); i.get(c2);
						if ( c == 'f' && c2 == 'v' ) {
	// Read a face vertex specification
							newfvptr = new DLFLFaceVertex;

							vindex = newfvptr->readDLFL(i); readTillEOL(i);
							newfvptr->vertex = vertex_array[vindex];

	// Add the pointer to the local face_vertex_array
	// for eash access when creating the faces and edges
							face_vertex_array.push_back(newfvptr);
						}
					}

					if ( !i ) {
						cerr << "Incomplete DLFL file" << endl;
						reset();
						return;
					}

		// Read the edges next. Stop when we get to a '#' sign at the beginning of a line
					if ( c2 != '\n' ) readTillEOL(i);

					c = ' ';
					while ( i && c != '#') {
						i.get(c); i.get(c2);
						if ( c == 'e' && c2 == ' ' ) {

	// Read a edge specification
							neweptr = new DLFLEdge;

							i >> fvindex1 >> fvindex2; readTillEOL(i);

	// Set the ends of the edge using the face vertex array
							neweptr->setFaceVertexPointers(face_vertex_array[fvindex1],face_vertex_array[fvindex2],false);

	// Update the EdgePtr fields of the face vertices
							neweptr->updateFaceVertices();

	// Add the edge to the edge list
							addEdgePtr(neweptr);
						}
					}

					if ( !i ) {
						cerr << "Incomplete DLFL file" << endl;
						reset();
						return;
					}

			// Read the faces next. Stop when we get to a '#' sign at the beginning of a line
					if ( c2 != '\n' ) readTillEOL(i);
					c = ' ';
					while ( i && c != '#')
					{
						i.get(c); i.get(c2);
						if ( c == 'f' && c2 == ' ' )
						{
								// Read a face specification
							newfptr = new DLFLFace;

								// Read a face specification. Get the face vertex index and find
								// the face vertex pointer from the array and add it to the face
							c = i.peek(); 
							while ( c != '\n' )
							{
								i >> fvindex;
								fvptr = face_vertex_array[fvindex];
								newfptr->addVertexPtr(fvptr);
								c = i.peek();
							}
							readTillEOL(i);

								// Set material for new face to be current material and
								// add pointer to new face to current material
							newfptr->setMaterial(cur_mtl);

								// Update the FacePtr fields of the face vertices
								// and add the face vertices to the face vertex list of vertices
							newfptr->updateFacePointers();
							newfptr->addFaceVerticesToVertices();

								// Add the face to the face list
							addFacePtr(newfptr);
						}
					}

		// Clear the temporary vertex array and face vertex array
					vertex_array.clear();
					face_vertex_array.clear();

					assignID();

		// Make all Vertexes, Edges and Faces unique
					makeUnique();
				}

				void DLFLObject::writeDLFL(ostream& o, bool reverse_faces) {
		// Write the object in DLFL format into give output stream

		// Write marker at beginning indicating DLFL format
					o << "DLFL" << endl;

		// Write the vertex list next. Update the vertex index also
					DLFLVertexPtrList::iterator vf = vertex_list.begin(), vl = vertex_list.end();
					uint vindex = 0;
					while ( vf != vl ) {
						(*vf)->writeDLFL(o,vindex++);
						++vf;
					}
					o << '#' << endl;

		// Write the face vertices and update the face vertex index also
					DLFLFacePtrList::iterator ff = face_list.begin(), fl = face_list.end();
					DLFLFacePtr fptr;
					uint fvindex = 0;
					while ( ff != fl ) {
						fptr = (*ff);
						DLFLFaceVertexPtr head;
						head = fptr->front();
						if ( head ) {
							DLFLFaceVertexPtr current = head;
							current->writeDLFL(o,fvindex++);
							current = current->next();
							while ( current != head ) {
								current->writeDLFL(o,fvindex++);
								current = current->next();
							}
						}
						++ff;
					}
					o << '#' << endl;

		// Write the edge list
					DLFLEdgePtrList::iterator ef = edge_list.begin(), el = edge_list.end();
					if ( reverse_faces ) {
						while ( ef != el ) {
							(*ef)->writeDLFLReverse(o);
							++ef;
						}
					}
					else {
						while ( ef != el ) {
							(*ef)->writeDLFL(o);
							++ef;
						}
					}

					o << '#' << endl;

		// Write the face list
					ff = face_list.begin(); fl = face_list.end();
					if ( reverse_faces ) {
						while ( ff != fl ) {
							(*ff)->writeDLFLReverse(o);
							++ff;
						}
					}
					else {
						while ( ff != fl ) {
							(*ff)->writeDLFL(o);
							++ff;
						}
					}
					o << '#' << endl;
				}

	void DLFLObject::writeLG3d(ostream& o, bool selected) {

		Vector3dArray coords; int i=0, j=0;		
		if (selected){
			vector<DLFLFacePtr>::iterator ff = this->sel_fptr_array.begin(), 
																		fl = this->sel_fptr_array.end();
			// Write the object in LG3d (*.m) format for use with the LiveGraphics3D live.jar java archive from Mathworld.com
			o << "Graphics3D[{";

			while ( ff != fl ) {
				//format: Polygon[{ {x,y,z}, {x,y,z}, {x,y,z}, {x,y,z} }],
				o << "Polygon[{";
				(*ff)->getVertexCoords(coords);
				for (i=0; i < coords.size(); i++){
					o << "{" << coords[i][0] << "," << coords[i][1] << "," << coords[i][2] << "}"; 
					if (i != coords.size()-1){
						o << ",";
					}
				}
				o << "}]";
				if ( j < num_faces()-1){
					o << ",";
				}
				++ff; j++;
			}
			//close out the file and make sure it doesn't have a "box" around it
			o << "}, { Boxed -> False }]";
			
		}
		else {
			DLFLFacePtrList::iterator ff, fl;
			ff = face_list.begin(); 
			fl = face_list.end();
			// Write the object in LG3d (*.m) format for use with the LiveGraphics3D live.jar java archive from Mathworld.com
			o << "Graphics3D[{";

			while ( ff != fl ) {
				//format: Polygon[{ {x,y,z}, {x,y,z}, {x,y,z}, {x,y,z} }],
				o << "Polygon[{";
				(*ff)->getVertexCoords(coords);
				for (i=0; i < coords.size(); i++){
					o << "{" << coords[i][0] << "," << coords[i][1] << "," << coords[i][2] << "}"; 
					if (i != coords.size()-1){
						o << ",";
					}
				}
				o << "}]";
				if ( j < num_faces()-1){
					o << ",";
				}
				++ff; j++;
			}
			//close out the file and make sure it doesn't have a "box" around it
			o << "}, { Boxed -> False }]";
		}
	}

} // end namespace
