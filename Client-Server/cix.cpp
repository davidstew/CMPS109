// $Id: cix.cpp,v 1.4 2016-05-09 16:01:56-07 - - $

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include "fstream"
using namespace std;

#include <libgen.h>
#include <sys/types.h>
#include <unistd.h>

#include "protocol.h"
#include "logstream.h"
#include "sockets.h"

logstream log (cout);
struct cix_exit: public exception {};

unordered_map<string,cix_command> command_map {
   {"exit", cix_command::EXIT},
   {"help", cix_command::HELP},
   {"ls"  , cix_command::LS  },
   {"get" , cix_command::GET },
   {"put" , cix_command::PUT },
   {"rm" ,  cix_command::RM  },
};

void cix_help() {
   static const vector<string> help = {
      "exit         - Exit the program.  Equivalent to EOF.",
      "get filename - Copy remote file to local host.",
      "help         - Print help summary.",
      "ls           - List names of files on remote server.",
      "put filename - Copy local file to remote host.",
      "rm filename  - Remove file from remote server.",
   };
   for (const auto& line: help) cout << line << endl;
}

void cix_ls (client_socket& server) {
   cix_header header;
   header.command = cix_command::LS;
   log << "sending header " << header << endl;
   send_packet (server, &header, sizeof header);
   recv_packet (server, &header, sizeof header);
   log << "received header " << header << endl;
   if (header.command != cix_command::LSOUT) {
      log << "sent LS, server did not return LSOUT" << endl;
      log << "server returned " << header << endl;
   }else {
      char buffer[header.nbytes + 1];
      recv_packet (server, buffer, header.nbytes); //payload
      log << "received " << header.nbytes << " bytes" << endl;
      buffer[header.nbytes] = '\0';
      cout << buffer;
   }
}

void cix_get(client_socket& server, string filename) { 

cix_header header;
header.command = cix_command::GET;
//strcpy(header.filename,filename.c_str());
snprintf(header.filename, filename.length() + 1, filename.c_str());
log << "sending header " << header << endl;
send_packet (server, &header, sizeof header);
recv_packet (server, &header, sizeof header);
   log << "received header " << header << endl;
   if (header.command != cix_command::FILE) {
      log << filename << " is not a file on the server" << endl;
      log << "server returned " << header << endl;
   }else {
      char buffer[header.nbytes + 1];
      recv_packet (server, buffer, header.nbytes); //payload
      log << "received " << header.nbytes << " bytes" << endl;
      buffer[header.nbytes] = '\0';
      std::ofstream os(header.filename, std::ofstream::binary);
      os.write(buffer, header.nbytes); 
   }
}

void cix_put(client_socket& server, string filename) {

cix_header header; 
//strcpy(header.filename,filename.c_str());
snprintf(header.filename, filename.length() + 1, filename.c_str());
std::ifstream is (header.filename, std::ifstream::binary);

if (is) {
   is.seekg(0, is.end);
   int length = is.tellg();
   is.seekg(0, is.beg);
   char buffer[length];

   is.read(buffer, length); 

   header.command = cix_command::PUT;
   header.nbytes = length;
   send_packet (server, &header, sizeof header); //invokes the put function on the server
   send_packet (server, buffer, length); //send the payload
   recv_packet (server, &header, sizeof header); //ACK or NAK?
}
else 
   cerr << "Error: could not find file: " << filename << endl;

if (header.command == cix_command::ACK)
   cout << "ACK received: put file on the server" << endl;
else if (header.command == cix_command::NAK)
   cerr << "NAK received: failed to put file on server" << endl;

is.close();
}

void cix_rm(client_socket& server, string filename) {

cix_header header;
//strcpy(header.filename,filename.c_str());
snprintf(header.filename, filename.length() + 1, filename.c_str());
header.command = cix_command::RM;
header.nbytes = 0;
send_packet (server, &header, sizeof header); //invokes the rm function on the server
recv_packet (server, &header, sizeof header); //ACK OR NAK?

if (header.command == cix_command::ACK)
   cout << "ACK received: succesfully deleted file: " << filename << endl;
else if (header.command == cix_command::NAK)
   cerr << "NAK received: failed to delete file: " << filename << endl;
}

void usage() {
   cerr << "Usage: " << log.execname() << " [host] [port]" << endl;
   throw cix_exit();
}

int main (int argc, char** argv) {
    vector<string> contents;
    string host;
    in_port_t port;
   log.execname (basename (argv[0]));
   log << "starting" << endl;
   vector<string> args (&argv[1], &argv[argc]);
   if (args.size() > 2) usage();
   if (args.size() == 1) { //works with only one argument: for SCORE rubric
   host = "localhost";
   port = get_cix_server_port (args, 0);
   log << to_string (hostinfo()) << endl;
   }
   else {
   host = get_cix_server_host (args, 0);
   port = get_cix_server_port (args, 1);
   log << to_string (hostinfo()) << endl;
   }
   try {
      log << "connecting to " << host << " port " << port << endl;
      client_socket server (host, port);
      log << "connected to " << to_string (server) << endl;
      for (;;) {
         string line;
         getline (cin, line);
         if (cin.eof()) throw cix_exit();
         std::istringstream ss(line);
         std::string token;
         while(std::getline(ss, token, ' ')) 
         contents.push_back(token);
         const auto& itor = command_map.find (contents[0]);
         cix_command cmd = itor == command_map.end()
                         ? cix_command::ERROR : itor->second;
         switch (cmd) {
            case cix_command::EXIT:
               throw cix_exit();
               break;
            case cix_command::HELP:
               cix_help();
               contents.clear();
               break;
            case cix_command::LS:
               cix_ls (server);
               contents.clear();
               break;
            case cix_command::GET: {
               cix_get (server, contents[1]);
               contents.clear();
               break;
            }
            case cix_command::PUT: {
               cix_put (server,contents[1]);
               contents.clear();
               break;
            }
            case cix_command::RM: {
               cix_rm (server,contents[1]);
               contents.clear();
               break;
            }

            default:
               log << line << ": invalid command" << endl;
               break;
         }
      }
   }catch (socket_error& error) {
      log << error.what() << endl;
   }catch (cix_exit& error) {
      log << "caught cix_exit" << endl;
   }
   log << "finishing" << endl;
   return 0;
}

