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

#include "MEM_guardedalloc.h"

#include "mydevice.h"

#include "DNA_object_types.h"
#include "DNA_mesh_types.h"

#include "TLI_dynamiclist.h"
#include "TLI_toplib.h"
#include "TLI_editVert.h"

#include "TKE_global.h"
#include "TKE_verse.h"
#include "TKE_mesh.h"
#include "TKE_depsgraph.h"
#include "TKE_utildefines.h"

#include "TIF_verse.h"
/*#include "TIF_editmesh.h"*/
/*#include "TIF_space.h"*/
/*#include "TIF_screen.h"*/

/*#include "BSE_edit.h"*/

/*#include "editmesh.h"*/

/* prototypes of static functions */

static void sync_verseface_with_editface(struct VLayer *vlayer, struct VerseFace *vface);
static void sync_verseverts_with_editverts(struct VerseVert *vvert);

/*=============================================================================
 *
 *                  functions handling verse/blender FACES
 *
 *===========================================================================*/

/*
 * this function sends information about face smoothness
 */
void b_verse_set_face_smooth(EditFace *efa)
{
	// struct VerseFace *vface = (VerseFace*)efa->vface;
	// 
	// send_verse_face_smoothness(vface, (efa->flag & ME_SMOOTH)?1:0);
}

/*
 * restore information about VerseFace from EditVert ... this function is used
 * during poping EditFace form undo stack
 */
void restore_verseface_from_undoface(VNode *vnode, EditFace *efa, VerseFace *old_vface, unsigned int vface_id)
{
	// struct VLayer *vlayer;
	// struct VerseFace *vface = NULL;
	// 
	// if(vnode->type != V_NT_GEOMETRY) return;
	// 
	// vlayer = find_verse_layer_type((VGeomData*)vnode->data, POLYGON_LAYER);
	// 
	// vface = (VerseFace*)TLI_dlist_find_link(&(vlayer->dl), vface_id);
	// 
	// if(vface) {
	// 	if(old_vface == vface) {
	// 		efa->vface = (void*)vface;
	// 		vface->face = (void*)efa;
	// 
	// 		sync_verseface_with_editface(vlayer, vface);
	// 
	// 		/* send information about smoothness, when information in efa differs
	// 		 * from infomration stored in vface */
	// 		if((vface->smooth) && (vface->smooth->value != (efa->flag & ME_SMOOTH)))
	// 			b_verse_set_face_smooth(efa);
	// 	}
	// 	else {
	// 		printf("\tUNDO: old VerseFace was deleted\n");
	// 		push_edit_face_to_verse_server(efa, vnode, vlayer);
	// 	}
	// }
	// else {
	// 	/* VerseFace doesn't exist ... we have to recreate it again */
	// 	push_edit_face_to_verse_server(efa, vnode, vlayer);
	// }
}

/*
 * create new VerseFace (polygon), due to information about MFace
 * put VerseFace to queue ... send to verse host (server)
 */
void push_mface_to_verse_server( MFace *fa, unsigned int face_id, VNode *vnode, VLayer *vlayer)
{
	// VerseFace *vface;
	// VerseVert *vvert;
	// 
	// vface = (VerseFace*)create_verse_face(vnode->vsession, vnode->id, vlayer->id, face_id,
	// 		fa->v1, fa->v2, fa->v3, fa->v4);
	// 
	// vface->face = (void*)fa;
	// 
	// vface->flag |= FACE_SEND_READY;
	// 
	// /* MVert #1 */
	// vvert = (VerseVert*)TLI_dlist_find_link(&(vlayer->dl), fa->v1);
	// if(vvert) {
	// 	vface->vvert0 = vvert;
	// 	if(vvert->flag & VERT_RECEIVED)
	// 		vface->counter--;
	// 	else
	// 		vface->flag &= ~FACE_SEND_READY;
	// }
	// 
	// /* MVert #2 */
	// vvert = (VerseVert*)TLI_dlist_find_link(&(vlayer->dl), fa->v1);
	// if(vvert) {
	// 	vface->vvert1 = vvert;
	// 	if(vvert->flag & VERT_RECEIVED)
	// 		vface->counter--;
	// 	else
	// 		vface->flag &= ~FACE_SEND_READY;
	// }
	// 
	// /* MVert #3 */
	// vvert = (VerseVert*)TLI_dlist_find_link(&(vlayer->dl), fa->v1);
	// if(vvert) {
	// 	vface->vvert2 = vvert;
	// 	if(vvert->flag & VERT_RECEIVED)
	// 		vface->counter--;
	// 	else
	// 		vface->flag &= ~FACE_SEND_READY;
	// }
	// 
	// /* MVert #4 */
	// vvert = (VerseVert*)TLI_dlist_find_link(&(vlayer->dl), fa->v1);
	// if(vvert) {
	// 	vface->vvert3 = vvert;
	// 	if(vvert->flag & VERT_RECEIVED)
	// 		vface->counter--;
	// 	else
	// 		vface->flag &= ~FACE_SEND_READY;
	// }
	// 
	// add_item_to_send_queue(&(vlayer->queue), (void*)vface, VERSE_FACE);
}

/*
 * create new VerseFace (polygon), due to information about EditFace
 * put VerseFace to queue ... send to verse host (server)
 */
void push_edit_face_to_verse_server(EditFace *efa, VNode *vnode, VLayer *vlayer)
{
	// struct VerseFace *vface;
	// 
	// vface = (VerseFace*)create_verse_face(vnode->vsession, vnode->id, vlayer->id, -1, -1, -1, -1, -1);
	// 
	// vface->face = (void*)efa;
	// efa->vface = (void*)vface;
	// 
	// vface->flag |= FACE_SEND_READY;
	// 
	// /* EditVert #1 */
	// if(efa->v1) {
	// 	if(efa->v1->vvert) {
	// 		vface->vvert0 = (VerseVert*)efa->v1->vvert;
	// 		if(((VerseVert*)efa->v1->vvert)->flag & VERT_RECEIVED) {
	// 			vface->v0 = ((VerseVert*)efa->v1->vvert)->id;
	// 			vface->counter--;
	// 		}
	// 		else
	// 			vface->flag &= ~FACE_SEND_READY;
	// 	}
	// }
	// else
	// 	vface->counter--;
	// 
	// /* EditVert #2 */
	// if(efa->v2) {
	// 	if(efa->v2->vvert) {
	// 		vface->vvert1 = (VerseVert*)efa->v2->vvert;
	// 		if(((VerseVert*)efa->v2->vvert)->flag & VERT_RECEIVED) {
	// 			vface->v1 = ((VerseVert*)efa->v2->vvert)->id;
	// 			vface->counter--;
	// 		}
	// 		else
	// 			vface->flag &= ~FACE_SEND_READY;
	// 	}
	// }
	// else
	// 	vface->counter--;
	// /* EditVert #3 */
	// if(efa->v3) {
	// 	if(efa->v3->vvert) {
	// 		vface->vvert2 = (VerseVert*)efa->v3->vvert;
	// 		if(((VerseVert*)efa->v3->vvert)->flag & VERT_RECEIVED) {
	// 			vface->v2 = ((VerseVert*)efa->v3->vvert)->id;
	// 			vface->counter--;
	// 		}
	// 		else
	// 			vface->flag &= ~FACE_SEND_READY;
	// 	}
	// }
	// else
	// 	vface->counter--;
	// /* EditVert #4 */
	// if(efa->v4) {
	// 	if(efa->v4->vvert) {
	// 		vface->vvert3 = (VerseVert*)efa->v4->vvert;
	// 		if(((VerseVert*)efa->v4->vvert)->flag & VERT_RECEIVED) {
	// 			vface->v3 = ((VerseVert*)efa->v4->vvert)->id;
	// 			vface->counter--;
	// 		}
	// 		else
	// 			vface->flag &= ~FACE_SEND_READY;
	// 	}
	// }
	// else
	// 	vface->counter--;
	// 
	// add_item_to_send_queue(&(vlayer->queue), (void*)vface, VERSE_FACE);
}

/*
 * creates new verse faces, add all of then to queue ... send to verse server
 */
void push_all_faces(VNode *vnode, VLayer *vlayer)
{
	// if(G.obedit) {
	// 	struct EditMesh *em;
	// 	struct EditFace *efa;
	// 
	// 	em = G.editMesh;
	// 
	// 	efa = em->faces.first;
	// 	/* push all EditFaces to the verse server */
	// 	while(efa){
	// 		push_edit_face_to_verse_server(efa, vnode, vlayer);
	// 		efa = efa->next;
	// 	}
	// }
	// else {
	// 	struct VGeomData *geom;
	// 	struct Mesh *me;
	// 	unsigned int i;
	// 
	// 	geom = (VGeomData*)vnode->data;
	// 	me = (Mesh*)geom->mesh;
	// 	for(i=0; i<me->totface; i++)
	// 		push_mface_to_verse_server(&(me->mface[i]), i, vnode, vlayer);
	// }
}

/*
 * this function will sync EditFace and VerseFace and it will send changes to
 * verse server too
 */
static void sync_verseface_with_editface(VLayer *vlayer, VerseFace *vface)
{
	// struct EditFace *efa = (EditFace*)vface->face;
	// unsigned int old_verts[4];
	// int dosend = 0;
	// 
	// if(!efa) return;
	// 
	// vface->flag |= FACE_SEND_READY;
	// 
	// vface->counter = 4;
	// 
	// old_verts[0] = vface->v0;
	// old_verts[1] = vface->v1;
	// old_verts[2] = vface->v2;
	// old_verts[3] = vface->v3;
	// 
	// /* 1st vertex */
	// if(vface->vvert0 && (vface->vvert0->vertex != efa->v1)) {
	// 	dosend = 1;
	// 	vface->vvert0->counter--;
	// 	vface->vvert0 = (VerseVert*)efa->v1->vvert;
	// 	vface->v0 = vface->vvert0->id;
	// 	if(vface->vvert0->flag & VERT_RECEIVED) {
	// 		vface->counter--;
	// 	}
	// 	else
	// 		vface->flag &= ~FACE_SEND_READY;
	// }
	// else
	// 	vface->counter--;
	// 
	// /* 2nd vertex */
	// if(vface->vvert1 && (vface->vvert1->vertex != efa->v2)) {
	// 	dosend = 1;
	// 	vface->vvert1->counter--;
	// 	vface->vvert1 = (VerseVert*)efa->v2->vvert;
	// 	vface->v1 = vface->vvert1->id;
	// 	if(vface->vvert1->flag & VERT_RECEIVED)
	// 		vface->counter--;
	// 	else
	// 		vface->flag &= ~FACE_SEND_READY;
	// }
	// else
	// 	vface->counter--;
	// 
	// /* 3th vertex */
	// if(vface->vvert2 && (vface->vvert2->vertex != efa->v3)) {
	// 	dosend = 1;
	// 	vface->vvert2->counter--;
	// 	vface->vvert2 = (VerseVert*)efa->v3->vvert;
	// 	vface->v2 = vface->vvert2->id;
	// 	if(vface->vvert2->flag & VERT_RECEIVED)
	// 		vface->counter--;
	// 	else
	// 		vface->flag &= ~FACE_SEND_READY;
	// }
	// else
	// 	vface->counter--;
	// 
	// /* 4th vertex */
	// if(vface->vvert3 && (vface->vvert3->vertex != efa->v4)) {
	// 	dosend = 1;
	// 	if(efa->v4) {
	// 		vface->vvert3->counter--;
	// 		vface->vvert3 = (VerseVert*)efa->v4->vvert;
	// 		vface->v3 = vface->vvert3->id;
	// 		if(vface->vvert3->flag & VERT_RECEIVED)
	// 			vface->counter--;
	// 		else
	// 			vface->flag &= ~FACE_SEND_READY;
	// 	}
	// 	else {
	// 		if(vface->vvert3) vface->vvert3->counter--;
	// 		vface->vvert3 = NULL;
	// 		vface->v3 = -1;
	// 		vface->counter--;
	// 	}
	// }
	// else
	// 	vface->counter--;
	// 
	// if(dosend) {
	// 	printf("\t\told: %d %d %d %d\n", old_verts[0], old_verts[1], old_verts[2], old_verts[3]);
	// 	printf("\tsending CHANGED FACE\n");
	// 	printf("\t\tnew: %d %d %d %d\n", vface->v0, vface->v1, vface->v2, vface->v3);
	// 	vface->flag |= FACE_CHANGED;
	// 	TLI_dlist_rem_item(&(vlayer->dl), vface->id);
	// 	add_item_to_send_queue(&(vlayer->queue), (void*)vface, VERSE_FACE);
	// }
}

/*
 * this function will sync all VerseFaces with coresponding EditFaces
 * this is useful, when some editmesh tool change editface pointers at
 * vertexes (edges), parameter of this function is geometry node
 */
void sync_all_versefaces_with_editfaces(VNode *vnode)
{
	// struct VLayer *vlayer;
	// struct VerseFace *vface, *nvface;
	// 
	// if(vnode->type != V_NT_GEOMETRY) return;
	// 
	// vlayer = find_verse_layer_type((VGeomData*)vnode->data, POLYGON_LAYER);
	// 
	// vface = vlayer->dl.lb.first;
	// 
	// while(vface) {
	// 	nvface = vface->next;
	// 	sync_verseface_with_editface(vlayer, vface);
	// 	vface = nvface;
	// }
}

/*
 * send delete polygon command to verse server
 */
void t_verse_send_face_delete(EditFace *efa)
{
	// ((VerseFace*)efa->vface)->face = NULL;
	// send_verse_face_delete((VerseFace*)efa->vface);
	// efa->vface = NULL;
}

/*
 * send new face to verse server, this function can be used only in edit mode
 */
void t_verse_send_face(EditFace *efa)
{
	// struct VNode *vnode;
	// struct VLayer *vlayer;
	// 
	// vnode = G.editMesh->vnode;
	// vlayer = find_verse_layer_type((VGeomData*)vnode->data, POLYGON_LAYER);
	// 
	// push_edit_face_to_verse_server(efa, vnode, vlayer);
	// 
}

/*
 * this function will destroy binding between EditFace and VersFace, it is used
 * before restoring EditMesh from "UndoMesh"
 */
static void destroy_binding_between_verseface_and_editface(VerseFace *vface)
{
	// if(vface->face) {
	// 	((EditFace*)vface->face)->vface = NULL;
	// 	vface->face = NULL;
	// }
}


/*=============================================================================
 *
 *                   functions handling verse/blender VERTEXES
 *
 *===========================================================================*/

/*
 * this function try to restore VerseVert from information about EditVert and
 * vvert_id (stored in "undo vert")
 */
void restore_versevert_from_undovert(VNode *vnode, EditVert *eve, VerseVert *old_vvert, unsigned int vvert_id)
{
	// struct VLayer *vlayer;
	// struct VerseVert *vvert = NULL;
	// 
	// if(vnode->type != V_NT_GEOMETRY) return;
	// 
	// vlayer = find_verse_layer_type((VGeomData*)vnode->data, VERTEX_LAYER);
	// 
	// vvert = (VerseVert*)TLI_dlist_find_link(&(vlayer->dl), vvert_id);
	// 
	// if(vvert) {
	// 	if(old_vvert == vvert) {
	// 		eve->vvert = (void*)vvert;
	// 		vvert->vertex = (void*)eve;
	// 
	// 		sync_verseverts_with_editverts(vvert);
	// 	}
	// 	else {
	// 		printf("\tUNDO: old VerseVert was deleted\n");
	// 		push_editvert_to_verse_server(eve, vnode, vlayer);
	// 	}
	// }
	// else {
	// 	/* VerseVert doesn't exist ... it is neccesary to it create again */
	// 	push_editvert_to_verse_server(eve, vnode, vlayer);
	// }
}

/*
 * this function will sync position of VerseVert with EditVert
 */
static void sync_verseverts_with_editverts(VerseVert *vvert)
{
	// struct EditVert *eve = (EditVert*)vvert->vertex;
	// 
	// if(!eve) return;
	// 
	// if((vvert->co[0] != eve->co[0]) || (vvert->co[1] != eve->co[1]) || (vvert->co[2] != eve->co[2])) {
	// 	printf("\tVERT sync\n");
	// 	VECCOPY(vvert->co, eve->co);
	// 
	// 	vvert->flag |= VERT_SEND_READY;
	// 	send_verse_vertex(vvert);
	// }
}

/*
 * this function will sync position of all VerseVerts with EditVerts
 * this function is called after actions: Smooth, Noise and To Sphere,
 * because position of vertexes isn't managed by transform system
 */
void sync_all_verseverts_with_editverts(VNode *vnode)
{
	// struct VLayer *vlayer;
	// struct VerseVert *vvert;
	// 
	// if(vnode->type != V_NT_GEOMETRY) return;
	// 
	// vlayer = find_verse_layer_type((VGeomData*)vnode->data, VERTEX_LAYER);
	// 
	// vvert = vlayer->dl.lb.first;
	// 
	// while(vvert) {
	// 	sync_verseverts_with_editverts(vvert);
	// 	vvert = vvert->next;
	// }
	// 
	// verse_callback_update(0);
}

/*
 * send delete vertex command to verse server
 */
void t_verse_send_vertex_delete(EditVert *eve)
{
	// ((VerseVert*)eve->vvert)->vertex = NULL;
	// send_verse_vertex_delete((VerseVert*)eve->vvert);
	// eve->vvert = NULL;
}

/*
 * This function will destroy binding between EditVert and VerseVert
 */
static void destroy_binding_between_versevert_and_editvert(VerseVert *vvert)
{
	// if(vvert->vertex){
	// 	((EditVert*)vvert->vertex)->vvert = NULL;
	// 	vvert->vertex = NULL;
	// }
}

/*
 * send position of verse vertex to verse server
 */
void t_verse_send_vertex_position(EditVert *eve)
{
	// struct VerseVert *vvert = (VerseVert*)eve->vvert;
	// 
	// if((vvert->flag & VERT_RECEIVED) && (vvert->flag & VERT_SEND_READY)){
	// 	VECCOPY(vvert->co, eve->co);
	// 
	// 	send_verse_vertex(vvert);
	// }
	// 
	// verse_callback_update(0);
}

/*
 * send new vertex to verse server
 */
void t_verse_send_vertex(EditVert *eve)
{
	// struct VNode *vnode;
	// struct VLayer *vlayer;
	// 
	// vnode = (VNode*)G.editMesh->vnode;
	// 
	// if(!vnode) return;
	// 
	// vlayer = find_verse_layer_type((VGeomData*)vnode->data, VERTEX_LAYER);
	// 
	// push_editvert_to_verse_server(eve, vnode, vlayer);
}

/*
 * create new VerseVert due to information about EditVert,
 * put VerseVert to queue ... send to verse host (server)
 */
void push_editvert_to_verse_server( EditVert *eve, VNode *vnode, VLayer *vlayer)
{
	// struct VerseVert *vvert;
	// 
	// vvert = create_verse_vertex(vnode->vsession, vnode->id, vlayer->id, -1,
	// 		eve->co[0], eve->co[1], eve->co[2]);
	// 
	// vvert->vertex = (void*)eve;
	// eve->vvert = (void*)vvert;
	// 
	// /* add vvert to sending queue */
	// add_item_to_send_queue(&(vlayer->queue), (void*)vvert, VERSE_VERT);
}

/*
 * create new VerseVert due to information about MVert (object mode),
 * put VerseVert to queue ... send to verse host (server)
 */
void push_mvert_to_verse_server( MVert *ve, unsigned int vert_id, VNode *vnode, VLayer *vlayer)
{
	// struct VerseVert *vvert;
	// 
	// vvert = create_verse_vertex(vnode->vsession, vnode->id, vlayer->id, vert_id,
	// 		ve->co[0], ve->co[1], ve->co[2]);
	// 
	// vvert->vertex = (void*)ve;
	// 
	// /* add vvert to sending queue */
	// add_item_to_send_queue(&(vlayer->queue), (void*)vvert, VERSE_VERT);
}

/*
 * create new verse vertexes due to all vertexes and send all of them to verse server 
 */
void push_all_verts(VNode *vnode, VLayer *vlayer)
{
	// if(G.obedit) {
	// 	struct EditMesh *em;
	// 	struct EditVert *eve;
	// 
	// 	em = G.editMesh;
	// 	eve = em->verts.first;
	// 
	// 	/* push all EditVertexes to the verse server */
	// 	while(eve){
	// 		push_editvert_to_verse_server(eve, vnode, vlayer);
	// 		eve = eve->next;
	// 	}
	// }
	// else {
	// 	struct VGeomData *geom;
	// 	struct Mesh *me;
	// 	unsigned int i;
	// 
	// 	geom = (VGeomData*)vnode->data;
	// 	me = (Mesh*)geom->mesh;
	// 	for(i=0; i<me->totvert; i++)
	// 		push_mvert_to_verse_server(&(me->mvert[i]), i, vnode, vlayer);
	// }
}

/* ===================================================================================
 *
 *             Function executed after execution of callback functions
 *
 * ===================================================================================*/

/*
 * Actions executed after new VerseLayer is created
 */
void post_layer_create(VNode *vnode, struct VLayer *vlayer)
{
}

/*
 * Actions after destroying of VerseLayer
 */
void post_layer_destroy(VNode *vnode, struct VLayer *vlayer)
{
}

/*
 * Actions executed after creating of new VerseVert, when object is in edit
 * mode, and this client didn't create this VerseVert (vvert->vertex is NULL),
 * then new editvert is created
 */
void post_vertex_create(VNode *vnode, VerseVert *vvert)
{
// 	struct VNode *obj_vnode;
// 	struct EditMesh *em=NULL;
// 
// 	if(G.obedit && (((Mesh*)G.obedit->data)->vnode==vnode)) {
// 		em = (EditMesh*)((VGeomData*)vnode->data)->editmesh;
// 	}
// 
// 	if(em && !(vvert->vertex)) {
// 		struct EditVert *eve;
// 
// 		printf("\tPOST_VERTEX_CREATE()\n");
// 
// 		/* to prevent never ending loop of sending and receiving
// 		 * vertexes, because addvertlist() sends new vertex to verse
// 		 * server if em->vnode isn't NULL */
// 		em->vnode = NULL;
// /*		eve = addvertlist(vvert->co);*/
// 		em->vnode = (void*)vnode;
// 
// 		eve->vvert = (void*)vvert;
// 		vvert->vertex = (void*)eve;
// 	
// /*		countall();*/
// 
// /*		recalc_editnormals();*/
// 	}
// 
// 	if(((VGeomData*)vnode->data)->vlink) {
// 		obj_vnode = ((VGeomData*)vnode->data)->vlink->source;
// /*		DAG_object_flush_update(G.scene, (Object*)((VObjectData*)obj_vnode->data)->object, OB_RECALC_DATA);*/
// 
// /*		allqueue(REDRAWVIEW3D, 1);*/
// 	}
}

/*
 * Actions executed, when position of VerseVert was changed
 * position of EditVert is changed in edit mode
 */
void post_vertex_set_xyz(VNode *vnode, VerseVert *vvert)
{
// 	struct VNode *obj_vnode;
// 	struct EditVert *eve = NULL;
// 
// 	if(G.obedit && (((Mesh*)G.obedit->data)->vnode==vnode)) {
// 		eve = (EditVert*)vvert->vertex;
// 		VECCOPY(eve->co, vvert->co);
// /*		recalc_editnormals();*/
// 	}
// 
// 	if(((VGeomData*)vnode->data)->vlink) {
// 		obj_vnode = ((VGeomData*)vnode->data)->vlink->source;
// /*		DAG_object_flush_update(G.scene, (Object*)((VObjectData*)obj_vnode->data)->object, OB_RECALC_DATA);*/
// 
// /*		allqueue(REDRAWVIEW3D, 1);*/
// 	}
}

/*
 * Actions executed after deleting of VerseVert
 */
void post_vertex_delete(VNode *vnode, VerseVert *vvert)
{
// 	struct VNode *obj_vnode;
// 	struct EditMesh *em = NULL;
// 	struct EditEdge *ed, *edn;
// 	struct EditVert *eve = NULL;
// 
// 	if(G.obedit && (((Mesh*)G.obedit->data)->vnode==vnode)) {
// 		em = (EditMesh*)((VGeomData*)vnode->data)->editmesh;
// 		eve = (EditVert*)vvert->vertex;
// 	}
// 
// 	if(em && eve) {
// 		printf("\tPOST_VERTEX_DELETE()\n");
// 
// 		/* delete all edges needing eve vertex */
// 		ed = em->edges.first;
// 		while(ed) {
// 			edn = ed->next;
// 			if(ed->v1==eve || ed->v2==eve) {
// /*				remedge(ed);*/
// /*				free_editedge(ed);*/
// 			}
// 			ed = edn;
// 		}
// 
// 		eve->vvert = NULL;
// 		TLI_remlink(&em->verts, eve);
// /*		free_editvert(eve);*/
// 		vvert->vertex = NULL;
// 	
// /*		countall();*/
// 
// /*		recalc_editnormals();*/
// 	}
// 
// 	if(((VGeomData*)vnode->data)->vlink) {
// 		obj_vnode = ((VGeomData*)vnode->data)->vlink->source;
// /*		DAG_object_flush_update(G.scene, (Object*)((VObjectData*)obj_vnode->data)->object, OB_RECALC_DATA);*/
// 
// /*		allqueue(REDRAWVIEW3D, 1);*/
// 	}
}

/*
 * free constraint between VerseVert and EditVert
 */
void post_vertex_free_constraint(VNode *vnode, VerseVert *vvert)
{
	// if(vvert->vertex) {
	// 	((EditVert*)vvert->vertex)->vvert=NULL;
	// 	vvert->vertex=NULL;
	// }
}

/*
 * Action executed after setting up uint8 value of polygon
 */
void post_polygon_set_uint8(VNode *vnode, VLayer *vlayer, VerseFace *vface)
{
// 	struct VNode *obj_vnode;
// 
// 	if(vlayer->content == POLYGON_SMOOTH_LAYER) {
// 		if(G.obedit && (((Mesh*)G.obedit->data)->vnode==vnode)) {
// 			if(vface->face) {
// 				if(vface->smooth->value)
// 					((EditFace*)vface->face)->flag |= ME_SMOOTH;
// 				else
// 					((EditFace*)vface->face)->flag &= ~ME_SMOOTH;
// 			}
// 		}
// 	}
// 
// 	obj_vnode = ((VGeomData*)vnode->data)->vlink->source;
// /*	DAG_object_flush_update(G.scene, (Object*)((VObjectData*)obj_vnode->data)->object, OB_RECALC_DATA);*/
// 
// /*	allqueue(REDRAWVIEW3D, 1);*/
}

/*
 * Action executed after creating of new VerseFace
 */
void post_polygon_create(VNode *vnode, VerseFace *vface)
{
// 	struct VNode *obj_vnode;
// 	struct EditMesh *em = NULL;
// 
// 	if(G.obedit && (((Mesh*)G.obedit->data)->vnode==vnode)) {
// 		em = (EditMesh*)((VGeomData*)vnode->data)->editmesh;
// 	}
// 
// 	if(em && !(vface->face) && (vface->counter==0)) {
// 		struct VLayer *vlayer;
// 		struct VerseVert *vvert;
// 		struct EditFace *efa;
// 		struct EditVert *eves[4]={NULL, NULL, NULL, NULL};
// 		uint32 vert_ids[4]={vface->v0, vface->v1, vface->v2, vface->v3};
// 		int i;
// 
// 		printf("\tPOST_POLYGON_CREATE()\n");
// 
// 		vlayer = find_verse_layer_type((VGeomData*)vnode->data, VERTEX_LAYER);
// 
// 		for(i=0; i<4; i++) {
// 			if(vert_ids[i] != -1) {
// 				vvert = TLI_dlist_find_link(&(vlayer->dl), vert_ids[i]);
// 				if(vvert) eves[i] = (EditVert*)vvert->vertex;
// 			}
// 		}
// 
// 		/* to prevent never ending loop of sending and receiving
// 		 * faces, because addfacelist() sends new face to verse
// 		 * server if em->vnode isn't NULL */
// 		em->vnode = NULL;
// /*		efa = addfacelist(eves[0], eves[1], eves[2], eves[3], NULL, NULL);*/
// 		em->vnode = vnode;
// 
// 		efa->vface = vface;
// 		vface->face = efa;
// 	
// /*		countall();*/
// 
// /*		recalc_editnormals();*/
// 	}
// 
// 	if(((VGeomData*)vnode->data)->vlink) {
// 		obj_vnode = ((VGeomData*)vnode->data)->vlink->source;
// /*		DAG_object_flush_update(G.scene, (Object*)((VObjectData*)obj_vnode->data)->object, OB_RECALC_DATA);*/
// 
// /*		allqueue(REDRAWVIEW3D, 1);*/
// 	}
}

/*
 * Action executed after changes of VerseFace
 * ... order of vertexes was fliped, etc.
 */
void post_polygon_set_corner(VNode *vnode, VerseFace *vface)
{
// 	struct VNode *obj_vnode;
// 	struct EditMesh *em = NULL;
// 	struct EditFace *efa = NULL;
// 	struct EditEdge *eed, *eedn;
// 
// 	if(G.obedit && (((Mesh*)G.obedit->data)->vnode==vnode)) {
// 		em = (EditMesh*)((VGeomData*)vnode->data)->editmesh;
// 		efa = (EditFace*)vface->face;
// 	}
// 
// 	if(em && efa) {
// 		printf("\tPOST_POLYGON_CHANGE()\n");
// 
// 		/* mark all edges, which are part of face efa */
// 		efa->e1->f2 = 1;
// 		efa->e2->f2 = 1;
// 		efa->e3->f2 = 1;
// 		if(efa->e4) efa->e4->f2 = 1;
// 
// 		/* change pointers at EdtitVerts and decrease counters of "old"
// 		 * VerseVertexes reference ... less VerseFaces will need them */
// 		if(vface->vvert0 != efa->v1->vvert)
// 			efa->v1 = (EditVert*)vface->vvert0->vertex;
// 		if(vface->vvert1 != efa->v2->vvert)
// 			efa->v2 = (EditVert*)vface->vvert1->vertex;
// 		if(vface->vvert2 != efa->v3->vvert)
// 			efa->v3 = (EditVert*)vface->vvert2->vertex;
// 		if(efa->v4) {
// 			if(!vface->vvert3)
// 				efa->v4 = NULL;
// 			else if(vface->vvert3 != efa->v4->vvert)
// 				efa->v4 = (EditVert*)vface->vvert3->vertex;
// 		}
// 
// 		/* change pointers at EditEdges */
// 
// 		/* 1st edge */
// /*		eed = findedgelist(efa->v1, efa->v2);
// 		if(eed) efa->e1 = eed;
// 		else efa->e1 = addedgelist(efa->v1, efa->v2, NULL);
// */
// 		/* 2nd edge */
// /*		eed = findedgelist(efa->v2, efa->v3);
// 		if(eed) efa->e2 = eed;
// 		else efa->e2 = addedgelist(efa->v2, efa->v3, NULL);
// */
// 		if(efa->v4) {
// 		/* 3th edge */
// /*			eed = findedgelist(efa->v2, efa->v3);
// 			if(eed) efa->e3 = eed;
// 			else efa->e3 = addedgelist(efa->v2, efa->v3, NULL);
// */			/* 4th edge */
// /*			eed = findedgelist(efa->v4, efa->v1);
// 			if(eed) efa->e4 = eed;
// 			else efa->e4 = addedgelist(efa->v4, efa->v1, NULL);
// */		}
// 		else {
// 		/* 3th edge */
// /*			eed = findedgelist(efa->v3, efa->v1);
// 			if(eed) efa->e3 = eed;
// 			else efa->e3 = addedgelist(efa->v3, efa->v1, NULL);
// */			/* 4th edge */
// 			efa->e4 = NULL;
// 		}
// 
// 		/* unmark needed edges */
// 		efa = em->faces.first;
// 		while(efa) {
// 			efa->e1->f2 = 0;
// 			efa->e2->f2 = 0;
// 			efa->e3->f2 = 0;
// 			if(efa->e4) efa->e4->f2 = 0;
// 			efa = efa->next;
// 		}
// 
// 		/* delete all unneeded edges */
// 		eed = em->edges.first;
// 		while(eed) {
// 			eedn = eed->next;
// 			if(eed->f2) {
// /*				remedge(eed);*/
// /*				free_editedge(eed);*/
// 			}
// 			eed = eedn;
// 		}
// 
// /*		countall();*/
// 
// /*		recalc_editnormals();*/
// 	}
// 
// 	if(((VGeomData*)vnode->data)->vlink) {
// 		obj_vnode = ((VGeomData*)vnode->data)->vlink->source;
// /*		DAG_object_flush_update(G.scene, (Object*)((VObjectData*)obj_vnode->data)->object, OB_RECALC_DATA);*/
// 
// /*		allqueue(REDRAWVIEW3D, 1);*/
// 	}
}

/*
 * Action executed after deleting of VerseFace
 */
void post_polygon_delete(VNode *vnode, VerseFace *vface)
{
// 	struct VNode *obj_vnode;
// 	struct EditMesh *em = NULL;
// 	struct EditFace *efa = NULL;
// 	struct EditEdge *eed, *eedn;
// 
// 	if(G.obedit && (((Mesh*)G.obedit->data)->vnode==vnode)) {
// 		em = (EditMesh*)((VGeomData*)vnode->data)->editmesh;
// 		efa = (EditFace*)vface->face;
// 	}
// 
// 	if(em && efa) {
// 		printf("\tPOST_POLYGON_DELETE()\n");
// 
// 		/* mark all edges, which are part of face efa */
// 		efa->e1->f2 = 1;
// 		efa->e2->f2 = 1;
// 		efa->e3->f2 = 1;
// 		if(efa->e4) efa->e4->f2 = 1;
// 
// 		efa->vface = NULL;
// 		TLI_remlink(&em->faces, efa);
// /*		free_editface(efa);*/
// 		vface->face = NULL;
// 
// 		/* following two crazy loops wouldn't be neccessary if verse spec
// 		 * would support edges */
// 
// 		/* unmark needed edges */
// 		efa = em->faces.first;
// 		while(efa) {
// 			efa->e1->f2 = 0;
// 			efa->e2->f2 = 0;
// 			efa->e3->f2 = 0;
// 			if(efa->e4) efa->e4->f2 = 0;
// 			efa = efa->next;
// 		}
// 
// 		/* delete all unneeded edges */
// 		eed = em->edges.first;
// 		while(eed) {
// 			eedn = eed->next;
// 			if(eed->f2) {
// /*				remedge(eed);*/
// /*				free_editedge(eed);*/
// 			}
// 			eed = eedn;
// 		}
// 	
// /*		countall();*/
// 	}
// 
// 	if(((VGeomData*)vnode->data)->vlink) {
// 		obj_vnode = ((VGeomData*)vnode->data)->vlink->source;
// /*		DAG_object_flush_update(G.scene, (Object*)((VObjectData*)obj_vnode->data)->object, OB_RECALC_DATA);*/
// 
// /*		allqueue(REDRAWVIEW3D, 1);*/
// 	}
}

/*
 * free constraint between VerseFace and EditFace
 */
void post_polygon_free_constraint(VNode *vnode, VerseFace *vface)
{
	// if(vface->face) {
	// 	((EditFace*)vface->face)->vface = NULL;
	// 	vface->face = NULL;
	// }
}

/*
 * free constraint between VGeomData, EditMesh and Mesh
 */
void post_geometry_free_constraint(VNode *vnode)
{
	// if(((VGeomData*)vnode->data)->editmesh) {
	// 	G.editMesh->vnode = NULL;
	// 	((VGeomData*)vnode->data)->editmesh = NULL;
	// }
	// if(((VGeomData*)vnode->data)->mesh) {
	// 	((Mesh*)((VGeomData*)vnode->data)->mesh)->vnode = NULL;
	// 	((VGeomData*)vnode->data)->mesh = NULL;
	// }
}

/* =========================================================================
 *
 *              Functions influencing whole EditMesh or VerseMesh
 *
 * ========================================================================= */

/*
 * this function will destroy all unused VerseFace and VerseVerts after poping
 * editmesh from undo stack
 */
void destroy_unused_geometry(VNode *vnode)
{
	// struct VLayer *vert_vlayer, *face_vlayer;
	// struct VerseFace *vface;
	// struct VerseVert *vvert;
	// 
	// if(vnode->type != V_NT_GEOMETRY) return;
	// 
	// vert_vlayer = find_verse_layer_type((VGeomData*)vnode->data, VERTEX_LAYER);
	// vvert = vert_vlayer->dl.lb.first;
	// while(vvert) {
	// 	if(!(vvert->vertex)) send_verse_vertex_delete(vvert);
	// 	vvert = vvert->next;
	// }
	// 
	// face_vlayer = find_verse_layer_type((VGeomData*)vnode->data, POLYGON_LAYER);
	// vface = face_vlayer->dl.lb.first;
	// while(vface) {
	// 	if(!(vface->face)) send_verse_face_delete(vface);
	// 	vface = vface->next;
	// }
}

/*
 * free all bindings between EditMesh and "verse mesh" ... it is called between
 * restorng editmesh from undo stack
 */
void destroy_binding_between_versemesh_and_editmesh(VNode *vnode)
{
	// struct VLayer *vert_vlayer, *face_vlayer;
	// struct VerseVert *vvert;
	// struct VerseFace *vface;
	// 
	// if(vnode->type != V_NT_GEOMETRY) return;
	// 
	// vert_vlayer = find_verse_layer_type((VGeomData*)vnode->data, VERTEX_LAYER);
	// vvert = vert_vlayer->dl.lb.first;
	// while(vvert) {
	// 	destroy_binding_between_versevert_and_editvert(vvert);
	// 	vvert = vvert->next;
	// }
	// 
	// face_vlayer = find_verse_layer_type((VGeomData*)vnode->data, POLYGON_LAYER);
	// vface = face_vlayer->dl.lb.first;
	// while(vface) {
	// 	destroy_binding_between_verseface_and_editface(vface);
	// 	vface = vface->next;
	// }
}

/*
 * Create EditMesh from VerseMesh and keep system in consitant state, this
 * function is called, when edit mode is entered ... edit mesh is generated
 * from verse mesh (not from Mesh: (Mesh*)ob->data)
 */
void create_edit_mesh_from_verse_node(VNode *vnode)
{
// 	struct VLayer *vert_layer, *face_layer;
// 	struct VerseVert *vvert;
// 	struct VerseFace *vface;
// 	struct Mesh *me;
// 	struct EditVert *eve, *eve0, *eve1, *eve2, *eve3;
// 	struct EditFace *efa;
// 	unsigned int keyindex;
// 
// 	if(!(G.obedit && G.obedit->type==OB_MESH)) return;
// 	me = (Mesh*)G.obedit->data;
// 	if(vnode!=(VNode*)me->vnode || vnode->type!=V_NT_GEOMETRY) return;
// 
// 	vert_layer = find_verse_layer_type((VGeomData*)vnode->data, VERTEX_LAYER);
// 	face_layer = find_verse_layer_type((VGeomData*)vnode->data, POLYGON_LAYER);
// 
// 	if(!(vert_layer && face_layer)) return;
// 
// /*	waitcursor(1);*/
// 
// 	/* free old editMesh */
// /*	free_editMesh(G.editMesh);*/
// 	
// 	G.editMesh->vnode = NULL;
// 
// 	vvert = vert_layer->dl.lb.first;
// 
// 	keyindex = 0;
// 
// 	/* create all EditVerts */
// 	while(vvert) {
// /*		eve = addvertlist(vvert->co);*/
// 
// 		eve->f = 0;
// 		eve->h = 0;
// 		eve->dw = NULL;
// 		eve->keyindex = keyindex;
// 		eve->vvert = (void*)vvert;
// 
// 		vvert->vertex = (void*)eve;
// 
// 		keyindex++;
// 		vvert = vvert->next;
// 	}
// 
// 	vface = face_layer->dl.lb.first;
// 
// 	/* create all EditFaces and EditEdges */
// 	while(vface) {
// 		if(vface->vvert0) eve0= vface->vvert0->vertex;
// 		else eve0 = NULL;
// 		if(vface->vvert1) eve1= vface->vvert1->vertex;
// 		else eve1 = NULL;
// 		if(vface->vvert2) eve2= vface->vvert2->vertex;
// 		else eve2 = NULL;
// 		if(vface->vvert3) eve3= vface->vvert3->vertex;
// 		else eve3 = NULL;
// 
// /*		efa= addfacelist(eve0, eve1, eve2, eve3, NULL, NULL);*/
// 		if(efa) {
// 			efa->f = 0;
// 			efa->flag = ((vface->smooth) && (vface->smooth->value))?ME_SMOOTH:0;
// 			efa->h = 0;
// 			efa->vface = (void*)vface;
// 			vface->face = (void*)efa;
// 		}
// 		vface = vface->next;
// 	}
// 	
// /*	countall();*/
// 
// /*	recalc_editnormals();*/
// 
// 	G.editMesh->vnode = (void*)vnode;
// 	((VGeomData*)vnode->data)->editmesh = (void*)G.editMesh;
// 
// /*	waitcursor(0);*/
}

/*
 * push whole geometry to verse server, vert layer as well face layer has to
 * exist. this function is used for example after editmesh undo pop, it is
 * not used, when we push new geometry to verse server
 */
void push_mesh_geometry(VNode *vnode)
{
	// struct VLayer *vert_vlayer, *face_vlayer;
	// 
	// if(vnode->type != V_NT_GEOMETRY) return;
	// 
	// vert_vlayer = find_verse_layer_type((VGeomData*)vnode->data, VERTEX_LAYER);
	// push_all_verts(vnode, vert_vlayer);
	// 
	// face_vlayer = find_verse_layer_type((VGeomData*)vnode->data, POLYGON_LAYER);
	// push_all_faces(vnode, face_vlayer);
}

/*
 * destroy bindings between EditMesh and VerseMesh and send delete commands
 * for all VerseVerts and VerseFaces to verse server, Verse Node has to be
 * geometry node
 */

void destroy_verse_mesh(VNode *vnode)
{
	// struct VLayer *vert_vlayer, *face_vlayer;
	// struct VerseFace *vface;
	// struct VerseVert *vvert;
	// 
	// if(vnode->type != V_NT_GEOMETRY) return;
	// 
	// face_vlayer = find_verse_layer_type((VGeomData*)vnode->data, POLYGON_LAYER);
	// vface = face_vlayer->dl.lb.first;
	// 
	// while(vface) {
	// 	((EditFace*)vface->face)->vface = NULL;
	// 	vface->face = NULL;	
	// 	vface = vface->next;
	// }
	// 
	// vert_vlayer = find_verse_layer_type((VGeomData*)vnode->data, VERTEX_LAYER);
	// vvert = vert_vlayer->dl.lb.first;
	// 
	// while(vvert) {
	// 	((EditVert*)vvert->vertex)->vvert = NULL;
	// 	vvert->vertex = NULL;
	// 	vvert = vvert->next;
	// }
	// 
	// destroy_geometry(vnode);
}

/*
 * create new Mesh from geometry verse node ... it is temp hack, because
 * this function creates only empty mesh without any data ... data will
 * be added in editmode (after receiving whole geometry)
 */
Mesh *create_mesh_from_verse_node(VNode *vnode)
{
// 	struct Mesh *me;
// 
// 	if(!(vnode && vnode->type==V_NT_GEOMETRY)) return NULL;
// 
// /*	me = add_mesh();*/
// 
// 	me->vnode = (void*)vnode;
// 	((VGeomData*)vnode->data)->mesh = (void*)me;
// 
// 	return me;
}


/*#endif*/

