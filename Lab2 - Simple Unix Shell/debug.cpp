// $Id: debug.cpp,v 1.2 2017-07-12 15:23:21-07 - - $

//By: David Stewart (daastewa@ucsc.edu)
//By: Christopher Hahn (cnhahn@ucsc.edu)

#include <climits>
#include <iostream>
#include <vector>

using namespace std;

#include "debug.h"
#include "util.h"

debugflags::flagset debugflags::flags {};

void debugflags::setflags (const string& initflags) {
   for (const unsigned char flag: initflags) {
      if (flag == '@') flags.set();
                  else flags.set (flag, true);
   }
}

// getflag -
//    Check to see if a certain flag is on.

bool debugflags::getflag (char flag) {
   // WARNING: Don't TRACE this function or the stack will blow up.
   return flags.test (static_cast<unsigned char> (flag));
}

void debugflags::where (char flag, const char* file, int line,
                        const char* func) {
   cout << execname() << ": DEBUG(" << flag << ") "
        << file << "[" << line << "] " << func << "()" << endl;
}
