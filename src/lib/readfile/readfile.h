/*
   Poli language - 2017, MIT license
   quick readfile
*/



#ifndef POLI_READFILE
#define POLI_READFILE



#include <stdio.h>   // fopen, fgets, fclose, FILE

#include "garbage/gm.h"    // alloc, gm_realloc
#include "string/string.h" // str_new
#include "except/except.h"



static Exception(FILE_OPEN_FAIL, "Failed to open file '%s'");



char** readfile (char* filename)
#define readfile(...) st_call(readfile(__VA_ARGS__))
{
   char **strings = alloc(sizeof(char*));
   strings[0] = 0;
   unsigned last_line = 0;

   char buf[256];

   FILE *f = fopen(filename, "r");

   if (!f) throw(FILE_OPEN_FAIL, filename);

   while (fgets(buf, sizeof(buf), f))
   {
      strings[last_line] = str_new(buf);
      strings = gm_realloc(strings, sizeof(char*) * ((++last_line) + 1));
      strings[last_line] = 0;
   }

   fclose(f);

   return strings;
}



#endif
