/*
   Poli language - 2017, MIT license

   syntax lexer
*/



#ifndef POLI_LEXER
#define POLI_LEXER



#include "lib/string.c"     // strf
#include "lib/types.c"      // datatypes



enum token_type
{
   tt_none,
   tt_indent,
   tt_dedent,
   tt_newline,
   tt_word,
   tt_number,
   tt_string,
   tt_symbol,
};


typedef
struct token
{
   enum token_type type;
   u32  line;
   str  from, to;

   struct token *next;
}
Token;



Token *
tk_new (enum token_type type, str from, str to, u32 line)
{
   Token *tk = alloc(sizeof(Token));

   tk->line = line;
   tk->from = from;
   tk->to   = to;
   tk->type = type;

   return tk;
}


Token *
tk_symbol (str *char_curr, u32 line)
{
   return tk_new(tt_symbol, *char_curr, *char_curr + 1, line);
}


Token *
tk_number (str *char_curr, u32 line)
{
   str char_start = *char_curr;

   while (chr_isdigit(**char_curr))
      (*char_curr)++;

   (*char_curr)--;

   return tk_new(tt_number, char_start, *char_curr+1, line);
}


i32 chr_escaped (str chr)
{
   return (*(chr - 2) != '\\') && (*(chr - 1) == '\\');
}

Token *
tk_string (str *char_curr, u32 line, str str_end)
{
   str char_start = (*char_curr);

   while (  ((**char_curr != '\n') && str_comparen(*char_curr, str_end, str_length(str_end)))
         || (  chr_escaped(*char_curr)
            &&!str_comparen(*char_curr, str_end, str_length(str_end)))
         )
      ++(*char_curr);

   *char_curr += str_length(str_end)-1;

   return tk_new(tt_string, char_start, *char_curr, line);
}



Token *
tk_word (str *char_curr, u32 line)
{

   str char_start = *char_curr;

   while (chr_isalnum(**char_curr))
      (*char_curr)++;

   (*char_curr)--;

   return tk_new(tt_word, char_start, *char_curr+1, line);
}


Token *
tk_newline (str *char_curr, u32 line)
{
   return tk_new(tt_newline, *char_curr, *char_curr + 1, line);
}

Token *
tk_indent (u32 line)
{
   return tk_new(tt_indent, 0,0, line);
}

Token *
tk_dedent (u32 line)
{
   return tk_new(tt_dedent, 0,0, line);
}


i32
tk_strstart (str *char_curr, str *sdelims)
{
   for (i32 i = 0; sdelims[i]; i+=2)

      if (!str_comparen(*char_curr, sdelims[i], str_length(sdelims[i])))
      {
         *char_curr = *char_curr + str_length(sdelims[i]);
         return i+1;
      }

   return -1;
}


Token *
tk_get (str *char_curr, u32 line, str *sdelims)
{
   Token *tk = NULL;
   i32 sdelim_end = -1;

   while (**char_curr && !tk)
   {
      if (chr_isdigit(**char_curr))
         tk = tk_number(char_curr, line);

      else if (chr_isalpha(**char_curr))
         tk = tk_word(char_curr, line);

      else if ((sdelim_end = tk_strstart(char_curr, sdelims)) != -1)
         tk = tk_string(char_curr, line, sdelims[sdelim_end]);

      else if ((**char_curr == '\n'))
         tk = tk_newline(char_curr, line);

      else
         tk = tk_symbol(char_curr, line);
   }

   do
      (*char_curr)++;
   while (**char_curr == ' ' || **char_curr == '\t');

   return tk;
}


Token *
tokenize_line (str string, u32 line, str *sdelims)
{
   Token *tk_first = NULL,
         *tk_curr  = NULL,
         *tk_prev  = NULL;

   str char_curr = string;

   while (*char_curr)
   {
      tk_curr = tk_get(&char_curr, line, sdelims);

      if (tk_curr)
      {
         if (tk_prev) tk_prev->next = tk_curr;

         tk_prev = tk_curr;

         if (!tk_first) tk_first = tk_curr;
      }
   }

   tk_curr->next = NULL;

   return tk_first;
}

u32
get_indent (str line)
{
   u32 indent = 0;
   while(line[indent] == ' ') indent++;
   return indent;
}


Token *
tk_link (Token *prev, Token *curr)
{
   if (prev) prev->next = curr;
   return curr;
}

void
rm_comments (str *source, str comments)
{
   str found;
   str* comment = str_split(comments, " ");

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


Token *
tokenize (str *source, str comments, str sdelim)
{
   Token *tk_first = NULL,
         *tk_curr  = NULL,
         *tk_prev  = NULL;

   u32 indent = 0;
   str* sdelims = str_split(sdelim, " "); // string start/end delimiter pairs

   rm_comments(source, comments);

   for (u32 line = 0; source[line]; line++)
   {
      if (indent != get_indent(source[line]) && *(source[line]+get_indent(source[line])+1))
      {
         if (indent < get_indent(source[line]))
            tk_curr = tk_indent(line);

         else
            tk_curr = tk_dedent(line);

         tk_prev = tk_link(tk_prev, tk_curr);

         indent = get_indent(source[line]);
      }

      source[line] = source[line] + get_indent(source[line]);

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



str
ttype (enum token_type t)
{
   switch (t)
   {
      case tt_none   : return "0";
      case tt_indent : return "i";
      case tt_dedent : return "d";
      case tt_newline: return "|";
      case tt_word   : return "w";
      case tt_number : return "n";
      case tt_string : return "s";
      case tt_symbol : return "$";

      default        : return "?";
   }
}



#endif
