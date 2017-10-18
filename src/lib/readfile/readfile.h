/*
   Poli language - 2017, MIT license
   quick readfile
*/



#ifndef POLI_READFILE
#define POLI_READFILE



#include <stdio.h>   // fopen, fgets, fclose, FILE

#include "lib/string/string.h" // str_new
#include "lib/except/except.h"
#include "lib/vector/vector.h"



static Exception(FILE_OPEN_FAIL, "Failed to open file '%s'");



vector(char*) readfile (char* filename)
#define readfile(...) st_call(readfile(__VA_ARGS__))
{
   vector(char*) strings = v_new();
   char buf[512];

   FILE *f = fopen(filename, "r");

   if (!f) throw(FILE_OPEN_FAIL, filename);

   while (fgets(buf, sizeof(buf), f))
      v_push(strings, str_new(buf));

   fclose(f);

   return strings;
}



#endif
