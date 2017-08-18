// $Id: libfns.cpp,v 1.4 2015-07-03 14:46:41-07 - - $

#include "libfns.h"

//
// This algorithm would be more efficient with operators
// *=, /=2, and is_odd.  But we leave it here.
//

bigint pow (const bigint& base_arg, const bigint& exponent_arg) {

   bigint base (base_arg);
   bigint exponent (exponent_arg);

   static const bigint ZERO (0);
   static const bigint ONE (1);
   static const bigint TWO (2);

   //DEBUGF ('^', "base = " << base << ", exponent = " << exponent);

   if (base == ZERO)
   {
       return ZERO;
    }

   bigint result = ONE;

   if (exponent < ZERO) {

      base = ONE / base;
      exponent = exponent - exponent;
      //added extra line to algorithm here:
      result = base;
   }


   while (ZERO < exponent) {

        if (exponent % TWO == ONE)
        {
            result = result * base;
            exponent = exponent - ONE;
        }

       else
       {
         base = base * base;
         exponent = exponent / TWO;
       }

   }

   return result;
}
