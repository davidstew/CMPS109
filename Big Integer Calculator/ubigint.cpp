// $Id: ubigint.cpp,v 1.14 2016-06-23 17:21:26-07 - - $

#include <cctype>
#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
#include <vector>
#include <cmath>
#include <sstream>
//#include<algorithm>
using namespace std;

#include "ubigint.h"
#include "debug.h"

ubigint::ubigint (unsigned long that) {

    stringstream ss;
    string that_;

    ss << that;
    ss >> that_;

    for (char digit : that_) {

    ubigvalue_t::iterator it = ubig_value.begin();

    ubig_value.insert(it, digit);

    if (not isdigit (digit)) {
         throw invalid_argument ("ubigint::ubigint(" + that_ + ")");
      }
//   DEBUGF ('~', this << " -> " << uvalue)

    //cout << "This: " << *this << endl;

    }
}

ubigint::ubigint (const string& that)  {

    //cout << " U constructor 2!" << endl;

    //DEBUGF ('~', "that = \"" << that << "\"");

    for (char digit : that) {

    ubigvalue_t::iterator it = ubig_value.begin();

    ubig_value.insert(it, digit);

    if (not isdigit (digit)) {
         throw invalid_argument ("ubigint::ubigint(" + that + ")");
      }
//      uvalue = uvalue * 10 + digit - '0';
//      uvalue = uvalue * 10 + digit - '0';
   }
/*
    for (unsigned int num = 0; num < ubig_value.size(); num++)
    {
        cout << ubig_value[num];
    }
*/
}

ubigint ubigint::operator+(const ubigint& that) const {


    //cout << "invoked!" << endl;

    ubigint Big_Int;

    int loop_length;
    int longer_number;
    int carry = 0;
    string number = "";
    int remainder;
    int op1, op2;

    //cout << "size of this " << this->ubig_value.size() << endl;

    //cout << "size of that " << that.ubig_value.size() << endl;

    if (this->ubig_value.size() > that.ubig_value.size())
    {
        longer_number = this->ubig_value.size();
        loop_length = that.ubig_value.size();
        number = "this";
    }

    else if(this->ubig_value.size() < that.ubig_value.size())
    {
        longer_number = that.ubig_value.size();
        loop_length = this->ubig_value.size();
        number = "that";
    }

    else
    {
        loop_length = this->ubig_value.size();
        longer_number = that.ubig_value.size();
        number = "";
    }

    for (int num = 0; num < loop_length; num++)
    {
        op1 = this->ubig_value[num] - 48; //convert to unsigned_char
        op2 = that.ubig_value[num] - 48; //convert to unsigned_char

        int sum = op1 + op2 + carry;

       // cout << "sum: " << sum << endl;

        remainder = sum % 10;

        unsigned char char_remainder = remainder + '0';

        if (sum >= 10)
        {
            carry = 1;
            Big_Int.ubig_value.push_back(char_remainder);
            //Big_Int.ubig_value.insert(it, char_remainder);
        }

        else
        {
            carry = 0;
            Big_Int.ubig_value.push_back(char_remainder);
            //Big_Int.ubig_value.insert(it, char_remainder);
        }
    }

    /*
     cout << "sum after first loop: " << endl;

    for (int num = 0; num <  Big_Int.ubig_value.size(); num++)
        cout << Big_Int.ubig_value[num];
    */

    if (number!= "")
    {
        if (number == "this")
        {
            //cout << "this" << endl;

            for (int num = loop_length; num < longer_number; ++num) {

                //cout << "num is: " << num << endl;

                op1 = this->ubig_value[num] - 48;

                 //cout << "op1: " << op1 << endl;
                 //cout << "carry: " << carry << endl;

                int sum = op1 + carry;

                //cout << "sum: " << sum << endl;

                remainder = sum % 10;

                unsigned char char_remainder = remainder + '0';

                if (sum >= 10)
                {
                    carry = 1;
                    Big_Int.ubig_value.push_back(char_remainder);
                    //Big_Int.ubig_value.insert(it, char_remainder);
                }

                else
                {
                    carry = 0;
                    Big_Int.ubig_value.push_back(char_remainder);
                    //Big_Int.ubig_value.insert(it, char_remainder);
                }
            }
        }

        else
        {
             //cout << "that" << endl;

              for (int num = loop_length; num < longer_number; ++num) {

                //cout << "inside" << endl;

                op1 = that.ubig_value[num] - 48;

                int sum = op1 + carry;

                remainder = sum % 10;

                unsigned char char_remainder = remainder + '0';

                if (sum >= 10)
                {
                    carry = 1;
                    Big_Int.ubig_value.push_back(char_remainder);
                    //Big_Int.ubig_value.insert(it, char_remainder);
                }

                else
                {
                    carry = 0;
                    Big_Int.ubig_value.push_back(char_remainder);
                    //Big_Int.ubig_value.insert(it, char_remainder);
                }
            }
        }
    }

    if (carry == 1)
    {
        unsigned char carry_ = carry + '0';
        Big_Int.ubig_value.push_back(carry_);
        //Big_Int.ubig_value.insert(it, carry_);
    }

    /*
    for (int num = 0; num <  Big_Int.ubig_value.size(); num++)
        cout << Big_Int.ubig_value[num];
    */
    return Big_Int;
}

ubigint ubigint::operator- (const ubigint& that) const {

   if (*this < that) throw domain_error ("ubigint::operator-(a<b)");


   ubigvalue_t this_vector = this->ubig_value;
   ubigvalue_t that_vector = that.ubig_value;
    /*
   cout << "this->:";

    for (int num = 0; num <  this_vector.size(); num++)
     {
         cout << this_vector[num];
     }

    cout << endl;

    cout <<  "that->:";

    for (int num = 0; num <  that_vector.size(); num++)
    {
         cout << that_vector[num];
    }
    */
    //cout << endl;

    ubigint Big_Int;

    int loop_length;
    int longer_number;
    string number = "";
    int op1, op2;
    int difference;
    unsigned char char_difference;

    //cout << "size of this " << this->ubig_value.size() << endl;

    //cout << "size of that " << that.ubig_value.size() << endl;

    if (this->ubig_value.size() > that.ubig_value.size())
    {
        unsigned char placeholder = 0 + '0';
        for (int num = that.ubig_value.size(); num <= this->ubig_value.size(); num++)
        that_vector.push_back(placeholder);
    }

    else if(this->ubig_value.size() < that.ubig_value.size())
    {
        unsigned char placeholder = 0 + '0';
        for (int num = this->ubig_value.size(); num <= that.ubig_value.size(); num++)
        this_vector.push_back(placeholder);
    }
    /*
     for (int num = 0; num <  this_vector.size(); num++)
     {
         cout << this_vector[num];
     }

     cout << endl;

     for (int num = 0; num <  this_vector.size(); num++)
     {
         cout << that_vector[num];
     }

     cout << endl;
    */
    for (int num = 0; num < this_vector.size(); num++)
    {
        op1 = this_vector[num] - 48; //convert to unsigned_char
        op2 = that_vector[num] - 48; //convert to unsigned_char

        //cout << op1 << " minus " << op2 << endl;

        if (op1 < op2)
        {
            if (this_vector[num + 1] == 0 and this_vector[num] == 0)
            this_vector[num + 1] = 9;

            op1 += 10;

            this_vector[num + 1] -= 1;

            //cout << "op1: " << op1 << "op2: " << op2 << endl;

            difference = op1 - op2;

            char_difference = difference + '0';

            //cout << "difference is: " << difference << endl;
        }

        else
        {
            difference = op1 - op2;

            char_difference = difference + '0';
        }

        Big_Int.ubig_value.push_back(char_difference);

    }
      for( int index = Big_Int.ubig_value.size() - 1;
         index >= 0; --index){
      if(Big_Int.ubig_value.size() == 1 or
         Big_Int.ubig_value[index-1] == '\0'){
         break;
      }
      if(Big_Int.ubig_value[index] == '0' and Big_Int.ubig_value[index-1] == '0'){
         Big_Int.ubig_value.pop_back();
      }else if(Big_Int.ubig_value[index] == '0' and Big_Int.ubig_value[index-1] != '0'){
         Big_Int.ubig_value.pop_back();
         break;
      }else{
         break;
      }

   }    //cout << Big_Int;

    return Big_Int;
}


ubigint ubigint::operator* (const ubigint& that) const {

    //cout << "inside mult!" << endl;

     ubigint Big_Int;
     vector<unsigned char> output(this->ubig_value.size() + that.ubig_value.size(), '0');

     Big_Int.ubig_value = output;
     int c, d;

    for (int num = 0; num < this->ubig_value.size(); num++) {

        c = 0;

        for (int index = 0; index < that.ubig_value.size(); index++) {

        //cout << "this->ubig_value[num]" << this->ubig_value[num] << endl;
        //cout << "that.ubig_value[index]" << that.ubig_value[index] << endl;

        //cout << "product " << (this->ubig_value[num] - 48) * (that.ubig_value[index] - 48) << endl;

         d = (Big_Int.ubig_value[num + index] - 48) + ((this->ubig_value[num] - 48) * (that.ubig_value[index] - 48)) + c;

         Big_Int.ubig_value[num + index] = ((d % 10) + '0');

         c = floor(d / 10);

        }

        Big_Int.ubig_value[num + that.ubig_value.size()] = c + '0';
    }

    for( int index = Big_Int.ubig_value.size() - 1;
         index >= 0; --index){
      if(Big_Int.ubig_value.size() == 1 or
         Big_Int.ubig_value[index-1] == '\0'){
         break;
      }
      if(Big_Int.ubig_value[index] == '0' and Big_Int.ubig_value[index-1] == '0'){
         Big_Int.ubig_value.pop_back();
      }else if(Big_Int.ubig_value[index] == '0' and Big_Int.ubig_value[index-1] != '0'){
         Big_Int.ubig_value.pop_back();
         break;
      }else{
         break;
      }

   }

   return Big_Int;

}

void ubigint::multiply_by_2() {

    //cout << "multiply this number by two: " << *this << endl;

    vector<unsigned char> output(this->ubig_value.size() + 1, '0');
    vector<unsigned char> that;

    that.push_back('2');

    ubigint Big_Int;

    Big_Int.ubig_value = output;
    int c, d;

    for (int num = 0; num < this->ubig_value.size(); num++) {

        c = 0;

        for (int index = 0; index < that.size(); index++) {

        //cout << "this->ubig_value[num]" << this->ubig_value[num] << endl;
        //cout << "that.ubig_value[index]" << that.ubig_value[index] << endl;

        //cout << "product " << (this->ubig_value[num] - 48) * (that.ubig_value[index] - 48) << endl;

         d = (Big_Int.ubig_value[num + index] - 48) + ((this->ubig_value[num] - 48) * (that[index] - 48)) + c;

         Big_Int.ubig_value[num + index] = ((d % 10) + '0');

         c = floor(d / 10);

        }

        Big_Int.ubig_value[num + that.size()] = c + '0';
    }

    for( int index = Big_Int.ubig_value.size() - 1;
         index >= 0; --index){
      if(Big_Int.ubig_value.size() == 1 or
         Big_Int.ubig_value[index-1] == '\0'){
         break;
      }
      if(Big_Int.ubig_value[index] == '0' and Big_Int.ubig_value[index-1] == '0'){
         Big_Int.ubig_value.pop_back();
      }else if(Big_Int.ubig_value[index] == '0' and Big_Int.ubig_value[index-1] != '0'){
         Big_Int.ubig_value.pop_back();
         break;
      }else{
         break;
      }

   }

    this->ubig_value.clear();
    this->ubig_value = Big_Int.ubig_value;

    /*
     for (int it = Big_Int.ubig_value.size() - 1; it >= 0; --it)
       this->ubig_value.push_back(Big_Int.ubig_value[it]);
    */
      //cout << "The answer is : " << *this << endl;
}

void ubigint::divide_by_2() {

    //cout << "dividing " << *this << " by two" << endl;

    ubigint two;
    two.ubig_value.push_back('2');
    vector<unsigned char> answer;
    int calculation;
    int subtraction;
    int stuff;
    int dividend = this->ubig_value.back() - 48;

    //cout << "dividend: " << dividend << endl;


    for (int num = this->ubig_value.size() - 1; num >= 0; --num)
    {
        //cout << "The current value is: " <<  this->ubig_value[num] << endl;

        if (dividend >= 2)
        {
            //calculation = (this->ubig_value[num] - 48) / 2;
            calculation = dividend / 2;
            //cout << "calculation: " << calculation << endl;
            answer.push_back(calculation + '0');

            if (num == 0)
                break;
            else
            {
                //subtraction = (this->ubig_value[num] - 48) - (calculation * 2);
                //subtraction = (this->ubig_value[num] - 48) - dividend;
                subtraction = dividend - (2 *calculation);
               // cout << "subtraction: " << subtraction << endl;
                dividend = (subtraction * 10) + (this->ubig_value[num - 1] - 48);

                //cout << "dividend: " << dividend << endl;
            }
        }

        else
        {
            //cout << "here!" << endl;
            if (dividend == 0)
            {
                answer.push_back('0');
                dividend = (this->ubig_value[num - 1] - 48);
            }
            else
            {
            //cout << "old dividend: " << dividend << endl;
            answer.push_back('0');
            //dividend = (this->ubig_value[num - 1] - 48) + '0';
            dividend = (this->ubig_value[num - 1] - 48) + (dividend * 10);
            //cout << "new dividend: " << dividend << endl;
            }
        }
    }

    this->ubig_value.clear();
    /*
    for (int it1 = 0; it1 < answer.size(); ++it1)
       cout << answer[it1];
    */
    //cout << "Actual answer for division: ";

     for (int it = answer.size() - 1; it >= 0; --it)
       this->ubig_value.push_back(answer[it]);
    /*
     for (int it = 0; it < this->ubig_value.size(); ++it)
       //cout << this->ubig_value[it];
    */
    for( int index = this->ubig_value.size() - 1; index >= 0; --index){

      if (this->ubig_value[index] != '0')
        break;
      else
        this->ubig_value.pop_back();
     }

}

struct quo_rem { ubigint quotient; ubigint remainder; };

quo_rem ubigint::udivide (const ubigint dividend, ubigint divisor) const {
   // Note: divisor is modified so pass by value (copy).

   //cout << "Inside division function" << endl;

   //cout << "dividend: " << dividend << endl;
   //cout << "divisor: " << divisor << endl;

   ubigint zero;
   zero.ubig_value.push_back('0');

   //cout << "zero: " << zero << endl;
   if (divisor == zero) throw domain_error ("udivide by zero");

   ubigint power_of_2;
   power_of_2.ubig_value.push_back('1');

   ubigint quotient;
   quotient.ubig_value.push_back('0');

   ubigint remainder;
   //cout << remainder <
   remainder.ubig_value = dividend.ubig_value;

    while (divisor < dividend) {

      divisor.multiply_by_2();
      power_of_2.multiply_by_2();
   }

   //cout << "divisor: " << divisor << endl;
   //cout << "power of 2:" << power_of_2 << endl;
/*
    divisor.divide_by_2();
    power_of_2.divide_by_2();

    cout << "divisor: " << divisor << endl;
   cout << "power of 2:" << power_of_2 << endl;
*/
    while (zero < power_of_2) {

        //cout << "remainder greater than or equal to divisor? " << remainder << " " << divisor << endl;

        //cout << "Here" << endl;

      if (divisor < remainder or (divisor == remainder)) {
         //cout << "inside the if statement" << endl;
         remainder = remainder - divisor;
         quotient = quotient + power_of_2;


        //cout << "quotient " << quotient << endl;
        //cout << "remainder " << remainder << endl;
      }

      divisor.divide_by_2();
      power_of_2.divide_by_2();

   }

   divisor.ubig_value.clear();
   power_of_2.ubig_value.clear();

   //cout << "quotient " << quotient << endl;
   //cout << "mod answer: " << remainder << endl;

   quo_rem ans {.quotient = quotient, .remainder = remainder};

   quotient.ubig_value.clear();
   remainder.ubig_value.clear();

   return ans;
}

ubigint ubigint::operator/ (const ubigint& that) const {

   return udivide (*this, that).quotient;

}

ubigint ubigint::operator% (const ubigint& that) const {

   return udivide(*this, that).remainder;
}

bool ubigint::operator== (const ubigint& that) const {

   int cint_size = this->ubig_value.size();
   int nint_size = that.ubig_value.size();

   bool flag = false;

   //cout << "bool ubigint::entered operator== " << endl;

   bool size_check = cint_size == nint_size;

   //bool sign_check;

   //check for signs of vectors are the same
   //check for length of vectors are the same
   if(size_check){
      flag = true;

      //cout << "currently: true" << endl;

      for(unsigned int i = 0; i < cint_size; ++i){
         if(this->ubig_value[i] != that.ubig_value[i]){
            flag = false;

            //cout << "currently: false" << endl;

            break;
         }
      }
   }

   return flag;
}

bool ubigint::operator< (const ubigint& that) const {

   int cint_size = this->ubig_value.size();

    //cout << "bool ubigint::entered operator< " << endl;

   if (cint_size > that.ubig_value.size())
   {
       //cout << "this size larger, returning true" << endl;
        return false;
   }
   else if (cint_size < that.ubig_value.size())
   {
    //cout << "this size larger, returning true" << endl;
    return true;
   }
   else {

       for(int i = cint_size - 1; i >= 0; --i){

        //cout << "comparing: " << this->ubig_value[i] << " and " <<  that.ubig_value[i] << endl;

          if((this->ubig_value[i] != that.ubig_value[i])
             and (this->ubig_value[i] < that.ubig_value[i])){

            //cout << "this is smaller than that" << endl;
            return true;
          }

          if((this->ubig_value[i] != that.ubig_value[i])
             and (this->ubig_value[i] > that.ubig_value[i])){

            //cout << "that is smaller than this" << endl;
            return false;
            }
        }
    }
//cout << "returning false" << endl;
return false;
}

ostream& operator<< (ostream& out, const ubigint& that) {

   int int_size = that.ubig_value.size();

   // verison that works for the vectors of this and that
   for( int index = int_size - 1 ; index >= 0;
    --index){
      out << that.ubig_value[index];
   }
   out << endl;

   return out;
}
