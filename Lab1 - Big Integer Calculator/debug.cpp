// $Id: debug.cpp,v 1.5 2017-07-06 23:27:29-07 - - $
//By: David Stewart (daastewa@ucsc.edu) 
//and Christopher Hahn (cnhahn@ucsc.edu)

#include <climits>
#include <iostream>
#include <vector>
using namespace std;

#include "debug.h"
#include "util.h"

vector<bool> debugflags::flags (UCHAR_MAX + 1, false);

void debugflags::setflags (const string& initflags) {
   for (const unsigned char flag: initflags) {
      if (flag == '@') flags.assign (flags.size(), true);
                  else flags[flag] = true;
   }
   if (getflag ('x')) {
      string flag_chars;
      for (size_t index = 0; index < flags.size(); ++index) {
         if (getflag (index)) flag_chars += static_cast<char> (index);
      }
   }
}

//
// getflag -
//    Check to see if a certain flag is on.
//

bool debugflags::getflag (char flag) {
   return flags[static_cast<unsigned char> (flag)];
}

void debugflags::where (char flag, const char* file, int line,
                        const char* func) {
   note() << "DEBUG(" << flag << ") " << file << "[" << line << "] "
          << func << "()" << endl;
}
