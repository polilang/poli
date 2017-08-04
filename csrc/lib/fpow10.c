/*
   Poli language - 2017, MIT license

   fast 10 powers
*/



#ifndef POLI_FPOW10
#define POLI_FPOW10



#include "types.c" // datatypes


f64 __fpow__ [617],
   *__fpowc__ = __fpow__ + 309;

f64 __fpowprep__()
{
   *__fpowc__ = 1.0;

   for (i32 i = 1; i < 308; i++)
   {
      __fpowc__[-i] = __fpowc__[-i+1] / 10.0;
      __fpowc__[ i] = __fpowc__[ i-1] * 10.0;
   }

   return 0.0;
}

#define fpow10(n) \
(*__fpowc__ ? ((n<=308&&n>=-308) ? __fpowc__[n] : 1.0) : __fpowprep__() + __fpowc__[n])

#endif
