#ifndef __POLI_DYNAMICS__
#define __POLI_DYNAMICS__

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>

#include "assert.c"

#define any(var) ((union any) var)

int intlen (long long n) // length of an integer string
{
   if (n == 0) return 1;
   if (n<0) return log10(-n)+1;
   return log10(n);
}


#define WRAPTYPE(alias, _type)                  \
struct alias {char type[4]; _type data;};       \
struct alias alias (_type val)                  \
{                                               \
   struct alias obj = {.type=#alias, .data=val};\
   return obj;                                  \
}

WRAPTYPE(i08, int8_t);
WRAPTYPE(i16, int16_t);
WRAPTYPE(i32, int32_t);
WRAPTYPE(i64, int64_t);

WRAPTYPE(u08, uint8_t);
WRAPTYPE(u16, uint16_t);
WRAPTYPE(u32, uint32_t);
WRAPTYPE(u64, uint64_t);

WRAPTYPE(f32, float);
WRAPTYPE(f64, double);

#undef WRAPTYPE

struct raw {char type[4];};

struct dyn
{
   char type[4];
   void *data;
   char* (*tostring) (struct dyn obj);
};

union any
{
   struct raw raw;
   struct i08 i08;
   struct i16 i16;
   struct i32 i32;
   struct i64 i64;
   struct u08 u08;
   struct u16 u16;
   struct u32 u32;
   struct u64 u64;
   struct f32 f32;
   struct f64 f64;
   struct dyn dyn;
};


#define ITOS(_type, fmt)                    \
if (!strcmp(obj.raw.type, #_type)) {        \
   str = malloc(intlen(obj._type.data) + 1);\
   sprintf(str, fmt, obj._type.data);       \
}

char* tostring (union any obj)
{
   char *str=NULL;
        ITOS(i08, "%i")
   else ITOS(i16, "%i")
   else ITOS(i32, "%i")
   else ITOS(i64, "%li")
   else ITOS(u08, "%i")
   else ITOS(u16, "%i")
   else ITOS(u32, "%i")
   else ITOS(u64, "%li")
   else ITOS(f32, "%f")
   else ITOS(f64, "%f")
   else if (!strcmp(obj.raw.type, "dyn")) {str = obj.dyn.tostring(obj.dyn);}
   else {error(1, "cannot stringify object of unknown type");}
   return str;
}

#undef ITOS


void print(char *fmt, ...)
{
   va_list args; va_start(args, fmt);

   char *ostr = NULL;

   while (*fmt != 0)
   {
      if (*fmt == '{' && *(fmt+1) == '}')
      {
         ostr = tostring(va_arg(args, union any));
         printf("%s", ostr);
         free(ostr);
         fmt++;
      }
      else putchar(*fmt);
      fmt++;
   }

   va_end(args);
}

#endif
