/*
   evolbug - 2017, MIT license
   call stack traces
*/



#include <stdlib.h>  // exit
#include <stdio.h>   // printf



#define ST_STACK_SIZE 512



struct st_entry
{
   char file[256], call[384];
   unsigned line;
};

static struct
{
   struct st_entry entry[ST_STACK_SIZE];
   unsigned curr;
}
ST = {.curr=0};



static
int st_entry_equal (struct st_entry* a, struct st_entry* b)
{
   int f = 0, c = 0; // file/call readers

   if (a->line != b->line) return 0;
   while (a->file[f] || b->file[f]) if (a->file[f] != b->file[f]) return 0; else f++;
   while (a->call[c] || b->call[c]) if (a->call[c] != b->call[c]) return 0; else c++;

   return 1;
}


void st_trace ()
{
   printf("\n\033[1mTraceback (last call first):\033[0m\n");

   struct st_entry *entry;
   int skip = 0;

   for (int u = ST.curr; u >= 0; u--)
   {
      entry = ST.entry + u-1;

      if (u)
      {
         if (st_entry_equal(entry, ST.entry + u-2))
            skip++;

         else
            printf("   \033[1;31m%s\033[0m ", entry->call);

         if (skip && !st_entry_equal(entry, ST.entry + u-2))
         {
            printf("\033[1;34m[x%d]\033[0m", skip+1);
            skip = 0;
         }

         if (!skip)
            printf("\n      in %s:%d\n", entry->file, entry->line);
      }

      else
         printf("   \033[1;32mmain()\033[0m \n");
   }

}


void st_push (char *file, char *call, unsigned line)
{
   for (int i=0; file[i]; i++) ST.entry[ST.curr].file[i] = file[i];
   for (int i=0; call[i]; i++) ST.entry[ST.curr].call[i] = call[i];
   ST.entry[ST.curr].line = line;

   if (++ST.curr >= ST_STACK_SIZE)
   {
      printf("\n\033[1;31mSTRACE: Call stack overflow\033[0m");
      st_trace(); exit(1);
   }
}


// pop last call
void st_pop ()
{
   ST.curr--;
}


// drop the current call stack frame
void st_drop ()
{
   while (ST.entry[ST.curr - 1].file && ST.curr) ST.curr--;
}


// create a call stack frame
void st_push_frame ()
{
   ST.entry[ST.curr] = (struct st_entry){0};
   ST.curr++;
}
