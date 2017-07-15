/*
   Poli language - 2017, MIT license
   blocktree generator
*/


#include <stdio.h>
#include <stdlib.h>


struct Blocktree
{

};

/*

gc +
   | frame1 +
            | ptr1
            | ptr2
            | frame2 +
                     | ptr3
                     | ptr4


static gc* G = gc_new();

string* mystr = new(string);
set(mystr, "hello world");



gc_free(G);
*/
