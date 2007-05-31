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

#include "TIF_verse.h"

#include "TKE_verse.h"

#include "verse.h"

/* function prototypes of static functions */

/* callback functions */
static void cb_o_transform_pos_real32(void *user_data, VNodeID node_id, uint32 time_s, uint32 time_f, const real32 *pos, const real32 *speed, const real32 *accelerate, const real32 *drag_normal, real32 drag);
static void cb_o_transform_rot_real32(void *user_data, VNodeID node_id, uint32 time_s, uint32 time_f, const VNQuat32 *rot, const VNQuat32 *speed, const VNQuat32 *accelerate, const VNQuat32 *drag_normal, real32 drag);
static void cb_o_transform_scale_real32(void *user_data, VNodeID node_id, real32 scale_x, real32 scale_y, real32 scale_z);
static void cb_o_link_set(void *user_data, VNodeID node_id, uint16 link_id, VNodeID link, const char *label, uint32 target_id);
static void cb_o_link_destroy(void *user_data, VNodeID node_id,uint16 link_id);

/* other functions */
static void set_target_node_link_pointer(struct VNode *vnode, struct VLink *vlink);
static void free_verse_link_data(struct VLink *vlink);

/*
 * find unsent VerseLink "pointing at this VerseNode"
 */
VLink *find_unsent_link(VerseSession *session, VNode *vnode)
{
	// struct VNode *tmp;
	// struct VLink *vlink;
	// 
	// tmp = session->nodes.lb.first;
	// 
	// while(tmp) {
	// 	if(tmp->type==V_NT_OBJECT) {
	// 		vlink = ((VObjectData*)tmp->data)->queue.first;
	// 		while(vlink) {
	// 			if(vlink->target == vnode)
	// 				return vlink;
	// 			vlink = vlink->next;
	// 		}
	// 	}
	// 	tmp = tmp->next;
	// }
	// return NULL;
}

/*
 * send object position to verse server
 */
void send_verse_object_position(VNode *vnode)
{
	// ((VObjectData*)vnode->data)->flag &= ~POS_SEND_READY;
	// 
	// verse_send_o_transform_pos_real32(
	// 		vnode->id,	/* node id */
	// 		0,		/* time_s ... no interpolation */
	// 		0,		/* time_f ... no interpolation */
	// 		((VObjectData*)vnode->data)->pos,
	// 		NULL,		/* speed ... no interpolation */
	// 		NULL,		/* accelerate  ... no interpolation */
	// 		NULL,		/* drag normal ... no interpolation */
	// 		0.0);		/* drag ... no interpolation */
}

/*
 * send object rotation to verse server
 */
void send_verse_object_rotation(VNode *vnode)
{
	// VNQuat32 rot;
	// 
	// rot.x = ((VObjectData*)vnode->data)->rot[0];
	//         rot.y = ((VObjectData*)vnode->data)->rot[1];
	//         rot.z = ((VObjectData*)vnode->data)->rot[2];
	//         rot.w = ((VObjectData*)vnode->data)->rot[3];
	// 
	// ((VObjectData*)vnode->data)->flag &= ~ROT_SEND_READY;
	// 
	// verse_send_o_transform_rot_real32(
	// 		vnode->id,	/* node id */
	// 		0,              /* time_s ... no interpolation */
	// 		0,              /* time_f ... no interpolation */
	// 		&rot,
	// 		NULL,		/* speed ... no interpolation */
	// 		NULL,           /* accelerate  ... no interpolation */
	// 		NULL,           /* drag normal ... no interpolation */
	// 		0.0);           /* drag ... no interpolation */
}

/*
 * send object rotation to verse server 
 */
void send_verse_object_scale(VNode *vnode)
{
	// ((VObjectData*)vnode->data)->flag &= ~SCALE_SEND_READY;
	// 
	// verse_send_o_transform_scale_real32(
	// 		vnode->id,
	// 		((VObjectData*)vnode->data)->scale[0],
	// 		((VObjectData*)vnode->data)->scale[1],
	// 		((VObjectData*)vnode->data)->scale[2]);
}

/*
 * send VerseLink to verse server
 */
void send_verse_link(VLink *vlink)
{
	// verse_session_set(vlink->vsession);
	// 
	// verse_send_o_link_set(
	// 		vlink->source->id,
	// 		vlink->id,
	// 		vlink->target->id,
	// 		vlink->label,
	// 		vlink->target_id);
}

/*
 * set up pointer at VerseLink of target node (geometry node, material node, etc.)
 */
static void set_target_node_link_pointer(VNode *vnode, VLink *vlink)
{
	// switch (vnode->type) {
	// 	case V_NT_GEOMETRY:
	// 		((VGeomData*)vnode->data)->vlink = vlink;
	// 		break;
	// 	default:
	// 		break;
	// }
}

/*
 * free VerseLink and it's label
 */
static void free_verse_link_data(VLink *vlink)
{
	// MEM_freeN(vlink->label);
}

/*
 * create new VerseLink
 */
VLink *create_verse_link(
		VSession vsession,
		VNode *source,
		VNode *target,
		uint16 link_id,
		uint32 target_id,
		const char *label)
{
	// struct VLink *vlink;
	// 
	// vlink = (VLink*)MEM_mallocN(sizeof(VLink), "VerseLink");
	// vlink->vsession = vsession;
	// vlink->source = source;
	// vlink->target = target;
	// vlink->id = link_id;
	// vlink->target_id = target_id;
	// 
	// set_target_node_link_pointer(target, vlink);
	// 
	// vlink->label = (char*)MEM_mallocN(sizeof(char)*(strlen(label)+1), "VerseLink label");
	// vlink->label[0] = '\0';
	// strcat(vlink->label, label);
	// 
	// vlink->flag = 0;
	// 
	// vlink->post_link_set = post_link_set;
	// vlink->post_link_destroy = post_link_destroy;
	// 
	// return vlink;
}

/*
 * free ObjectData (links, links in queue and lables of links)
 */
void free_object_data(VNode *vnode)
{
	// struct VObjectData *obj = (VObjectData*)vnode->data;
	// struct VLink *vlink;
	// 
	// if(!obj) return;
	// 
	// /* free all labels of links in dlist */
	// vlink = obj->links.lb.first;
	// while(vlink){
	// 	free_verse_link_data(vlink);
	// 	vlink = vlink->next;
	// }
	// 
	// /* free all labels of links waiting in queue */
	// vlink = obj->queue.first;
	// while(vlink){
	// 	free_verse_link_data(vlink);
	// 	vlink = vlink->next;
	// }
	// 
	// /* free constraint between VerseNode and Object */
	// obj->post_object_free_constraint(vnode);
	// 
	// /* unsubscribe from receiving changes of transformation matrix */
	// verse_send_o_transform_unsubscribe(vnode->id, 0);
	// 
	// /* free dynamic list and sendig queue of links */
	// TLI_dlist_destroy(&(obj->links));
	// TLI_freelistN(&(obj->queue));
}

/*
 * create new object data
 */
VObjectData *create_object_data(void)
{
	// VObjectData *obj;
	// 
	// obj = (VObjectData*)MEM_mallocN(sizeof(VObjectData), "VerseObjectData");
	// obj->object = NULL;
	// TLI_dlist_init(&(obj->links));
	// obj->queue.first = obj->queue.last = NULL;
	// obj->flag = 0;
	// 
	// /* transformation matrix */
	// obj->pos[0] = obj->pos[1] = obj->pos[2] = 0.0;
	// obj->rot[0] = obj->rot[1] = obj->rot[2] = 0.0; obj->rot[3] = 1;
	// obj->scale[0] = obj->scale[1] = obj->scale[2] = 1.0;
	// 
	// /* transformation flags */
	// obj->flag |= POS_SEND_READY;
	// obj->flag |= ROT_SEND_READY;
	// obj->flag |= SCALE_SEND_READY;
	// 
	// /* set up pointers at post callback functions */
	// obj->post_transform = post_transform;
	// obj->post_object_free_constraint = post_object_free_constraint;
	// 
	// return obj;
}

/*
 * callback function: 
 */
static void cb_o_transform_pos_real32(
		void *user_data,
		VNodeID node_id,
		uint32 time_s,
		uint32 time_f,
		const real32 *pos,
		const real32 *speed,
		const real32 *accelerate,
		const real32 *drag_normal,
		real32 drag)
{
	// struct VerseSession *session = (VerseSession*)current_verse_session();
	// struct VNode *vnode;
	// float vec[3], dt;
	// 
	// if(!session) return;
	// 
	// vnode = TLI_dlist_find_link(&(session->nodes), (unsigned int)node_id);
	// 
	// ((VObjectData*)vnode->data)->flag |= POS_SEND_READY;
	// 
	// /* verse server sends automaticaly some stupid default values ...
	//  * we have to ignore these values, when we created this object node */
	// if( (vnode->owner_id==VN_OWNER_MINE) && !(((VObjectData*)vnode->data)->flag & POS_RECEIVE_READY) ) {
	// 	((VObjectData*)vnode->data)->flag |= POS_RECEIVE_READY;
	// 	return;
	// }
	// 
	// dt = time_s + time_f/(0xffff);
	// 
	// if(pos) {
	// 	vec[0] = pos[0];
	// 	vec[1] = pos[1];
	// 	vec[2] = pos[2];
	// }
	// 
	// if(speed) {
	// 	vec[0] += speed[0]*dt;
	// 	vec[1] += speed[1]*dt;
	// 	vec[2] += speed[2]*dt;
	// }
	// 
	// if(accelerate) {
	// 	vec[0] += accelerate[0]*dt*dt/2;
	// 	vec[1] += accelerate[1]*dt*dt/2;
	// 	vec[2] += accelerate[2]*dt*dt/2;
	// }
	// 
	// if( (((VObjectData*)vnode->data)->pos[0] != vec[0]) ||
	// 		(((VObjectData*)vnode->data)->pos[1] != vec[1]) ||
	// 		(((VObjectData*)vnode->data)->pos[2] != vec[2]))
	// {
	// 	((VObjectData*)vnode->data)->pos[0] = vec[0];
	// 	((VObjectData*)vnode->data)->pos[1] = vec[1];
	// 	((VObjectData*)vnode->data)->pos[2] = vec[2];
	// 
	// 	((VObjectData*)vnode->data)->post_transform(vnode);
	// }
}

/*
 * callback function:
 */
static void cb_o_transform_rot_real32(void *user_data, VNodeID node_id, uint32 time_s, uint32 time_f, const VNQuat32 *rot, 
																			const VNQuat32 *speed, const VNQuat32 *accelerate, const VNQuat32 *drag_normal, real32 drag)
{
	// struct VerseSession *session = (VerseSession*)current_verse_session();
	// struct VNode *vnode;
	// float vec[4]={0, 0, 0, 0}, dt;
	// 
	// if(!session) return;
	// 
	// vnode = TLI_dlist_find_link(&(session->nodes), (unsigned int)node_id);
	// 
	// ((VObjectData*)vnode->data)->flag |= ROT_SEND_READY;
	// 
	// /* verse server sends automaticaly some stupid default values ...
	//  * we have to ignore these values, when we created this object node */
	// if( (vnode->owner_id==VN_OWNER_MINE) && !(((VObjectData*)vnode->data)->flag & ROT_RECEIVE_READY) ) {
	// 	((VObjectData*)vnode->data)->flag |= ROT_RECEIVE_READY;
	// 	return;
	// }
	// 
	// dt = time_s + time_f/(0xffff);
	// 
	// if(rot) {
	// 	vec[0] = rot->x;
	// 	vec[1] = rot->y;
	// 	vec[2] = rot->z;
	// 	vec[3] = rot->w;
	// }
	// 
	// if(speed) {
	// 	vec[0] += speed->x*dt;
	// 	vec[1] += speed->y*dt;
	// 	vec[2] += speed->z*dt;
	// 	vec[3] += speed->w*dt;
	// }
	// 
	// if(accelerate) {
	// 	vec[0] += accelerate->x*dt*dt/2;
	// 	vec[1] += accelerate->y*dt*dt/2;
	// 	vec[2] += accelerate->z*dt*dt/2;
	// 	vec[3] += accelerate->w*dt*dt/2;
	// }
	// 
	// if( (((VObjectData*)vnode->data)->rot[0] != vec[0]) ||
	// 		(((VObjectData*)vnode->data)->rot[1] != vec[1]) ||
	// 		(((VObjectData*)vnode->data)->rot[2] != vec[2]) ||
	// 		(((VObjectData*)vnode->data)->rot[3] != vec[3]))
	// {
	// 	((VObjectData*)vnode->data)->rot[0] = vec[0];
	// 	((VObjectData*)vnode->data)->rot[1] = vec[1];
	// 	((VObjectData*)vnode->data)->rot[2] = vec[2];
	// 	((VObjectData*)vnode->data)->rot[3] = vec[3];
	// 
	// 	((VObjectData*)vnode->data)->post_transform(vnode);
	// }
}

/*
 * callback function:
 */
static void cb_o_transform_scale_real32( void *user_data, VNodeID node_id, real32 scale_x, real32 scale_y, real32 scale_z)
{
	// struct VerseSession *session = (VerseSession*)current_verse_session();
	// struct VNode *vnode;
	// 
	// if(!session) return;
	// 
	// vnode = TLI_dlist_find_link(&(session->nodes), (unsigned int)node_id);
	// 
	// ((VObjectData*)vnode->data)->flag |= SCALE_SEND_READY;
	// 
	// /* verse server sends automaticaly some stupid default values ...
	//  * we have to ignore these values, when we created this object node */
	// if( (vnode->owner_id==VN_OWNER_MINE) && !(((VObjectData*)vnode->data)->flag & SCALE_RECEIVE_READY) ) {
	// 	((VObjectData*)vnode->data)->flag |= SCALE_RECEIVE_READY;
	// 	return;
	// }
	// 
	// if( (((VObjectData*)vnode->data)->scale[0] != scale_x) ||
	// 		(((VObjectData*)vnode->data)->scale[1] != scale_y) ||
	// 		(((VObjectData*)vnode->data)->scale[2] != scale_z))
	// {
	// 	((VObjectData*)vnode->data)->scale[0] = scale_x;
	// 	((VObjectData*)vnode->data)->scale[1] = scale_y;
	// 	((VObjectData*)vnode->data)->scale[2] = scale_z;
	// 
	// 	((VObjectData*)vnode->data)->post_transform(vnode);
	// }
}

/*
 * callback function: link between object node and some other node was created
 */
static void cb_o_link_set( void *user_data, VNodeID node_id, uint16 link_id, VNodeID link, const char *label, uint32 target_id)
{
	// struct VLink *vlink;
	// struct VNode *source;
	// struct VNode *target;
	// 
	// struct VerseSession *session = (VerseSession*)current_verse_session();
	// 
	// if(!session) return;
	// 
	// source = TLI_dlist_find_link(&(session->nodes), (unsigned int)node_id);
	// target = TLI_dlist_find_link(&(session->nodes), (unsigned int)link);
	// 
	// if(!(source && target)) return;
	// 
	// vlink = ((VObjectData*)source->data)->queue.first;
	// 
	// if(vlink && (vlink->source==source) && (vlink->target==target)) {
	// 	/* remove VerseLink from sending queue */
	// 	TLI_remlink(&(((VObjectData*)source->data)->queue), vlink);
	// 	/* add VerseLink to dynamic list of VerseLinks */
	// 	TLI_dlist_add_item_index(&(((VObjectData*)source->data)->links), vlink, (unsigned int)link_id);
	// 	/* send next link from sending queue */
	// 	if(((VObjectData*)source->data)->queue.first)
	// 		send_verse_link(((VObjectData*)source->data)->queue.first);
	// 	/* set up VerseLink variables */
	// 	vlink->flag = 0;
	// 	vlink->id = link_id;
	// 	vlink->target_id = target_id;
	// }
	// else {
	// 	/* create new VerseLink */
	// 	vlink = create_verse_link(session->vsession, source, target, link_id, target_id, label);
	// 	/* add VerseLink to dynamic list of VerseLinks */
	// 	TLI_dlist_add_item_index(&(((VObjectData*)source->data)->links), vlink, (unsigned int)link_id);
	// }
	// 
	// vlink->post_link_set(vlink);
}

/*
 * callback function: destroy link between two VerseNodes
 */
static void cb_o_link_destroy( void *user_data, VNodeID node_id, uint16 link_id)
{
	// struct VerseSession *session = (VerseSession*)current_verse_session();
	// struct VNode *vnode;
	// struct VLink *vlink;
	// 
	// if(!session) return;
	// 
	// vnode = TLI_dlist_find_link(&(session->nodes), (unsigned int)node_id);
	// 
	// vlink = TLI_dlist_find_link(&(((VObjectData*)vnode->data)->links), link_id);
	// 
	// if(vlink) {
	// 	free_verse_link_data(vlink);
	// 	TLI_dlist_free_item(&(((VObjectData*)vnode->data)->links), link_id);
	// }
	// 
	// vlink->post_link_destroy(vlink);
}

void set_object_callbacks(void)
{
	/* position of object was changed */
	verse_callback_set((void*)verse_send_o_transform_pos_real32, (void*)cb_o_transform_pos_real32, NULL);
	/* rotation of object was changed */
	verse_callback_set((void*)verse_send_o_transform_rot_real32, (void*)cb_o_transform_rot_real32, NULL);
	/* size of object was changed  */
	verse_callback_set((void*)verse_send_o_transform_scale_real32, (void*)cb_o_transform_scale_real32, NULL);
	/* new link between nodes was created */
	verse_callback_set((void*)verse_send_o_link_set, (void*)cb_o_link_set, NULL);
	/* link between nodes was destroyed */
	verse_callback_set((void*)verse_send_o_link_destroy, (void*)cb_o_link_destroy, NULL);
}

/*#endif*/
