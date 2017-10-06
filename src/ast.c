#include <stdio.h>


typedef
struct
{
   str name;
   enum
   {
      a_token,
      a_rule,
      a_word,
      a_group
   }
}
Atom;


typedef
struct
{
   str name;
   vector(Atom) atoms;
}
Rule;
