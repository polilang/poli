/*
   evolbug - 2017, MIT license
   vector/dynamic array implementation
*/



#ifndef EVOLBUG_VECTOR
#define EVOLBUG_VECTOR


#include <stdarg.h>
#include <stdlib.h>

#include "../strace/strace.h"
#include "../except/except.h"
#include "../term/term.h"
#include "../garbage/gm.h"



const Exception
(
   VECTOR_INIT,
   "vector ["Fgreen Mbold"%s"Treset"] not initialized with ["Fblue Mbold"v_new()"Treset"]"
);

const Exception
(
   VECTOR_EMPTY,
   "vector ["Fgreen Mbold"%s"Treset"] is empty"
);

const Exception
(
   VECTOR_INDEX,
   "index ["Fgreen Mbold"%s"Treset"["Fblue Mbold"%s"Treset"]"Treset"] is out of range"
);

const Exception
(
   VECTOR_INCOMPATIBLE,
   "inserting incompatible type of size ["Fblue Mbold"%s"Treset"] in vector ["Mbold Fgreen"%s"Treset"]"
);



#define V_DATAP(real) ((__typeof__(real))((unsigned*)real + 1))
#define V_REALP(data) ((unsigned*)data - 1)

#define V_CHK_PTR(v)    ({assert(gm_ismanaged(V_REALP(v)), VECTOR_INIT, #v);})
#define V_CHK_LEN(v)    ({assert(*V_REALP(v), VECTOR_EMPTY, #v);})
#define V_CHK_IDX(v, i) ({assert(i <= *V_REALP(v), VECTOR_INDEX, #v, #i);})


#define v_size(v) *V_REALP(v)


void* v_new ()
#define v_new() st_call(v_new())
{
   unsigned *vector = galloc(sizeof(unsigned));
   vector[0] = 0;
   return V_DATAP(vector);
}

void* v_insert(unsigned index, unsigned esize, unsigned ecount, char **vector, char *e)
#define v_insert(index, vector, ...) {\
   st_push(v_insert(index, vector, ##__VA_ARGS__));\
   V_CHK_PTR(vector); V_CHK_IDX(vector, index);\
   __typeof__(vector[0]) temp[] = {__VA_ARGS__};\
   vector = v_insert(index, sizeof(vector[0]), sizeof(temp)/sizeof(vector[0]), (char**)&vector, (char*)temp);\
   st_pop();}
{
   *vector = V_DATAP(
      grealloc(V_REALP(*vector), sizeof(unsigned) + esize * (*V_REALP(*vector) + ecount + 1))
   );

   for (unsigned i = index; i < v_size(*vector); i++)
      for (unsigned b = 0; b < esize; b++)
         (*vector)[(i + ecount) * esize + b] = (*vector)[i * esize + b];

   v_size(*vector) += ecount;

   for (unsigned i = 0; i < ecount; i++)
      for (unsigned b = 0; b < esize; b++)
         (*vector)[(i + index) * esize + b] = e[i * esize + b];

   return *vector;
}

#define v_push(vector, ...) {\
   st_push(v_push(vector, ##__VA_ARGS__));\
   v_insert(v_size(vector), vector, ##__VA_ARGS__);\
   st_pop();}
/*



#define v_remove(vector, index) ({\
   st_push(v_remove(vector, index));\
\
   V_CHK_PTR(vector);\
   unsigned *v = V_REALP(vector);\
   V_CHK_LEN(v);\
   V_CHK_IDX(v, index);\
\
   __typeof__(vector[0]) r = vector[index]; v--;\
   for (unsigned i = index; i <= v; i++) vector[i] = vector[i+1];\
\
   vector = V_DATAP(grealloc(V_REALP(vector),\
      sizeof(struct Vector) + sizeof(vector[0]) * v));\
\
   st_pop();\
   r; })

#define v_pop(vector) v_remove(vector, V_REALP(vector)-1)
*/
/*
#undef V_DATAP
#undef V_REALP
#undef V_CHK_PTR
#undef V_CHL_LEN
#undef V_CHK_IDX
*/
#define vector(type) type*

#endif
