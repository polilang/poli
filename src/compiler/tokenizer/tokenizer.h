/*
   Poli language - 2017, MIT license
   Tokenizer
*/



#ifndef POLI_TOKENIZER_H
#define POLI_TOKENIZER_H



#include "compiler/context/context.h"
#include "lib/vector/vector.h"



typedef
enum TokenType
{
   NONE,
   INDENT,
   DEDENT,
   NEWLINE,

   STRING,
   NUMBER,

   WORD,

   SYMBOL,
}
TokenType;



typedef
struct Lexeme
{
   unsigned line;
   char
      *start,
      *end,
      *string;
}
Lexeme;


typedef
struct Token
{
   TokenType type;
   Lexeme lexeme;
}
Token;



Token String   (Compiler *c);
Token Number   (Compiler *c);
Token Word     (Compiler *c);
Token Symbol   (Compiler *c);
Token Newline  (Compiler *c);
Token Indent   (Compiler *c);
Token Dedent   (Compiler *c);



void tokenize (Compiler *c);



#endif
