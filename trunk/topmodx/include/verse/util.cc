/* util.c
 *
 * various string, file, list operations.
 *
 *
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
 * The Original Code is Copyright (C) 2001-2002 by NaN Holding BV.
 * All rights reserved.
 *
 * The Original Code is: all of this file.
 *
 * Contributor(s): none yet.
 *
 * ***** END GPL/BL DUAL LICENSE BLOCK *****
 * 
 */

#include <stdio.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "MEM_guardedalloc.h"

#include "TLI_toplib.h"
#include "DNA_listBase.h"
/*#include "TLI_storage.h"*/
/*#include "BLI_storage_types.h"*/
#include "TLI_dynamiclist.h"

#include "TLI_util.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef WIN32 
#include <unistd.h>
#else
#include <io.h>
#endif

#ifdef WIN32
/*#include "BLI_winstuff.h"*/
#endif


#ifndef WIN32
#include <sys/time.h>
#endif

#ifdef __APPLE__
#include <sys/param.h>
#include <CoreFoundation/CoreFoundation.h>
#endif


// copies from BKE_utildefines
#ifndef FILE_MAXDIR
#define FILE_MAXDIR  160
#endif

#ifndef FILE_MAXFILE
#define FILE_MAXFILE 80
#endif

/* local */

static int add_win32_extension(char *name);

/* implementation */

/* Ripped this from blender.c
 */
void addlisttolist(ListBase *list1, ListBase *list2)
{

	if(list2->first==0) return;

	if(list1->first==0) {
		list1->first= list2->first;
		list1->last= list2->last;
	}
	else {
		((struct Link *)list1->last)->next= (Link*)list2->first;
		((struct Link *)list2->first)->prev= (Link*)list1->last;
		list1->last= list2->last;
	}
	list2->first= list2->last= 0;
}


void TLI_addhead(ListBase *listbase, void *vlink)
{
	struct Link *link= (Link*)vlink;

	if (link == 0) return;
	if (listbase == 0) return;

	link->next = (Link*)listbase->first;
	link->prev = 0;

	if (listbase->first) ((struct Link *)listbase->first)->prev = link;
	if (listbase->last == 0) listbase->last = link;
	listbase->first = link;
}


void TLI_addtail(ListBase *listbase, void *vlink)
{
	struct Link *link= (Link*)vlink;

	if (link == 0) return;
	if (listbase == 0) return;

	link->next = 0;
	link->prev = (Link*)listbase->last;

	if (listbase->last) ((struct Link *)listbase->last)->next = link;
	if (listbase->first == 0) listbase->first = link;
	listbase->last = link;
}


void TLI_remlink(ListBase *listbase, void *vlink)
{
	struct Link *link= (Link*)vlink;

	if (link == 0) return;
	if (listbase == 0) return;

	if (link->next) link->next->prev = link->prev;
	if (link->prev) link->prev->next = link->next;

	if (listbase->last == link) listbase->last = link->prev;
	if (listbase->first == link) listbase->first = link->next;
}


void TLI_freelinkN(ListBase *listbase, void *vlink)
{
	struct Link *link= (Link*)vlink;

	if (link == 0) return;
	if (listbase == 0) return;

	TLI_remlink(listbase,link);
	MEM_freeN(link);
}


void TLI_insertlink(ListBase *listbase, void *vprevlink, void *vnewlink)
{
	struct Link *prevlink= (Link*)vprevlink, *newlink= (Link*)vnewlink;

	/* newlink comes after prevlink */

	if (newlink == 0) return;
	if (listbase == 0) return;

	if(listbase->first==0) { /* empty list */
		listbase->first= newlink;
		listbase->last= newlink;
		return;
	}
	if (prevlink== 0) {	/* insert before first element */
		newlink->next= (Link*)listbase->first;
		newlink->prev= 0;
		newlink->next->prev= (Link*)newlink;
		listbase->first= (Link*)newlink;
		return;
	}

	if (listbase->last== prevlink) /* at end of list */
		listbase->last = newlink;

	newlink->next= prevlink->next;
	prevlink->next= newlink;
	if(newlink->next) newlink->next->prev= newlink;
	newlink->prev= prevlink;
}

void TLI_insertlinkbefore(ListBase *listbase, void *vnextlink, void *vnewlink)
{
	struct Link *nextlink= (Link*)vnextlink, *newlink= (Link*)vnewlink;

	/* newlink before nextlink */

	if (newlink == 0) return;
	if (listbase == 0) return;

	if(listbase->first==0) { /* empty list */
		listbase->first= newlink;
		listbase->last= newlink;
		return;
	}
	if (nextlink== 0) {	/* insert at end of list */
		newlink->prev= (Link*)listbase->last;
		newlink->next= 0;
		((struct Link *)listbase->last)->next= newlink;
		listbase->last= newlink;
		return;
	}

	if (listbase->first== nextlink) /* at beginning of list */
		listbase->first = newlink;

	newlink->next= nextlink;
	newlink->prev= nextlink->prev;
	nextlink->prev= newlink;
	if(newlink->prev) newlink->prev->next= newlink;
}


void TLI_freelist(ListBase *listbase)
{
	struct Link *link,*next;

	if (listbase == 0) return;
	link= (Link*)listbase->first;
	while(link) {
		next= link->next;
		free(link);
		link= next;
	}
	listbase->first=0;
	listbase->last=0;
}

void TLI_freelistN(ListBase *listbase)
{
	struct Link *link,*next;

	if (listbase == 0) return;
	link= (Link*)listbase->first;
	while(link) {
		next= link->next;
		MEM_freeN(link);
		link= next;
	}
	listbase->first=0;
	listbase->last=0;
}


int TLI_countlist(ListBase *listbase)
{
	Link * link;
	int count = 0;
	
	if (listbase){
		link = (Link*)listbase->first;
		while(link) {
			count++;
			link= link->next;
		}
	}
	return(count);
}

void * TLI_findlink(ListBase *listbase, int number)
{
	Link * link = NULL;

	if (number >= 0) {
		link = (Link*)listbase->first;
		while (link != NULL && number != 0) {
			number--;
			link = link->next;
		}
	}

	return (link);
}

/*=====================================================================================*/
/* Methods for access array (realloc) */
/*=====================================================================================*/

/* remove item with index */
static void rem_array_item(struct DynamicArray *da, unsigned int index)
{
	da->items[index]=NULL;
	da->count--;
	if(index==da->last_item_index){
		while((!da->items[da->last_item_index]) && (da->last_item_index>0)){
			da->last_item_index--;
		}
	}
}

/* add array (if needed, then realloc) */
static void add_array_item(struct DynamicArray *da, void *item, unsigned int index)
{
	/* realloc of access array */
	if(da->max_item_index < index){
		unsigned int i, max = da->max_item_index;
		void **nitems;

		do {
			da->max_item_index += PAGE_SIZE;	/* OS can allocate only PAGE_SIZE Bytes */
		} while(da->max_item_index<=index);

		nitems = (void**)MEM_mallocN(sizeof(void*)*(da->max_item_index+1), "dlist access array");
		for(i=0;i<=max;i++)
			nitems[i] = da->items[i];

		/* set rest pointers to the NULL */
		for(i=max+1; i<=da->max_item_index; i++)
			nitems[i]=NULL;

		MEM_freeN(da->items);	/* free old access array */
		da->items = nitems;
	}

	da->items[index] = item;
	da->count++;
	if(index > da->last_item_index) da->last_item_index = index;
}

/* free access array */
static void destroy_array(DynamicArray *da)
{
	da->count=0;
	da->last_item_index=0;
	da->max_item_index=0;
	MEM_freeN(da->items);
}

/* init dynamic array */
static void init_array(DynamicArray *da)
{
	unsigned int i;

	da->count=0;
	da->last_item_index=0;
	da->max_item_index = PAGE_SIZE-1;
	da->items = (void**)MEM_mallocN(sizeof(void*)*(da->max_item_index+1),"dlist access array");
	for(i=0; i<=da->max_item_index; i++) da->items[i]=NULL;
}


/*=====================================================================================*/
/* Methods for two way dynamic list with access array */
/*=====================================================================================*/

/* create new two way dynamic list with access arrayfrom two way dynamic list
 * it doesn't copy any items to new array or something like this It is strongly
 * recomended to use TLI_dlist_ methods for adding/removing items from dynamic list
 * unless you can end with inconsistence system !!! */
DynamicList *TLI_dlist_from_listbase(ListBase *lb)
{
	DynamicList *dlist;
	Link *item;
	int i=0, count;
	
	if(!lb) return NULL;
	
	count = TLI_countlist(lb);

	dlist = (DynamicList*)MEM_mallocN(sizeof(DynamicList), "temp dynamic list");
	/* ListBase stuff */
	dlist->lb.first = lb->first;
	dlist->lb.last = lb->last;
	/* access array stuff */
	dlist->da.count=count;
	dlist->da.max_item_index = count-1;
	dlist->da.last_item_index = count -1;
	dlist->da.items = (void**)MEM_mallocN(sizeof(void*)*count, "temp dlist access array");

	item = (Link*)lb->first;
	while(item){
		dlist->da.items[i] = (void*)item;
		item = item->next;
		i++;
	}

	/* to prevent you of using original ListBase :-) */
	lb->first = lb->last = NULL;

	return dlist;
}

/* take out ListBase from DynamicList and destroy all temporary structures of DynamicList */
ListBase *TLI_listbase_from_dlist(DynamicList *dlist, ListBase *lb)
{
	if(!dlist) return NULL;

	if(!lb) lb = (ListBase*)MEM_mallocN(sizeof(ListBase), "ListBase");
	
	lb->first = dlist->lb.first;
	lb->last = dlist->lb.last;

	/* free all items of access array */
	MEM_freeN(dlist->da.items);
	/* free DynamicList*/
	MEM_freeN(dlist);

	return lb;
}

/* return pointer at item from th dynamic list with access array */
void *TLI_dlist_find_link(DynamicList *dlist, unsigned int index)
{
	if(!dlist) return NULL;

	if((index <= dlist->da.last_item_index) && (index >= 0)){
      		return dlist->da.items[index];
	}
	else {
		return NULL;
	}
}

/* return count of items in the dynamic list with access array */
unsigned int TLI_count_items(DynamicList *dlist)
{
	if(!dlist) return 0;

	return dlist->da.count;
}

/* free item from the dynamic list with access array */
void TLI_dlist_free_item(DynamicList *dlist, unsigned int index)
{
	if(!dlist) return;
	
	if((index <= dlist->da.last_item_index) && (dlist->da.items[index])){
		TLI_freelinkN(&(dlist->lb), dlist->da.items[index]);
		rem_array_item(&(dlist->da), index);
	}
}

/* remove item from the dynamic list with access array */
void TLI_dlist_rem_item(DynamicList *dlist, unsigned int index)
{
	if(!dlist) return;
	
	if((index <= dlist->da.last_item_index) && (dlist->da.items[index])){
		TLI_remlink(&(dlist->lb), dlist->da.items[index]);
		rem_array_item(&(dlist->da), index);
	}
}

/* add item to the dynamic list with access array (index) */
void* TLI_dlist_add_item_index(DynamicList *dlist, void *item, unsigned int index)
{
	if(!dlist) return NULL;

	if((index <= dlist->da.max_item_index) && (dlist->da.items[index])) {
		MEM_freeN(item);
		return NULL;
	}
	else {
		add_array_item(&(dlist->da), item, index);
		TLI_addtail(&(dlist->lb), item);
		return item;
	}
}

/* destroy dynamic list with access array */
void TLI_dlist_destroy(DynamicList *dlist)
{
	if(!dlist) return;

	TLI_freelistN(&(dlist->lb));
	destroy_array(&(dlist->da));
}

/* init dynamic list with access array */
void TLI_dlist_init(DynamicList *dlist)
{
	if(!dlist) return;

	dlist->lb.first = NULL;
	dlist->lb.last = NULL;

	init_array(&(dlist->da));
}

/*=====================================================================================*/

char *TLI_strdupn(char *str, int len) {
	char *n= (char*)MEM_mallocN(len+1, "strdup");
	memcpy(n, str, len);
	n[len]= '\0';
	
	return n;
}
char *TLI_strdup(char *str) {
	return TLI_strdupn(str, strlen(str));
}

char *TLI_strncpy(char *dst, char *src, int maxncpy) {
	int srclen= strlen(src);
	int cpylen= (srclen>(maxncpy-1))?(maxncpy-1):srclen;
	
	memcpy(dst, src, cpylen);
	dst[cpylen]= '\0';
	
	return dst;
}

int TLI_streq(char *a, char *b) {
	return (strcmp(a, b)==0);
}
int TLI_strcaseeq(char *a, char *b) {
	return (TLI_strcasecmp(a, b)==0);
}

void TLI_makestringcode(char *fromfile, char *str)
{
	char *slash, len, temp[512];

	strcpy(temp, fromfile);
	
	TLI_char_switch(temp, '\\', '/');
	TLI_char_switch(str, '\\', '/');

	/* Find the last slash */
	slash = strrchr(temp, '/');
	if(slash) {
		*(slash+1)= 0;
		len= strlen(temp);
		if(len) {
			if(strncmp(str, temp, len)==0) {
				temp[0]= '/';
				temp[1]= '/';
				strcpy(temp+2, str+len);
#ifdef			WIN32
				TLI_char_switch(temp+2, '/', '\\');
#endif
				strcpy(str, temp);
			}
		}
	}
}

void TLI_clean(char *path)
{
	if(path==0) return;
#ifdef WIN32
	if(path && strlen(path)>2) {
		TLI_char_switch(path+2, '/', '\\');
	}
#else
	TLI_char_switch(path, '\\', '/');
#endif
}

void TLI_char_switch(char *string, char from, char to) 
{
	if(string==0) return;
	while (*string != 0) {
		if (*string == from) *string = to;
		string++;
	}
}



int TLI_testextensie(char *str, char *ext)
{
	short a, b;
	int retval;

	a= strlen(str);
	b= strlen(ext);

	if(a==0 || b==0 || b>=a) {
		retval = 0;
	} else if (TLI_strcasecmp(ext, str + a - b)) {
		retval = 0;	
	} else {
		retval = 1;
	}

	return (retval);
}


/* 
 * returns absolute path to the app bundle
 * only useful on OS X 
 */
#ifdef __APPLE__
char* TLI_getbundle(void) {
	CFURLRef bundleURL;
	CFStringRef pathStr;
	static char path[MAXPATHLEN];
	CFBundleRef mainBundle = CFBundleGetMainBundle();

	bundleURL = CFBundleCopyBundleURL(mainBundle);
	pathStr = CFURLCopyFileSystemPath(bundleURL, kCFURLPOSIXPathStyle);
	CFStringGetCString(pathStr, path, MAXPATHLEN, kCFStringEncodingASCII);
	return path;
}
#endif

int TLI_strcasecmp(const char *s1, const char *s2) {
	int i;

	for (i=0; ; i++) {
		char c1 = tolower(s1[i]);
		char c2 = tolower(s2[i]);

		if (c1<c2) {
			return -1;
		} else if (c1>c2) {
			return 1;
		} else if (c1==0) {
			break;
		}
	}

	return 0;
}

int TLI_strncasecmp(const char *s1, const char *s2, int n) {
	int i;

	for (i=0; i<n; i++) {
		char c1 = tolower(s1[i]);
		char c2 = tolower(s2[i]);

		if (c1<c2) {
			return -1;
		} else if (c1>c2) {
			return 1;
		} else if (c1==0) {
			break;
		}
	}

	return 0;
}
