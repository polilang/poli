/*
   Poli language - 2017, MIT license

   quick readfile
*/



#include <stdio.h>   // fopen, fgets, fclose, FILE

#include "gc.c"      // alloc, gm_realloc
#include "types.c"   // datatypes
#include "string.c"  // strf.new



str* readfile (str filename)
{
   str *strings = alloc(sizeof(str));
   strings[0] = 0;
   u32 last_line = 0;

   i08 buf[256];

   FILE *f = fopen(filename, "r");

   while (fgets(buf, sizeof(buf), f))
   {
      strings[last_line] = strf.new(buf);
      strings = gm_realloc(strings, sizeof(str)*((++last_line)+1));
      strings[last_line] = 0;
   }

   fclose(f);

   return strings;
}
