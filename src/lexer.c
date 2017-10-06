/*
   Poli language - 2017, MIT license

   syntax lexer
*/



#include <stdio.h>
#include <stdarg.h>

#include "lib/string/string.h"
#include "lib/garbage/gm.h"
#include "lib/vector/vector.h"


struct Compiler
{
   char **source;

   unsigned curr_line;
   unsigned curr_char;

   struct
   {
      vector(char*) string;
      vector(char*) comment;
   }
   delimiter;

   vector(struct Token) tokens;
   vector(struct Token) symbols;
   vector(struct Node) tree;
};


typedef
enum token_type
{
   NONE,

   INDENT,
   DEDENT,
   NEWLINE,

   WORD,

   NUMBER,
   STRING,
   SYMBOL,
}
TokenType;


typedef
struct Lexeme
{
   unsigned line;
   char *start, *end, *string;
}
Lexeme;


typedef
struct Token
{
   TokenType type;
   Lexeme lexeme;
}
Token;



Token tk_new (TokenType type, char *from, char *to, unsigned line)
{
   return (Token){ type, {line, from, to, str_slicep(from, to)} };
}


Token Symbol (char **char_curr, unsigned line)
{
   return tk_new(SYMBOL, *char_curr, *char_curr + 1, line);
}


Token Number (char **char_curr, unsigned line)
{
   char* char_start = *char_curr;

   while (chr_isdigit(**char_curr))
      (*char_curr)++;

   (*char_curr)--;

   return tk_new(NUMBER, char_start, *char_curr+1, line);
}


int chr_escaped (char *chr)
{
   return (*(chr - 2) != '\\') && (*(chr - 1) == '\\');
}

Token String (char **char_curr, unsigned line, char *str_end)
{
   char *char_start = (*char_curr);

   while (  ((**char_curr != '\n') && str_comparen(*char_curr, str_end, str_length(str_end)))
         || (  chr_escaped(*char_curr)
            &&!str_comparen(*char_curr, str_end, str_length(str_end)))
         )
      ++(*char_curr);

   *char_curr += str_length(str_end)-1;

   return tk_new(STRING, char_start, *char_curr, line);
}



Token Word (char **char_curr, unsigned line)
{
   char *char_start = *char_curr;

   while (chr_isalnum(**char_curr))
      (*char_curr)++;

   (*char_curr)--;

   return tk_new(WORD, char_start, *char_curr+1, line);
}


Token Newline (char **char_curr, unsigned line)
{
   return tk_new(NEWLINE, *char_curr, *char_curr + 1, line);
}

Token Indent (unsigned line)
{
   return tk_new(INDENT, 0,0, line);
}

Token Dedent (unsigned line)
{
   return tk_new(DEDENT, 0,0, line);
}


int tk_strstart (char **char_curr, char **sdelims)
{
   for (int i = 0; sdelims[i]; i += 2)

      if (!str_comparen(*char_curr, sdelims[i], str_length(sdelims[i])))
      {
         *char_curr = *char_curr + str_length(sdelims[i]);
         return i+1;
      }

   return -1;
}


Token tk_get (struct Compiler *context)
{
   Token tk = {0};
   int sdelim_end = -1;

   while (**char_curr && tk.type == NONE)
   {
      if (chr_isdigit(context->curr_char))
         tk = Number(char_curr, line);

      else
      if (chr_isalpha(**char_curr))
         tk = Word(char_curr, line);

      else
      if ((sdelim_end = tk_strstart(char_curr, sdelims)) != -1)
         tk = String(char_curr, line, sdelims[sdelim_end]);

      else
      if ((**char_curr == '\n'))
         tk = Newline(char_curr, line);

      else
         tk = Symbol(char_curr, line);
   }

   do
      (*char_curr)++;
   while (**char_curr == ' ' || **char_curr == '\t');

   return tk;
}


vector(Token) tokenize_line (struct Compiler *context)
{
   vector(Token) line = v_new();

   while (context->curr_char)
   {
      v_push(line, tk_get(context));
   }

   return line;
}

unsigned get_indent (char* line)
{
   unsigned indent = 0;
   while(line[indent] == ' ') indent++;
   return indent;
}


void rm_comments (struct Compiler *context)
{
   char *found;
   char **comment = str_split(comments, " ");

   while (*source)
   {
      if ((found = str_pfind(*source, comment[1]))) // multiline
      {
         if (found && (found > str_plast(*source, "\"") || found < str_pfind(*source, "\"")))
         {
            *found = '\n';
            *(found + 1) = 0;

            source++;

            while (*(source+1) && !(found = str_pfind(*source, comment[2]))) // end of multiline
            {
               **source = '\n';
               *(*source + 1) = 0;
               source++;
            }

            **source = '\n';
            *(*source + 1) = 0;
         }
      }

      else if ((found = str_pfind(*source, comment[0]))) // single line
      {
         if (found > str_plast(*source, "\"") || found < str_pfind(*source, "\""))
         {
            *found = '\n';
            *(found + 1) = 0;
         }
      }

      source++;
   }
}


unsigned line_end (char *line)
{
   return !*line || (*line == '\n') || (*line == '\r' && line[1] == '\n');
}


vector(Token) tokenize (struct Compiler *context)
{
   context->tokens = v_new();

   unsigned
      indents[256] = {0},
      *curr_indent = indents + 1;

   char **sdelims = str_split(context->delimiter->string, " "); // string start/end delimiter pairs


   rm_comments(context);


   for (unsigned line = 0; source[line]; line++)
   {
      *curr_indent = get_indent(source[line]);

      if (*curr_indent != curr_indent[-1] && !line_end(source[line] + *curr_indent))
      {
         if (*curr_indent > curr_indent[-1])
         {
            tk_curr = tk_indent(line);
            curr_indent = curr_indent+1;
            tk_prev = tk_link(tk_prev, tk_curr);
         }

         else
         {
            while (curr_indent[-1] != get_indent(source[line]) && curr_indent != indents)
            {
               tk_curr = tk_dedent(line);
               curr_indent = curr_indent-1;
               tk_prev = tk_link(tk_prev, tk_curr);
            }
         }

         *curr_indent = get_indent(source[line]);
      }

      source[line] = source[line] + *curr_indent;

      if((tk_curr = tokenize_line(source[line], line, sdelims)))
      {
         if (!tk_first) tk_first = tk_curr; // keep the token which starts the stream

         if (tk_prev) tk_prev->next = tk_curr; // link any previous tokens to new chunk

         while (tk_curr->next)
            tk_curr = tk_curr->next; // catch up to the last element

         tk_prev = tk_curr; // store the last token
      }
   }


   tk_curr->next = NULL;


   return tk_first;
}



char* ttype (enum token_type t)
{
   switch (t)
   {
      case NONE   : return "0";
      case INDENT : return "i";
      case DEDENT : return "d";
      case NEWLINE: return "|";
      case WORD   : return "w";
      case NUMBER : return "n";
      case STRING : return "s";
      case SYMBOL : return "$";

      default        : return "?";
   }
}
/*
Token* tk_eat (Token **stream)
{
   Token *tk = *stream;
   *stream = (*stream)->next;
   return tk;
}

int tk_check (Token *stream, char count, ...)
{
   va_list args; va_start(args, count);

   for (int i = 0; i < count; i++)
   {
      Token tk = va_arg(args, Token);

      if (!stream || (  (tk.type != stream->type) && !str_equal(tk.lexeme, stream->lexeme)))
      {
         va_end(args);
         return 0;
      }

      stream = stream->next;
   }

   va_end(args);
   return 1;
}

int tk_match (Token **stream, char count, ...)
{
   va_list args; va_start(args, count);
   Token *walker = *stream;

   for (int i = 0; i < count; i++)
   {
      Token tk = va_arg(args, Token);

      if (!stream || (  (tk.type != walker->type) && !str_equal(tk.lexeme, walker->lexeme)))
      {
         va_end(args);
         return 0;
      }

      walker = walker->next;
   }

   *stream = walker;
   va_end(args);
   return 1;
}


void tkdump (Token *tk)
{
   while (tk)
   {
      printf("\033[2m%s\033[0;1m%s\033[0m", ttype(tk->type), tk->lexeme?tk->lexeme:"");
      tk = tk->next;
   }
}
*/
