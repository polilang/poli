/*
   Poli language - 2017, MIT license
   Tokenizer
*/



#include "tokenizer.h"

#include "compiler/context/context.h"
#include "lib/vector/vector.h"
#include "lib/string/string.h"
#include "lib/strace/strace.h"


unsigned get_indent (char *line)
{
   unsigned indent = 0;
   while (line[indent] == ' ') indent++;
   return indent;
}

unsigned line_end (char *line)
{
   return !line[0] || (line[0] == '\n') || (line[0] == '\r' && line[1] == '\n');
}


void tokenize (struct Compiler *c)
#define tokenize(...) st_call(tokenize(__VA_ARGS__))
{
   unsigned indent_stack[256] = {0}, *indent = (unsigned*)indent_stack + 1;
   c->tokens = v_new();


   for (c->line = 0; c->source[c->line]; c->line++)
   {
      c->curr_char = c->source[c->line];
      indent[0] = get_indent(c->source[c->line]);

      if (indent[0] != indent[-1] && !line_end(c->source[c->line] + indent[0]))
      {
         if (indent[0] > indent[-1])
         {
            v_push(c->tokens, Indent(c));
            indent++;
         }

         else
         {
            while (indent[-1] != get_indent(c->source[c->line]) && indent != indent_stack)
            {
               v_push(c->tokens, Dedent(c));
               indent--;
            }
         }

         indent[0] = get_indent(c->source[c->line]);
      }

      c->source[c->line] += indent[0];


      while (c->curr_char[0])
      {
         if (chr_isdigit(c->curr_char[0]))
            v_push(c->tokens, Number(c));

         else
         if (chr_isalpha(c->curr_char[0]))
            v_push(c->tokens, Word(c));

         else
         if (c->curr_char[0] == '"' || c->curr_char[0] == '\'')
            v_push(c->tokens, String(c));

         else
         if (line_end(c->curr_char))
            v_push(c->tokens, Newline(c));

         else
         if (chr_iswhite(c->curr_char[0]))
            c->curr_char++;

         else
            v_push(c->tokens, Symbol(c));
      }
   }
}



Token token (Compiler *c, TokenType type, unsigned l)
#define token(...) st_call(token(__VA_ARGS__))
{
   Token t =
   {
      type,
      {
         c->line,
         c->curr_char, c->curr_char + l,
         str_slicel(c->curr_char, l)
      }
   };

   c->curr_char += l;

   return t;
}



Token String (Compiler *c)
#define String(...) st_call(String(__VA_ARGS__))
{
   char *start = ++c->curr_char, *end = start;

   while (  (!line_end(end) && end[0] != start[-1]) // newline or end of string
         || ( (end[-2] != '\\') && (end[-1] == '\\') && (end[0] == start[-1]) ) // escaped quote
         )
      end++;

   return token(c, STRING, end-start);
}


Token Number (Compiler *c)
#define Number(...) st_call(Number(__VA_ARGS__))
{
   char *start = c->curr_char, *end = start;
   while (chr_isdigit(*end)) end++;
   return token(c, NUMBER, end-start);
}


Token Word (Compiler *c)
#define Word(...) st_call(Word(__VA_ARGS__))
{
   char *start = c->curr_char, *end = start;
   while (chr_isalnum(*end)) end++;
   return token(c, WORD, end-start);
}


Token Symbol (Compiler *c)
#define Symbol(...) st_call(Symbol(__VA_ARGS__))
{
   return token(c, SYMBOL, 1);
}


Token Newline (Compiler *c)
#define Newline(...) st_call(Newline(__VA_ARGS__))
{
   if (c->curr_char[0] == '\r') // crlf - win32
      return token(c, NEWLINE, 2);
   return token(c, NEWLINE, 1); // lf - unix
}


Token Indent (Compiler *c)
#define Indent(...) st_call(Indent(__VA_ARGS__))
{
   return token(c, INDENT, 1);
}


Token Dedent (Compiler *c)
#define Dedent(...) st_call(Dedent(__VA_ARGS__))
{
   return token(c, DEDENT, 1);
}
