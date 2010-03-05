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

#include "TKE_verse.h"

#include "MEM_guardedalloc.h"

#include "DNA_mesh_types.h"	/* temp */
#include "DNA_listBase.h"

#include "TLI_dynamiclist.h"
#include "TLI_toplib.h"

/*#include "TIF_screen.h"*/
#include "TIF_verse.h"

#include "TKE_global.h"	

#include "verse.h"

#include "VerseTopMod.hh"

class VerseTopMod;

struct ListBase session_list={NULL, NULL};

/* list of static function prototypes */
static void cb_connect_terminate(const char *address, const char *bye);
static void cb_connect_accept(void *user_data, uint32 avatar, void *address, void *connection, const uint8 *host_id);
static void set_all_callbacks(void);
static void free_verse_session_data(struct VerseSession *session);

/*
 * callback function for connection terminated
 */
static void cb_connect_terminate(const char *address, const char *bye)
{
	VerseSession *session = (VerseSession*)current_verse_session();

	if(!session) return;

	/* remove session from list of session */
	TLI_remlink(&session_list, session);
	/* do post connect operations */
	session->post_connect_terminated(session);
	/* free session data */
	free_verse_session_data(session);
	/* free session */
	MEM_freeN(session);
}

/*
 * callback function for accepted connection to verse server
 */
static void cb_connect_accept( void *user_data, uint32 avatar, void *address, void *connection, const uint8 *host_id)
{
	struct VerseSession *session = (VerseSession*)current_verse_session();
	uint32 i, mask=0;

	if(!session) return;

	session->flag |= VERSE_CONNECTED;
	session->flag &= ~VERSE_CONNECTING;

	// VerseTopMod::Instance()->write("\nTopMod was connected to verse server: ");//" + address + "\n");
	// VerseTopMod::Instance()->write(QString("VerseSession->counter: %1\n").arg(session->counter));
	printf("\nTopMod was connected to verse server: %s\n", (char*)address);
	printf("\tVerseSession->counter: %d\n", session->counter);

	session->avatar = avatar;

	session->post_connect_accept(session);

	for(i = 0; i < V_NT_NUM_TYPES; i++)
		mask = mask | (1 << i);
	verse_send_node_index_subscribe(mask);
}

/*
 * set up all callbacks for sessions
 */
void set_verse_session_callbacks(void)
{
	/* connection */
	verse_callback_set((void*)verse_send_connect_accept, (void*)cb_connect_accept, NULL);
	/* connection was terminated */
	verse_callback_set((void*)verse_send_connect_terminate, (void*)cb_connect_terminate, NULL);
}

/*
 * set all callbacks used in Blender
 */
static void set_all_callbacks(void)
{
	/* set up all callbacks for sessions */
	set_verse_session_callbacks();

	/* set up callbacks for nodes */
	set_node_callbacks();

	/* set up all callbacks for object nodes */
	set_object_callbacks();

	/* set up all callbacks for geometry nodes */
	set_geometry_callbacks();	
}

/*
 * this function sends and receive all packets for all sessions
 */
void t_verse_update(void)
{
	VerseSession *session, *next_session;

	session = (VerseSession*)session_list.first;
	while(session){
		next_session = session->next;
		verse_session_set(session->vsession);
		if((session->flag & VERSE_CONNECTED) || (session->flag & VERSE_CONNECTING)) {
			verse_callback_update(1000);
			session->post_connect_update(session);
		}
		session = next_session;
	}
}

/*
 * returns VerseSession coresponding to vsession pointer
 */
VerseSession *versesession_from_vsession(VSession vsession)
{
	struct VerseSession *session;

	session = (VerseSession*)session_list.first;

	while(session) {
		if(session->vsession==vsession) return session;
		session = session->next;
	}
	
	return session;
}

/*
 * returns pointer at current VerseSession
 */
VerseSession *current_verse_session(void)
{
	struct VerseSession *session;
	VSession vsession = verse_session_get();

	session = (VerseSession*)session_list.first;

	while(session){
		if(session->vsession == vsession)
			return session;
		session = session->next;
	}

	VerseTopMod::Instance()->write("error: non-existing SESSION occured!\n");
	// printf("error: non-existing SESSION occured!\n");
	return NULL;
}

/*
 * free VerseSession
 */
static void free_verse_session_data(VerseSession *session)
{
	struct VNode *vnode;

	/* free data of all nodes */
	vnode = (VNode*)session->nodes.lb.first;
	while(vnode){
		free_verse_node_data(vnode);
		vnode = vnode->next;
	}

	/* free data of nodes waiting in queue */
	vnode = (VNode*)session->queue.first;
	while(vnode){
		free_verse_node_data(vnode);
		vnode = vnode->next;
	}

	/* free all VerseNodes */
	TLI_dlist_destroy(&(session->nodes));
	/* free all VerseNodes waiting in queque */
	TLI_freelistN(&(session->queue));

	/* free name of verse host for this session */
	MEM_freeN(session->address);
}

/*
 * free VerseSession
 */
void free_verse_session(VerseSession *session)
{
	/* remove session from session list*/
	TLI_remlink(&session_list, session);
	/* do post terminated operations */
	session->post_connect_terminated(session);
	/* free session data (nodes, layers) */
	free_verse_session_data(session);
	/* free session */
	MEM_freeN(session);
}

/*
 * create new verse session and return coresponding data structure
 */
VerseSession *create_verse_session( const char *name, const char *pass, const char *address, uint8 *expected_key)
{
	struct VerseSession *session;
	VSession vsession;
	
	vsession = verse_send_connect(name, pass, address, expected_key);

	if(!vsession) return NULL;

	session = (VerseSession*)MEM_mallocN(sizeof(VerseSession), "VerseSession");

	session->flag = VERSE_CONNECTING;

	session->vsession = vsession;
	session->avatar = -1;

	session->address = (char*)MEM_mallocN(sizeof(char)*(strlen(address)+1),"session adress name");
	strcpy(session->address, address);

	session->connection = NULL;
	session->host_id = NULL;
	session->counter = 0;

	/* initialize dynamic list of nodes and node queue */
	TLI_dlist_init(&(session->nodes));
	session->queue.first = session->queue.last = NULL;

	/* set up all client dependent functions */
	session->post_connect_accept = post_connect_accept;
	session->post_connect_terminated = post_connect_terminated;
	session->post_connect_update = post_connect_update;

	return session;
}

/*
 * end verse session and free all session data
 */
void end_verse_session(VerseSession *session)
{
	verse_send_connect_terminate(session->address, "topmod: bye bye");
	verse_session_destroy(session->vsession);
	free_verse_session(session);
}

/*
 * end connection to all verse hosts (servers) ... free all VerseSessions
 */
void end_all_verse_sessions(void)
{
	VerseSession *session;

	session = (VerseSession*)session_list.first;

	while(session) {
		free_verse_session_data(session);
		session->post_connect_terminated(session);
		session = session->next;
	}

	TLI_freelistN(&session_list);
}

/*
 * connect to verse host, set up all callbacks, create session
 */
void t_verse_connect(char *address)
{
	VerseSession *session;
	
	/* if no session was created before, then set up all callbacks */
	if((session_list.first==NULL) && (session_list.last==NULL)){
		VerseTopMod::Instance()->write("establishing first connection to verse server...");
		set_all_callbacks();
	}

	/* create new session */
	if(address)
		session = create_verse_session("TopMod", "pass", address, NULL);

	if(session) {
		/* add new session to the list of sessions */
		TLI_addtail(&session_list, session);

		/* add verse handler if this is first session */
		if(session_list.first == session_list.last){
			t_verse_update();
		}
/*			add_screenhandler(G.curscreen, SCREEN_HANDLER_VERSE, 1);*/

	}
}

/*#endif*/