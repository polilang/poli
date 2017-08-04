// stack traces
#ifndef POLI_STRACE
#define POLI_STRACE


#include <stdlib.h>  // exit
#include <stdio.h>   // printf


#define ST_STACK_SIZE 512

struct st_entry
{
   char file[64], call[64];
   unsigned int line;
};

struct {struct st_entry entry[ST_STACK_SIZE]; unsigned int curr;} ST = {.curr=0};


#define st_push(fn)\
   ST.entry[ST.curr] = \
      (struct st_entry) {.file=__FILE__, .call=#fn, .line=__LINE__};\
   if (++ST.curr >= ST_STACK_SIZE) {puts("\n\e[31mError: Call stack overflow\e[0m"); st_trace(); exit(1);}


#define st_assert(cond, fmt, ...) \
   ({if (!(cond)){\
      st_push(st_assert(cond, msg));\
      printf("\n"fmt, ##__VA_ARGS__);\
      st_trace(); exit(1);}\
   cond;})


int st_entry_equal (struct st_entry* a, struct st_entry* b)
{
   int f = 0, c = 0; // file/call readers

   if (a->line != b->line) return 0;
   while (a->file[f]!=0 || b->file[f]!=0) if (a->file[f] != b->file[f]) return 0; else f++;
   while (a->call[c]!=0 || b->call[c]!=0) if (a->call[c] != b->call[c]) return 0; else c++;

   return 1;
}



// typed call
#define st_c(rtype, fn) ({ st_push(fn); rtype r = fn; ST.curr--; r;})

// void call
#define st_v(fn) ({ st_push(fn); fn; ST.curr--;})



void
st_trace ()
{
   printf("\nTraceback (last call first):\n");

   struct st_entry *entry;
   int skip = 0;

   for (int u = ST.curr-1; u >= 0; u--)
   {
      entry = &ST.entry[u-1];

      if (skip)
      {
         if (!st_entry_equal(entry, &ST.entry[u]))
         {
            printf(" %d recursive entries ...\e[0m\n\n", skip);
            skip = 0;
         }

         else
            skip++;
      }

      if (u)
      {
         if (!skip)
         {
            printf("   \e[31;1m%s\e[0m \n      in %s:%d\n\n", entry->call, entry->file, entry->line);

            if (st_entry_equal(entry, &ST.entry[u]))
            {
               printf("   \033[34;1m%s", "...");
               skip++;
            }
         }
      }

      else
         printf("   \e[32;1mmain()\e[0m \n      in %s\n", __BASE_FILE__);
   }

}

#endif
