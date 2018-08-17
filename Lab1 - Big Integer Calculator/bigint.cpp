// $Id: bigint.cpp,v 1.4 2017-07-06 23:27:29-07 - - $
//By: David Stewart (daastewa@ucsc.edu) 
//and Christopher Hahn (cnhahn@ucsc.edu)

#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
using namespace std;

#include "bigint.h"
#include "debug.h"
#include "relops.h"

bigint::bigint (long that): uvalue (that), is_negative (that < 0) {
   DEBUGF ('~', this << " -> " << uvalue)
}

bigint::bigint (const ubigint& uvalue, bool is_negative):
   is_negative(is_negative) {
}

bigint::bigint (const string& that) {
   this->is_negative = that.size() > 0 and that[0] == '_';
   uvalue = ubigint (that.substr (is_negative ? 1 : 0));
}

bigint bigint::operator+ () const {
   return *this;
}

bigint bigint::operator- () const {
   return {uvalue, not is_negative};
}

bigint bigint::operator+ (const bigint& that) const {
   bigint result;
   if ((that.is_negative == true && this->is_negative == true)
    || (that.is_negative == false && this->is_negative == false))
   {
      result.uvalue = uvalue + that.uvalue;
      result.is_negative = this->is_negative;
   }
   else
   {
      if (this->uvalue < that.uvalue)
      {
         result.uvalue = that.uvalue - this->uvalue;
         result.is_negative = that.is_negative;
      }
      else if (this->uvalue > that.uvalue)
      {
         result.uvalue = this->uvalue - that.uvalue;
         result.is_negative = this->is_negative;
      }
      else if (this->uvalue == that.uvalue)
      {
         result.uvalue = that.uvalue - this->uvalue;
         result.is_negative = false;
      }
   }
   return result;
}

bigint bigint::operator- (const bigint& that) const {
   bigint result;
   if((that.is_negative == true && this->is_negative == true)
      || (that.is_negative == false && this->is_negative == false))
   {
      if (this->uvalue < that.uvalue)
      {
         result.uvalue = that.uvalue - this->uvalue;
         result.is_negative = !(that.is_negative);
      }
      else if (this->uvalue > that.uvalue)
      {
         result.uvalue = this->uvalue - that.uvalue;
         result.is_negative = this->is_negative;
      }
      else if (this->uvalue == that.uvalue)
      {
         result.uvalue = that.uvalue - this->uvalue;
         result.is_negative = false;
      }
   }
   else
   {
      result.uvalue = uvalue + that.uvalue;
      result.is_negative = this->is_negative;
   }
   return result;
}

bigint bigint::operator* (const bigint& that) const {
   bigint result;
   result.uvalue = this->uvalue * that.uvalue;
   if((that.is_negative == true && this->is_negative == true)
      || (that.is_negative == false && this->is_negative == false))
   {
      result.is_negative = false;
   }
   else
   {
      result.is_negative = true;
   }
   return result;
}

bigint bigint::operator/ (const bigint& that) const {
   bigint result;
   result.uvalue = uvalue / that.uvalue;
   static const bigint ZERO (0);
   if(that == ZERO)
   {
      cout << "ydc: divide by zero" << endl;
   }

   if (result == ZERO)
   {
      result.is_negative = false;
   }
   else if ((that.is_negative == true && this->is_negative == true)
      || (that.is_negative == false && this->is_negative == false))
   {
      result.is_negative = false;
   }
   else
   {
      result.is_negative = true;
   }
   return result;
}

bigint bigint::operator% (const bigint& that) const {
   bigint result;
   result.uvalue = this->uvalue % that.uvalue;
   static const bigint ZERO (0);
   if(that == ZERO)
   {
      cout << "ydc: divide by zero" << endl;
   }

   if(result == ZERO){
      result.is_negative = false;
   }
   else if (that.is_negative == true && 
      this->is_negative == true)
   {
      result.is_negative = true;
   }
   else if(that.is_negative == false && 
      this->is_negative == false)
   {
      result.is_negative = false;
   }
   else if(that.is_negative == false && 
      this->is_negative == true)
   {
      result.is_negative = true;
   }
   else
   {
      result.is_negative = false;
   }
   return result;
}

bool bigint::operator== (const bigint& that) const {
   return is_negative == that.is_negative and uvalue == that.uvalue;
}

bool bigint::operator< (const bigint& that) const {
   if (is_negative != that.is_negative) return is_negative;
      return is_negative ? !(uvalue < that.uvalue)
                   : uvalue < that.uvalue;
}

ostream& operator<< (ostream& out, const bigint& that) {
   return out << (that.is_negative ? "-" : "") << that.uvalue;
}
