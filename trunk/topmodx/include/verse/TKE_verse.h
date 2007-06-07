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

/* #define WITH_VERSE */

#ifndef TKE_VERSE_H
#define TKE_VERSE_H

#include "DNA_listBase.h"
#include "TLI_dynamiclist.h"

#include "verse.h"

#ifndef T_VERSE_CONNECTED
#define T_VERSE_CONNECTED  (1 << 10)
#endif
// static int VerseConnected;
static bool VerseConnected = false;

struct VNode;

/*
 * verse data: 4 float value
 */
typedef struct quat_real32_item {
	struct quat_real32_item *next, *prev;
	real32 value[4];
} quat_real32_item;

/*
 * verse data: 4 uint32 values
 */
typedef struct quat_uint32_item {
	struct quat_uint32_item *next, *prev;
	uint32 value[4];
} quat_uint32_item;

/*
 * verse data: 3 float values
 */
typedef struct vec_real32_item {
	struct vec_real32_item *next, *prev;
	real32 value[3];
} vec_real32_item;

/*
 * verse data: float value (weight)
 */
typedef struct real32_item {
	struct real32_item *next, *prev;
	real32 value;
} real32_item;

/*
 * verse data: uint32 value
 */
typedef struct uint32_item {
	struct uint32_item *next, *prev;
	uint32 value;
} uint32_item;

/*
 * verse data: uint8 value (smooth)
 */
typedef struct uint8_item {
	struct uint8_item *next, *prev;
	uint8 value;
} uint8_item;

/*
 * verse data: vertex
 */
typedef struct VerseVert {
	struct VerseVert *next, *prev;
	/* verse data */
	VSession vsession;		/* session pointer */
	VNodeID node_id;		/* vertex belongs to node with node_id */
	VLayerID layer_id;		/* vertex belongs to layer with layer_id */
	uint32 id;			/* id of vertex */
	real32 co[3];			/* x,y,z-coordinates of vertex */
	real32 no[3];			/* normal of vertex */
	/* blender internals */
	short flag;			/* flags: VERT_DELETED, VERT_RECEIVED */
	void *vertex;			/* pointer at EditVert or MVert */
	int counter;			/* counter of VerseFaces using this VerseVert */
	unsigned int index;		/* counter need during transformation to mesh */
	float *cos;			/* modified coordinates of vertex */
} VerseVert;

/*
 * verse data: polygon
 */
typedef struct VerseFace {
	struct VerseFace *next, *prev;
	/* verse data */
	VSession vsession;		/* session pointer */
	VNodeID node_id;		/* face belongs to node with node_id */
	VLayerID layer_id;		/* face belongs to vlayer with layer_id */
	uint32 id;			/* id of face */
	struct VerseVert *vvert0;	/* pointer at 1st VerseVert */
	struct VerseVert *vvert1;	/* pointer at 2nd VerseVert */
	struct VerseVert *vvert2;	/* pointer at 3th VerseVert */
	struct VerseVert *vvert3;	/* pointer at 4th VerseVert */
	unsigned int v0, v1, v2, v3;	/* indexes of VerseVerts ... needed during receiving */
	/* blender internals */
	short flag;			/* flags: FACE_SEND_READY, FACE_SENT, FACE_RECEIVED, FACE_CHANGED*/
	short counter;			/* counter of missed VerseVertexes */
	void *face;			/* pointer at EditFace or MFace */
	float no[3];			/* normal vector */
	struct uint8_item *smooth;	/* smoothness */
} VerseFace;

/*
 * verse data: layer
 */
typedef struct VLayer {
	struct VLayer *next, *prev;
	/* verse data*/
	VSession vsession;		/* session pointer */
	VNodeID node_id;		/* layer belongs to node with node_id */
	uint16 id;			/* id of layer */
	char *name;			/* name of layer */
	VNGLayerType type;		/* type of layer (VN_G_LAYER_VERTEX_XYZ, VN_G_LAYER_POLYGON_CORNER_UINT32) */
	uint32 def_int;			/* default integer value */
	real64 def_real;		/* default float value */
	/* blender internals */
	short content;			/* type of content (VERTEX_LAYER, POLYGON_LAYER, POLYGON_SMOOTH_LAYER) */
	struct DynamicList dl;		/* vertexes, polygons, etc. */
	struct ListBase queue;		/* queue of vertexes, polygons, etc. waiting for sending to verse server */
	struct ListBase orphans;	/* list of versedata (polygons, etc.), that can be added to the DynamicList
					 * due to not received VerseVerts */
	/* client dependent methods */
	void (*post_layer_create)(struct VNode *vnode, struct VLayer *vlayer);
	void (*post_layer_destroy)(struct VNode *vnode, struct VLayer *vlayer);
} VLayer;

/*
 * verse data: link
 */
typedef struct VLink{
	struct VLink *next, *prev;
	/* verse data */
	VSession vsession;		/* session pointer */
	struct VNode *source;		/* object VerseNode "pointing" at some other VerseNode */
	struct VNode *target;		/* VerseNode linked with some object node */
	unsigned int id;		/* id of VerseLink */
	unsigned int target_id;		/* some unknow id*/
	char *label;			/* name/label of VerseLink */
	/* blender internals */
	short flag;			/* flag: LINK_SEND_READY */
	/* client dependent methods */
	void (*post_link_set)(struct VLink *vlink);
	void (*post_link_destroy)(struct VLink *vlink);
} VLink;

/* 
 * data of geometry node
 */
typedef struct VGeomData {
	struct DynamicList layers;	/* dynamic list with access array of Layers */
	struct VLink *vlink;		/* pointer at VerseLink connecting object node and geom node */
	struct ListBase queue;		/* queue of our layers waiting for receiving from verse server */
	void *mesh;			/* pointer at Mesh (object node) */
	void *editmesh;			/* pointer at EditMesh (edit mode) */
	unsigned int smooth_layer;	/* number of verse layer containing information about face smoothness */
	/* client dependent methods */
	void (*post_vertex_create)(struct VNode *vnode, struct VerseVert *vvert);
	void (*post_vertex_set_xyz)(struct VNode *vnode, struct VerseVert *vvert);
	void (*post_vertex_delete)(struct VNode *vnode, struct VerseVert *vvert);
	void (*post_vertex_free_constraint)(struct VNode *vnode, struct VerseVert *vvert);
	void (*post_polygon_create)(struct VNode *vnode, struct VerseFace *vface);
	void (*post_polygon_set_corner)(struct VNode *vnode, struct VerseFace *vface);
	void (*post_polygon_delete)(struct VNode *vnode, struct VerseFace *vface);
	void (*post_polygon_free_constraint)(struct VNode *vnode, struct VerseFace *vface);
	void (*post_geometry_free_constraint)(struct VNode *vnode);
	void (*post_polygon_set_uint8)(struct VNode *vnode, struct VLayer *vlayer, struct VerseFace *vface);
} VGeomData;

/*
 * data of object node
 */
typedef struct VObjectData {
	struct DynamicList links;	/* dynamic list with access array of links between other nodes */
	struct ListBase queue;		/* queue of links waiting for sending and receiving from verse server */
	float pos[3];			/* position of object VerseNode */
	float rot[4];			/* rotation of object VerseNode stored in quat */
	float scale[3];			/* scale of object VerseNode */
	void *object;			/* pointer at object */
	short flag;			/* flag: POS_RECEIVE_READY, ROT_RECEIVE_READY. SCALE_RECEIVE_READY */
	/* client dependent methods */
	void (*post_transform)(struct VNode *vnode);
	void (*post_object_free_constraint)(struct VNode *vnode);
} VObjectData;

/*
 * Verse Tag
 */
typedef struct VTag {
	struct VTag *next, *prev;
	/* verse data*/
	VNodeID node_id;	/* verse node containing this tag */
	uint16 group_id;	/* tag group containing this tag */
	uint16 id;		/* id of this tag */
	char *name;		/* name of this tag*/
	VNTagType type;		/* type: VN_TAG_BOOLEAN, VN_TAG_UINT32, VN_TAG_REAL64, VN_TAG_REAL64_VEC3,
				   VN_TAG_LINK, VN_TAG_ANIMATION, VN_TAG_BLOB */
	VNTag *tag;		/* pointer at value (enum: vboolean, vuint32, vreal64, vstring,
				   vreal64_vec3, vlink, vanimation, vblob)*/
	/* blender internals */
	void *value;		/* pointer at blender value */
} VTag;

/*
 * Verse Tag Group (verse tags are grouped in tag groups)
 */
typedef struct VTagGroup {
	struct VTagGroup *next, *prev;
	/* verse data*/
	VNodeID node_id;		/* verse node containing this tag group */
	uint16 id;			/* id of this tag group */
	char *name;			/* name of this tag group */
	/* blender internals */
	struct DynamicList tags;	/* dynamic list with access array containing tags */
	struct ListBase queue;		/* list of tags waiting for receiving from verse server */
	/* client dependent methods */
	void (*post_tag_change)(struct VNode *vnode, struct VTag *vatg);
	void (*post_taggroup_create)(struct VNode *vnode, struct VTagGroup *vtaggroup);
} VTagGroup;

/*
 * Verse Node
 */
typedef struct VNode {
	struct VNode *next, *prev;
	/* verse data*/
	VSession vsession;		/* session pointer */
	VNodeID id;			/* node id */
	VNodeID owner_id;		/* owner's id of this node */
	char *name;			/* name of this node */
	uint32 type;			/* type of node (V_NT_OBJECT, V_NT_GEOMETRY) */
	/* blender internals */
	struct DynamicList taggroups;	/* dynamic list with access array of taggroups */
	struct ListBase queue;		/* list of taggroups waiting for receiving from verse server */
	void *data;			/* generic pointer at some data (VObjectData, VGeomData, ...) */
	/* client dependent methods */
	void (*post_node_create)(struct VNode *vnode);	
	void (*post_node_destroy)(struct VNode *vnode);
	void (*post_node_name_set)(struct VNode *vnode);
} VNode;

/*
 * Verse Session: verse client can be connected to several verse servers
 * it is neccessary to store some information about each session
 */
typedef struct VerseSession {
	struct VerseSession *next, *prev;
	/* verse data */
	VSession vsession;		/* stupid pointer , index would be better */
	uint32 avatar;			/* id of avatar */
	char *address;			/* string containg IP/domain name of verse server and number of port */
	void *connection;		/* no clue */
	uint8 *host_id;			/* no clue */
	/* blender internals */
	short flag;			/* flag: VERSE_DISCONNECTED, VERSE_CONNECTING, VERSE_CONNECTED */
	DynamicList nodes;		/* list of verse nodes */
	ListBase queue;			/* list of nodes waiting for sending to verse server */
	unsigned int counter;		/* count of events, when connection wasn't accepted */
	/* client dependent methods */
	void (*post_connect_accept)(struct VerseSession *session);
	void (*post_connect_terminated)(struct VerseSession *session);
	void (*post_connect_update)(struct VerseSession *session);
} VerseSession;

/* VerseSession->flag */
#define VERSE_DISCONNECTED	1
#define VERSE_CONNECTING	2
#define VERSE_CONNECTED		4

/* max VerseSession->counter value */
#define MAX_UNCONNECTED_EVENTS	100

/* VLink->flag */
#define LINK_SEND_READY		1

/* VObjectData->flag */
#define POS_RECEIVE_READY	1
#define ROT_RECEIVE_READY	2
#define SCALE_RECEIVE_READY	4
#define POS_SEND_READY		8
#define ROT_SEND_READY		16
#define SCALE_SEND_READY	32

/* VLayer->content */
#define VERTEX_LAYER		0
#define POLYGON_LAYER		1
#define POLYGON_SMOOTH_LAYER	2

/* VerseVert->flag */
#define VERT_DELETED		1
#define VERT_RECEIVED		2
#define VERT_SEND_READY		4

/* VerseFace->flag */
#define FACE_SEND_READY		1
#define FACE_RECEIVED		2
#define FACE_SENT		4
#define FACE_DELETED		8
#define FACE_CHANGED		16

/* queue type */
#define VERSE_NODE		1
#define VERSE_LINK		2
#define VERSE_LAYER		3
#define VERSE_VERT		4
#define VERSE_FACE		5
#define VERSE_TAG		6
#define VERSE_TAG_GROUP		7

/* function prototypes */

/* functions from verse_session.c */
void set_verse_session_callbacks(void);
struct VerseSession *versesession_from_vsession(VSession vsession);
struct VerseSession *current_verse_session(void);
void end_verse_session(struct VerseSession *session);
struct VerseSession *create_verse_session(const char *name, const char *pass, const char *address, uint8 *expected_key);
void free_verse_session(struct VerseSession *session);
void t_verse_update(void);
void t_verse_connect(char *address);
void end_verse_session(struct VerseSession *session);
void end_all_verse_sessions(void);

/* functions from verse_node.c */
void send_verse_tag(struct VTag *vtag);
void send_verse_taggroup(struct VTagGroup *vtaggroup);
void send_verse_node(struct VNode *vnode);
void free_verse_node_data(struct VNode *vnode);
void free_verse_node(struct VNode *vnode);
struct VNode* create_verse_node(VSession vsession, VNodeID node_id, uint8 type, VNodeID owner_id);
void set_node_callbacks(void);

/* functions from verse_object_node.c */
struct VLink *find_unsent_link(struct VerseSession *session, struct VNode *vnode);
struct VLink *create_verse_link(VSession vsession, struct VNode *source, struct VNode *target, uint16 link_id, uint32 target_id, const char *label);
void send_verse_object_position(struct VNode *vnode);
void send_verse_object_rotation(struct VNode *vnode);
void send_verse_object_scale(struct VNode *vnode);
void send_verse_link(struct VLink *vlink);

void free_object_data(struct VNode *vnode);
void set_object_callbacks(void);
struct VObjectData *create_object_data(void);

/* functions from verse_geometry_node.c */
struct VerseFace* create_verse_face(VSession vsession, VNodeID node_id, VLayerID layer_id, uint32 polygon_id, uint32 v0, uint32 v1, uint32 v2, uint32 v3);
VerseVert* create_verse_vertex(VSession vsession, VNodeID node_id, VLayerID layer_id, uint32 vertex_id, real32 x, real32 y, real32 z);
struct VGeomData *create_geometry_data(void);

void send_verse_vertex_delete(struct VerseVert *vvert);
void send_verse_vertex(struct VerseVert *vvert);
void send_verse_face_delete(struct VerseFace *vface);
void send_verse_face_smoothness(struct VerseFace *vface, uint8 value);

void destroy_geometry(struct VNode *vnode);

struct VLayer* find_verse_layer_type(struct VGeomData *geom, short content);
void add_item_to_send_queue(struct ListBase *lb, void *item, unsigned int type);
void free_geom_data(struct VNode *vnode);
void set_geometry_callbacks(void);

#endif // 
