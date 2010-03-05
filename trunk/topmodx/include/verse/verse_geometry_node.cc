/**
 * $Id$
 *
 * ***** BEGIN GPL/BL DUAL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version. The Blender
 * Foundation also sells licenses for use in proprietary software under
 * the Blender License.  See http://www.blender.org/BL/ for information
 * about this.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Contributor(s): Jiri Hnidek.
 *
 * ***** END GPL/BL DUAL LICENSE BLOCK *****
 */

/*#ifdef WITH_VERSE*/

#include <string.h>

#include "MEM_guardedalloc.h"

#include "DNA_listBase.h"

#include "TLI_dynamiclist.h"
#include "TLI_toplib.h"
#include "TLI_arithb.h"

#include "TKE_verse.h"
#include "TKE_utildefines.h"

#include "TIF_verse.h"

#include "verse.h"

/* function prototypes of static functions */

/* callback functions */
static void cb_g_layer_create(void *user_data, VNodeID node_id, VLayerID layer_id, const char *name, VNGLayerType type, uint32 def_integer, real64 def_real);
static void cb_g_layer_destroy(void *user_data, VNodeID node_id, VLayerID layer_id);
static void cb_g_vertex_set_xyz_real32(void *user_data, VNodeID node_id, VLayerID layer_id, uint32 vertex_id, real32 x, 	real32 y, real32 z);
static void cb_g_polygon_set_corner_uint32(void *user_data, VNodeID node_id, VLayerID layer_id, uint32 polygon_id, uint32 v0, uint32 v1, uint32 v2, uint32 v3);
static void cb_g_vertex_delete_real32(void *user_data, VNodeID node_id, uint32 vertex_id);
static void cb_g_polygon_delete(void *user_data, VNodeID node_id, uint32 polygon_id);
static void cb_g_crease_set_edge(void *user_data, VNodeID node_id, const char *layer, uint32 def_crease);
static void cb_g_crease_set_vertex(void *user_data, VNodeID node_id, const char *layer, uint32 def_crease);

/* other static functions */

static VLayer *create_vlayer(struct VerseSession *session, VNodeID node_id, VLayerID layer_id, const char *name, VNGLayerType type, uint32 def_integer, real64 def_real);

static uint8_item *get_face_smooth_item(struct VerseFace *vface);
static void free_unneeded_verseverts_of_verseface(struct VNode *vnode, struct VerseFace *vface);
static void free_verse_vertex(struct VLayer *vlayer, struct VerseVert *vvert);
static void free_verse_face(struct VLayer *vlayer, struct VerseFace *vface);
static void free_verse_layer_data(struct VNode *vnode, struct VLayer *vlayer);

static void send_verse_layer(struct VLayer *vlayer);
static void send_verse_face(struct VerseFace *vface);

static VerseVert* find_verse_vert_in_queue(struct VLayer *vlayer, VNodeID node_id, VLayerID layer_id, uint32 vertex_id, real32 x, real32 y, real32 z);
static VerseFace* find_verse_face_in_queue(struct VLayer *vlayer, VNodeID node_id, VLayerID layer_id, uint32 polygon_id, uint32 v0, uint32 v1, uint32 v2, uint32 v3);

static unsigned short test_incoming_verseface(struct VGeomData *geom, struct VerseFace *vface);
static void find_unsent_faces(struct VNode *vnode, struct VerseVert *vvert);
static void find_vlayer_orphans(struct VNode *vnode, struct VerseVert *vvert);
static void move_face_orphan_to_dlist(struct VNode *vnode, struct VLayer *vlayer, struct VerseFace *vface);
static void increase_verse_verts_references(struct VerseFace *vface);
static void recalculate_verseface_normals(struct VNode *vnode);

/*
 * recalcute normals of all VerseFaces
 */
static void recalculate_verseface_normals(VNode *vnode)
{
	struct VLayer *vert_layer, *face_layer;
	struct VerseFace *vface;
	struct VerseVert *vvert;

	if(vnode->type != V_NT_GEOMETRY) return;

	vert_layer = find_verse_layer_type((VGeomData*)vnode->data, VERTEX_LAYER);
	face_layer = find_verse_layer_type((VGeomData*)vnode->data, POLYGON_LAYER);

	vvert = (VerseVert*)vert_layer->dl.lb.first;
	while(vvert) {
		vvert->no[0] = vvert->no[1] = vvert->no[2] = 0.0;
		vvert = vvert->next;
	}

	vface = (VerseFace*)face_layer->dl.lb.first;
	while(vface) {
		/* calculate face normals */
/*		if(vface->vvert3) {
			CalcNormFloat4(vface->vvert0->co, vface->vvert1->co,
					vface->vvert2->co, vface->vvert3->co, vface->no);
			VecAddf(vface->vvert3->no, vface->vvert3->no, vface->no);
		}
		else
			CalcNormFloat(vface->vvert0->co, vface->vvert1->co,
					vface->vvert2->co, vface->no);
*/
		/* calculate vertex normals ... it is averadge of all face normals using the vertex */
/*		VecAddf(vface->vvert0->no, vface->vvert0->no, vface->no);*/
/*		VecAddf(vface->vvert1->no, vface->vvert1->no, vface->no);*/
/*		VecAddf(vface->vvert2->no, vface->vvert2->no, vface->no);*/

		vface = vface->next;
	}

	/* we have to normalise all vertex normals */
	vvert = (VerseVert*)vert_layer->dl.lb.first;
	while(vvert) {
/*		Normalise(vvert->no);*/
		vvert = vvert->next;
	}
}

/*
 * add item to the queue and send it if possible
 */
void add_item_to_send_queue(ListBase *lb, void *item, unsigned int type)
{
	TLI_addtail(lb, item);
	switch (type) {
		case VERSE_NODE:
			if(lb->first==lb->last) send_verse_node((VNode*)item);
			break;
		case VERSE_LINK:
			if(((VLink*)item)->flag & LINK_SEND_READY) send_verse_link((VLink*)item);
			break;
		case VERSE_LAYER:
			send_verse_layer((VLayer*)item);
			break;
		case VERSE_VERT:
			send_verse_vertex((VerseVert*)item);
			break;
		case VERSE_FACE:
			if(((VerseFace*)item)->flag & FACE_SEND_READY)
				send_verse_face((VerseFace*)item);
			break;
		case VERSE_TAG:
			send_verse_tag((VTag*)item);
			break;
		case VERSE_TAG_GROUP:
			send_verse_taggroup((VTagGroup*)item);
			break;
	}
}

/*
 * return VerseLayer with certain content (vertexes, polygons, in the
 * future: weight, red color, etc.)
 */
VLayer* find_verse_layer_type(VGeomData *geom, short content)
{
	struct VLayer *vlayer = NULL;
	
	switch(content) {
		case VERTEX_LAYER:
			/* VERTEX_LAYER equals 0 and vertex layer is
			 * always in 1st layer */
			vlayer = (VLayer*)geom->layers.da.items[VERTEX_LAYER];
			break;
		case POLYGON_LAYER:
			/* POLYGON_LAYER equals 1 and vertex layer is
			 * always in 2nd layer */
			vlayer = (VLayer*)geom->layers.da.items[POLYGON_LAYER];
			break;
		case POLYGON_SMOOTH_LAYER:
			/* POLYGON_SMOOTH_LAYER can be in various layer
			 * we store id of this layer in geom node
			 * variable, when this layer is created */
			vlayer = (VLayer*)TLI_dlist_find_link(&(geom->layers), geom->smooth_layer);
			break;
	}

	return vlayer;
}

/*
 * increase references of VerseVerts of new VerseFace
 */
static void increase_verse_verts_references(VerseFace *vface)
{
	struct VerseSession *session = (VerseSession*)current_verse_session();
	struct VNode *vnode;
	struct VLayer *vlayer;

	if(!session) return;

	vnode = (VNode*)TLI_dlist_find_link(&(session->nodes), vface->node_id);

	vlayer = find_verse_layer_type((VGeomData*)vnode->data, VERTEX_LAYER);

	if(vface->vvert0) vface->vvert0->counter++;
	if(vface->vvert1) vface->vvert1->counter++;
	if(vface->vvert2) vface->vvert2->counter++;
	if(vface->vvert3) vface->vvert3->counter++;
}

/*
 * move VerseFace from list of orphans to dlist of VerseFaces (if VerseFace was only changed
 * then this VerseFace is only removed from list of orphans)
 */
static void move_face_orphan_to_dlist(VNode *vnode, VLayer *vlayer, VerseFace *vface)
{
	/* remove vface from list of orphans */
	TLI_remlink(&(vlayer->orphans), vface);
	/* increase references of all vertexes beying part of this face*/
	increase_verse_verts_references(vface);

	if(vface->flag & FACE_RECEIVED) {
		/* set up vface flag */
		vface->flag &= ~FACE_RECEIVED;
		/* move vface to dynamic list of faces */
		TLI_dlist_add_item_index(&(vlayer->dl), (void*)vface, vface->id);
		/* try to set up pointer at face value */
		if(!(vface->smooth)) vface->smooth = get_face_smooth_item(vface);
		/* recalculate all vertex and faces normals */
		recalculate_verseface_normals(vnode);
		/* post create action (change local data) */
		((VGeomData*)vnode->data)->post_polygon_create(vnode, vface);
	}
	else if(vface->flag & FACE_CHANGED) {
		/* set up vface flag */
		vface->flag &= ~FACE_CHANGED;
		/* move vface to dynamic list of faces */
		TLI_dlist_add_item_index(&(vlayer->dl), (void*)vface, vface->id);
		/* recalculate all vertex and faces normals */
		recalculate_verseface_normals(vnode);
		/* post create action (change local data) */
		((VGeomData*)vnode->data)->post_polygon_set_corner(vnode, vface);
	}
}

/*
 * find all VerseFaces waiting in queue, which needs id of new VerseVert
 */
static void find_unsent_faces(VNode *vnode, VerseVert *vvert)
{
	VLayer *vlayer;
	VerseFace *vface, *next_vface;

	vlayer = find_verse_layer_type((VGeomData*)vnode->data, POLYGON_LAYER);

	if(vlayer) {
		vface = (VerseFace*)vlayer->queue.first;
		while(vface) {
			next_vface = vface->next;
			if(vface->vvert0==vvert) {
				vface->v0 = vvert->id;
				vface->counter--;
			}
			else if(vface->vvert1==vvert) {
				vface->v1 = vvert->id;
				vface->counter--;
			}
			else if(vface->vvert2==vvert) {
				vface->v2 = vvert->id;
				vface->counter--;
			}
			else if(vface->vvert3==vvert){
				vface->v3 = vvert->id;
				vface->counter--;
			}

			if(vface->counter<1 && !(vface->flag & FACE_SENT)) {
				/* when VerseFace was only changed, then it is neccessary
				 * put it back to list of VerseFaces*/
				if(vface->flag & FACE_CHANGED) {
					TLI_remlink(&(vlayer->queue), vface);
					TLI_dlist_add_item_index(&(vlayer->dl), vface, vface->id);
				}
				send_verse_face(vface);
			}

			vface = next_vface;
		}
	}
}

/*
 * find all VerseFace orphans, which needs incoming VerseVert
 */
static void find_vlayer_orphans(VNode *vnode, VerseVert *vvert)
{
	VLayer *vlayer;
	VerseFace *vface, *next_vface;
	unsigned int vertex_id = vvert->id;

	vlayer = find_verse_layer_type((VGeomData*)vnode->data, POLYGON_LAYER);

	if(vlayer) {
		vface = (VerseFace*)vlayer->orphans.first;
		while(vface){
			next_vface = vface->next;
			if(vface->v0 == vertex_id) {
				vface->vvert0 = vvert;
				vface->counter--;
			}
			else if(vface->v1 == vertex_id) {
				vface->vvert1 = vvert;
				vface->counter--;
			}
			else if(vface->v2 == vertex_id) {
				vface->vvert2 = vvert;
				vface->counter--;
			}
			else if(vface->v3 == vertex_id) {
				vface->vvert3 = vvert;
				vface->counter--;
			}
			if(vface->counter<1) {
				/* moving VerseFace orphan to dlist */
				move_face_orphan_to_dlist(vnode, vlayer, vface);
			}
			vface = next_vface;
		}
	}
}

/*
 * return number of VerseVerts missing to incoming VerseFace, set up pointers
 * at VerseVerts
 */
static unsigned short test_incoming_verseface(VGeomData *geom, VerseFace *vface)
{
	struct VLayer *vert_layer;
	struct VerseVert *vvert; 
	int counter=0;

	vert_layer = find_verse_layer_type(geom, VERTEX_LAYER);

	if(vface->v0 != -1){
		vvert = (VerseVert*)TLI_dlist_find_link(&(vert_layer->dl), vface->v0);
		if(vvert==NULL) counter++;
		else vface->vvert0 = vvert;
	}
	if(vface->v1 != -1){
		vvert = (VerseVert*)TLI_dlist_find_link(&(vert_layer->dl), vface->v1);
		if(vvert==NULL) counter++;
		else vface->vvert1 = vvert;
	}
	if(vface->v2 != -1){
		vvert = (VerseVert*)TLI_dlist_find_link(&(vert_layer->dl), vface->v2);
		if(vvert==NULL) counter++;
		else vface->vvert2 = vvert;
	}
	if(vface->v3 != -1){
		vvert = (VerseVert*)TLI_dlist_find_link(&(vert_layer->dl), vface->v3);
		if(vvert==NULL) counter++;
		else vface->vvert3 = vvert;
	}
	
	return counter;
}

/*
 * try to find VerseFace in queue
 */
static VerseFace* find_verse_face_in_queue(
		VLayer *vlayer,
		VNodeID node_id,
		VLayerID layer_id,
		uint32 polygon_id,
		uint32 v0,
		uint32 v1,
		uint32 v2,
		uint32 v3)
{
	struct VerseFace *vface = (VerseFace*)vlayer->queue.first;

	while(vface){
		if((vface->v0==v0) && (vface->v1==v1) && (vface->v2==v2) && (vface->v3==v3)){
			vface->id = polygon_id;
			vface->layer_id = layer_id;
			return vface;
		}
		vface = vface->next;
	}
	return NULL;
}

/*
 * try to find VerseVert in queue
 */
static VerseVert* find_verse_vert_in_queue(
		VLayer *vlayer,
		VNodeID node_id,
		VLayerID layer_id,
		uint32 vertex_id,
		real32 x,
		real32 y,
		real32 z)
{
	VerseVert *vvert=NULL;

	vvert= (VerseVert*)vlayer->queue.first;

	while(vvert){
		if((vvert->node_id == node_id) && (vvert->co[0] == x) && (vvert->co[1] == y) && (vvert->co[2] == z))
		{
			/* set layers */
			vvert->id = vertex_id;
			vvert->layer_id = layer_id;

			return vvert;
		}
		vvert = vvert->next;
	}

	return NULL;
}

/*
 * send delete command to verse server
 */
void send_verse_vertex_delete(VerseVert *vvert)
{
	verse_session_set(vvert->vsession);

	verse_send_g_vertex_delete_real32(vvert->node_id, vvert->id);
}

/*
 * send VerseLayer to verse server
 */
static void send_verse_layer(VLayer *vlayer)
{
	verse_session_set(vlayer->vsession);

	verse_send_g_layer_create(
			vlayer->node_id,
			vlayer->id,
			vlayer->name,
			vlayer->type,
			vlayer->def_int,
			vlayer->def_real);
}

/* 
 * send VerseVert to verse server
 */
void send_verse_vertex(VerseVert *vvert)
{
	verse_session_set(vvert->vsession);

	vvert->flag &= ~VERT_SEND_READY;

	verse_send_g_vertex_set_xyz_real32(
			vvert->node_id,
			vvert->layer_id,
			vvert->id,
			vvert->co[0],
			vvert->co[1],
			vvert->co[2]);
}

/*
 * if exists VerseLayer containing smoothness, then this function try
 * to return pointer at coresponding item storing information about
 * face smoothness for this VerseFace
 */
static uint8_item *get_face_smooth_item(VerseFace *vface)
{
	struct VerseSession *session;
	struct VNode *vnode;
	struct VLayer *vlayer;

	verse_session_set(vface->vsession);
	session = (VerseSession*)current_verse_session();
	vnode = (VNode*)TLI_dlist_find_link(&(session->nodes), vface->node_id);
	vlayer = find_verse_layer_type((VGeomData*)vnode->data, POLYGON_SMOOTH_LAYER);
	if(vlayer) return (uint8_item*)TLI_dlist_find_link(&(vlayer->dl), vface->id);
	else return NULL;
}

/*
 * send face smoothnes to verse server
 */
void send_verse_face_smoothness(VerseFace *vface, uint8 value)
{
	struct VerseSession *session;
	struct VNode *vnode;
	struct VLayer *smooth_layer;

	verse_session_set(vface->vsession);
	session = (VerseSession*)current_verse_session();
	vnode = (VNode*)TLI_dlist_find_link(&(session->nodes), vface->node_id);
	smooth_layer = find_verse_layer_type((VGeomData*)vnode->data, POLYGON_SMOOTH_LAYER);

	verse_send_g_polygon_set_face_uint8(
			vface->node_id,
			smooth_layer->id,
			vface->id,
			value);
}

/*
 * send delete command to verse server
 */
void send_verse_face_delete(VerseFace *vface)
{
	verse_session_set(vface->vsession);

	vface->flag |= FACE_DELETED;

	verse_send_g_polygon_delete(vface->node_id, vface->id);
}

/*
 * send VerseFace to verse server
 */
static void send_verse_face(VerseFace *vface)
{
	verse_session_set(vface->vsession);

	vface->flag |= FACE_SENT;

	if(vface->v3 != -1) {
		verse_send_g_polygon_set_corner_uint32(
				vface->node_id,
				vface->layer_id,
				vface->id,
				vface->v0,
				vface->v3,	/* verse use clock-wise winding */
				vface->v2,
				vface->v1);	/* verse use clock-wise winding */
	}
	else {
		verse_send_g_polygon_set_corner_uint32(
				vface->node_id,
				vface->layer_id,
				vface->id,
				vface->v0,
				vface->v2,	/* verse use clock-wise winding */
				vface->v1,	/* verse use clock-wise winding */
				vface->v3);
	}
}

/*
 * free VerseVert (and blender vert)
 */
static void free_verse_vertex(VLayer *vlayer, VerseVert *vvert)
{
	/* free VerseVert */
	TLI_dlist_free_item(&(vlayer->dl), (unsigned int)vvert->id);
}

/*
 * free VerseFace (and blender face)
 */
static void free_verse_face(VLayer *vlayer, VerseFace *vface)
{
	/* free VerseFace */
	TLI_dlist_free_item(&(vlayer->dl), (unsigned int)vface->id);

}

/*
 * free VerseLayer data
 */
static void free_verse_layer_data(VNode *vnode, VLayer *vlayer)
{
	struct VerseFace *vface;
	struct VerseVert *vvert;

	/* set up EditVert->vvert and EditFace->vface pointers to NULL */
	switch(vlayer->content) {
		case VERTEX_LAYER:
			vvert = (VerseVert*)vlayer->dl.lb.first;
			while(vvert) {
				((VGeomData*)vnode->data)->post_vertex_free_constraint(vnode, vvert);
				vvert = vvert->next;
			}
			break;
		case POLYGON_LAYER:
			vface = (VerseFace*)vlayer->dl.lb.first;
			while(vface) {
				((VGeomData*)vnode->data)->post_polygon_free_constraint(vnode, vface);
				vface = vface->next;
			}
			break;
		default:
			break;
	}
	/* free Verse Layer name */
	MEM_freeN(vlayer->name);
	/* destroy VerseLayer data (vertexes, polygons, etc.) */
	TLI_dlist_destroy(&(vlayer->dl));
	/* free unsent data */
	TLI_freelistN(&(vlayer->queue));
	/* free orphans */
	TLI_freelistN(&(vlayer->orphans));
}

/*
 * free all unneeded VerseVerts waiting for deleting
 */
static void free_unneeded_verseverts_of_verseface(VNode *vnode, VerseFace *vface)
{
	struct VLayer *vlayer;
	struct VerseVert *vverts[4]={NULL, NULL, NULL, NULL};
	int i;

	vverts[0] = vface->vvert0;
	vverts[1] = vface->vvert1;
	vverts[2] = vface->vvert2;
	vverts[3] = vface->vvert3;

	/* find layer containing vertexes */
	vlayer = find_verse_layer_type((VGeomData*)vnode->data, VERTEX_LAYER);

	/* free all "deleted" VerseVert waiting for deleting this VerseFace */
	for(i=0; i<4; i++) {
		if(vverts[i]){
			if((vverts[i]->counter<1) && (vverts[i]->flag & VERT_DELETED)) {
				((VGeomData*)vnode->data)->post_vertex_delete(vnode, vverts[i]);
				free_verse_vertex(vlayer, vverts[i]);
			}
		}
	}
}

/*
 * This function create VerseVert and returns pointer on this vertex
 */
VerseVert* create_verse_vertex(
		VSession vsession,
		VNodeID node_id,
		VLayerID layer_id,
		uint32 vertex_id,
		real32 x,
		real32 y,
		real32 z)
{
	VerseVert *vvert;
	
	vvert = (VerseVert*)MEM_mallocN(sizeof(VerseVert), "VerseVert");
	
	/* verse data */
	vvert->vsession = vsession;
	vvert->node_id = node_id;
	vvert->layer_id = layer_id;
	vvert->id = vertex_id;
	/* position */
	vvert->co[0] = x;
	vvert->co[1] = y;
	vvert->co[2] = z;
	/* normal */
	vvert->no[0] = vvert->no[1] = vvert->no[2] = 0.0;
	/* blender internals */
	vvert->flag = 0;
	vvert->counter = 0;
	vvert->vertex = NULL;

	return vvert;
}

/*
 * this function will create new VerseFace and will return pointer on such Face
 */
VerseFace* create_verse_face(
		VSession vsession,
		VNodeID node_id,
		VLayerID layer_id,
		uint32 polygon_id,
		uint32 v0,
		uint32 v1,
		uint32 v2,
		uint32 v3)
{
	struct VerseFace *vface;

	vface = (VerseFace*)MEM_mallocN(sizeof(VerseFace), "VerseFace");

	/* verse data */
	vface->vsession = vsession;
	vface->node_id = node_id;
	vface->layer_id = layer_id;
	vface->id = polygon_id;

	vface->vvert0 = NULL;
	vface->vvert1 = NULL;
	vface->vvert2 = NULL;
	vface->vvert3 = NULL;

	vface->v0 = v0;
	vface->v1 = v1;
	vface->v2 = v2;
	vface->v3 = v3;

	/* blender data */
	vface->face = NULL;
	vface->flag = 0;
	vface->counter = 4;

	vface->smooth = get_face_smooth_item(vface);

	return vface;
}

/*
 * create and return VerseLayer
 */
static VLayer *create_vlayer(
		VerseSession *session,
		VNodeID node_id,
		VLayerID layer_id,
		const char *name,
		VNGLayerType type,
		uint32 def_integer,
		real64 def_real)
{
	struct VNode *vnode=NULL;
	struct VLayer *vlayer=NULL;

	/* get vnode from the DynamicList due to node_id */
	vnode = (VNode*)TLI_dlist_find_link(&(session->nodes), (unsigned int)node_id);
	if(!vnode) return NULL;

	/* add layer to the DynamicList */
	vlayer = (VLayer*)MEM_mallocN(sizeof(VLayer), "VerseLayer");

	/* store all relevant info to the vlayer and set up vlayer */
	vlayer->vsession = session->vsession;
	vlayer->node_id = node_id;
	vlayer->id = layer_id;
	vlayer->name = (char*)MEM_mallocN(sizeof(char)*(sizeof(name)+1),"Verse Layer name");
	strcpy(vlayer->name, name);
	vlayer->type = type;
	vlayer->def_int = def_integer;
	vlayer->def_real = def_real;

	if((type == VN_G_LAYER_VERTEX_XYZ) && (layer_id == 0))
		vlayer->content = VERTEX_LAYER;
	else if((type == VN_G_LAYER_POLYGON_CORNER_UINT32) && (layer_id == 1))
		vlayer->content = POLYGON_LAYER;
	else if((type == VN_G_LAYER_POLYGON_FACE_UINT8) && (strcmp(name, "smooth")==0))
		vlayer->content = POLYGON_SMOOTH_LAYER;	
	else
		vlayer->content = -1;

	/* initialize DynamicList in the vlayer (vertexes, polygons, etc.)*/
	TLI_dlist_init(&(vlayer->dl));
	/* initialization of queue of layer */
	vlayer->queue.first = vlayer->queue.last = NULL;
	/* initialization of list of orphans */
	vlayer->orphans.first = vlayer->orphans.last = NULL;

	/* set up methods */
	vlayer->post_layer_create = post_layer_create;
	vlayer->post_layer_destroy = post_layer_destroy;

	return vlayer;
}

/*
 * create geometry data
 */
VGeomData *create_geometry_data(void)
{
	struct VGeomData *geom;

	geom = (VGeomData*)MEM_mallocN(sizeof(VGeomData), "VerseGeometryData");
	TLI_dlist_init(&(geom->layers));
	geom->vlink = NULL;
	geom->queue.first = geom->queue.last = NULL;
	geom->mesh = NULL;
	geom->editmesh = NULL;

	/* set up methods */
	geom->post_vertex_create = post_vertex_create;
	geom->post_vertex_set_xyz = post_vertex_set_xyz;
	geom->post_vertex_delete = post_vertex_delete;
	geom->post_vertex_free_constraint = post_vertex_free_constraint;
	geom->post_polygon_create = post_polygon_create;
	geom->post_polygon_set_corner = post_polygon_set_corner;
	geom->post_polygon_delete = post_polygon_delete;
	geom->post_polygon_free_constraint = post_polygon_free_constraint;
	geom->post_geometry_free_constraint = post_geometry_free_constraint;
	geom->post_polygon_set_uint8 = post_polygon_set_uint8;

	return geom;
}

/*
 * callback function: vertex crease was set
 */
static void cb_g_crease_set_vertex(
		void *user_data,
		VNodeID node_id,
		const char *layer,
		uint32 def_crease)
{
/*	struct VerseSession *session = (VerseSession*)current_verse_session();
	struct VNode *vnode;*/
}

/*
 * callback function: edge crease was set
 */
static void cb_g_crease_set_edge(
		void *user_data,
		VNodeID node_id,
		const char *layer,
		uint32 def_crease)
{
/*	struct VerseSession *session = (VerseSession*)current_verse_session();
	struct VNode *vnode;*/
}

/*
 * callback function: float value for polygon was set up
 */
static void cb_g_polygon_set_face_real32(
		void *user_def,
		VNodeID node_id,
		VLayerID layer_id,
		uint32 polygon_id,
		real32 value)
{
/*	struct VerseSession *session = (VerseSession*)current_verse_session();
	struct VNode *vnode;*/
}

/*
 * callback function: int values for polygon was set up
 */
static void cb_g_polygon_set_face_uint32(
		void *user_def,
		VNodeID node_id,
		VLayerID layer_id,
		uint32 polygon_id,
		uint32 value)
{
/*	struct VerseSession *session = (VerseSession*)current_verse_session();
	struct VNode *vnode;*/
}

static uint8_item *create_uint8_item(void)
{
	struct uint8_item *item;

	item = (uint8_item*)MEM_mallocN(sizeof(uint8_item), "uint8_item");
	item->value = 0;

	return item;
}

/*
 * callback function: uint8 value for polygon was set up
 */
static void cb_g_polygon_set_face_uint8(
		void *user_def,
		VNodeID node_id,
		VLayerID layer_id,
		uint32 polygon_id,
		uint8 value)
{
	struct VerseSession *session = (VerseSession*)current_verse_session();
	struct VNode *vnode;
	struct VLayer *vlayer, *polygon_layer;
	struct VerseFace *vface;
	struct uint8_item *item;

	if(!session) return;

	/* find needed node (we can be sure, that it is geometry node) */
	vnode = (VNode*)TLI_dlist_find_link(&(session->nodes), (unsigned int)node_id);

	/* find layer containing face smoothness */
	vlayer = (VLayer*)TLI_dlist_find_link(&(((VGeomData*)vnode->data)->layers), (unsigned int)layer_id);

	item = (uint8_item*)TLI_dlist_find_link(&(vlayer->dl), polygon_id);

	if(item) {
		item->value = value;
	}
	else {
		item = create_uint8_item();
		TLI_dlist_add_item_index(&(vlayer->dl), item, polygon_id);
		item->value = value;
	}

	/* find layer containing polygons */
	polygon_layer = find_verse_layer_type((VGeomData*)vnode->data, POLYGON_LAYER);
	/* find VerseFace with id polygon_id */
	vface = (VerseFace*)TLI_dlist_find_link(&(polygon_layer->dl), polygon_id);

	if(vface) {
		if(strcmp(vlayer->name, "smooth")==0) {
			if(!vface->smooth) vface->smooth = item;
		}
		((VGeomData*)vnode->data)->post_polygon_set_uint8(vnode, vlayer, vface);
	}
}

/*
 * callback function: float value for polygon corner was set up
 */
static void cb_g_polygon_set_corner_real32(
		void *user_def,
		VNodeID node_id,
		VLayerID layer_id,
		uint32 polygon_id,
		real32 v0,
		real32 v1,
		real32 v2,
		real32 v3)
{
}

/*
 * callback function: polygon is deleted
 */
static void cb_g_polygon_delete(
		void *user_data,
		VNodeID node_id,
		uint32 polygon_id)
{
	struct VerseSession *session = (VerseSession*)current_verse_session();
	VNode *vnode;
	VLayer *vlayer;
	VerseFace *vface;

	if(!session) return;

	/* find needed node (we can be sure, that it is geometry node) */
	vnode = (VNode*)TLI_dlist_find_link(&(session->nodes), node_id);

	/* find layer containing faces */
	vlayer = (VLayer*)find_verse_layer_type((VGeomData*)vnode->data, POLYGON_LAYER);

	/* find wanted VerseFace */
	vface = (VerseFace*)TLI_dlist_find_link(&(vlayer->dl), polygon_id);

	if(!vface) return;
	
	((VGeomData*)vnode->data)->post_polygon_delete(vnode, vface);

	/* decrease references at coresponding VerseVertexes */
	vface->vvert0->counter--;
	vface->vvert1->counter--;
	vface->vvert2->counter--;
	if(vface->vvert3) vface->vvert3->counter--;

	/* delete unneeded VerseVertexes */
	free_unneeded_verseverts_of_verseface(vnode, vface);
	
	free_verse_face(vlayer, vface);
}

/*
 * callback function: new polygon (face) created
 */
static void cb_g_polygon_set_corner_uint32(
		void *user_data,
		VNodeID node_id,
		VLayerID layer_id,
		uint32 polygon_id,
		uint32 v0,
		uint32 v1,
		uint32 v2,
		uint32 v3)
{
	struct VerseSession *session = (VerseSession*)current_verse_session();
	struct VNode *vnode;
	struct VLayer *vlayer;
	struct VerseFace *vface=NULL;
	
	if(!session) return;

	vnode = (VNode*)TLI_dlist_find_link(&(session->nodes), (unsigned int)node_id);
	if(!vnode) return;

	vlayer = (VLayer*)TLI_dlist_find_link(&(((VGeomData*)vnode->data)->layers), (unsigned int)layer_id);
	if(!vlayer) return;

	if(v3!=-1) { /* quat swap */
		unsigned int v; v = v1; v1 = v3; v3 = v;
	}
	else { /* triangle swap */
		unsigned int v; v = v1; v1 = v2; v2 = v;
	}

	vface = (VerseFace*)TLI_dlist_find_link(&(vlayer->dl), (unsigned int)polygon_id);

	if(!vface) {
		/* try to find VerseFace in list of VerseVaces created by me and set up polygon and
		 * layer ids */
		vface = find_verse_face_in_queue(vlayer, node_id, layer_id, polygon_id, v0, v1, v2, v3);
		
		if(vface){
			/* I creeated this face ... remove VerseFace from queue */
			TLI_remlink(&(vlayer->queue), (void*)vface);
			vface->smooth = get_face_smooth_item(vface);
		}
		else {
			/* some other client created this face*/
			vface = create_verse_face(session->vsession, node_id, layer_id, polygon_id, v0, v1, v2, v3);
		}

		vface->flag &= ~FACE_SENT;

		/* return number of missing verse vertexes */
		vface->counter = test_incoming_verseface((VGeomData*)vnode->data, vface);

		if(vface->counter<1) {
			/* when VerseFace received all needed VerseFaces, then it is moved
			 * to list of VerseFaces */
			TLI_dlist_add_item_index(&(vlayer->dl), (void*)vface, (unsigned int)polygon_id);
			increase_verse_verts_references(vface);
			recalculate_verseface_normals(vnode);
			((VGeomData*)vnode->data)->post_polygon_create(vnode, vface);
		}
		else {
			/* when all needed VerseVertexes weren't received, then VerseFace is moved to
			 * the list of orphans waiting on needed vertexes */
			vface->flag |= FACE_RECEIVED;
			TLI_addtail(&(vlayer->orphans), (void*)vface);
		}
	}
	else {
		/* VerseVertexes of existing VerseFace were changed (VerseFace will use some different
		 * VerseVertexes or it will use them in different order) */
		printf("\tFACE was changed: %d %d %d %d\n", v0, v1, v2, v3);

		/* decrease references of old VerseVertexes and set up new ids */
		if(vface->vvert0->id != v0) {
			vface->vvert0->counter--;
			vface->v0 = v0;
		}
		if(vface->vvert1->id != v1) {
			vface->vvert1->counter--;
			vface->v1 = v1;
		}
		if(vface->vvert2->id != v2) {
			vface->vvert2->counter--;
			vface->v2 = v2;
		}
		if(vface->vvert3) {
			if(vface->vvert3->id != v3) {
				vface->vvert3->counter--;
				vface->v3 = v3;
			}
		}
		else
			vface->v3 = -1;

		/* delete unneeded VerseVertexes */
		free_unneeded_verseverts_of_verseface(vnode, vface);
		
		vface->flag &= ~FACE_SENT;
		vface->flag |= FACE_CHANGED;

		/* set up new pointers at VerseVerts and return number of missing VerseVertexes */
		vface->counter = test_incoming_verseface((VGeomData*)vnode->data, vface);

		if(vface->counter<1) {
			vface->flag &= ~FACE_CHANGED;
			increase_verse_verts_references(vface);
			recalculate_verseface_normals(vnode);
			((VGeomData*)vnode->data)->post_polygon_set_corner(vnode, vface);
		}
		else {
			/* when all needed VerseVertexes weren't received, then VerseFace is added to
			 * the list of orphans waiting on needed vertexes */
			printf("\tchanged FACE was moved to ORPHAN list\n");
			TLI_dlist_rem_item(&(vlayer->dl), vface->id);
			TLI_addtail(&(vlayer->orphans), (void*)vface);
		}
	}
}

/*
 * callback function: float value was set up for VerseVert with vertex_id
 */
static void cb_g_vertex_set_real32(
		void *user_def,
		VNodeID node_id,
		VLayerID layer_id,
		uint32 vertex_id,
		real32 value)
{
/*	struct VerseSession *session = (VerseSession*)current_verse_session();
	struct VNode *vnode;*/
}

/*
 * callback function: int value was set up for VerseVert with vertex_id
 */
static void cb_g_vertex_set_uint32(
		void *user_def,
		VNodeID node_id,
		VLayerID layer_id,
		uint32 vertex_id,
		uint32 value)
{
/*	struct VerseSession *session = (VerseSession*)current_verse_session();
	struct VNode *vnode;*/
}

/*
 * callback function: polygon was deleted
 */
static void cb_g_vertex_delete_real32(
		void *user_data,
		VNodeID node_id,
		uint32 vertex_id)
{
	struct VerseSession *session = (VerseSession*)current_verse_session();
	VNode *vnode=NULL;
	VLayer *vlayer=NULL;
	VerseVert *vvert=NULL;

	if(!session) return;

	vnode = (VNode*)TLI_dlist_find_link(&(session->nodes), (unsigned int)node_id);

	vlayer = (VLayer*)find_verse_layer_type((VGeomData*)vnode->data, VERTEX_LAYER);

	vvert = (VerseVert*)TLI_dlist_find_link(&(vlayer->dl), (unsigned int)vertex_id);

	if(!vvert) return;

	if(vvert->counter < 1) {
		((VGeomData*)vnode->data)->post_vertex_delete(vnode, vvert);
		free_verse_vertex(vlayer, vvert);
	}
	else {
		/* some VerseFace(s) still need VerseVert, it is only marked as deleted */
		vvert->flag |= VERT_DELETED;
	}
}

/*
 * callback function: position of one vertex was changed or new vertex was created
 */
static void cb_g_vertex_set_xyz_real32(
		void *user_data,
		VNodeID node_id,
		VLayerID layer_id,
		uint32 vertex_id,
		real32 x,
		real32 y,
		real32 z)
{
	struct VerseSession *session = (VerseSession*)current_verse_session();
	struct VNode *vnode = NULL;
	struct VLayer *vlayer = NULL;
	struct VerseVert *vvert = NULL;

	if(!session) return;

	vnode = (VNode*)TLI_dlist_find_link(&(session->nodes), (unsigned int)node_id);
	if(!vnode)return;

	vlayer = (VLayer*)TLI_dlist_find_link(&(((VGeomData*)vnode->data)->layers), (unsigned int)layer_id);
	if(!vlayer) return;

	if(vlayer->id == 0) {
		/* try to pick up verse vert from DynamicList */
		vvert = (VerseVert*)TLI_dlist_find_link(&(vlayer->dl), (unsigned int)vertex_id);

		if(vvert) {
			/* set VerseVert flags */
			vvert->flag |= VERT_SEND_READY;

			if((vvert->co[0]!=x) || (vvert->co[1]!=y) || (vvert->co[2]!=z)) {
				vvert->co[0] = x;
				vvert->co[1] = y;
				vvert->co[2] = z;

				recalculate_verseface_normals(vnode);
				((VGeomData*)vnode->data)->post_vertex_set_xyz(vnode, vvert);
			}
		}
		else {
			/* create new verse vert */

			/* test if we are authors of this vertex :-) */
			vvert = find_verse_vert_in_queue(vlayer, node_id, layer_id, vertex_id, x, y, z);

			if(vvert) {
				/* remove vert from queue */
				TLI_remlink(&(vlayer->queue), (void*)vvert);
				/* add vvert to the dynamic list */
				TLI_dlist_add_item_index(&(vlayer->dl), (void*)vvert, (unsigned int)vertex_id);
				/* set VerseVert flags */
				vvert->flag |= VERT_RECEIVED;
				vvert->flag |= VERT_SEND_READY;
				/* find VerseFaces orphans */
				find_vlayer_orphans(vnode, vvert);
				/* find unsent VerseFaces */
				find_unsent_faces(vnode, vvert);
			}
			else {
				/* create new VerseVert */
				vvert = create_verse_vertex(session->vsession, node_id, layer_id, vertex_id, x, y, z);
				/* add VerseVert to list of VerseVerts */
				TLI_dlist_add_item_index(&(vlayer->dl), (void*)vvert, (unsigned int)vertex_id);
				/* set VerseVert flags */
				vvert->flag |= VERT_RECEIVED;
				vvert->flag |= VERT_SEND_READY;
				/* find VerseFaces orphans */
				find_vlayer_orphans(vnode, vvert);
			}

			((VGeomData*)vnode->data)->post_vertex_create(vnode, vvert);
		}
	}
}

/*
 * callback function for destroyng of verse layer
 */
static void cb_g_layer_destroy(
		void *user_data,
		VNodeID node_id,
		VLayerID layer_id)
{
	struct VerseSession *session = (VerseSession*)current_verse_session();
	struct VNode *vnode;
	struct VLayer *vlayer;

	if(!session) return;

	vnode = (VNode*)TLI_dlist_find_link(&(session->nodes), node_id);
	vlayer = (VLayer*) TLI_dlist_find_link(&(((VGeomData*)vnode->data)->layers), layer_id);

	if(vlayer){
		/* free VerseLayer data */
		free_verse_layer_data(vnode, vlayer);
		/* remove VerseLayer from list of verse layers */
		TLI_dlist_rem_item(&(((VGeomData*)vnode->data)->layers), layer_id);
		/* do client dependent actions */
		vlayer->post_layer_destroy(vnode, vlayer);
		/* free vlayer itself */
		MEM_freeN(vlayer);
	}

}

/*
 * callback function: new layer was created
 */
static void cb_g_layer_create(
		void *user_data,
		VNodeID node_id,
		VLayerID layer_id,
		const char *name,
		VNGLayerType type,
		uint32 def_integer,
		real64 def_real)
{
	struct VerseSession *session = (VerseSession*)current_verse_session();
	struct VNode *vnode=NULL;
	struct VLayer *vlayer=NULL;

	if(!session) return;

	/* find node of this layer*/
	vnode = (VNode*)TLI_dlist_find_link(&(session->nodes), node_id);

	if(!vnode) return;

	/* temp hack due to siggraph presentation ... client subscribe for all verse layers */
	verse_send_g_layer_subscribe(node_id, layer_id, (VNRealFormat)0);

	/* create new VerseLayer */
	vlayer = create_vlayer(session, node_id, layer_id, name, type, def_integer, def_real);
	/* add layer to the list of VerseLayers */
	TLI_dlist_add_item_index(&(((VGeomData*)vnode->data)->layers), (void*)vlayer, (unsigned int)layer_id);

	/* if we are owners of VerseNode, then push geometry to verse server */
	if(vnode->owner_id == VN_OWNER_MINE) {
		switch(type){
			case VN_G_LAYER_VERTEX_XYZ:
				if(layer_id==0) push_all_verts(vnode, vlayer);
				break;
			case VN_G_LAYER_POLYGON_CORNER_UINT32:
				if(layer_id==1) push_all_faces(vnode, vlayer);
				break;
			case VN_G_LAYER_VERTEX_UINT32:
			case VN_G_LAYER_VERTEX_REAL:
			case VN_G_LAYER_POLYGON_CORNER_REAL:
				break;
			case VN_G_LAYER_POLYGON_FACE_UINT8:
				if(strcmp(name, "smooth")==0)
					((VGeomData*)vnode->data)->smooth_layer = layer_id;
				break;
			case VN_G_LAYER_POLYGON_FACE_UINT32:
			case VN_G_LAYER_POLYGON_FACE_REAL:
				break;
		}
	}
	else {
		switch(type) {
			case VN_G_LAYER_VERTEX_XYZ:
			case VN_G_LAYER_POLYGON_CORNER_UINT32:
			case VN_G_LAYER_VERTEX_UINT32:
			case VN_G_LAYER_VERTEX_REAL:
			case VN_G_LAYER_POLYGON_CORNER_REAL:
				break;
			case VN_G_LAYER_POLYGON_FACE_UINT8:
				if(strcmp(name, "smooth")==0)
					((VGeomData*)vnode->data)->smooth_layer = layer_id;
				break;
			case VN_G_LAYER_POLYGON_FACE_UINT32:
			case VN_G_LAYER_POLYGON_FACE_REAL:
				break;
		}
	}

	vlayer->post_layer_create(vnode, vlayer);
}

/*
 * this function will send destroy commands for all VerseVertexes and
 * VerseFaces to verse server, but it will not send destroy commands
 * for VerseLayers or geometry node, it can be used in other functions
 * (undo, destroy geom node, some edit mesh commands, ... ), parameter of
 * this function has to be geometry verse node
 */
void destroy_geometry(VNode *vnode)
{
	struct VLayer *vert_vlayer, *face_vlayer;
	struct VerseFace *vface;
	struct VerseVert *vvert;

	if(vnode->type != V_NT_GEOMETRY) return;

	face_vlayer = find_verse_layer_type((VGeomData*)vnode->data, POLYGON_LAYER);
	vface = (VerseFace*)face_vlayer->dl.lb.first;

	while(vface) {
		send_verse_face_delete(vface);
		vface = vface->next;
	}

	vert_vlayer = find_verse_layer_type((VGeomData*)vnode->data, VERTEX_LAYER);
	vvert = (VerseVert*)vert_vlayer->dl.lb.first;

	while(vvert) {
		send_verse_vertex_delete(vvert);
		vvert = vvert->next;
	}

	/* own destruction of local verse date will be executed, when client will
	 * receive apropriate callback commands from verse server */
}

/*
 * free VGeomData
 */
void free_geom_data(VNode *vnode)
{
	struct VLayer *vlayer;

	if(vnode->data){
		vlayer = (VLayer*)((VGeomData*)vnode->data)->layers.lb.first;
		while(vlayer){
			/* free VerseLayer data */
			free_verse_layer_data(vnode, vlayer);
			/* unsunscribe from layer */
			verse_send_g_layer_unsubscribe(vnode->id, vlayer->id);
			/* next layer */
			vlayer = vlayer->next;
		}
		/* free constraint between vnode and mesh */
		((VGeomData*)vnode->data)->post_geometry_free_constraint(vnode);
		/* free all VerseLayers */
		TLI_dlist_destroy(&(((VGeomData*)vnode->data)->layers));
	}
}

void set_geometry_callbacks(void)
{
	/* new layer created */
	verse_callback_set((void*)verse_send_g_layer_create, (void*)cb_g_layer_create, NULL);
	/* layer was destroyed */
	verse_callback_set((void*)verse_send_g_layer_destroy, (void*)cb_g_layer_destroy, NULL);

	/* position of vertex was changed */
	verse_callback_set((void*)verse_send_g_vertex_set_xyz_real32, (void*)cb_g_vertex_set_xyz_real32, NULL);
	/* vertex was deleted */
	verse_callback_set((void*)verse_send_g_vertex_delete_real32, (void*)cb_g_vertex_delete_real32, NULL);

	/* callback functions for values being associated with vertexes */
	verse_callback_set((void*)verse_send_g_vertex_set_uint32, (void*)cb_g_vertex_set_uint32, NULL);
	verse_callback_set((void*)verse_send_g_vertex_set_real32, (void*)cb_g_vertex_set_real32, NULL);

	/* new polygon was created / vertex of polygon was set */
	verse_callback_set((void*)verse_send_g_polygon_set_corner_uint32, (void*)cb_g_polygon_set_corner_uint32, NULL);
	/* polygon was deleted */
	verse_callback_set((void*)verse_send_g_polygon_delete, (void*)cb_g_polygon_delete, NULL);

	/* callback functions for values being associated with polygon corners */
	verse_callback_set((void*)verse_send_g_polygon_set_corner_real32, (void*)cb_g_polygon_set_corner_real32, NULL);
	/* callback functions for values being associated with faces */
	verse_callback_set((void*)verse_send_g_polygon_set_face_uint8, (void*)cb_g_polygon_set_face_uint8, NULL);
	verse_callback_set((void*)verse_send_g_polygon_set_face_uint32, (void*)cb_g_polygon_set_face_uint32, NULL);
	verse_callback_set((void*)verse_send_g_polygon_set_face_real32, (void*)cb_g_polygon_set_face_real32, NULL);

	/* crease of vertex was set */
	verse_callback_set((void*)verse_send_g_crease_set_vertex, (void*)cb_g_crease_set_vertex, NULL);
	/* crease of edge was set */
	verse_callback_set((void*)verse_send_g_crease_set_edge, (void*)cb_g_crease_set_edge, NULL);
}

/*#endif*/
