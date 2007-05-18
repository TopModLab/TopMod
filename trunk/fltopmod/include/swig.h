
/**********************************************************************
 * Simplified Wrapper and Interface Generator  (SWIG)
 * 
 * Dave Beazley
 * 
 * Theoretical Division (T-11)           Department of Computer Science
 * Los Alamos National Laboratory        University of Utah
 * Los Alamos, New Mexico  87545         Salt Lake City, Utah  84112
 * beazley@lanl.gov                      beazley@cs.utah.edu
 *
 * Copyright (c) 1995-1997
 * The Regents of the University of California and the University of Utah
 * All Rights Reserved
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that 
 * (1) The above copyright notice and the following two paragraphs
 * appear in all copies of the source code and (2) redistributions
 * including binaries reproduces these notices in the supporting
 * documentation.   Substantial modifications to this software may be
 * copyrighted by their authors and need not follow the licensing terms
 * described here, provided that the new terms are clearly indicated in
 * all files where they apply.
 * 
 * IN NO EVENT SHALL THE AUTHOR, THE UNIVERSITY OF CALIFORNIA, THE 
 * UNIVERSITY OF UTAH OR DISTRIBUTORS OF THIS SOFTWARE BE LIABLE TO ANY
 * PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
 * DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION,
 * EVEN IF THE AUTHORS OR ANY OF THE ABOVE PARTIES HAVE BEEN ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE AUTHOR, THE UNIVERSITY OF CALIFORNIA, AND THE UNIVERSITY OF UTAH
 * SPECIFICALLY DISCLAIM ANY WARRANTIES,INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE. THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND 
 * THE AUTHORS AND DISTRIBUTORS HAVE NO OBLIGATION TO PROVIDE MAINTENANCE,
 * SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 **************************************************************************/
/***********************************************************************
 * $Header: /root/TopMod/include/swig.h,v 1.1 2006/09/04 19:22:53 stuart Exp $
 *
 * swig.h
 *
 * This is the header file containing the main class definitions and
 * declarations.   Should be included in all extensions and code
 * modules.
 *
 ***********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SWIG_MAJOR_VERSION 1
#define SWIG_MINOR_VERSION 1
#define SWIG_SPIN "(Patch 5)"

/* Global variables.   Needs to be cleaned up */

#ifdef MACSWIG
#define Status Swig_Status
#undef stderr
#define stderr swig_log
extern  FILE   *swig_log;
#endif

extern  FILE      *f_header;                        // Some commonly used
extern  FILE      *f_wrappers;                      // FILE pointers
extern  FILE      *f_init;
extern  FILE      *f_input;
extern  char      InitName[256];             
extern  char      LibDir[512];                      // Library directory
extern  char     **InitNames;                       // List of other init functions
extern  int       Status;                           // Variable creation status
extern  int       TypeStrict;                       // Type checking strictness
extern  int       Verbose;
extern  int       yyparse();
extern  int       line_number;
extern  int       start_line;
extern  char     *input_file;                       // Current input file
extern  int       CPlusPlus;                        // C++ mode
extern  int       ObjC;                             // Objective-C mode
extern  int       ObjCClass;                        // Objective-C style class
extern  int       AddMethods;                       // AddMethods mode
extern  int       NewObject;                        // NewObject mode
extern  int       Inline;                           // Inline mode
extern  int       NoInclude;                        // NoInclude flag
extern  char     *typemap_lang;                     // Current language name
extern  int       error_count;
extern  char     *copy_string(char *);
extern  char      output_dir[512];                  // Output directory

#define FatalError()   if ((error_count++) > 20) { fprintf(stderr,"Confused by earlier errors. Bailing out\n"); SWIG_exit(1); }

/* Miscellaneous stuff */

#define  STAT_READONLY  1
#define  MAXSCOPE       16

// -----------------------------------------------------------------------
// String class 
// -----------------------------------------------------------------------

class String {
private:
  int   maxsize;            // Max size of current string
  void  add(const char *newstr);  // Function to add a new string
  void  add(char c);              // Add a character				       
  void  insert(const char *newstr);  
  int   len;
public:
  String();
  String(const char *s);
  ~String();
  char  *get() const;                
  char  *str;               // String data 
  friend String& operator<<(String&,const char *s);
  friend String& operator<<(String&,const int);
  friend String& operator<<(String&,const char);
  friend String& operator<<(String&,String&);
  friend String& operator>>(const char *s, String&);
  friend String& operator>>(String&,String&);
  String& operator=(const char *);   
  operator char*() const { return str; }
  void   untabify();
  void   replace(char *token, char *rep);
  void   replaceid(char *id, char *rep);
  void   strip();
};

#define  tab2   "  "
#define  tab4   "    "
#define  tab8   "        "
#define  br     "\n"
#define  endl   "\n"
#define  quote  "\""

// -------------------------------------------------------------------
// Hash table class
// -------------------------------------------------------------------

class Hash {
private:
  struct Node {
    Node(const char *k, void *obj, void (*d)(void *)) {
      key = new char[strlen(k)+1];
      strcpy(key,k);
      object = obj;
      del_proc = d;
      next = 0;
    };
    ~Node() {
      delete key;
      if (del_proc) (*del_proc)(object);
    };
    char        *key;
    void        *object;
    struct Node *next;
    void (*del_proc)(void *);
  };
  int    h1(const char *key);      // Hashing function
  int    hashsize;                 // Size of hash table
  Node  **hashtable;               // Actual hash table
  int    index;                    // Current index (used by iterators)
  Node   *current;                 // Current item in hash table
public:
  Hash();
  ~Hash();
  int    add(const char *key, void *object);
  int    add(const char *key, void *object, void (*del)(void *));
  void  *lookup(const char *key);
  void   remove(const char *key);
  void  *first();
  void  *next();
  char  *firstkey();
  char  *nextkey();
};

/************************************************************************
 * class DataType
 *
 * Defines the basic datatypes supported by the translator.
 *
 ************************************************************************/

#define    T_INT       1
#define    T_SHORT     2
#define    T_LONG      3
#define    T_UINT      4
#define    T_USHORT    5
#define    T_ULONG     6
#define    T_UCHAR     7
#define    T_SCHAR     8
#define    T_BOOL      9
#define    T_DOUBLE    10
#define    T_FLOAT     11
#define    T_CHAR      12
#define    T_USER      13
#define    T_VOID      14
#define    T_SYMBOL    98
#define    T_ERROR     99

// These types are now obsolete, but defined for backwards compatibility

#define    T_SINT      90
#define    T_SSHORT    91
#define    T_SLONG     92

// Class for storing data types

#define MAX_NAME 96

class DataType {
private:
  static Hash       *typedef_hash[MAXSCOPE];
  static int         scope;
public:
  int         type;          // SWIG Type code
  char        name[MAX_NAME];      // Name of type
  char        is_pointer;    // Is this a pointer?
  char        implicit_ptr;  // Implicit ptr				  
  char        is_reference;  // A C++ reference type
  char        status;        // Is this datatype read-only?
  char        *qualifier;    // A qualifier string (ie. const).
  char        *arraystr;     // String containing array part
  int         id;            // type identifier (unique for every type).				  
  DataType();
  DataType(DataType *);
  DataType(int type);
  ~DataType();
  void        primitive();   // Turn a datatype into its primitive type
  char       *print_type();  // Return string containing datatype
  char       *print_full();  // Return string with full datatype
  char       *print_cast();  // Return string for type casting
  char       *print_mangle();// Return mangled version of type
  char       *print_real(char *local=0);  // Print the real datatype (as far as we can determine)
  char       *print_arraycast(); // Prints an array cast
  char       *print_mangle_default(); // Default mangling scheme

  // Array query functions
  int        array_dimensions();   // Return number of array dimensions (if any)
  char       *get_dimension(int);  // Return string containing a particular dimension
  char       *get_array();         // Returns the array string for a datatype

  // typedef support

  void       typedef_add(char *name, int mode = 0); // Add this type to typedef list
  void       typedef_resolve(int level = 0);        // See if this type has been typedef'd
  void       typedef_replace();                     // Replace this type with it's original type
static int   is_typedef(char *name);                // See if this is a typedef
  void       typedef_updatestatus(int newstatus);   // Change status of a typedef
static void  init_typedef(void);                    // Initialize typedef manager   
static void  merge_scope(Hash *h);                  // Functions for managing scoping of datatypes
static void  new_scope(Hash *h = 0); 
static Hash *collapse_scope(char *);
  int        check_defined();                       // Check to see if type is defined by a typedef.
};

#define STAT_REPLACETYPE   2

/************************************************************************
 * class Parm
 *
 * Structure for holding information about function parameters
 *   
 *      CALL_VALUE  -->  Call by value even though function parameter
 *                       is a pointer.   
 *                          ex :   foo(&_arg0);  
 *      CALL_REF    -->  Call by reference even though function parameter
 *                       is by value
 *                          ex :   foo(*_arg0);
 * 
 ************************************************************************/

#define CALL_VALUE      0x01
#define CALL_REFERENCE  0x02
#define CALL_OUTPUT     0x04

struct Parm {
  DataType   *t;                // Datatype of this parameter
  int        call_type;         // Call type (value or reference or value)
  char       *name;             // Name of parameter (optional)
  char       *defvalue;         // Default value (as a string)
  int        ignore;            // Ignore flag
  char       *objc_separator;   // Parameter separator for Objective-C
  Parm(DataType *type, char *n);
  Parm(Parm *p);
  ~Parm();
};

// -------------------------------------------------------------
// class ParmList
//
// This class is used for manipulating parameter lists in
// function and type declarations.
// -------------------------------------------------------------

#define MAXPARMS   16

class ParmList {
private:
  int   maxparms;                 // Max parms possible in current list
  Parm  **parms;                  // Pointer to parms array
  void  moreparms();              // Increase number of stored parms
  int    current_parm;            // Internal state for get_first,get_next
public:
  int   nparms;                   // Number of parms in list
  void  append(Parm *p);          // Append a parameter to the end
  void  insert(Parm *p, int pos); // Insert a parameter into the list
  void  del(int pos);             // Delete a parameter at position pos
  int   numopt();                 // Get number of optional arguments
  int   numarg();                 // Get number of active arguments				       
  Parm *get(int pos);             // Get the parameter at position pos
  Parm &operator[](int);          // An alias for get().  
  ParmList(); 
  ParmList(ParmList *l); 
  ~ParmList();

  // Keep this for backwards compatibility

  Parm  *get_first();              // Get first parameter from list
  Parm  *get_next();               // Get next parameter from list      
  void   print_types(FILE *f);     // Print list of datatypes
  void   print_types(String &f);   // Generate list of datatypes.
  void   print_args(FILE *f);      // Print argument list				     
  int    check_defined();          // Checks to make sure the arguments are defined
  void   sub_parmnames(String &s); // Remaps real parameter names in code fragment
};

// Modes for different types of inheritance

#define INHERIT_FUNC       0x1
#define INHERIT_VAR        0x2
#define INHERIT_CONST      0x4
#define INHERIT_ALL        (INHERIT_FUNC | INHERIT_VAR | INHERIT_CONST)

/************************************************************************
 * class language:
 *
 * This class defines the functions that need to be supported by the
 * scripting language being used.    The translator calls these virtual
 * functions to output different types of code for different languages.
 *
 * By implementing this using virtual functions, hopefully it will be
 * easy to support different types of scripting languages.
 *
 * The following functions are used :
 *
 *    parse_args(argc, argv)
 *           Parse the arguments used by this language.
 *
 *    parse()
 *           Entry function that starts parsing of a particular language
 *
 *    create_function(fname, iname, rtype, parmlist)
 *           Creates a function wrappper.
 *
 *    link_variable(vname, iname, type)
 *           Creates a link to a variable.
 *    
 *    declare_const(cname, type, value)
 *           Creates a constant (for #define).
 *
 *    initialize(char *fn)
 *           Produces initialization code.
 *
 *    headers()
 *           Produce code for headers
 *
 *    close()
 *           Close up files
 *
 *    usage_var(iname,type,string)
 *           Produces usage string for variable declaration.
 *
 *    usage_func(iname,rttype, parmlist, string)
 *           Produces usage string for function declaration.
 *
 *    usage_const(iname, type, value, string)
 *           Produces usage string for constants
 *           
 *    set_module(char *modname)
 *           Sets the name of the module (%module directive)
 *
 *    set_init(char *initname)
 *           Sets name of initialization function (an alternative to set_module)
 *    add_native(char *name, char *funcname);
 *           Adds a native wrapper function to the initialize process
 *
 *    type_mangle(DataType *t);
 *           Mangles the name of a datatype.
 * --- C++ Functions ---
 *
 *    These functions are optional additions to any of the target
 *    languages.   SWIG handles inheritance, symbol tables, and other
 *    information.
 *
 *    cpp_open_class(char *classname, char *rname) 
 *          Open a new C++ class definition.
 *    cpp_close_class(char *)
 *          Close current C++ class
 *    cpp_member_func(char *name, char *rname, DataType *rt, ParmList *l)
 *          Create a C++ member function
 *    cpp_constructor(char *name, char *iname, ParmList *l) 
 *          Create a C++ constructor.
 *    cpp_destructor(char *name, char *iname) 
 *          Create a C++ destructor
 *    cpp_variable(char *name, char *iname, DataType *t)
 *          Create a C++ member data item.
 *    cpp_declare_const(char *name, char *iname, int type, char *value)
 *          Create a C++ constant.
 *    cpp_inherit(char *baseclass)
 *          Inherit data from baseclass.
 *    cpp_static_func(char *name, char *iname, DataType *t, ParmList *l) 
 *          A C++ static member function.
 *    cpp_static_var(char *name, char *iname, DataType *t)
 *          A C++ static member data variable.
 *
 *************************************************************************/

class Language {
public:
  virtual void parse_args(int argc, char *argv[]) = 0;
  virtual void parse() = 0;
  virtual void create_function(char *, char *, DataType *, ParmList *) = 0; 
  virtual void link_variable(char *, char *, DataType *)  = 0;
  virtual void declare_const(char *, char *, DataType *, char *) = 0;
  virtual void initialize(void) = 0;
  virtual void headers(void) = 0;
  virtual void close(void) = 0;
  virtual void set_module(char *mod_name,char **mod_list) = 0;
  virtual void set_init(char *init_name);
  virtual void add_native(char *, char *);
  virtual char *type_mangle(DataType *t) {
    return t->print_mangle_default();
  }
  virtual void add_typedef(DataType *t, char *name);
  virtual void create_command(char *cname, char *iname);

  //
  // C++ language extensions.
  // You can redefine these, or use the defaults below
  //
       
  virtual void cpp_member_func(char *name, char *iname, DataType *t, ParmList *l);
  virtual void cpp_constructor(char *name, char *iname, ParmList *l);
  virtual void cpp_destructor(char *name, char *newname);
  virtual void cpp_open_class(char *name, char *rename, char *ctype, int strip);
  virtual void cpp_close_class();
  virtual void cpp_cleanup();
  virtual void cpp_inherit(char **baseclass, int mode = INHERIT_ALL);
  virtual void cpp_variable(char *name, char *iname, DataType *t);
  virtual void cpp_static_func(char *name, char *iname, DataType *t, ParmList *l);
  virtual void cpp_declare_const(char *name, char *iname, DataType *type, char *value);
  virtual void cpp_static_var(char *name, char *iname, DataType *t);


  // Pragma directive 

  virtual void pragma(char *, char *, char *);

  // Declaration of a class, but not a full definition

  virtual void cpp_class_decl(char *, char *, char *);

  // Import directive

  virtual void import(char *filename);

};

class Documentation;

// --------------------------------------------------------------------
// class DocEntry
//
// Base class for the documentation system.   Basically everything is
// a documentation entry of some sort.   Specific derived classes
// are created internally and shouldn't be accessed by third-party
// modules.
// --------------------------------------------------------------------

class DocEntry {
public:
  char        *name;                 // Name of the entry
  String      usage;                 // Short description (optional)
  String      cinfo;                 // Information about C interface (optional).
  String      text;                  // Supporting text (optional)
  DocEntry    *parent;               // Parent of this entry (optional)
  DocEntry    *child;                // Children of this entry (optional)
  DocEntry    *next;                 // Next entry (or sibling)
  DocEntry    *previous;             // Previous entry
  int          counter;              // Counter for section control
  int          is_separator;         // Is this a separator entry?
  int          sorted;               // Sorted?
  int          line_number;          // Line number
  int          end_line;             // Ending line number
  int          format;               // Format this documentation entry
  int          print_info;           // Print C information about this entry
  char        *file;                 // File
  virtual ~DocEntry();               // Destructor (common to all subclasses)

  // Methods applicable to all documentation entries

  virtual void output(Documentation *d);    
  void add(DocEntry *de);            // Add documentation entry to the list
  void addchild(DocEntry *de);       // Add documentation entry as a child
  void sort_children();              // Sort all of the children
  void remove();                     // Remove this doc entry
  void parse_args(int argc, char **argv); // Parse command line options
  void style(char *name,char *value);// Change doc style.
  static DocEntry  *dead_entries;    // Dead documentation entries
};  

extern DocEntry      *doc_entry;

// Default DocEntry style parameters

#define SWIGDEFAULT_SORT            0
#define SWIGDEFAULT_FORMAT          1
#define SWIGDEFAULT_INFO            1

// ----------------------------------------------------------------------
// Documentation module base class
//
// This class defines methods that need to be implemented for a 
// documentation module.
//
// title()          - Print out a title entry
// newsection()     - Start a new section (may be nested to form subsections)
// endsection()     - End a section
// print_decl()     - Print a standard declaration
// print_text()     - Print standard text
// init()           - Initialize the documentation module
// close()          - Close documentation module           
// ----------------------------------------------------------------------

class Documentation {
public:
  virtual void parse_args(int argc, char **argv) = 0;
  virtual void title(DocEntry *de) = 0;
  virtual void newsection(DocEntry *de, int sectnum) = 0;
  virtual void endsection() = 0;
  virtual void print_decl(DocEntry *de) = 0;
  virtual void print_text(DocEntry *de) = 0;
  virtual void separator() = 0;
  virtual void init(char *filename) = 0;
  virtual void close(void) = 0;
  virtual void style(char *name, char *value) = 0;
};

/* Emit functions */

extern  void  emit_extern_var(char *, DataType *, int, FILE *);
extern  void  emit_extern_func(char *, DataType *, ParmList *, int, FILE *);
extern  int   emit_args(DataType *, ParmList *, FILE *);

extern  void  emit_func_call(char *, DataType *, ParmList *, FILE *);

extern  void  emit_hex(FILE *);
extern  void  emit_set_get(char *, char *, DataType *);
extern  void  emit_banner(FILE *);
extern  void  emit_ptr_equivalence(FILE *);
extern  int   SWIG_main(int, char **, Language *, Documentation *);
extern  void  make_wrap_name(char *);

// Some functions for emitting some C++ helper code

extern void cplus_emit_member_func(char *classname, char *classtype, char *classrename,
                                   char *mname, char *mrename, DataType *type, ParmList *l,
                                   int mode);

extern void cplus_emit_static_func(char *classname, char *classtype, char *classrename,
                                   char *mname, char *mrename, DataType *type, ParmList *l, 
                                   int mode);

extern void cplus_emit_destructor(char *classname, char *classtype, char *classrename,
                                  char *name, char *iname, int mode);

extern void cplus_emit_constructor(char *classname, char *classtype, char *classrename, 
                                   char *name, char *iname, ParmList *l, int mode);

extern void cplus_emit_variable_get(char *classname, char *classtype, char *classrename,
				    char *name, char *iname, DataType *type, int mode);

extern void cplus_emit_variable_set(char *classname, char *classtype, char *classrename,
				    char *name, char *iname, DataType *type, int mode);

extern char *cplus_base_class(char *name);

extern void cplus_support_doc(String &f);

/* Function for building search directories */

extern  void  add_directory(char *dirname);
extern  int   insert_file(char *, FILE *);
extern  int   get_file(char *filename, String &str);
extern  int   checkout_file(char *filename, char *dest);
extern  int   checkin_file(char *dir, char *lang, char *source, char *dest);
extern  int   include_file(char *filename);

/* Miscellaneous */

extern void check_options();
extern void init_args(int argc, char **);
extern void mark_arg(int n);
extern void arg_error();

extern void library_add(char *name);
extern void library_insert();

// -----------------------------------------------------------------------
//  Class for Creating Wrapper Functions
// -----------------------------------------------------------------------

class WrapperFunction {
private:
  Hash    h;
public:
  String  def;
  String  locals;
  String  code;
  String  init;                    
  void    print(FILE *f);
  void    print(String &f); 
  void    add_local(char *type, char *name, char *defvalue = 0);
  char   *new_local(char *type, char *name, char *defvalue = 0);
static    void    del_type(void *obj);
};

extern  int   emit_args(DataType *, ParmList *, WrapperFunction &f);
extern  void  emit_func_call(char *, DataType *, ParmList *, WrapperFunction &f);
extern  void  SWIG_exit(int);

// Symbol table management

extern int  add_symbol(char *, DataType *, char *);
extern void remove_symbol(char *);
extern int  update_symbol(char *, DataType *, char *);
extern char *lookup_symvalue(char *);
extern DataType *lookup_symtype(char *);
extern int  lookup_symbol(char *);

// -----------------------------------------------------------------------
// Typemap support
// -----------------------------------------------------------------------

extern void    typemap_register(char *op, char *lang, DataType *type, char *pname, char *code, ParmList *l = 0);
extern void    typemap_register(char *op, char *lang, char *type, char *pname, char *code,ParmList *l = 0);
extern void    typemap_register_default(char *op, char *lang, int type, int ptr, char *arraystr, char *code, ParmList *l = 0);
extern char   *typemap_lookup(char *op, char *lang, DataType *type, char *pname, char *source, char *target,
                              WrapperFunction *f = 0);
extern void    typemap_clear(char *op, char *lang, DataType *type, char *pname);
extern void    typemap_copy(char *op, char *lang, DataType *stype, char *sname, DataType *ttype, char *tname);
extern char   *typemap_check(char *op, char *lang, DataType *type, char *pname);
extern void    typemap_apply(DataType *tm_type, char *tmname, DataType *type, char *pname);
extern void    typemap_clear_apply(DataType *type, char *pname);


// -----------------------------------------------------------------------
// Code fragment support
// -----------------------------------------------------------------------

extern void    fragment_register(char *op, char *lang, char *code);
extern char   *fragment_lookup(char *op, char *lang, int age);
extern void    fragment_clear(char *op, char *lang);


extern  void  emit_ptr_equivalence(WrapperFunction &);

// -----------------------------------------------------------------------
// Naming system
// -----------------------------------------------------------------------

#define AS_IS      1

extern void   name_register(char *method, char *format);
extern int    name_scope(int);
extern char  *name_wrapper(char *fname, char *prefix, int suppress=0);
extern char  *name_member(char *fname, char *classname, int suppress=0);
extern char  *name_get(char *vname, int suppress=0);
extern char  *name_set(char *vname, int suppress=0);
extern char  *name_construct(char *classname, int suppress=0);
extern char  *name_destroy(char *classname, int suppress=0);

/***********************************************************************
 * -- Revision History
 * $Log: swig.h,v $
 * Revision 1.1  2006/09/04 19:22:53  stuart
 * Added includes and libs.
 *
 * Revision 1.45  1997/06/23 19:20:49  beazley
 * Added objc_separator to Parm data structure.
 *
 * Revision 1.44  1997/06/20 05:26:10  beazley
 * Added checkin_file().
 *
 * Revision 1.43  1997/06/17 04:51:17  beazley
 * Added naming services.
 *
 * Revision 1.42  1997/05/28 05:43:34  beazley
 * Increased type-name slightly.  Minor changes.
 *
 * Revision 1.41  1997/05/25 23:06:56  beazley
 * Added Objective-C flags
 *
 * Revision 1.40  1997/05/24 05:16:28  beazley
 * Minor changes.
 *
 * Revision 1.39  1997/05/05 15:58:46  beazley
 * Added new typemap handling functions for %apply and %clear.
 *
 * Revision 1.38  1997/03/29 17:41:50  beazley
 * Added error_count variable and FatalError() macro.
 *
 * Revision 1.37  1997/03/16 21:28:53  beazley
 * Minor changes
 *
 * Revision 1.36  1997/03/13 07:19:01  beazley
 * Added MACSWIG define
 *
 * Revision 1.35  1997/03/12 05:01:52  beazley
 * Minor changes
 *
 * Revision 1.34  1997/03/08 19:18:36  beazley
 * Added a few functions for managing arrays.
 *
 * Revision 1.33  1997/02/19 23:02:24  beazley
 * Added code fragment functions.
 *
 * Revision 1.32  1997/01/17 06:14:26  beazley
 * Updated version
 *
 * Revision 1.31  1997/01/15 05:46:52  beazley
 * Made a few slight modifications related to hash tables and scoping
 * of typedefs.
 *
 * Revision 1.30  1997/01/06 17:13:17  beazley
 * Changes a few function names. Modifications for typemaps and
 * multiple inheritance.
 *
 * Revision 1.29  1996/12/26 05:05:00  beazley
 * Modified String class to add replace method.
 *
 * Revision 1.28  1996/10/29 19:20:56  beazley
 * Rewrote most of the parameter list handling code
 *
 * Revision 1.27  1996/10/22 16:45:24  beazley
 * Added 'bool' type.  Also reorganized type codes somewhat.  The
 * T_SINT, T_SSHORT, T_SLONG types are now obsolete.
 *
 * Revision 1.26  1996/09/26 21:49:46  dmb
 * Minor changes
 *
 * Revision 1.25  1996/08/31 21:54:55  dmb
 * Minor changes
 *
 * Revision 1.24  1996/08/28 16:43:11  dmb
 * Minor changes
 *
 * Revision 1.23  1996/08/27 22:57:56  dmb
 * Added SWIG_exit() prototype
 *
 * Revision 1.22  1996/08/27 05:06:24  dmb
 * Added field to sstring class to get big performance boost
 *
 * Revision 1.21  1996/08/25 00:04:21  dmb
 * Added parameter to cpp_inherit() function in the Language class.
 * Added pragma function to Language class
 *
 * Revision 1.20  1996/08/21 16:49:50  dmb
 * Minor cleanup to eliminate warnings.
 *
 * Revision 1.19  1996/08/21 05:46:09  dmb
 * Minor modifications to language class in order to support better
 * parsing of structs and classes.
 *
 * Revision 1.18  1996/08/15 05:07:36  dmb
 * Add type-mangling as a language-dependent option (in order to
 * support Perl5 pointers).
 *
 * Revision 1.17  1996/08/12 01:53:06  dmb
 * Changes to language class structure
 *
 * Revision 1.16  1996/06/10 21:50:18  beazley
 * Changed version number
 *
 * Revision 1.15  1996/06/02  00:16:55  beazley
 * Cleaned up a few things
 *
 * Revision 1.14  1996/05/22  20:24:20  beazley
 * Added some more emit functions
 *
 * Revision 1.13  1996/05/20  23:36:25  beazley
 * Extended language class with C++ functions.   Default methods
 * are provided, but each target language can certainly override
 * these.
 *
 * Revision 1.12  1996/05/17  05:52:15  beazley
 * A few more minor changes
 *
 * Revision 1.11  1996/05/14  23:24:40  beazley
 * Minor changes
 *
 * Revision 1.10  1996/05/13  23:47:26  beazley
 * Changed Language class for better handling of module names
 *
 * Revision 1.9  1996/05/10  23:39:23  beazley
 * Removed some obsolete variables.  Added a mode field to Documentation class.
 *
 * Revision 1.8  1996/05/03  22:30:05  dmb
 * Added IncludeFile() function.
 *
 * Revision 1.7  1996/05/03 05:12:10  dmb
 * Added CPlusPlus variable.
 *
 * Revision 1.6  1996/05/01 22:43:00  dmb
 * Added check_options() stub.
 *
 * Revision 1.5  1996/04/14 15:25:05  dmb
 * Took out <unistd.h>
 *
 * Revision 1.4  1996/04/03 22:49:21  beazley
 * Added IgnoreComments variable.
 *
 * Revision 1.3  1996/03/28  02:48:34  beazley
 * Added implicit_ptr field to datatypes in order to handle certain types of
 * datatypes correctly.
 *
 * Revision 1.2  1996/03/22  23:43:56  beazley
 * Changed class structure slightly. Cleaned stuff up.
 *
 * Revision 1.1  1996/03/16  06:30:32  beazley
 * Initial revision
 *
 * Revision 1.12  1996/03/04  21:30:22  beazley
 * Changed usage() function prototypes.
 *
 * Revision 1.11  1996/02/20  04:16:16  beazley
 * Took out stream library.
 *
 * Revision 1.10  1996/02/16  08:31:01  beazley
 * Added <stdlib.h>
 *
 * Revision 1.9  1996/02/16  05:21:35  beazley
 * Added Perl5 support back
 *
 * Revision 1.8  1996/02/15  22:38:57  beazley
 * Changed copyright.  Added typedef stuff.
 *
 * Revision 1.7  1996/02/12  08:21:07  beazley
 * Minor changes
 *
 * Revision 1.6  1996/02/07  05:21:00  beazley
 * Added declarations for documentation.  Cleaned up alot of stuff.
 *
 * Revision 1.5  1996/01/16  00:56:00  beazley
 * Minor changes.
 *
 * Revision 1.4  1996/01/15  22:12:17  beazley
 * Added guile support
 *
 * Revision 1.3  1996/01/13  01:33:38  beazley
 * Added Perl 4.0 include
 *
 * Revision 1.2  1996/01/05  22:41:04  dmb
 * *** empty log message ***
 *
 * Revision 1.1  1995/12/30 04:34:43  dmb
 * Initial revision
 *
 ***********************************************************************/
