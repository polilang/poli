#ifndef __POLI_TYPES__
#define __POLI_TYPES__

#include <stdlib.h>
#include <string.h>

#include "types.c"

char* string_tostring(struct dyn obj)
{
   char *ostr = malloc(strlen((char*) obj.data));
   strcpy(ostr, (char*) obj.data);
   return ostr;
}

struct dyn string(char *val)
{
   struct dyn obj = {.type = "dyn", .tostring=string_tostring};

   obj.data = malloc(strlen(val)+1);
   strcpy(obj.data, val);

   return obj;
}

#endif
