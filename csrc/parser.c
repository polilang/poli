/*
   Poli language - 2017, MIT license

   token stream parser
*/



#include <stdio.h>

#include "lexer.c"

#include "lib/string.c"     // strf
#include "lib/types.c"      // datatypes
#include "lib/readfile.c"   // readfile
#include "lib/strace.c"     // st_assert, st_c, st_v






i32
main ()
{
   atexit(gm_purge);

   str *source = readfile("lexer.c");

   Token *stream = tokenize(source, "// /* */", "\" \"");

   while (stream->next)
   {
      printf("\e[30;1m%s\e[0m\e[1m%s\e[0m", ttype(stream->type), str_slicep(stream->from, stream->to));
      stream = stream->next;
   }

}
