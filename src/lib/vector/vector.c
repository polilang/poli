/*
   evolbug - 2017, MIT license
   vector/dynamic array implementation
*/



#include <stdarg.h>
#include <stdlib.h>

#include "../strace/strace.h"
#include "../except/except.h"
#include "../garbage/gm.h"



#define V_DATAP(real) ((__typeof__(real))((unsigned*)real + 1))
#define V_REALP(data) ((unsigned*)data - 1)



const Exception
(
   VECTOR_INIT,
   "vector [\033[1;32m%s\033[0m] not initialized with [\033[1;34mv_new()\033[0m]"
);

const Exception
(
   VECTOR_EMPTY,
   "vector [\033[1;32m%s\033[0m] is empty"
);

const Exception
(
   VECTOR_INDEX,
   "index [\033[1;32m%s\033[0m[\033[1;34m%s\033[0m]] is out of range"
);


unsigned v_size(void* vector)
{
   return *((unsigned*)vector - 1);
}


void* v_new ()
{
   unsigned *vector = galloc(sizeof(unsigned));
   vector[0] = 0;
   return V_DATAP(vector);
}


void v_insert(unsigned index, unsigned esize, unsigned ecount, char **vector, char *e)
{
   *vector = V_DATAP(
      grealloc(V_REALP(*vector), sizeof(unsigned) + esize * (*V_REALP(*vector) + ecount + 1))
   );

   for (unsigned i = index; i < v_size(*vector); i++)
      for (unsigned b = 0; b < esize; b++)
         (*vector)[(i + ecount) * esize + b] = (*vector)[i * esize + b];

   *(((unsigned*)(*vector)) - 1) += ecount;

   for (unsigned i = 0; i < ecount; i++)
      for (unsigned b = 0; b < esize; b++)
         (*vector)[(i + index) * esize + b] = e[i * esize + b];
}

void v_remove(unsigned index, unsigned esize, char **vector)
{
   *vector = V_DATAP(
      grealloc(V_REALP(*vector), sizeof(unsigned) + esize * (*V_REALP(*vector)))
   );

   (*V_REALP(*vector))--; // size--

   for (unsigned i = index; i < v_size(*vector); i++)
      for (unsigned b = 0; b < esize; b++)
         (*vector)[i * esize + b] = (*vector)[(i + 1) * esize + b];
}

