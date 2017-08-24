/*
   Poli language - 2017, MIT license

   token stream parser
*/



#include <stdio.h>

#include "lexer.c"

#include "lib/string.c"     // strf
#include "lib/types.c"      // datatypes
#include "lib/readfile.c"   // readfile
#include "lib/strace.c"     // st_assert, st_c, st_v
#include "lib/gc.c"



enum node_type
{
   tn_none,
   tn_decl,
   tn_func,
   tn_expr,
   tn_cast,
   tn_while,
   tn_plist,
   tn_alist,
};


typedef struct Node
{
   enum node_type type;
   struct Node *next;
}
Node;

struct Declare
{
   Node HEAD;
   Token *type, *ident;
};

struct ParamList
{
   Node HEAD;
   Token *type, *ident;
   struct ParamList *next;
};

struct ArgumList
{
   Node HEAD;
   Token *type, *ident;
   struct ArgumList *next;
};

struct Function
{
   Node HEAD;
   Token *type, *ident;
   struct ParamList params;
   Node *body;
};

struct Expression
{
   Node HEAD;
   Node *left, *right;
   Token *op;
};

struct While
{
   Node HEAD;
   struct Expression *cond;
   Node *body;
};

struct AST
{
   Node *HEAD, *TAIL;
};



void putNode(struct AST *tree, Node *node)
{
   if (tree->TAIL)
   {
      tree->TAIL->next = node;
   }

   tree->TAIL = node;

   if (!tree->HEAD) tree->HEAD = tree->TAIL;
}




Token* tk_eat (Token **curr)
{
   Token *tk = *curr;
   *curr = (*curr)->next;
   return tk;
}

Token* tk_peek (Token *curr, u32 offset)
{
   return offset<=0 ? curr : tk_peek(curr->next, offset-1);
}

u32 tk_match (Token **curr, u32 count, ...)
{
   va_list args; va_start(args, count);
   Token *walker = *curr;

   for (u32 i=0; i<count; i++)
   {
      Token arg = va_arg(args, Token);

      if (str_equal(arg.lexeme, walker->lexeme) || (arg.type == walker->type))
         walker = walker->next;
      else
         return 0;
   }

   while (walker != *curr) *curr = (*curr)->next;
   return 1;
}


void ts_push (Token **stack, i16 *index, Token *token)
{
   stack[++(*index)] = token;
}

Token* ts_pop (Token **stack, i16 *index)
{
   Token *tk = stack[*index];
   stack[*index] = 0;
   (*index)--;
   return tk;
}

Token* ts_peek (Token **stack, i16 index)
{
   return stack[index];
}


struct AST *
parse (Token *stream, str keywords)
{
   struct AST *tree = alloc(sizeof(struct AST));
   tree->HEAD = 0; tree->TAIL = 0;

   Token *stack[256] = {0};
   i16 index = -1;

   while (stream->next)
   {
      printf("\e[30;1m%s\e[0m\e[1m%s\e[0m", ttype(stream->type), stream->lexeme?stream->lexeme:"");

      switch (stream->type)
      {/*
         case tt_word:
         {
            if (!str_pfind(keywords, stream->lexeme))
               ts_push(stack, &index, stream);


            stream = stream->next;
         }
         break;

         case tt_newline:
         {
            if (index>=0 && ts_peek(stack, index)->type == tt_word)
            {
               Token *temp = ts_pop(stack, &index);

               if (index>=0 && ts_peek(stack, index)->type == tt_word) // declaration
               {
                  struct Declare *node = alloc(sizeof(struct Declare));
                  ((Node*)node)->type  = tn_decl;
                  node->ident          = temp;
                  node->type           = ts_pop(stack, &index);
                  putNode(tree, (Node*)node);
               }

            }
            stream = stream->next;
         }
         break;

         case tt_symbol:
         {
            if (tk_match(&stream, 1, (Token){.lexeme="{"})) // function
            {

            }

            else
               stream = stream->next;

         }
         break;
*/
         default:
            stream = stream->next;

      }
//      stream = stream->next;


   }
}

void astdump (struct AST *tree)
{
   if (tree && tree->HEAD)
   {
      Node* walker = tree->HEAD;

      while (walker != tree->TAIL)
      {
         printf("%d", walker->type);
         walker = walker->next;
      }
   }
}

i32
main ()
{
   atexit(gm_purge);

   str *source = st_c(str*, readfile("../examples/basics.pi"));

   Token *stream = tokenize(source, "# ### ###", "\" \" ' '");

   struct AST *tree = parse(stream, "while for return from with");

   astdump(tree);
}
