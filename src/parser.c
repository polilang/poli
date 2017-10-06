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

   v_push(v, {1, 1}, {2, 2}, {3, 3});
   v_push(v, {4, 4}, {5, 5}, {6, 6});
   v_push(v, {1, 1});

   while (1)
   {
      struct pair p = v_pop(v);
      printf("%li:%li ", p.a, p.b);
   }

   puts("hello !");
}
