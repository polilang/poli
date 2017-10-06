/*
   Poli language - 2017, MIT license

   token stream parser
*/



#include <stdio.h>

#include "lib/vector/vector.h"

struct pair
{
   long a, b;
};


int main ()
{
   vector(struct pair) v = v_new();

   v_push(v, {1, 1}, {2, 3}, {3, 4});
   v_push(v, {4, 5}, {6, 7}, {8, 9});

   for (unsigned i = 0; i < v_size(v); i++)
      printf("%li:%li ", v[i].a, v[i].b);

   puts("hello !");
}
