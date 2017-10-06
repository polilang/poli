/*
   evolbug - 2017, MIT license
   call stack traces
*/



#ifndef EVOLBUG_STRACE_H
#define EVOLBUG_STRACE_H



#undef st_trace
#undef st_push
#undef st_pop
#undef st_push_frame
#undef st_drop
#undef st_call



void st_trace ();

void st_push (char *file, char *call, unsigned line);
void st_pop  ();

void st_push_frame ();
void st_drop ();

#define st_push(fn) st_push(__FILE__, #fn, __LINE__)


/*
function call tracker macro
suggested use:

   type foo (args...)
   #define foo(...) st_call(foo(__VA_ARGS__))
   {
      body...
   }
*/
#define st_call(fn) ({ st_push(fn); __typeof__(fn) r = fn; st_pop(); r;})



#endif
