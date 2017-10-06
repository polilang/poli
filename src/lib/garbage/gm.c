/*
   evolbug 2017, MIT license
   Garbage managed memory allocator
*/



#include <stdlib.h>
#include "gm.h"



struct gm_frame
{
   unsigned size; // pool size
   struct gm_frame* prev; // previous stack frame
   void** tracker; // pointer tracker
};



// global garbage manager pointer
struct gm_frame *GM = NULL;



// add stack frame
void gm_push ()
{
   struct gm_frame *new_frame = malloc(sizeof(struct gm_frame));

   new_frame->tracker = NULL;
   new_frame->size = 0;
   new_frame->prev = GM;

   GM = new_frame;
}


// release stack frame with all allocated variables
void gm_pop ()
{
   if (GM != NULL) // stack frame exists
   {
      if (GM->tracker != NULL) // pointers registered
      {
         for (unsigned p = 0; p < GM->size; p++)
            if (GM->tracker[p] != NULL)
               free(GM->tracker[p]);

         free(GM->tracker);
      }

      struct gm_frame *prev = GM->prev; // previous stack frame
      free(GM); // release current frame
      GM = prev; // set to previous frame
   }
}


// register a pointer to the garbage manager
void* gm_register (void *ptr)
{
   if (GM == NULL) gm_push(); // push frame if none present

   GM->size++; // increase size counter

   // grow tracker
   GM->tracker = realloc(GM->tracker, GM->size * sizeof(void*));

   GM->tracker[GM->size-1] = ptr;

   return ptr;
}


int gm_ismanaged(void *ptr)
{
   if (GM != NULL)
      for (unsigned p = 0; p < GM->size; p++)
         if (GM->tracker[p] == ptr) // query manager pool
            return 1; // is managed

   return 0;
}


// allocate managed memory
void* galloc (unsigned bytes)
{
   //allocate and return memory pointer
   return gm_register(malloc(bytes + 1));
}


void* grealloc (void *ptr, unsigned bytes)
{
   if (gm_ismanaged(ptr))
   {
      for (unsigned p = 0; p < GM->size; p++)

         if (GM->tracker[p] == ptr)
         {
            GM->tracker[p] = realloc(GM->tracker[p], bytes);
            return GM->tracker[p];
         }
   }

   else if (ptr == NULL)
      return galloc(bytes);

   return NULL;
}


void* gfree (void *ptr)
{
   if (ptr)
   {
      for (unsigned p = 0; p < GM->size; p++)

         // query manager pool
         if (GM->tracker[p] == ptr)
         {
            // stop tracking and free
            free(GM->tracker[p]);
            GM->tracker[p] = NULL;

            // for zeroing out external pointer
            return NULL;
         }

      free(ptr); // free pointer regardless
   }

   // for zeroing out external pointer
   return NULL;
}


void* gexport (void *ptr) // export a variable from inner GM scope to outer
{
   if (!gm_ismanaged(ptr) || GM->prev == NULL) return NULL;

   for (unsigned p = 0; p < GM->size; p++)

      // query manager pool
      if (GM->tracker[p] == ptr)
      {
         struct gm_frame* prev = GM->prev;

         // increase size counter
         prev->size++;

         // grow trakcer
         prev->tracker = realloc(prev->tracker, prev->size * sizeof(void*));

         // move pointer up
         prev->tracker[prev->size-1] = GM->tracker[p];

         // remove from current scope so it doesn't get freed
         GM->tracker[p] = NULL;

         return ptr;
      }

   return NULL;
}


// release all stack frames
void gm_purge ()
{
   while (GM != NULL) gm_pop();
}
