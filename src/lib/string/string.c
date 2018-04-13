/*
   evolbug 2017, MIT license
   string library
*/



#ifndef POLI_STRING
#define POLI_STRING



#include <stdio.h>   // vsprintf
#include <stdarg.h>  // va_list, va_start, va_end

#include "garbage/gm.h"      // galloc, gfree, gregalloc
#include "fpow10/fpow10.h"  // fast 10 powers
#include "string.h"



int chr_isalpha (const char c)
{
   return ((c >= 'a') && (c <= 'z'))
       || ((c >= 'A') && (c <= 'Z'));
}


int chr_isdigit (const char c)
{
   return (c >= '0')
       && (c <= '9');
}


int chr_isalnum (const char c)
{
   return chr_isdigit(c)
       || chr_isalpha(c);
}


int chr_iswhite (const char c)
{
   return c == ' '
       || c == '\t'
       || c == '\r'
       || c == '\n';
}


int str_islower (const char* s)
{
   while (*s) if (*s < 'a' || *s > 'z') return 0; else s++;
   return 1;
}


int str_isupper (const char* s)
{
   while (*s) if (*s < 'A' || *s > 'Z') return 0; else s++;
   return 1;
}



unsigned str_length (const char* s)
{
   unsigned len = 0;
   while (s[len]) len++;
   return len;
}


char* str_new (const char* s) //char* constructor
{
   char* mem = galloc(str_length(s) + 1);
   for (unsigned i = 0; i <= str_length(s); i++) mem[i] = s[i];
   return mem;
}


char* str_lower (const char* s)
{
   char* ret = galloc(str_length(s) + 1);

   for (unsigned i = 0; i <= str_length(s); i++)

      if (s[i] >= 'A' && s[i] <= 'Z')
         ret[i] = s[i] + 0x20;

      else
         ret[i] = s[i];

   return ret;
}


char* str_upper (const char* s)
{
   char* ret = galloc(str_length(s) + 1);

   for (unsigned i = 0; i <= str_length(s); i++)

      if (s[i] >= 'a' && s[i] <= 'z')
         ret[i] = s[i] - 0x20;

      else
         ret[i] = s[i];

   return ret;
}


int str_equal (const char* a, const char* b)
{
   if (a && b && *a && *b)
      if (str_length(a) == str_length(b))
      {
         for (unsigned i = 0; i < str_length(a); i++)
            if (a[i] != b[i])
               return 0;

         return 1;
      }

   return 0;
}


int str_comparen (const char* a, const char* b, int len)
{
   if (a && b)
   {
      while (*a == *b && --len)
      {
         a++;
         b++;
      }

      return *a - *b;
   }
   return -1;
}


int str_compare (const char* a, const char* b)
{
   return str_comparen(a, b, str_length(b)+1);
}


// find first occurrence of a string in string
// return index
long str_find (const char* in, const char* what)
{
   if (in && what && *in && *what)
   {
      unsigned l_what = str_length(what),
               l_in   = str_length(in);

      if (l_in >= l_what) //string has to be >= than the slice to be found
         for (unsigned i = 0; i < l_in; i++)
            if (in[i] == *what)
               if (str_comparen(in + i, what, l_what) == 0)
                  return i;
   }

   return -1; // not found
}


// find first occurrence of a string in string
// return pointer
char* str_pfind (const char* in, const char* what)
{
   long index = str_find(in, what);

   if (index != -1)
      return (char*)in + index;

   return NULL;
}


// find first occurrence of characters in string
// return index
long str_findc (const char* in, const char* what)
{
   if (*in && *what)
   {
      unsigned
         l_what = str_length(what),
         l_in   = str_length(in);

      if (l_in >= l_what) //string has to be >= than the slice to be found
         for (unsigned i = 0; i < l_in;   i++)
         for (unsigned j = 0; j < l_what; j++)
            if (in[i] == what[j])
               return i;
   }

   return -1; // not found
}


// find first occurrence of characters in string
// return pointer
char* str_pfindc (const char* in, const char* what)
{
   long index = str_findc(in, what);

   if (index != -1)
      return (char*)in + index;

   return NULL;
}


char* str_plast (const char* in, const char* what)
{
   const char *last = in, *found = NULL;

   while ((found = str_pfind(last, what)))
      last = found + 1;

   return last != in ? (char*)last-1 : NULL;
}


long str_last (const char* in, const char* what)
{
   char* found = str_plast(in, what);
   return found ? found - in : -1;
}


// count occurrences of a substring
unsigned str_occurs (const char* in, const char* what)
{
   unsigned i = 0; in--;
   while ((in = str_pfind(in + 1, what))) i++;
   return i;
}


char* str_from_fmt (const char* fmt, ...)
{
   va_list arg; va_start(arg, fmt);

   char buf[128];
   vsprintf(buf, fmt, arg);

   va_end(arg);

   return str_new(buf);
}


char* str_from_i (long v)
{
   char buf[30];
   sprintf(buf, "%li", v);
   return str_new(buf);
}


char* str_from_u (unsigned long v)
{
   char buf[30];
   sprintf(buf, "%lu", v);
   return str_new(buf);
}


char* str_from_f (double v)
{
   char buf[128];
   sprintf(buf, "%lf", v);

   // remove trailing zeros
   for (int i = str_length(buf)-1; buf[i]=='0' || buf[i]=='.' || buf[i]==','; i--)
      buf[i] = 0;

   return str_new(buf);
}


long str_to_i (const char* s)
{
   long final = 0, sign = 1, mult = 1;

   switch (*s)
   {
      case '-': sign = -1;
      case '+': s++;
   }

   unsigned nlen = str_length(s);

   for (long ch = nlen-1; ch >= 0 ; ch--) // read the number
   {
      final += (long)(s[ch] - '0') * (long)mult; // digit * 10^pos_in_str
      mult *= 10;
   }

   return sign * final;
}


unsigned long str_to_u (const char* s)
{
   return str_to_i(s);
}


double str_to_f (const char* s)
{
   double sign = 1.0;

   switch(*s)
   {
      case '-': sign = -1.0;
      case '+': s++;
   }

   const char* position_of_point = str_pfindc(s, ".,");
   const char* strend = s + str_length(s);
   const char* next_point;

   if ((next_point = position_of_point ? str_pfindc(position_of_point+1, ".,"): NULL))
      strend = next_point;

   int decimal_places = position_of_point ? strend - position_of_point - 1 : 0;

   double factor = fpow10(-decimal_places);
   double total = 0.0;

   for (const char* it = strend; it > s;)
   {
      if (--it == position_of_point) continue;

      total += (double)(*it - '0') * factor;
      factor *= 10.0;
   }

   return total * sign;
}


char* str_concat (const char* a, const char* b)
{
   long len = str_length(a) + str_length(b),
       off = 0;

   if (len < 0) return NULL; // strings are too long / overflow

   char* mem = galloc(len + 1);

   while (*a) *(mem + off++) = *a++;
   while (*b) *(mem + off++) = *b++;
   *(mem + off) = 0;

   return mem;
}


char* str_extend (char* a, const char* b)
{
   long len = str_length(a) + str_length(b),
        off = str_length(a);

   if (len < 0) return NULL; // strings are too long / overflow

   a = grealloc(a, len + 1);

   while (*b) *(a + off++) = *b++;
   *(a + off) = 0;

   return a;
}


char* str_join(const char** list)
{
   char* str = str_new("");
   for (; *list; list++) str = str_extend(str, *list);
   return str;
}


char* str_triml (const char* s) // trim whitespace from left side
{
   // trim forward
   while (chr_iswhite(*s)) s++;
   return str_new(s);
}


char* str_trimr (const char* s) // trim whitespace from right side
{
   char* result;

   char* fwd = str_new(s); // copy

   char* bwd = fwd + str_length(fwd) -1; // point to end and trim "backward"
   while (chr_iswhite(*bwd)) *bwd-- = 0;

   result = str_new(fwd);

   gfree(fwd);

   return result;
}


char* str_trim (const char* s) // trim whitespace from both sides
{
   char* result;

   char* fwd = str_new(s); // copy
   while (chr_iswhite(*fwd)) fwd++;

   char* bwd = fwd + str_length(fwd) -1; // point to end and trim "backward"
   while (chr_iswhite(*bwd)) *bwd-- = 0;

   result = str_new(fwd); // quick copy

   gfree(fwd);

   return result;
}


char* str_format (const char* s, ...)
{
   va_list args; va_start(args, s);

   char* result = galloc(str_length(s)*3 + 128);
   vsprintf(result, s, args);

   char* ret = str_new(result);
   gfree(result);

   va_end(args);
   return ret;
}


char* str_slice (const char* s, long from, long to)
{
   if (from < 0)              from = str_length(s) + from;
   if (from > str_length(s))  from = str_length(s);

   if (to < 0)             to = str_length(s) + to;
   if (to > str_length(s)) to = str_length(s);

   if (from > to) return "";

   char* ret = galloc(to - from + 1);

   for (unsigned i = from; i < to; i++)
      ret[i - from] = s[i];

   ret[to - from] = 0;

   return ret;
}

char* str_slicep (const char* from, const char* to)
{
   char* ret = NULL;

   if (!to) to = from + str_length(from);

   if (from)
   {
      ret = galloc(to - from + 1);

      for (const char* i = from; i < to; i++)
         ret[i - from] = *i;

      ret[to - from] = 0;
   }

   return ret;
}

char* str_slicel (const char* from, long l)
{
   return str_slicep(from, from+l);
}


char* str_splitl (const char **last, const char *delim)
{
   char* ret = NULL;

   if (*last)
   {
      char* next = str_pfind(*last, delim);
      ret = str_slicep(*last, next);
      *last = next ? next + str_length(delim) : NULL;
   }

   return ret;
}


char** str_split (const char* s, const char* delim)
{
   unsigned
      i = 0,
      o = str_occurs(s, delim) + 1;

   char **strings = galloc(sizeof(const char*) * (o+1));
   const char *last = s;

   while ((strings[i] = str_splitl(&last, delim))) i++;

   return strings;
}



#endif
