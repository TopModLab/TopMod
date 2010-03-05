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
	/* for tags */
static void free_verse_tag_data(struct VTag *vtag);
static struct VTag *find_tag_in_queue(struct VTagGroup *vtaggroup, const char *name);
static struct VTag *create_verse_tag(VNodeID node_id, uint16 group_id, uint16 tag_id, const char *name, VNTagType type, const VNTag *tag);
	/* for verse tag groups */
static void free_verse_taggroup_data(struct VTagGroup *taggroup);
static struct VTagGroup *find_taggroup_in_queue(struct VNode *vnode, const char *name);
static struct VTagGroup *create_verse_taggroup(VNodeID node_id, uint16 group_id, const char *name);
	/* for verse nodes */
static void move_verse_node_to_dlist(struct VerseSession *session, VNodeID vnode_id);
	/* function prototypes of node callback functions */
static void cb_tag_destroy(void *user_data, VNodeID node_id, uint16 group_id, uint16 tag_id);
static void cb_tag_create(void *user_data, VNodeID node_id, uint16 group_id, uint16 tag_id, const char *name, VNTagType type, const VNTag *tag);
static void cb_tag_group_destroy(void *user_data, VNodeID node_id, uint16 group_id);
static void cb_tag_group_create(void *user_data, VNodeID node_id, uint16 group_id, const char *name);
static void cb_node_name_set(void *user_data, VNodeID node_id, const char *name);
static void cb_node_destroy(void *user_data, VNodeID node_id);
static void cb_node_create(void *user_data, VNodeID node_id, uint8 type, VNodeID owner_id);

/*
 * send to verse server new tag
 */
void send_verse_tag(VTag *vtag)
{
	// verse_send_tag_create(vtag->node_id,
	// 		vtag->group_id,
	// 		vtag->id,
	// 		vtag->name,
	// 		vtag->type,
	// 		vtag->tag);
}

/*
 * free tag data
 */
static void free_verse_tag_data(VTag *vtag)
{
	// /* free name of verse tag */
	// MEM_freeN(vtag->name);
	// /* free value of tag */
	// MEM_freeN(vtag->tag);
}

/*
 * try to find tag in sending queue ... it tag will be found, then
 * this function will removed tag from queue and will return pointer
 * at this tag
 */
static VTag *find_tag_in_queue(VTagGroup *vtaggroup, const char *name)
{
	// struct VTag *vtag;
	// 
	// vtag = vtaggroup->queue.first;
	// 
	// while(vtag) {
	// 	if(strcmp(vtag->name, name)==0) {
	// 		TLI_remlink(&(vtaggroup->queue), vtag);
	// 		break;
	// 	}
	// 	vtag = vtag->next;
	// }
	// 
	// return vtag;
}

/*
 * create new verse tag
 */
static VTag *create_verse_tag(VNodeID node_id, uint16 group_id, uint16 tag_id, const char *name, VNTagType type, const VNTag *tag) {
	// struct VTag *vtag;
	// 
	// vtag = (VTag*)MEM_mallocN(sizeof(VTag), "VTag");
	// 
	// vtag->node_id = node_id;
	// vtag->group_id = group_id;
	// vtag->id = tag_id;
	// vtag->name = (char*)MEM_mallocN(sizeof(char)*(strlen(name)+1), "VTag name");
	// strcpy(vtag->name, name);
	// vtag->type = type;
	// 
	// vtag->tag = (VNTag*)MEM_mallocN(sizeof(VNTag), "VNTag");
	// *vtag->tag = *tag;
	// 
	// vtag->value = NULL;
	// 
	// return vtag;
}

/*
 * send taggroup to verse server
 */
void send_verse_taggroup(VTagGroup *vtaggroup)
{
	// verse_send_tag_group_create(
	// 		vtaggroup->node_id,
	// 		vtaggroup->id,
	// 		vtaggroup->name);
}

/*
 * free taggroup data
 */
static void free_verse_taggroup_data(VTagGroup *taggroup)
{
	// struct VTag *vtag;
	// 
	// vtag = taggroup->tags.lb.first;
	// 
	// while(vtag) {
	// 	free_verse_tag_data(vtag);
	// 	vtag = vtag->next;
	// }
	// 
	// /* unsubscribe from taggroup */
	// verse_send_tag_group_unsubscribe(taggroup->node_id, taggroup->id);
	// 
	// TLI_dlist_destroy(&(taggroup->tags));
	// MEM_freeN(taggroup->name);
}

/*
 * move taggroup from queue to dynamic list with access array,
 * set up taggroup id and return pointer at this taggroup
 */
static VTagGroup *find_taggroup_in_queue(VNode *vnode, const char *name)
{
	// struct VTagGroup *vtaggroup;
	// 
	// vtaggroup = vnode->queue.first;
	// 
	// while(vtaggroup) {
	// 	if(strcmp(vtaggroup->name, name)==0) {
	// 		TLI_remlink(&(vnode->queue), vtaggroup);
	// 		break;
	// 	}
	// 	vtaggroup = vtaggroup->next;
	// }
	// 
	// return vtaggroup;
}

/*
 * create new verse group of tags
 */
static VTagGroup *create_verse_taggroup(VNodeID node_id, uint16 group_id, const char *name)
{
	// struct VTagGroup *taggroup;
	// 
	// taggroup = (VTagGroup*)MEM_mallocN(sizeof(VTagGroup), "VTagGroup");
	// 
	// taggroup->node_id = node_id;
	// taggroup->id = group_id;
	// taggroup->name = (char*)MEM_mallocN(sizeof(char)*(strlen(name)+1), "VTagGroup name");
	// strcpy(taggroup->name, name);
	// 
	// TLI_dlist_init(&(taggroup->tags));
	// taggroup->queue.first = taggroup->queue.last = NULL;
	// 
	// taggroup->post_tag_change = post_tag_change;
	// taggroup->post_taggroup_create = post_taggroup_create;
	// 
	// return taggroup;
}

/*
 * move first VerseNode waiting in sending queue to dynamic list of VerseNodes
 * (it usually happens, when "our" VerseNode was received from verse server)
 */
static void move_verse_node_to_dlist(VerseSession *session, VNodeID vnode_id)
{
	// VNode *vnode;
	// 
	// vnode = session->queue.first;
	// 
	// if(vnode) {
	// 	TLI_remlink(&(session->queue), vnode);
	// 	TLI_dlist_add_item_index(&(session->nodes), (void*)vnode, vnode_id);
	// }
}

/*
 * send VerseNode to verse server
 */
void send_verse_node(VNode *vnode)
{
	// /* set verse node session */
	// struct VerseSession *session;
	// session = versesession_from_vsession(vnode->vsession);
	// 
	// verse_send_node_create(
	// 		vnode->id,
	// 		vnode->type,
	// 		session->avatar);
}

/*
 * free Verse Node data
 */
void free_verse_node_data(VNode *vnode)
{
	// struct VTagGroup *vtaggroup;
	// 
	// /* free node data (object, geometry, etc.) */
	// switch(vnode->type){
	// 	case V_NT_OBJECT:
	// 		free_object_data(vnode);
	// 		break;
	// 	case V_NT_GEOMETRY:
	// 		free_geom_data(vnode);
	// 		break;
	// 	default:
	// 		break;
	// }
	// 
	// /* free all tag groups in dynamic list with access array */
	// vtaggroup = vnode->taggroups.lb.first;
	// while(vtaggroup) {
	// 	free_verse_taggroup_data(vtaggroup);
	// 	vtaggroup = vtaggroup->next;
	// }
	// TLI_dlist_destroy(&(vnode->taggroups));
	// 
	// /* free all tag groups still waiting in queue */
	// vtaggroup = vnode->queue.first;
	// while(vtaggroup) {
	// 	free_verse_taggroup_data(vtaggroup);
	// 	vtaggroup = vtaggroup->next;
	// }
	// TLI_freelistN(&(vnode->queue));
	// 
	// /* free node name */
	// MEM_freeN(vnode->name);
	// /* free node data */
	// MEM_freeN(vnode->data);
	// /* unsubscribe from node */
	// verse_send_node_unsubscribe(vnode->id);
}

/*
 * free VerseNode
 */
void free_verse_node(VNode *vnode)
{
	// struct VerseSession *session;
	// 
	// free_verse_node_data(vnode);
	// 
	// session = versesession_from_vsession(vnode->vsession);
	// 
	// TLI_dlist_free_item(&(session->nodes), vnode->id);
}

/*
 * create new Verse Node
 */
VNode* create_verse_node(VSession vsession, VNodeID node_id, uint8 type, VNodeID owner_id)
{
	// struct VNode *vnode;
	// 
	// vnode = (VNode*)MEM_mallocN(sizeof(VNode), "VerseNode");
	// 
	// vnode->vsession = vsession;
	// vnode->id = node_id;
	// vnode->owner_id = owner_id;
	// vnode->name = NULL;
	// vnode->type = type;
	// 
	// TLI_dlist_init(&(vnode->taggroups));
	// vnode->queue.first = vnode->queue.last = NULL;
	// 
	// vnode->data = NULL;
	// 
	// vnode->post_node_create = post_node_create;
	// vnode->post_node_destroy = post_node_destroy;
	// vnode->post_node_name_set = post_node_name_set;
	// 
	// return vnode;
}

/*
 * callback function: tag was destroyed
 */
static void cb_tag_destroy( void *user_data, VNodeID node_id, uint16 group_id, uint16 tag_id)
{
	// struct VerseSession *session = (VerseSession*)current_verse_session();
	// struct VNode *vnode;
	// struct VTagGroup *vtaggroup;
	// struct VTag *vtag;
	// 
	// if(!session) return;
	// 
	// vnode = (VNode*)TLI_dlist_find_link(&(session->nodes), (unsigned int)node_id);
	// if(!vnode) return;
	// 
	// /* try to find tag group in list of tag groups */
	// vtaggroup = TLI_dlist_find_link(&(vnode->taggroups), group_id);
	// 
	// if(!vtaggroup) return;
	// 
	// /* try to find verse tag in dynamic list of tags in tag group */
	// vtag = (VTag*)TLI_dlist_find_link(&(vtaggroup->tags), tag_id);
	// 
	// if(vtag) {
	// 	free_verse_tag_data(vtag);
	// 	TLI_dlist_free_item(&(vtaggroup->tags), vtag->id);
	// }
}

/*
 * callback function: new tag was created
 */
static void cb_tag_create(void *user_data, VNodeID node_id, uint16 group_id, uint16 tag_id, const char *name, VNTagType type, const VNTag *tag)
{
	// struct VerseSession *session = (VerseSession*)current_verse_session();
	// struct VNode *vnode;
	// struct VTagGroup *vtaggroup;
	// struct VTag *vtag;
	// 
	// if(!session) return;
	// 
	// vnode = (VNode*)TLI_dlist_find_link(&(session->nodes), (unsigned int)node_id);
	// if(!vnode) return;
	// 
	// /* try to find tag group in list of tag groups */
	// vtaggroup = TLI_dlist_find_link(&(vnode->taggroups), group_id);
	// 
	// if(!vtaggroup) return;
	// 
	// /* try to find verse tag in dynamic list of tags in tag group */
	// vtag = (VTag*)TLI_dlist_find_link(&(vtaggroup->tags), tag_id);
	// 
	// if(!vtag) {
	// 	/* we will try to find vtag in sending queue */
	// 	vtag = find_tag_in_queue(vtaggroup, name);
	// 
	// 	/* when we didn't create this tag, then we will have to create one */
	// 	if(!vtag) vtag = create_verse_tag(node_id, group_id, tag_id, name, type, tag);
	// 	else vtag->id = tag_id;
	// 
	// 	/* add tag to the list of tags in tag group */
	// 	TLI_dlist_add_item_index(&(vtaggroup->tags), vtag, tag_id);
	// 
	// 	/* post create method */
	// 	vtaggroup->post_tag_change(vnode, vtag);
	// }
	// else {
	// 	/* this tag exists, then we will propably change value of this tag */
	// 	if((vtag->type != type) || (strcmp(vtag->name, name)!=0)) {
	// 		/* changes of type or name are not allowed and such
	// 		 * stupid changes will be returned back */
	// 		send_verse_tag(vtag);
	// 	}
	// 	else {
	// 		/* post create method */
	// 		vtaggroup->post_tag_change(vnode, vtag);
	// 	}
	// }
}

/*
 * callback function: tag group was destroyed
 */
static void cb_tag_group_destroy(	void *user_data, VNodeID node_id, uint16 group_id)
{
	// struct VerseSession *session = (VerseSession*)current_verse_session();
	// struct VNode *vnode;
	// struct VTagGroup *vtaggroup;
	// 
	// if(!session) return;
	// 
	// vnode = (VNode*)TLI_dlist_find_link(&(session->nodes), (unsigned int)node_id);
	// if(!vnode) return;
	// 
	// vtaggroup = TLI_dlist_find_link(&(vnode->taggroups), group_id);
	// 
	// if(vtaggroup) {
	// 	free_verse_taggroup_data(vtaggroup);
	// 	TLI_dlist_free_item(&(vnode->taggroups), vtaggroup->id);
	// }
}

/*
 * callback function: new tag group was created
 */
static void cb_tag_group_create( void *user_data, VNodeID node_id, uint16 group_id, const char *name) {
	// struct VerseSession *session = (VerseSession*)current_verse_session();
	// struct VNode *vnode;
	// struct VTagGroup *vtaggroup;
	// 
	// if(!session) return;
	// 
	// vnode = (VNode*)TLI_dlist_find_link(&(session->nodes), (unsigned int)node_id);
	// if(!vnode) return;
	// 
	// /* name of taggroup has to begin with string "topmod:" */
	// if(strncmp("topmod:", name, 8)) return;
	// 
	// /* try to find tag group in list of tag groups */
	// vtaggroup = TLI_dlist_find_link(&(vnode->taggroups), group_id);
	// 
	// if(!vtaggroup) {
	// 	/* subscribe to tag group (when new tag will be created, then blender will
	// 	 * receive command about it) */
	// 	verse_send_tag_group_subscribe(node_id, group_id);
	// 
	// 	/* try to find taggroup in waiting queue */
	// 	vtaggroup = find_taggroup_in_queue(vnode, name);
	// 
	// 	/* if no taggroup exist, then new has to be created */
	// 	if(!vtaggroup) vtaggroup = create_verse_taggroup(node_id, group_id, name);
	// 	else vtaggroup->id = group_id;
	// 
	// 	/* add tag group to dynamic list with access array */
	// 	TLI_dlist_add_item_index(&(vnode->taggroups), (void*)vtaggroup, (unsigned int)group_id);
	// 
	// 	/* post create method */
	// 	vtaggroup->post_taggroup_create(vnode, vtaggroup);
	// }
	// else {
	// 	/* this taggroup exist and somebody try to change its name */
	// 	if(strcmp(vtaggroup->name, name)!=0) {
	// 		/* blender doesn't allow such stupid and dangerous things */
	// 		send_verse_taggroup(vtaggroup);
	// 	}
	// }
}

/*
 * callback function: change name of node
 */
static void cb_node_name_set( void *user_data, VNodeID node_id,	const char *name)
{
	// struct VerseSession *session = (VerseSession*)current_verse_session();
	// struct VNode *vnode;
	// 
	// if(!session) return;
	// 
	// vnode = (VNode*)TLI_dlist_find_link(&(session->nodes), (unsigned int)node_id);
	// if(vnode && name) {
	// 	if(!vnode->name) {
	// 		vnode->name = (char*)MEM_mallocN(sizeof(char)*(strlen(name)+1), "VerseNode name");
	// 	}
	// 	else if(strlen(name) > strlen(vnode->name)) {
	// 		MEM_freeN(vnode->name);
	// 		vnode->name = (char*)MEM_mallocN(sizeof(char)*(strlen(name)+1), "VerseNode name");
	// 	}
	// 	strcpy(vnode->name, name);
	// 
	// 	vnode->post_node_name_set(vnode);
	// }
}

/*
 * callback function for deleting node
 */
static void cb_node_destroy( void *user_data, VNodeID node_id)
{
	// struct VerseSession *session = (VerseSession*)current_verse_session();
	// struct VNode *vnode;
	// 
	// if(!session) return;
	// 
	// vnode = TLI_dlist_find_link(&(session->nodes), (unsigned int)node_id);
	// 
	// if(vnode) {
	// 	/* remove VerseNode from dynamic list */
	// 	TLI_dlist_rem_item(&(session->nodes), (unsigned int)node_id);
	// 	/* do post destroy operations */
	// 	vnode->post_node_destroy(vnode);
	// 	/* free verse data */
	// 	free_verse_node_data(vnode);
	// 	/* free VerseNode */
	// 	MEM_freeN(vnode);
	// };
}


/*
 * callback function for new created node
 */
static void cb_node_create( void *user_data, VNodeID node_id, uint8 type, VNodeID owner_id)
{
	printf("cb_node_create called");
	
	struct VerseSession *session = (VerseSession*)current_verse_session();
	struct VNode *vnode;
	
	if(!session) return;
	
	if((type==V_NT_OBJECT) || (type==V_NT_GEOMETRY))
		verse_send_node_subscribe(node_id);
	else
		return;
	
	switch(type){
		case V_NT_OBJECT :
			if(owner_id==VN_OWNER_MINE) {
				/* collect VerseNode from VerseNode queue */
				move_verse_node_to_dlist(session, node_id);
				/* send next VerseNode waiting in queue */
				if(session->queue.first) send_verse_node((VNode*)session->queue.first);
				/* get received VerseNode from list of VerseNodes */
				vnode = (VNode*)TLI_dlist_find_link(&(session->nodes), node_id);
				/* set up ID */
				vnode->id = node_id;
				/* send name of object node */
				verse_send_node_name_set(node_id, vnode->name);
				/* subscribe to changes of object node transformations */
				verse_send_o_transform_subscribe(node_id, (VNRealFormat)0);
				/* send object transformation matrix */
				send_verse_object_position(vnode);
				send_verse_object_rotation(vnode);
				send_verse_object_scale(vnode);
			}
			else {
				/* create new VerseNode */
				vnode = create_verse_node(session->vsession, node_id, type, owner_id);
				/* add VerseNode to list of nodes */
				TLI_dlist_add_item_index(&(session->nodes), (void*)vnode, (unsigned int)node_id);
				/* create object data */
				vnode->data = create_object_data();
				/* set up avatar's name */
				if(node_id == session->avatar) {
					char *client_name;
					client_name = verse_client_name();
					verse_send_node_name_set(node_id, client_name);
					MEM_freeN(client_name);
				}
			}
			break;
		case V_NT_GEOMETRY :
			if(owner_id==VN_OWNER_MINE){
				struct VLink *vlink;
				/* collect VerseNode from VerseNode queue */
				move_verse_node_to_dlist(session, node_id);
				/* send next VerseNode waiting in queue */
				if(session->queue.first) send_verse_node((VNode*)session->queue.first);
				/* get received VerseNode from list of VerseNodes */
				vnode = (VNode*)TLI_dlist_find_link(&(session->nodes), node_id);
				/* set up ID */
				vnode->id = node_id;
				/* find unsent link pointing at this VerseNode */
				vlink = find_unsent_link(session, vnode);
				/* send VerseLink */
				send_verse_link(vlink);
				/* send name of geometry node */
				verse_send_node_name_set(node_id, vnode->name);
				/* send two verse layers to verse server */
				verse_send_g_layer_create(node_id, 0, "vertex", VN_G_LAYER_VERTEX_XYZ, 0, 0);
				verse_send_g_layer_create(node_id, 1, "polygon", VN_G_LAYER_POLYGON_CORNER_UINT32, 0, 0);
				verse_send_g_layer_create(node_id, -1, "smooth", VN_G_LAYER_POLYGON_FACE_UINT8, 0, 0);
			}
			else {
				/* create new VerseNode*/
				vnode = create_verse_node(session->vsession, node_id, type, owner_id);
				/* add VerseNode to dlist of nodes */
				TLI_dlist_add_item_index(&(session->nodes), (void*)vnode, (unsigned int)node_id);
				/* create geometry data */
				vnode->data = create_geometry_data();
			}
			break;
		default:
			break;
	}
	
	vnode->post_node_create(vnode);
}

/*
 * set up all callbacks for verse nodes
 */
void set_node_callbacks(void)
{
	/* new node created */
	verse_callback_set((void*)verse_send_node_create, (void*)cb_node_create, NULL);
	/* node was deleted */
	verse_callback_set((void*)verse_send_node_destroy, (void*)cb_node_destroy, NULL);
	/* name of node was set */
	verse_callback_set((void*)verse_send_node_name_set, (void*)cb_node_name_set, NULL);

	/* new tag group was created */
	verse_callback_set((void*)verse_send_tag_group_create, (void*)cb_tag_group_create, NULL);
	/* tag group was destroy */
	verse_callback_set((void*)verse_send_tag_group_destroy, (void*)cb_tag_group_destroy, NULL);

	/* new tag was created */
	verse_callback_set((void*)verse_send_tag_create, (void*)cb_tag_create, NULL);
	/* tag was destroy */
	verse_callback_set((void*)verse_send_tag_destroy, (void*)cb_tag_destroy, NULL);
}

/*#endif*/
