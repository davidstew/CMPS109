// $Id: division.cpp,v 1.13 2016-06-13 13:44:52-07 - - $
//By: David Stewart (daastewa@ucsc.edu) 
//and Christopher Hahn (cnhahn@ucsc.edu)
#include <cstdlib>
#include <iostream>
#include <locale>
#include <stdexcept>

using namespace std;

using ulong = unsigned long;
using uupair = pair<ulong,ulong>;

uupair divide (const ulong& dividend, const ulong& divisor) {
   if (divisor == 0) throw domain_error ("divide(_,0)");
   ulong powerof2 = 1;
   ulong divisor_ = divisor;
   while (divisor_ < dividend) {
      divisor_ *= 2;
      powerof2 *= 2;
   }
   ulong quotient = 0;
   ulong remainder = dividend;
   while (powerof2 > 0) {
      if (divisor_ <= remainder) {
         remainder -= divisor_;
         quotient += powerof2;
      }
      divisor_ /= 2;
      powerof2 /= 2;
   }
   return uupair (quotient, remainder);
}


ostream& operator<< (ostream& out, const uupair& pair) {
   out << pair.first << " Rem " << pair.second;
   return out;
}

uupair tests[] = {
   {                 0L, 1'024L},
   {                 5L,     7L},
   {               100L,     0L},
   {               100L,    50L},
   {               320L,    20L},
   {               963L,   71L},
   {12'345'678'912'345L, 9'876L},
};

int main (int, char**) {
   cout.imbue (locale (""));
   uupair* testend = tests + sizeof tests / sizeof *tests;
   for (uupair* itor = tests; itor < testend; ++itor) {
      ulong dividend = itor->first;
      ulong divisor = itor->second;
      cout << dividend << " / " << divisor << " = ";
      try {
         uupair result = divide (dividend, divisor);
         cout << result;
         uupair tested = uupair (dividend / divisor,
                                 dividend % divisor);
         if (tested != result) {
            cout << ": wrong " << tested;
         }
      }catch (domain_error& error) {
         cout << "domain_error: " << error.what();
      }
      cout << endl;
   }
   return EXIT_SUCCESS;
}

//TEST// ./division 2>&1 >division.output
//TEST// mkpspdf division.ps division.cpp* division.output
