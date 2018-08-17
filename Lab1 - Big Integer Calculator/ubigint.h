// $Id: ubigint.h,v 1.4 2017-07-06 23:27:29-07 - - $
//By: David Stewart (daastewa@ucsc.edu) 
//and Christopher Hahn (cnhahn@ucsc.edu)
#ifndef __UBIGINT_H__
#define __UBIGINT_H__

#include <exception>
#include <iostream>
#include <limits>
#include <utility>
#include <vector>
using namespace std;

#include "debug.h"
#include "relops.h"

struct quo_rem;

class ubigint {
   friend ostream& operator<< (ostream&, const ubigint&);
   private:
      using udigit_t = unsigned char;
      using ubigvalue_t = vector<udigit_t>;
      ubigvalue_t ubig_value;

   public:
      void multiply_by_2();
      void divide_by_2();
      quo_rem udivide (const ubigint dividend, ubigint divisor) const;

      ubigint() = default; // Need default ctor as well.
      ubigint (unsigned long);
      ubigint (const string&);

      ubigint operator+ (const ubigint&) const;
      ubigint operator- (const ubigint&) const;
      ubigint operator* (const ubigint&) const;
      ubigint operator/ (const ubigint&) const;
      ubigint operator% (const ubigint&) const;

      bool operator== (const ubigint&) const;
      bool operator<  (const ubigint&) const;
};

#endif
