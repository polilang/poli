#include <stdlib.h>
#include <stdio.h>
#include "types.c"

struct gmf
{
   int         size;   // pool size
   struct gmf* last;   // last stack frame
   void**      p_real; // real pointer tracker
   u08*        o_data; // data offset tracker
};



// global garbage manager pointer
struct gmf* GM = NULL;



void gm_push () // add stack frame
{
   struct gmf* new_frame = malloc(sizeof(struct gmf));

   new_frame->o_data = NULL;
   new_frame->p_real = NULL;
   new_frame->size   = 0;
   new_frame->last   = GM;

   GM = new_frame;
}


void gm_pop () // release stack frame with all allocated variables
{
   if (GM != NULL) // stack frame exists
   {
      if (GM->p_real != NULL) // pointers registered
      {
         for (int i=0; i < GM->size; i++)
            if (GM->p_real[i] != NULL)
               free(GM->p_real[i]);

         free(GM->p_real);
         free(GM->o_data);
      }

      struct gmf* last = GM->last; // temporary
      free(GM); // release current frame
      GM = last; // set to last frame
   }
}


void* gm_register (void* real, u32 o_data) // register a pointer to the garbage manager
{
   if (GM == NULL) gm_push(); // push frame if none present

   ++GM->size; // increase size counter

   // grow lists
   GM->p_real = realloc(
      GM->p_real,
      GM->size * sizeof(void*)
   );

   GM->o_data = realloc(
      GM->o_data,
      GM->size * sizeof(u08)
   );

   GM->p_real[GM->size-1] = real;
   GM->o_data[GM->size-1] = o_data;

   // return registered real pointer
   return real;
}

/*
   0: false
   1: true
*/
int gm_ismanaged(void* var)
{
   if (GM != NULL)
      for (int i = 0; i < GM->size; i++)
         if (GM->p_real[i] == (void*)((char*)var - GM->o_data[i])) // query manager pool
            return 1; // is managed

   return 0;
}


void* gm_realptr (void* dataptr)
{
   if (gm_ismanaged(dataptr))
      if (GM != NULL)
         for (int i = 0; i < GM->size; i++)
            if (GM->p_real[i] == (void*)((char*)dataptr - GM->o_data[i])) // query manager pool
               return GM->p_real[i]; // is managed

   return NULL;
}

void* alloc (size_t bytes) // allocate managed memory
{
   //allocate and return memory pointer
   char* mem = gm_register(malloc(bytes + 1), 1);
   *mem = 0; // null typecheck guard
   return mem + 1; // pointer to data
}


void* alloct (size_t bytes, char* type)
{
   int tlen = 0; // typename length
   while (type[tlen] != 0) tlen++;

   char* mem = gm_register(malloc(bytes + tlen + 1), tlen+1); // allocate memory with typename space
   for (int i = 0; i <= tlen; i++) mem[i] = type[tlen - i]; // copy typename over !INVERTED!

   return mem + tlen + 1; // pointer to data
}


void* gm_realloc (void* p_data, size_t bytes)
{
   if (gm_ismanaged(p_data))
      for (int i = 0; i < GM->size; i++)
         if (GM->p_real[i] == (void*)((u08*)p_data - GM->o_data[i]))
         {
            GM->p_real[i] = realloc(GM->p_real[i], bytes + GM->o_data[i]);
            return GM->p_real[i] + GM->o_data[i];
         }

   return NULL;
}


void delete (void* p_data)
{
   for (int i = 0; i < GM->size; i++)

      if (GM->p_real[i] == (void*)((u08*)p_data - GM->o_data[i])) // query manager pool
      {
         free(GM->p_real[i]); // release variable
         GM->p_real[i] = NULL; // remove reference
      }
}


void gm_purge () // release all stack frames
{
   while (GM != NULL) gm_pop();
}



/*
   -1: unmanaged
    0: false
    1: true
*/
int gm_istype(void* var, char* type) // check type for a managed variable
{
   if (!gm_ismanaged(var))
      return -1;

   char* bytes = (char*) var; // byte view of the variable

   while (*(bytes--) != 0) // read backwards until NULL
      if (*type++ != *bytes) // doesn't match
         return 0;

   return 1;
}


void* gm_export (void* var) // export a variable from inner GM scope to outer
{
   if (!gm_ismanaged(var) || GM->last == NULL) return NULL;

   for (int i = 0; i < GM->size; i++)

      if (GM->p_real[i] == (void*)((u08*)var - GM->o_data[i])) // query manager pool
      {
         struct gmf* up = GM->last;

         ++up->size; // increase size counter

         // grow lists
         up->p_real = realloc(
            up->p_real,
            up->size * sizeof(void*)
         );

         up->o_data = realloc(
            up->o_data,
            up->size * sizeof(u08)
         );

         up->p_real[up->size-1] = GM->p_real[i]; // move pointer up
         up->o_data[up->size-1] = GM->o_data[i];

         GM->p_real[i] = NULL; // remove from current scope so it doesn't get freed
         GM->o_data[i] = 0;

         return var;
      }
}


#ifdef GM_SCOPE_MACRO

#define $(...)\
{gm_push();\
__VA_ARGS__ \
gm_pop();}

#endif

// cleaner alloc() proxy -- causes problems on word boundaries
/*
#include "macros.c"

#define new(...) __JOIN__(__new_, __VA_COUNT__(__VA_ARGS__), __VA_ARGS__)

#define __JOIN__(head, tail, ...) __CAT__(head, tail)(__VA_ARGS__)
#define __new_1(type)       alloc(sizeof(type))
#define __new_2(type, size) alloc(sizeof(type)*size)
*/
