/*
   evolbug 2017, MIT license
   Garbage managed memory allocator
*/



#ifndef EVOLBUG_GARBAGE
#define EVOLBUG_GARBAGE



#include <stdlib.h>



static const unsigned gm_manager_lock = 0; // default lock global

// create a garbage managed scope
void  gm_push     ();

// pop a garbage managed scope freeing everything allocated in it
void  gm_pop      ();

// register a pointer in the current scope
void* gm_register (void* ptr);

// check if a pointer is managed
int   gm_ismanaged(void* pointer);

// release all scopes
void  gm_purge    ();



// allocate a managed memory slot
void* galloc   (unsigned bytes);

// reallocated a managed memory slot
void* grealloc (void* ptr, unsigned bytes);

// free a managed memory slot
void* gfree    (void* ptr);

// export a variable to previous scope so it doesn't get freed when current one is released
void* gexport  (void* ptr);



// garbage managed main redefinition
int __managed_main__();
#define main(...) \
   main (int argc, char* argv[]) {gm_push(); atexit(gm_purge); return __managed_main__(argc, argv);} \
   int __managed_main__(__VA_ARGS__)


// macros to define scopes, use these in start/end of functions
#define GM_MANAGE_SCOPE \
   static const unsigned gm_manager_lock = 1; gm_push();

#define GM_END_SCOPE \
   if (gm_manager_lock) gm_pop();


// return redefinition to work in conjunction with scope macros
#define greturn(ret) \
   ({__typeof__(ret) r = ret; if (gm_manager_lock) gm_pop(); return r;})



#endif
