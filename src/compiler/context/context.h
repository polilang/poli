/*
   Poli lanugage - 2017, MIT license
   Compiler context
*/



#ifndef POLI_CONTEXT_H
#define POLI_CONTEXT_H



#include "lib/vector/vector.h"



typedef
struct Compiler
{
   vector(char*) source;
   char* curr_char;

   unsigned line;

   vector(struct Token) tokens;
   vector(struct Token) symbols;
   vector(struct Node) tree;
}
Compiler;



#endif
