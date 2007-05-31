#ifndef TLI_TOPLIB_H
#define TLI_TOPLIB_H

/* braindamage for the masses... needed
	because fillfacebase and fillvertbase are used outside */
#include "DNA_listBase.h" 

extern ListBase fillfacebase;
extern ListBase fillvertbase;
/**
 * @attention Defined in scanfill.c
 */
extern ListBase filledgebase;
extern int totblock;

struct chardesc;
struct direntry;
struct rctf;
struct rcti;
struct EditVert;
struct PackedFile;
struct LinkNode;
struct DynamicList;

#ifdef __cplusplus
extern "C" {
#endif

/* util.c */
char *TLI_gethome(void);
void TLI_make_file_string(char *relabase, char *string,  char *dir,  char *file);
void TLI_make_exist(char *dir);
void TLI_split_dirfile(char *string, char *dir, char *file);
int TLI_testextensie(char *str, char *ext);
void addlisttolist(ListBase *list1, ListBase *list2);
void TLI_insertlink(struct ListBase *listbase, void *vprevlink, void *vnewlink);
void * TLI_findlink(struct ListBase *listbase, int number);

void TLI_freelistN(struct ListBase *listbase);

void TLI_addtail(struct ListBase *listbase, void *vlink);
void TLI_remlink(struct ListBase *listbase, void *vlink);
void TLI_newname(char * name, int add);
int TLI_stringdec(char *string, char *kop, char *staart, unsigned short *numlen);
void TLI_stringenc(char *string, char *kop, char *staart, unsigned short numlen, int pic);
void TLI_addhead(struct ListBase *listbase, void *vlink);
void TLI_insertlinkbefore(struct ListBase *listbase, void *vnextlink, void *vnewlink);

void TLI_freelist(struct ListBase *listbase);

int TLI_countlist(struct ListBase *listbase);
void TLI_freelinkN(ListBase *listbase, void *vlink);

struct DynamicList *TLI_dlist_from_listbase(struct ListBase *lb);
struct ListBase *TLI_listbase_from_dlist(struct DynamicList *dlist, struct ListBase *lb);
void * TLI_dlist_find_link(struct DynamicList *dlist, unsigned int index);
unsigned int TLI_count_items(struct DynamicList *dlist);
void TLI_dlist_free_item(struct DynamicList *dlist, unsigned int index);
void TLI_dlist_rem_item(struct DynamicList *dlist, unsigned int index);
void * TLI_dlist_add_item_index(struct DynamicList *dlist, void *item, unsigned int index);

void TLI_dlist_destroy(struct DynamicList *dlist);
void TLI_dlist_init(struct DynamicList *dlist);

void TLI_splitdirstring(char *di,char *fi);

	/**
	 * Blender's path code replacement function.
	 * Bases @a path strings leading with "//" by the
	 * directory @a basepath, and replaces instances of
	 * '#' with the @a framenum. Results are written
	 * back into @a path.
	 * 
	 * @a path The path to convert
	 * @a basepath The directory to base relative paths with.
	 * @a framenum The framenumber to replace the frame code with.
	 * @retval Returns true if the path was relative (started with "//").
	 */
int TLI_convertstringcode(char *path, char *basepath, int framenum);

void TLI_makestringcode(char *fromfile, char *str);

	/**
	 * Change every @a from in @a string into @a to. The
	 * result will be in @a string
	 *
	 * @a string The string to work on
	 * @a from The character to replace
	 * @a to The character to replace with
	 */
void TLI_char_switch(char *string, char from, char to);

	/**
	 * Makes sure @a path has platform-specific slashes.
	 * 
	 * @a path The path to 'clean'
	 */
void TLI_clean(char *path);
	/**
	 * Duplicates the cstring @a str into a newly mallocN'd
	 * string and returns it.
	 * 
	 * @param str The string to be duplicated
	 * @retval Returns the duplicated string
	 */
char* TLI_strdup(char *str);

	/**
	 * Duplicates the first @a len bytes of cstring @a str 
	 * into a newly mallocN'd string and returns it. @a str
	 * is assumed to be at least len bytes long.
	 * 
	 * @param str The string to be duplicated
	 * @param len The number of bytes to duplicate
	 * @retval Returns the duplicated string
	 */
char* TLI_strdupn(char *str, int len);

	/**
	 * Like strncpy but ensures dst is always
	 * '\0' terminated.
	 * 
	 * @param dst Destination for copy
	 * @param src Source string to copy
	 * @param maxncpy Maximum number of characters to copy (generally
	 *   the size of dst)
	 * @retval Returns dst
	 */
char* TLI_strncpy(char *dst, char *src, int maxncpy);

	/**
	 * Compare two strings
	 * 
	 * @retval True if the strings are equal, false otherwise.
	 */
int TLI_streq(char *a, char *b);

	/**
	 * Compare two strings without regard to case.
	 * 
	 * @retval True if the strings are equal, false otherwise.
	 */
int TLI_strcaseeq(char *a, char *b);

	/**
	 * Read a file as ASCII lines. An empty list is
	 * returned if the file cannot be opened or read.
	 * 
	 * @attention The returned list should be free'd with
	 * TLI_free_file_lines.
	 * 
	 * @param name The name of the file to read.
	 * @retval A list of strings representing the file lines.
	 */
struct LinkNode *TLI_read_file_as_lines(char *name);

	/**
	 * Free the list returned by TLI_read_file_as_lines.
	 */
void TLI_free_file_lines(struct LinkNode *lines);

	/**
	 * Checks if name is a fully qualified filename to an executable.
	 * If not it searches $PATH for the file. On Windows it also
	 * adds the correct extension (.com .exe etc) from
	 * $PATHEXT if necessary. Also on Windows it translates
	 * the name to its 8.3 version to prevent problems with
	 * spaces and stuff. Final result is returned in fullname.
	 *
	 * @param fullname The full path and full name of the executable
	 * @param name The name of the executable (usually argv[0]) to be checked
	 */
void TLI_where_am_i(char *fullname, char *name);

	/**
	 * determines the full path to the application bundle on OS X
	 *
	 * @return path to application bundle
	 */
#ifdef __APPLE__
char* TLI_getbundle(void);
#endif

#ifdef WIN32
int TLI_getInstallationDir( char * str );
#endif


int TLI_strcasecmp(const char *s1, const char *s2);
int TLI_strncasecmp(const char *s1, const char *s2, int n);

#define PRNTSUB(type,arg)			printf(#arg ": %" #type " ", arg)

#ifndef PRINT
#define PRINT(t,v)					{PRNTSUB(t,v); printf("\n");}
#define PRINT2(t1,v1,t2,v2)			{PRNTSUB(t1,v1); PRNTSUB(t2,v2); printf("\n");}
#define PRINT3(t1,v1,t2,v2,t3,v3)	{PRNTSUB(t1,v1); PRNTSUB(t2,v2); PRNTSUB(t3,v3); printf("\n");}
#define PRINT4(t1,v1,t2,v2,t3,v3,t4,v4)	{PRNTSUB(t1,v1); PRNTSUB(t2,v2); PRNTSUB(t3,v3); PRNTSUB(t4,v4); printf("\n");}
#endif

/**
 * @param array The array in question
 * @retval The number of elements in the array.
 */
#define TLI_ARRAY_NELEMS(array)		(sizeof((array))/sizeof((array)[0]))

/**
 * @param strct The structure of interest
 * @param member The name of a member field of @a strct
 * @retval The offset in bytes of @a member within @a strct
 */
#define TLI_STRUCT_OFFSET(strct, member)	((int) &((strct*) 0)->member)


#ifdef __cplusplus
}
#endif

#endif