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

// #ifdef WITH_VERSE

#ifndef TIF_VERSE_H
#define TIF_VERSE_H

#include "TKE_verse.h"
#include "DNA_meshdata_types.h"

struct Object;

struct EditVert;
struct EditFace;
struct MVert;
struct Mface;

/* verse_object.c */
void test_and_send_idbutton_cb(void *obj, void *ob_name);

void t_verse_pop_node(struct VNode *vnode);
void t_verse_unsubscribe_node(struct VNode *vnode);
void t_verse_push_object(struct VerseSession *session, struct Object *ob);
void t_verse_delete_object(struct Object *ob);

void post_transform(struct VNode *vnode);
void post_link_set(struct VLink *vlink);
void post_link_destroy(struct VLink *vlink);
void post_object_free_constraint(struct VNode *vnode);

struct Object *create_object_from_verse_node(struct VNode *vnode);
void t_verse_send_transformation(struct Object *ob);

/* verse_mesh.c */
void t_verse_send_vertex_delete(struct EditVert *eve);
void t_verse_send_vertex_position(struct EditVert *eve);
void t_verse_send_vertex(struct EditVert *eve);

void t_verse_send_face_delete(struct EditFace *efa);
void t_verse_send_face(struct EditFace *efa);
void t_verse_set_face_smooth(struct EditFace *efa);

void sync_all_versefaces_with_editfaces(struct VNode *vnode);
void sync_all_verseverts_with_editverts(struct VNode *vnode);

void restore_versevert_from_undovert(struct VNode *vnode, struct EditVert *eve, struct VerseVert *old_vvert, unsigned int vvert_id);
void push_editvert_to_verse_server(struct EditVert *ev, struct VNode *vnode, struct VLayer *vlayer);
void push_mvert_to_verse_server(struct MVert *vert, unsigned int i, struct VNode *vnode, struct VLayer *vlayer);
void push_all_verts(struct VNode *vnode, struct VLayer *vlayer);

void restore_verseface_from_undoface(struct VNode *vnode, struct EditFace *efa, struct VerseFace *old_vface, unsigned int vface_id);
void push_mface_to_verse_server(struct MFace *fa, unsigned int i, struct VNode *vnode, struct VLayer *vlayer);
void push_edit_face_to_verse_server(struct EditFace *efa, struct VNode *vnode, struct VLayer *vlayer);
void push_all_faces(struct VNode *vnode, struct VLayer *vlayer);

void create_edit_mesh_from_verse_node(struct VNode *vnode);
struct Mesh *create_mesh_from_verse_node(struct VNode *vnode);

void destroy_unused_geometry(struct VNode *vnode);
void destroy_binding_between_versemesh_and_editmesh(struct VNode *vnode);

void destroy_verse_mesh(struct VNode *vnode);
void push_mesh_geometry(struct VNode *vnode);

void post_layer_create(struct VNode *vnode, struct VLayer *vlayer);
void post_layer_destroy(struct VNode *vnode, struct VLayer *vlayer);

void post_vertex_create(struct VNode *vnode, struct VerseVert *vvert);
void post_vertex_set_xyz(struct VNode *vnode, struct VerseVert *vvert);
void post_vertex_delete(struct VNode *vnode, struct VerseVert *vvert);
void post_vertex_free_constraint(struct VNode *vnode, struct VerseVert *vvert);
void post_polygon_set_uint8(struct VNode *vnode, struct VLayer *vlayer, struct VerseFace *vface);
void post_polygon_create(struct VNode *vnode, struct VerseFace *vface);
void post_polygon_set_corner(struct VNode *vnode, struct VerseFace *vface);
void post_polygon_delete(struct VNode *vnode, struct VerseFace *vface);
void post_polygon_free_constraint(struct VNode *vnode, struct VerseFace *vface);

void post_geometry_free_constraint(struct VNode *vnode);

/* verse_common.c */
struct VerseSession *session_menu(void);
char *verse_client_name(void);

void post_tag_change(struct VNode *vnode, struct VTag *vatg);
void post_taggroup_create(struct VNode *vnode, struct VTagGroup *vtaggroup);

void post_node_create(struct VNode *vnode);
void post_node_destroy(struct VNode *vnode);
void post_node_name_set(struct VNode *vnode);

void post_connect_accept(struct VerseSession *session);
void post_connect_terminated(struct VerseSession *session);
void post_connect_update(struct VerseSession *session);

// #endif

#endif
