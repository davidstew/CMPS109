// $Id: bigint.cpp,v 1.76 2016-06-14 16:34:24-07 - - $

#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
using namespace std;

#include "bigint.h"
#include "debug.h"
#include "relops.h"

bigint::bigint (long that): uvalue (that), is_negative (that < 0) {

   //DEBUGF ('~', this << " -> " << uvalue)

}

bigint::bigint (const ubigint& uvalue, bool is_negative):
                is_negative(is_negative) {

                //cout << "here 2!!!" << endl;
}

bigint::bigint (const string& that) {

   //cout << "constructor!" << endl;

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

    if ((that.is_negative == true && this->is_negative == true) || (that.is_negative == false && this->is_negative == false))
    {
        //cout << "same signs" << endl;
        result.uvalue = uvalue + that.uvalue;
        result.is_negative = this->is_negative;
    }

    else
    {
       //cout << " opposite signs " << endl;

        if (this->uvalue < that.uvalue)
        {
         result.uvalue = that.uvalue - this->uvalue;
        result.is_negative = that.is_negative; //might need to be switched
        }

        else if (this->uvalue > that.uvalue)
        {
        result.uvalue = this->uvalue - that.uvalue;
        result.is_negative = this->is_negative; //might need to be switched
        }

        else if (this->uvalue == that.uvalue)
        {
            result.uvalue = that.uvalue - this->uvalue;
            result.is_negative = false;
        }

    }


   // cout << "sign: " << result.is_negative << endl;

    //cout << result;


   return result;
}

bigint bigint::operator- (const bigint& that) const {

     bigint result;

     //cout << "BIGINT this:" << this->uvalue << endl;
     //cout << "BIGINT that:" << that.uvalue << endl;

    if ((that.is_negative == true && this->is_negative == true) || (that.is_negative == false && this->is_negative == false))
    {
        //cout << "same sign " << endl;

        if (this->uvalue < that.uvalue)
        {
        result.uvalue = that.uvalue - this->uvalue;
        result.is_negative = !(that.is_negative); //might need to be switched
        }

        else if (this->uvalue > that.uvalue)
        {
        result.uvalue = this->uvalue - that.uvalue;
        result.is_negative = this->is_negative; //might need to be switched
        }

        else if (this->uvalue == that.uvalue)
        {
            result.uvalue = that.uvalue - this->uvalue;
            result.is_negative = false;
        }
    }

    else
    {
        //cout << " opposite signs " << endl;
        result.uvalue = uvalue + that.uvalue;
        result.is_negative = this->is_negative;
    }


    //cout << "sign: " << result.is_negative << endl;

    //cout << result;


   return result;
}

bigint bigint::operator* (const bigint& that) const {

   bigint result;
   result.uvalue = this->uvalue * that.uvalue;

    if ((that.is_negative == true && this->is_negative == true) || (that.is_negative == false && this->is_negative == false))
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

    if (result == ZERO)
    {
        result.is_negative = false;
    }

    else if ((that.is_negative == true && this->is_negative == true) || (that.is_negative == false && this->is_negative == false))
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

   if (that.is_negative == true && this->is_negative == true)
   result.is_negative = true;

    else if (that.is_negative == false && this->is_negative == false)
    result.is_negative = false;

    else if (that.is_negative == false && this->is_negative == true)
   result.is_negative = true;

    else
    result.is_negative = false;

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
