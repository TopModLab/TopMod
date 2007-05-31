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

#include "mydevice.h"

#include "DNA_object_types.h"
#include "DNA_mesh_types.h"
#include "DNA_listBase.h"

#include "TLI_dynamiclist.h"
#include "TLI_toplib.h"
#include "TLI_editVert.h"
#include "TLI_arithb.h"

#include "TIF_verse.h"
#include "TIF_space.h"
#include "TIF_editmesh.h"

#include "TKE_verse.h"
#include "TKE_global.h"
#include "TKE_object.h"
#include "TKE_utildefines.h"
#include "TKE_depsgraph.h"

/*#include "BDR_editobject.h"*/

#include "verse.h"

/* button callback function, it test object name and send new name to verse server */
void test_and_send_idbutton_cb(void *obj, void *ob_name)

{
// 	struct Object *ob = (Object*)obj;
// 	char *name= (char*)ob_name;
// 
// /*	test_idbutton(name+2);*/
// 
// 	if(ob->vnode) verse_send_node_name_set(((VNode*)ob->vnode)->id, name+2);
}

/*
 * temp hack: this function push mesh objects (edit mode only) to verse server
 */
void t_verse_push_object(VerseSession *session, Object *ob)
{
// 	struct VNode *obj_vnode, *geom_vnode;
// 
// 	if(!session) return;
// 
// 	if(!(session->flag & VERSE_CONNECTED)) return;
// 
// 	/* create "my" new object VerseNode */
// 	obj_vnode= create_verse_node(session->vsession, -1 , V_NT_OBJECT, VN_OWNER_MINE);
// 	/* create object data */
// 	obj_vnode->data = create_object_data();
// 
// 	/* set up name of VerseNode */
// 	obj_vnode->name = (char*)MEM_mallocN(sizeof(char*)*(strlen(ob->id.name)-1), "object node name");
// 	obj_vnode->name[0] = '\0';
// 	strcat(obj_vnode->name, ob->id.name+2);
// 
// 	/* set up object node transformation */
// 	VECCOPY(((VObjectData*)obj_vnode->data)->pos, ob->loc);
// /*	EulToQuat(ob->rot, ((VObjectData*)obj_vnode->data)->rot);*/
// 	VECCOPY(((VObjectData*)obj_vnode->data)->scale, ob->size);
// 
// 	/* set up pointers between Object and VerseNode */
// 	((VObjectData*)obj_vnode->data)->object = (void*)ob;
// 	ob->vnode = (void*)obj_vnode;
// 
// 	/* add node to sending queue */
// 	add_item_to_send_queue(&(session->queue), obj_vnode, VERSE_NODE);
// 
// 	if(ob->type==OB_MESH) {
// 		struct VLink *vlink; 
// 		struct EditMesh *em;
// 
// 		/* create "my" new geometry VerseNode */
// 		geom_vnode= create_verse_node(session->vsession, -1 , V_NT_GEOMETRY, VN_OWNER_MINE);
// 		/* create new geometry data */
// 		geom_vnode->data = create_geometry_data();
// 
// 		/* create new link between VereseNodes */
// 		vlink = create_verse_link(session->vsession, obj_vnode, geom_vnode, -1, -1, "geometry");
// 		/* send link to verse server */
// 		add_item_to_send_queue(&(((VObjectData*)obj_vnode->data)->queue), vlink, VERSE_LINK);
// 
// 		/* set up name of VerseNode */
// 		geom_vnode->name = (char*)MEM_mallocN(sizeof(char*)*(strlen(((Mesh*)ob->data)->id.name)-1),
// 				"object node name");
// 		geom_vnode->name[0] = '\0';
// 		strcat(geom_vnode->name, ((Mesh*)ob->data)->id.name+2);
// 
// 		((Mesh*)ob->data)->vnode = (void*)geom_vnode;
// 
// 		if(G.obedit==ob) {
// 			/* set up pointers between EditMesh and VerseNode */
// 			em = G.editMesh;
// 			em->vnode = (void*)geom_vnode;
// 			((VGeomData*)geom_vnode->data)->editmesh = (void*)em;
// 			((VGeomData*)geom_vnode->data)->mesh = ob->data;
// 		}
// 		else {
// 			/* set up pointer beteen Mesh and VerseNode */
// 			((VGeomData*)geom_vnode->data)->mesh = ob->data;
// 			((Mesh*)ob->data)->vnode = (void*)geom_vnode;
// 		}
// 
// 		/* add node to sending queue */
// 		add_item_to_send_queue(&(session->queue), geom_vnode, VERSE_NODE);
// 	}
}

/*
 * create blender Object from object verse node, it will subscribe this verse
 * node for changes of transformation matrix (subscription for changes of trans.
 * mat. wasn't neccessary, when object verse node wasn't visualised)
 */
Object *create_object_from_verse_node(VNode *vnode)
{
// 	struct Object *ob;
// 
// 	if(!(vnode && vnode->type==V_NT_OBJECT)) return NULL;
// 
// /*	ob = add_object(OB_MESH);*/
// 
// 	ob->vnode = (void*)vnode;
// 	((VObjectData*)vnode->data)->object = (void*)ob;
// 
// 	((VObjectData*)vnode->data)->flag |= POS_RECEIVE_READY;
// 	((VObjectData*)vnode->data)->flag |= ROT_RECEIVE_READY;
// 	((VObjectData*)vnode->data)->flag |= SCALE_RECEIVE_READY;
// 
// 	if(ob) {
// 		char *str;
// 		str = (char*)malloc(sizeof(char)*(strlen(vnode->name)+3));
// 		str[0] = '\0';
// 		strcat(str, "OB");
// 		strcat(str, vnode->name);
// 		strncpy(ob->id.name, str, 23);
// 		printf("\tob->id.name: %s\n", ob->id.name);
// 		free(str);
// 	}
// 
// 	verse_send_o_transform_subscribe(vnode->id, 0);
// 
// 	return ob;
}

/*
 * Create blender object-mesh from verse object node, verse geometry node,
 * object will be in edit mode after "subscribing"
 */
void t_verse_pop_node(VNode *vnode)
{
// 	struct VNode *geom_node=NULL;
// 	struct VLink *vlink;
// 	struct Object *ob;
// 	struct Mesh *me;
// 
// 	if(!(vnode && vnode->type==V_NT_OBJECT)) return;
// 
// 	if(((VObjectData*)vnode->data)->object) {
// 		printf("\talready subscribed to object node ... can't subscribe again\n");
// 		return;
// 	}
// 
// 	vlink = ((VObjectData*)vnode->data)->links.lb.first;
// 
// 	while(vlink) {
// 		if(vlink->target && vlink->target->type==V_NT_GEOMETRY){
// 			geom_node = vlink->target;
// 			break;
// 		}
// 		vlink = vlink->next;
// 	}
// 
// 	if(!geom_node) return;
// 
// /*	if(G.obedit) exit_editmode(2);*/
// 	
// 	ob = create_object_from_verse_node(vnode);
// 
// 	me = create_mesh_from_verse_node(geom_node);
// 
// 	if(ob && me) {
// 		ob->data = me;
// 
// 		G.obedit = ob;
// 
// 		create_edit_mesh_from_verse_node(geom_node);
// 	}
}

/*
 * "unsubscribe" verse node ... VerseNode is still subscribed, but constraint
 * between VerseNode and Object is "deleted" as well constraint between Mesh and
 * mesh geom VerseNode.
 */
void t_verse_unsubscribe_node(VNode *vnode)
{
// 	struct Object *ob;
// 	struct Mesh *me;
// 	struct EditMesh *em;
// 	struct VNode *geom_node=NULL;
// 	struct VLink *vlink;
// 
// 	if(!(vnode && vnode->type==V_NT_OBJECT)) return;
// 
// 	if(!((VObjectData*)vnode->data)->object) {
// 		printf("\tnot subscribed to object node ... can't unsubscribe ;-)\n");
// 		return;
// 	}
// 
// 	vlink = ((VObjectData*)vnode->data)->links.lb.first;
// 
// 	while(vlink) {
// 		if(vlink->target && vlink->target->type==V_NT_GEOMETRY){
// 			geom_node = vlink->target;
// 			break;
// 		}
// 		vlink = vlink->next;
// 	}
// 
// 	if(!geom_node) return;
// 
// 	/* leave edit mode before unsubscribing */
// /*	if(G.obedit==((VObjectData*)vnode->data)->object) load_editMesh();*/
// 
// 	/* delete constraint between Object and object VerseNode */
// 	ob = (Object*)((VObjectData*)vnode->data)->object;
// 	ob->vnode = NULL;
// 	((VObjectData*)vnode->data)->object = NULL;
// 
// 	/* delete constraint between Mesh and geometry VerseNode */
// 	me = (Mesh*)((VGeomData*)geom_node->data)->mesh;
// 	me->vnode = NULL;
// 	((VGeomData*)geom_node->data)->mesh = NULL;
// 
// 	/* delete constraint between Mesh and geometry VerseNode */
// 	em = (EditMesh*)((VGeomData*)geom_node->data)->editmesh;
// 	if(em) {
// 		em->vnode = NULL;
// 		((VGeomData*)geom_node->data)->editmesh = NULL;
// 	}
}

/*
 * when blender Object is deleted, then we have to unsubscribe and free all
 * VerseNode dependent on this object
 */
void t_verse_delete_object(Object *object)
{
	// struct VNode *vnode;
	// struct VLink *vlink;
	// 
	// vnode = (VNode*)object->vnode;
	// 
	// if(vnode) {
	// 	/* free all child VerseNodes */
	// 	vlink = ((VObjectData*)vnode->data)->links.lb.first;
	// 	while(vlink) {
	// 		free_verse_node(vlink->target);
	// 		vlink = vlink->next;
	// 	}
	// 
	// 	free_verse_node(vnode);
	// }
}

/*
 * whe VerseLink is created between two nodes, the Object start to point at
 * coresponding data
 */
void post_link_set(VLink *vlink)
{
	// struct VNode *target, *source;
	// struct Object *ob=NULL;
	// struct Mesh *me=NULL;
	// 
	// source = vlink->source;
	// target = vlink->target;
	// 
	// if(source->type==V_NT_OBJECT && target->type==V_NT_GEOMETRY){
	// 	if(((VObjectData*)source->data)->object)
	// 		ob = (Object*)((VObjectData*)source->data)->object;
	// 	if(((VGeomData*)target->data)->mesh)
	// 		me = (Mesh*)((VGeomData*)target->data)->mesh;
	// 	if(ob && me && ob->data!=me) ob->data = me;
	// }

/*	allqueue(REDRAWALL, 1);*/
}

/*
 * when VerseLink is deleted, then bindings between Object and data should be removed
 */
void post_link_destroy(VLink *vlink)
{
	// struct VNode *source, *target;
	// struct Object *ob;
	// 
	// source = vlink->source;
	// target = vlink->target;
	// 
	// if(source->type==V_NT_OBJECT && target->type==V_NT_GEOMETRY) {
	// 	if(((VObjectData*)source->data)->object) {
	// 		ob = (Object*)((VObjectData*)source->data)->object;
	// 		ob->data=NULL;
	// 	}
	// }
	// 
/*	allqueue(REDRAWALL, 1);*/
}

/*
 * recalculate transformation matrix of object
 */
void post_transform(VNode *vnode)
{
// 	struct VObjectData *obj_data = (VObjectData*)vnode->data;
// 	float mat_s[4][4];
// 	float mat_r[4][4];
// 	float mat_p[4][4];
// 	float mat[4][4];
// 
// 	if(!obj_data->object) return;
// 
// /*	Mat4One(mat_s);
// 	Mat4One(mat_r);
// 	Mat4One(mat_p);
// */
// 	/* scale */
// 	VECCOPY(((Object*)obj_data->object)->size, obj_data->scale);
// 	mat_s[0][0] = obj_data->scale[0];
// 	mat_s[1][1] = obj_data->scale[1];
// 	mat_s[2][2] = obj_data->scale[2];
// 
// 	/* rotate */
// /*	QuatToEul(obj_data->rot, ((Object*)obj_data->object)->rot);*/
// /*	QuatToMat4(obj_data->rot, mat_r);*/
// 
// 	/* position */
// 	VECCOPY(((Object*)obj_data->object)->loc, obj_data->pos)
// 	mat_p[3][0] = obj_data->pos[0];
// 	mat_p[3][1] = obj_data->pos[1];
// 	mat_p[3][2] = obj_data->pos[2];

	/* matrix multiplication */
/*	Mat4MulMat4(mat, mat_r, mat_p);*/
/*	Mat4MulMat4(((Object*)obj_data->object)->obmat, mat_s, mat);*/

/*	DAG_object_flush_update(G.scene, (Object*)obj_data->object, OB_RECALC_OB);*/
	
/*	allqueue(REDRAWVIEW3D, 1);*/
}

/*
 * free constraint between object VerseNode and blender Object
 */
void post_object_free_constraint(VNode *vnode)
{
	// if(((VObjectData*)vnode->data)->object) {
	// 	((Object*)((VObjectData*)vnode->data)->object)->vnode = NULL;
	// 	((VObjectData*)vnode->data)->object = NULL;
	// }
}

/*
 * send transformation of Object to verse server
 */
void t_verse_send_transformation(Object *ob)
{
// 	struct VNode *vnode= ob->vnode;
// 	float *pos = ((VObjectData*)vnode->data)->pos;
// 	float quat[4];
// 	float *scale = ((VObjectData*)vnode->data)->scale;
// 
// 	if(!vnode) return;
// 
// 	/* if last sent position wasn't received yet, then next change of position
// 	 * can't be send until last send change is received */
// 	if( ((VObjectData*)vnode->data)->flag & POS_SEND_READY ) {
// 		if((pos[0]!=ob->loc[0]) || (pos[1]!=ob->loc[1]) || (pos[2]!=ob->loc[2])) {
// 			VECCOPY(pos, ob->loc);
// 			send_verse_object_position(vnode);
// 		}
// 	}
// 
// 	/* if last sent rotation wasn't received yet, then next change of rotation
// 	 * can't be send until last send change is received */
// 	if( ((VObjectData*)vnode->data)->flag & ROT_SEND_READY ) {
// /*		EulToQuat(ob->rot, quat);*/
// 
// 		if((((VObjectData*)vnode->data)->rot[0] != quat[0]) ||
// 				(((VObjectData*)vnode->data)->rot[1] != quat[1]) ||
// 				(((VObjectData*)vnode->data)->rot[2] != quat[2]) ||
// 				(((VObjectData*)vnode->data)->rot[3] != quat[3])) {
// 			QUATCOPY(((VObjectData*)vnode->data)->rot, quat);
// 			send_verse_object_rotation(vnode);
// 		}
// 	}
// 
// 	/* if last sent object size wasn't received yet, then next change of object size
// 	 * can't be send until last send change is received */
// 	if( ((VObjectData*)vnode->data)->flag & SCALE_SEND_READY ) {
// 		if((scale[0]!=ob->size[0]) || (scale[1]!=ob->size[1]) || (scale[2]!=ob->size[2])) {
// 			VECCOPY(scale, ob->size);
// 			send_verse_object_scale(vnode);
// 		}
// 	}
// 
// 	verse_callback_update(0);
}

/*#endif*/

