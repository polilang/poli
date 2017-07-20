/*
   Poli language - 2017, MIT license

   blocktree generator
*/


#ifndef POLI_BLOCKTREE
#define POLI_BLOCKTREE



#include <stdio.h>
#include <stdlib.h>

#include "string.c"
#include "types.c"
#include "gc.c"



struct bt_line
{
   str string;
   u32 index;
   u32 indent;
};

typedef
struct bt_node
{
   struct bt_node *next, *prev;

   enum { none,line,node } type;

   union
   {
      struct bt_line  line;
      struct bt_node* node;
   };

}
bt_node;

bt_node*
bt_new_node ()
{
   bt_node* node = alloc(sizeof(bt_node));
   node->next = NULL;
   node->prev = NULL;
   node->type = none;
//   node->line = NULL;
   return node;
}

u32
getIndent (str line)
{
   u32 indent = 0;
   while(line[indent] == ' ') indent++;
   return indent;
}


bt_node*
bt_mktree (str* strings, u32 *lastindent, u64 *curr_line)
{
   bt_node *first_node = NULL,
           *prev_node  = NULL,
           *curr_node  = NULL;


   while (strings[*curr_line])
   {
      if (strf.length(strings[*curr_line]) && strings[*curr_line][0] != '\n')
      {
         if (getIndent(strings[*curr_line]) == *lastindent)
         {
            // create new node
            curr_node = bt_new_node();

            // init node
            curr_node->type = line;
            curr_node->line = (struct bt_line) {strf.trim(strings[*curr_line]), *curr_line, *lastindent};

            // link nodes
            if (prev_node)
            {
               prev_node->next = curr_node;
               curr_node->prev = prev_node;
            }

            // move forward
            prev_node = curr_node;
            curr_node = curr_node->next;

            // have to keep track of first node for return
            if (!first_node) first_node = prev_node;
         }

         else if (getIndent(strings[*curr_line]) > *lastindent)
         {
            *lastindent = getIndent(strings[*curr_line]);

            // recurse
            curr_node = bt_mktree(strings, lastindent, curr_line);
            (*curr_line)--;

            // link nodes
            prev_node->next = curr_node;
            curr_node->prev = prev_node;

            // move forward
            prev_node = curr_node;
            curr_node = curr_node->next;
         }

         else if (getIndent(strings[*curr_line]) < *lastindent)
         {
            *lastindent = getIndent(strings[*curr_line]);
            return first_node;
         }
      }
      (*curr_line)++;
   }

   return first_node;
}


bt_node*
bt_new (str* strings)
{
   u32 last_indent = 0;
   u64 curr_line = 0;

   bt_node* tree = bt_mktree(strings, &last_indent, &curr_line);

   return tree;
}


void
bt_dumptree (bt_node* tree)
{
   if (tree->type == line)
      printf("% 5d : % 3d | %s\n", tree->line.index, tree->line.indent, tree->line.string);

   else if (tree->type == node)
      bt_dumptree(tree->node);

   if (tree->next)
      bt_dumptree(tree->next);
}



#endif
