/*
   evolbug - 2017, MIT license
   exception system
*/



#ifndef EVOLBUG_EXCEPTIONS_H
#define EVOLBUG_EXCEPTIONS_H



#include <stdio.h>
#include <setjmp.h>



struct exception
{
   const char* fmt;
   const long ID;
};

#define Exception(name, fmt) struct exception name = {fmt, (long)&name}
// const Exception(EXC, "EXC called with argument '%s'");
// throw(EXC, "hello");


struct exception_stack
{
   struct exception_stack *prev;
   long ex_id;
   jmp_buf jump;
}
*ex_stack = 0;


int caught(int id, struct exception *exlist)
{
   while (exlist->ID != 0)
      if ((int) exlist->ID == id)
         return 1;
      else
         exlist++;

   return 0;
}


/*
   This exception system can in conjunction with my stack tracer,
   providing call stack unrolling in case of an uncaught exception.
   The tracer must be included first for this to work,
   otherwise a primitive file/line reporting is used instead.
*/

#ifndef EVOLBUG_STRACE_H

   #define st_push_frame()
   #define st_push()
   #define st_trace() printf("\nin %s:%d", __FILE__, __LINE__)
   #define st_drop()

#endif

/*
   !!! WARNING !!!
   Do NOT return from within the try:catch block in any circumstances,
   as it will break the exception stack.
*/

#define try\
   { struct exception_stack ex_node; ex_node.prev = ex_stack; ex_stack=&ex_node;\
      if(!(ex_node.ex_id = (int)setjmp(ex_node.jump)))\
      { st_push_frame();


#define except(...)\
   } else if (caught(ex_node.ex_id, (struct exception[]){__VA_ARGS__, {0}}))\
   { ex_stack=ex_node.prev;


#define except_any \
   } else { ex_stack=ex_node.prev;


#define catch \
   if (ex_stack == &ex_node) ex_stack=ex_node.prev; }}


#define finally \
   }{ if (ex_stack == &ex_node) ex_stack=ex_node.prev;


#define throw(ex, ...) \
   ex_stack ? \
      ({st_drop();longjmp(ex_stack->jump, ex.ID);})\
   :  ({printf("\n\e[1mException:\e[0m "); printf(ex.fmt,##__VA_ARGS__); \
        st_push(throw(ex));\
        st_trace(); exit(ex.ID);})

#define assert(cond, ex, ...) if (!(cond)) throw(ex, ##__VA_ARGS__);
/*
try
{
   //code
}

except (EXCEPTION, ...)
{
   //code that runs if any of the specified exceptions is caught
}

except_any
{
   //code that will run on any exception
}

finally
{
   //code that will run after the try block regardless of exception
}
catch;
*/



#endif
