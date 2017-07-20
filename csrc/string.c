/*
   Poli language - 2017, MIT license

   string class
   ASCII only
*/



#ifndef POLI_STRING
#define POLI_STRING



#include <stdio.h>   // vsprintf
#include <stdarg.h>  // va_list, va_start, va_end

#include "gc.c"      // alloct, delete, realloc
#include "types.c"   // datatypes
#include "fpow10.c"  // fast 10 powers



static u32
str_length (str s)
{
   u32 len = 0;
   while (s[len] != 0) len++;
   return len;
}


static str
str_new (str s) //str constructor
{
   str mem = alloct(str_length(s) + 1, "str");
   for (u32 i = 0; i < str_length(s) + 1; i++) mem[i] = s[i];
   return mem;
}


static str
str_lower (str s)
{
   str ret = alloc(str_length(s) + 1);

   for (u32 i=0; i <= str_length(s); i++)

      if (s[i] >= 0x41 && s[i] <= 0x5A)
         ret[i] = s[i] + 0x20;

      else
         ret[i] = s[i];

   return ret;
}


static str
str_upper (str s)
{
   str ret = alloc(str_length(s) + 1);

   for (u32 i=0; i <= str_length(s); i++)

      if (s[i] >= 0x61 && s[i] <= 0x7A)
         ret[i] = s[i] - 0x20;

      else
         ret[i] = s[i];

   return ret;
}


static i32
str_equal (str a, str b)
{
   if (str_length(a) == str_length(b))
   {
      for (u32 i = 0; i < str_length(a); i++)
         if (a[i] != b[i])
            return 0;

      return 1;
   }

   return 0;
}


static i32
str_comparen (str a, str b, i32 len)
{
   while (*a == *b && --len)
   {
      if (*a == '\0')
         return *a - *b;

      a++; b++;
   }

   return *a - *b;
}


static i32
str_compare (str a, str b)
{
   return str_comparen(a, b, str_length(b));
}


// find first occurrence of a string in string
// return index
static i64
str_find (str in, str what)
{
   if (*in && *what)
   {
      u32 l_what = str_length(what),
          l_in   = str_length(in);

      if (l_in >= l_what) //string has to be >= than the slice to be found
         for (u32 i = 0; i < l_in; i++)
            if (in[i] == *what)
               if (str_comparen(in + i, what, l_what) == 0)
                  return i;
   }

   return -1; // not found
}


// find first occurrence of a string in string
// return pointer
static str
str_pfind (str in, str what)
{
   i64 index = str_find(in, what);

   if (index != -1)
      return in + index;

   return NULL;
}


// find first occurrence of characters in string
// return index
static i64
str_findc (str in, str what)
{
   if (*in && *what)
   {
      u32 l_what = str_length(what),
          l_in   = str_length(in);

      if (l_in >= l_what) //string has to be >= than the slice to be found
         for (u32 i = 0; i < l_in;   i++)
         for (u32 j = 0; j < l_what; j++)
            if (in[i] == what[j])
               return i;
   }

   return -1; // not found
}


// find first occurrence of characters in string
// return pointer
static str
str_pfindc (str in, str what)
{
   i64 index = str_findc(in, what);

   if (index != -1)
      return in + index;

   return NULL;
}


// count occurrences of a substring
static u32
str_occurs (str in, str what)
{
   u32 i = 0; in--;
   while ((in = str_pfind(in + 1, what))) i++;
   return i;
}

static str
str_from_fmt (str fmt, ...)
{
   va_list arg; va_start(arg, fmt);

   i08 buf[128];
   vsprintf(buf, fmt, arg);

   va_end(arg);

   return str_new(buf);
}


static str
str_from_i64 (i64 v)
{
   i08 buf[30];
   sprintf(buf, "%li", v);
   return str_new(buf);
//   return strcpy(alloc(str_length(buf)+1), buf);
}


static str
str_from_u64 (u64 v)
{
   i08 buf[30];
   sprintf(buf, "%lu", v);
   return str_new(buf);
//   return strcpy(alloc(str_length(buf)+1), buf);
}


static str
str_from_f64 (f64 v)
{
   i08 buf[128];
   sprintf(buf, "%lf", v);

   // remove trailing zeros
   for (i32 i = str_length(buf)-1; buf[i]=='0' || buf[i]=='.' || buf[i]==','; i--)
      buf[i]=0;

   return str_new(buf);
//   return strcpy(alloc(str_length(buf)+1), buf);
}


static i64
str_to_i64 (str s)
{
   i64 final = 0, sign = 1, mult = 1;

   switch (*s)
   {
      case '-': sign = -1;
      case '+': s++;
   }

   u32 nlen = str_length(s);

   for (i64 ch = nlen-1; ch >= 0 ; ch--) // read the number
   {
//      final += (i64)(s[ch] - '0') * fpow10(nlen-ch-1); // digit * 10^pos_in_str

      final += (i64)(s[ch] - '0') * (i64)mult; // digit * 10^pos_in_str
      mult *= 10;
   }

   return sign * final;
}


static u64
str_to_u64 (str s)
{
   return str_to_i64(s);
}


static f64
str_to_f64 (str s)
{
   f64 sign = 1.0;

   switch(*s)
   {
      case '-': sign = -1.0;
      case '+': s++;
   }

   str position_of_point = str_pfindc(s, ".,");
   str strend = s + str_length(s);
   str next_point;

   if ((next_point = position_of_point ? str_pfindc(position_of_point+1, ".,"): NULL))
      strend = next_point;

   i32 decimal_places = position_of_point ? strend - position_of_point - 1 : 0;

   f64 factor = fpow10(-decimal_places);
   f64 total = 0.0;

   for (str it = strend; it > s;)
   {
      if (--it == position_of_point) continue;

      total += (f64)(*it - '0') * factor;
      factor *= 10.0;
   }

   return total * sign;
}


static str
str_concat (str a, str b)
{
   i64 len = str_length(a) + str_length(b),
       off = 0;

   if (len < 0) return NULL; // strings are too long / overflow

   str mem = alloct(len + 1, "str");

   while (*a) *(mem + off++) = *a++;
   while (*b) *(mem + off++) = *b++;
   *(mem + off) = 0;

   return mem;
}


static i08
str_iswhitec (str c)
{
   switch (*c)
   {
      case '\t':
      case '\r':
      case '\n':
      case ' ' :
         return 1;

      default:
         return 0;
   }
}


static str
str_ltrim (str s) // trim whitespace from left side
{
   // trim forward
   while (str_iswhitec(s)) s++;
   return str_new(s);
}


static str
str_rtrim (str s) // trim whitespace from right side
{
   str result;

   str fwd = str_new(s); // copy

   str bwd = fwd + str_length(fwd) -1; // point to end and trim "backward"
   while (str_iswhitec(bwd)) *bwd-- = 0;

   result = str_new(fwd);

   delete(fwd);

   return result;
}


static str
str_trim (str s) // trim whitespace from both sides
{
   str result;

   str fwd = str_new(s); // copy
   while (str_iswhitec(fwd)) fwd++;

   str bwd = fwd + str_length(fwd) -1; // point to end and trim "backward"
   while (str_iswhitec(bwd)) *bwd-- = 0;

   result = str_new(fwd); // quick copy

   delete(fwd);

   return result;
}


static str
str_format (str s, ...)
{
   va_list args; va_start(args, s);

   str result = alloc(str_length(s)*2 + 128);
   vsprintf(result, s, args);

   va_end(args);

   str ret = str_new(result);
   delete(result);

   return ret;
}


static str
str_slice (str s, u32 from, u32 to)
{
   if (to > str_length(s)) to = str_length(s);

   str ret = alloct(to - from + 1, "str");

   for (u32 i = from; i < to; i++)
      ret[i - from] = s[i];

   ret[to - from] = 0;

   return ret;
}

static str
str_slicep (str s, str from, str to)
{
   if (!to || to > s + str_length(s))
      to = s + str_length(s);

   str ret = alloct(to - from + 1, "str");

   for (str i = from; i < to; i++)
      ret[i-from] = *i;

   ret[to-from] = 0;

   return ret;
}


static str
str_split_lazy (str s, str delim, str* last)
{
   str ret = NULL;

   if (*last)
   {
      str next = str_pfind(*last, delim);

      ret = str_slicep(s, *last, next);

      *last = next ? next + str_length(delim) : NULL;
   }

   return ret;
}


static str*
str_split (str s, str delim)
{
   str* strings = NULL;
   str last = s;
   u32 i = 0, o = str_occurs(s, delim)+1;

   strings = alloc(sizeof(str) * (o+1));

   while ((strings[i] = str_split_lazy(s, delim, &last))) i++;

   return strings;
}



struct
{
   str   (*new)      ();
   u32   (*length)   ();
   str   (*lower)    ();
   str   (*upper)    ();
   str   (*from_fmt) (str, ...);
   str   (*from_i)   ();
   str   (*from_u)   ();
   str   (*from_f)   ();
   i64   (*to_i)     ();
   u64   (*to_u)     ();
   f64   (*to_f)     ();
   i32   (*equal)    ();
   i32   (*compare)  ();
   i32   (*comparen) ();
   i08   (*iswhitec) ();
   i64   (*find)     ();
   str   (*pfind)    ();
   i64   (*findc)    ();
   str   (*pfindc)   ();
   u32   (*occurs)   ();
   str   (*trim)     ();
   str   (*triml)    ();
   str   (*trimr)    ();
   str   (*concat)   ();
   str   (*format)   (str, ...);
   str   (*slice)    ();
   str   (*slicep)   ();
   str*  (*split)    ();
   str   (*splitl)   ();
}
strf =
{
   str_new,

   str_length,

   str_lower,
   str_upper,

   str_from_fmt,
   str_from_i64,
   str_from_u64,
   str_from_f64,
   str_to_i64,
   str_to_u64,
   str_to_f64,

   str_equal,
   str_compare,
   str_comparen,
   str_iswhitec,

   str_find,
   str_pfind,
   str_findc,
   str_pfindc,
   str_occurs,

   str_trim,
   str_ltrim,
   str_rtrim,

   str_concat,
   str_format,

   str_slice,
   str_slicep,
   str_split,
   str_split_lazy,
};


#endif
