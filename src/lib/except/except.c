/*
   evolbug - 2017, MIT license
   exception system
*/



#include <setjmp.h>



typedef
struct exception
{
   const char* fmt;
   const long ID;
}
Exception;

struct exception_stack
{
   struct exception_stack *prev;
   long ex_id;
   jmp_buf jump;
}
*ex_stack = 0;


int caught(int id, Exception *exlist)
{
   while (exlist->ID != 0)
      if ((int) exlist->ID == id)
         return 1;
      else
         exlist++;

   return 0;
}
