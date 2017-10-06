/*
   evolbug - 2017, MIT license
   fast 10 powers
*/



#ifndef EVOLBUG_FPOW10
#define EVOLBUG_FPOW10



#include "fpow10.h"



double
   fpow [617],
   *fpowc = 0;


void fpow_generate()
{
   fpowc = fpow + 309; // center of power list
   *fpowc = 1.0;

   for (int i = 1; i < 308; i++)
   {
      fpowc[-i] = (double)fpowc[-i+1] / (double)10.0;
      fpowc[ i] = (double)fpowc[ i-1] * (double)10.0;
   }
}


double fpow10 (short n)
{
   if (!fpowc) fpow_generate();
   return fpow[n];
}


#endif
