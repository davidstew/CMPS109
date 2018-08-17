// $Id: util.cpp,v 1.2 2017-07-12 15:23:21-07 - - $

//By: David Stewart (daastewa@ucsc.edu)
//By: Christopher Hahn (cnhahn@ucsc.edu)

#include <cstdlib>
#include <unistd.h>

using namespace std;

#include "util.h"
#include "debug.h"

int exit_status::status = EXIT_SUCCESS;
static string execname_string;

void exit_status::set (int new_status) {
   status = new_status;
}

int exit_status::get() {
   return status;
}

void execname (const string& name) {
   execname_string =  name.substr (name.rfind ('/') + 1);
   DEBUGF ('u', execname_string);
}

string& execname() {
   return execname_string;
}

bool want_echo() {
   constexpr int CIN_FD {0};
   constexpr int COUT_FD {1};
   bool cin_is_not_a_tty = not isatty (CIN_FD);
   bool cout_is_not_a_tty = not isatty (COUT_FD);
   DEBUGF ('u', "cin_is_not_a_tty = " << cin_is_not_a_tty
          << ", cout_is_not_a_tty = " << cout_is_not_a_tty);
   return cin_is_not_a_tty or cout_is_not_a_tty;
}

wordvec split (const string& line, const string& delimiters) {
   wordvec words;
   size_t end = 0;

   // Loop over the string, splitting out words, and for each word
   // thus found, append it to the output wordvec.
   for (;;) {
      size_t start = line.find_first_not_of (delimiters, end);
      if (start == string::npos) break;
      end = line.find_first_of (delimiters, start);
      words.push_back (line.substr (start, end - start));
   }
   DEBUGF ('u', words);
   return words;
}

void exit_status::exit(const wordvec& words){

   string tstring;
   int integer = 0, zero = 0, isChar = 127;


   if(words.size() == 1 or (words.size() == 2 and words[1] == "0"))
   {
      exit_status::set(zero);
   }
   else
   {
      //cout << "words.size() != 1" << endl;
     for(unsigned int i = 1; i < words.size(); i++)
      {
         tstring += words[i] + " ";
      }
      int digit = 0;
      for(unsigned int j = 0; tstring.size() -1 > j; j++)
      {
         integer = integer * 10;
         digit = static_cast<int>(tstring[j]) - 48;
         //cout << "current string: " << tstring[j] << endl;
         //cout << "current int of string: " 
         //<< static_cast<int>(tstring[j]) << endl;
         if(digit <= 10)
         {
            integer += digit;
         }
         else
         {
            //cout << "Error: character found!" << endl;
            exit_status::set(isChar);
            return;
         }
      }
      //cout << integer << endl;
      exit_status::set(integer);
   }
}


ostream& complain() {
   exit_status::set (EXIT_FAILURE);
   cerr << execname() << ": ";
   return cerr;
}
