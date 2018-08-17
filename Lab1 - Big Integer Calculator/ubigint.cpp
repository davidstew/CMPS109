// $Id: ubigint.cpp,v 1.4 2017-07-06 23:27:29-07 - - $
//By: David Stewart (daastewa@ucsc.edu) 
//and Christopher Hahn (cnhahn@ucsc.edu)

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

ubigint::ubigint (unsigned long that)
{
   stringstream ss;
   string that_;

   ss << that;
   ss >> that_;

   for (char digit : that_)
   {
      ubigvalue_t::iterator it = ubig_value.begin();
      ubig_value.insert(it, digit);
      if (not isdigit (digit))
      {
         throw invalid_argument ("ubigint::ubigint(" + that_ + ")");
      }
      //   DEBUGF ('~', this << " -> " << uvalue)
   }
}

ubigint::ubigint (const string& that)  
{
   for (char digit : that)
   {
      ubigvalue_t::iterator it = ubig_value.begin();
      ubig_value.insert(it, digit);
      if (not isdigit (digit))
      {
         throw invalid_argument ("ubigint::ubigint(" + that + ")");
      }
   }
}

ubigint ubigint::operator+(const ubigint& that) const 
{
   ubigint Big_Int;
   int loop_length;  
   int longer_number;
   int carry = 0;
   string number = "";
   int remainder;
   int op1, op2;
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

   for(int num = 0; num < loop_length; num++)
   {
      op1 = this->ubig_value[num] - 48; //convert to unsigned_char
      op2 = that.ubig_value[num] - 48; //convert to unsigned_char
      int sum = op1 + op2 + carry;
      remainder = sum % 10;
      unsigned char char_remainder = remainder + '0';

      if (sum >= 10)
      {
         carry = 1;
         Big_Int.ubig_value.push_back(char_remainder);
      }
      else
      {
         carry = 0;
         Big_Int.ubig_value.push_back(char_remainder);
      }
   }
   if (number!= "")
   {
      if (number == "this")
      {
         for (int num = loop_length; num < longer_number; ++num) 
         {
            op1 = this->ubig_value[num] - 48;
            int sum = op1 + carry;
            remainder = sum % 10;
            unsigned char char_remainder = remainder + '0';

            if (sum >= 10)
            {
               carry = 1;
               Big_Int.ubig_value.push_back(char_remainder);
            }
            else
            {
               carry = 0;
               Big_Int.ubig_value.push_back(char_remainder);
            }
         }
      }
      else
      {
         for (int num = loop_length; num < longer_number; ++num)
         {
            op1 = that.ubig_value[num] - 48;
            int sum = op1 + carry;
            remainder = sum % 10;
            unsigned char char_remainder = remainder + '0';
            if (sum >= 10)
            {
               carry = 1;
               Big_Int.ubig_value.push_back(char_remainder);
            }
            else
            {
               carry = 0;
               Big_Int.ubig_value.push_back(char_remainder);
            }
         }
      }
   }
if (carry == 1)
{
   unsigned char carry_ = carry + '0';
   Big_Int.ubig_value.push_back(carry_);
}
   return Big_Int;
}

ubigint ubigint::operator- (const ubigint& that) const
{
   if (*this < that) throw domain_error ("ubigint::operator-(a<b)");
   ubigvalue_t this_vector = this->ubig_value;
   ubigvalue_t that_vector = that.ubig_value;
   ubigint Big_Int;
   string number = "";
   int op1, op2;
   int difference;
   unsigned char char_difference;
   if (this->ubig_value.size() > that.ubig_value.size())
   {
      unsigned char placeholder = 0 + '0';
      for (unsigned int num = that.ubig_value.size();
         num <= this->ubig_value.size(); num++)
      {
         that_vector.push_back(placeholder);
      }
   }
   else if(this->ubig_value.size() < that.ubig_value.size())
   {
      unsigned char placeholder = 0 + '0';
      for(unsigned int num = this->ubig_value.size();
         num <= that.ubig_value.size(); num++)
      {
         this_vector.push_back(placeholder);
      }
   }
   for(unsigned int num = 0; num < this_vector.size(); num++)
   {
      op1 = this_vector[num] - 48; //convert to unsigned_char
      op2 = that_vector[num] - 48; //convert to unsigned_char
      if (op1 < op2)
      {
         if (this_vector[num + 1] == 0 and this_vector[num] == 0)
         {  
            this_vector[num + 1] = 9;
         }
         op1 += 10;
         this_vector[num + 1] -= 1;
         difference = op1 - op2;
         char_difference = difference + '0';
      }
      else
      {
         difference = op1 - op2;
         char_difference = difference + '0'; 
      }
      Big_Int.ubig_value.push_back(char_difference);
   }
   for(int index = Big_Int.ubig_value.size() - 1;
   index >= 0; --index)
   {
      if(Big_Int.ubig_value.size() == 1 or
         Big_Int.ubig_value[index-1] == '\0')
      {
         break;
      }
      if(Big_Int.ubig_value[index] == '0'
         and Big_Int.ubig_value[index-1] == '0')
      {
         Big_Int.ubig_value.pop_back();
      }else if(Big_Int.ubig_value[index] == '0'
         and Big_Int.ubig_value[index-1] != '0')
      {
         Big_Int.ubig_value.pop_back();
         break;
      }
      else
      {
         break;
      }
   }

   return Big_Int;
}

ubigint ubigint::operator* (const ubigint& that) const 
{
   ubigint Big_Int;
   vector<unsigned char> output(this->ubig_value.size()
    + that.ubig_value.size(), '0');
   Big_Int.ubig_value = output;
   int c, d;
   for(unsigned int num = 0; num < this->ubig_value.size(); num++)
   {
      c = 0;
      for(unsigned int index = 0; 
         index < that.ubig_value.size(); index++)
      {
         d = (Big_Int.ubig_value[num + index] - 48)
          + ((this->ubig_value[num] - 48) *
            (that.ubig_value[index] - 48))
               + c;
         Big_Int.ubig_value[num + index] = ((d % 10) + '0');

         c = floor(d / 10);
      }
      Big_Int.ubig_value[num + that.ubig_value.size()] = c + '0';
   }
   for(int index = Big_Int.ubig_value.size() - 1;
      index >= 0; --index)
   {
      if(Big_Int.ubig_value.size() == 1 or
      Big_Int.ubig_value[index-1] == '\0')
      {
         break;
      }
      if(Big_Int.ubig_value[index] == '0' and 
         Big_Int.ubig_value[index-1] == '0')
      {
      Big_Int.ubig_value.pop_back();
      }
      else if(Big_Int.ubig_value[index] == '0' and
         Big_Int.ubig_value[index-1] != '0')
      {
         Big_Int.ubig_value.pop_back();
         break;
      }
      else
      {
         break;
      }
   }
   return Big_Int;
}

void ubigint::multiply_by_2()
{
   vector<unsigned char> output(this->ubig_value.size() + 1, '0');
   vector<unsigned char> that;
   that.push_back('2');
   ubigint Big_Int;
   Big_Int.ubig_value = output;
   int c, d;
   for(unsigned int num = 0; num < this->ubig_value.size(); num++)
   {
      c = 0;
      for(unsigned int index = 0; index < that.size(); index++)
      {
         d = (Big_Int.ubig_value[num + index] - 48) + 
            ((this->ubig_value[num] - 48) * (that[index] - 48)) + c;
         Big_Int.ubig_value[num + index] = ((d % 10) + '0');
         c = floor(d / 10);
      }
      Big_Int.ubig_value[num + that.size()] = c + '0';
   }

   for(int index = Big_Int.ubig_value.size() - 1;
      index >= 0; --index){
      if(Big_Int.ubig_value.size() == 1 or
         Big_Int.ubig_value[index-1] == '\0')
      {
         break;
      }
      if(Big_Int.ubig_value[index] == '0' and
          Big_Int.ubig_value[index-1] == '0')
      {
         Big_Int.ubig_value.pop_back();
      }
      else if(Big_Int.ubig_value[index] == '0' and
         Big_Int.ubig_value[index-1] != '0')
      {
         Big_Int.ubig_value.pop_back();
         break;
      }
      else
      {
         break;
      }
   }
   this->ubig_value.clear();
   this->ubig_value = Big_Int.ubig_value;
}

void ubigint::divide_by_2() {
   ubigint two;
   two.ubig_value.push_back('2');
   vector<unsigned char> answer;
   int calculation;
   int subtraction;
   int dividend = this->ubig_value.back() - 48;

   for(int num = this->ubig_value.size() - 1;
    num >= 0; --num)
   {
      if(dividend >= 2)
      {
         calculation = dividend / 2;
         answer.push_back(calculation + '0');
         if(num == 0)
         {
             break;
         }
         else
         {
            subtraction = dividend - (2 *calculation);
            dividend = (subtraction * 10)
               + (this->ubig_value[num - 1] - 48);
         }
      }
      else
      {
         if(dividend == 0)
         {
            answer.push_back('0');
            dividend = (this->ubig_value[num - 1] - 48);
         }
         else
         {
            answer.push_back('0');
            dividend = (this->ubig_value[num - 1] - 48)
               + (dividend * 10);
         }
      }
   }
   this->ubig_value.clear();
   for(int it = answer.size() - 1; it >= 0; --it)
   {
      this->ubig_value.push_back(answer[it]);
   }
   for(int index = this->ubig_value.size() - 1;
    index >= 0; --index)
   {
      if (this->ubig_value[index] != '0')
      {
         break;
      }
      else
      {
         this->ubig_value.pop_back();
      }
   }
}

struct quo_rem { ubigint quotient; ubigint remainder; };

quo_rem ubigint::udivide (const ubigint dividend,
          ubigint divisor) const 
{
   // Note: divisor is modified so pass by value (copy).
   ubigint zero;

   ubigint power_of_2;
   power_of_2.ubig_value.push_back('1');
   
   ubigint quotient;
   quotient.ubig_value.push_back('0');
   
   ubigint remainder;
   remainder.ubig_value = dividend.ubig_value;

   zero.ubig_value.push_back('0');
   if (divisor == zero)
   {
      divisor.ubig_value.clear();
      power_of_2.ubig_value.clear();

      quotient.ubig_value = zero.ubig_value;
      remainder.ubig_value = zero.ubig_value;

      quo_rem answer {.quotient = quotient, .remainder = remainder};

      return answer;
   }

   while(divisor < dividend)
   {
      divisor.multiply_by_2();
      power_of_2.multiply_by_2();
   }
   while(zero < power_of_2)
   {
      if (divisor < remainder or (divisor == remainder))
      {
         remainder = remainder - divisor;
         quotient = quotient + power_of_2;
      }
      divisor.divide_by_2();
      power_of_2.divide_by_2();
   }
   divisor.ubig_value.clear();
   power_of_2.ubig_value.clear();

   quo_rem answer {.quotient = quotient, .remainder = remainder};

   quotient.ubig_value.clear();
   remainder.ubig_value.clear();

   return answer;
}

ubigint ubigint::operator/ (const ubigint& that) const
{
   return udivide (*this, that).quotient;
}

ubigint ubigint::operator% (const ubigint& that) const
{
   return udivide(*this, that).remainder;
}

bool ubigint::operator== (const ubigint& that) const
{
   int cint_size = this->ubig_value.size();
   int nint_size = that.ubig_value.size();

   bool flag = false;
   bool size_check = cint_size == nint_size;

   if(size_check)
   {
      flag = true;
      for(int i = 0; i < cint_size; ++i)
      {
         if(this->ubig_value[i] != that.ubig_value[i])
         {
            flag = false;
            break;
         }
      }
   }
   return flag;
}

bool ubigint::operator< (const ubigint& that) const 
{
   unsigned int cint_size = this->ubig_value.size();
   if(cint_size > that.ubig_value.size())
   {
      return false;
   }
   else if(cint_size < that.ubig_value.size())
   {
      return true;
   }  
   else
   {
      for(int i = cint_size - 1; i >= 0; --i)
      {
         if((this->ubig_value[i] != that.ubig_value[i])
            and (this->ubig_value[i] < that.ubig_value[i]))
         {
            return true;
         }
         if((this->ubig_value[i] != that.ubig_value[i])
            and (this->ubig_value[i] > that.ubig_value[i]))
         {
            return false;
         }
      }
   }
   return false;
}

ostream& operator<< (ostream& out, const ubigint& that)
{
   int int_size = that.ubig_value.size();
   // verison that works for the vectors of this and that
   for(int index = int_size - 1 ; index >= 0;
   --index)
   {
      out << that.ubig_value[index];
   }
   return out;
}
