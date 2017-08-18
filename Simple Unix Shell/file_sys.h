// $Id: file_sys.h,v 1.5 2016-04-07 13:36:11-07 - - $

#ifndef __INODE_H__
#define __INODE_H__

#include <exception>
#include <iostream>
#include <memory>
#include <map>
#include <vector>
using namespace std;

#include "util.h"

// inode_t -
//    An inode is either a directory or a plain file.

enum class file_type {PLAIN_TYPE, DIRECTORY_TYPE};
class inode;
class base_file;
class plain_file;
class directory;
using inode_ptr = shared_ptr<inode>;
using base_file_ptr = shared_ptr<base_file>;
ostream& operator<< (ostream&, file_type);

// inode_state -
//    A small convenient class to maintain the state of the simulated
//    process:  the root (/), the current directory (.), and the
//    prompt.

class inode_state {
   friend class inode;
   friend ostream& operator<< (ostream& out, const inode_state&);
   private:
      inode_ptr root {nullptr};
      inode_ptr cwd {nullptr};
      string prompt_ {"% "};
      //add something to set the prompt
   public:
      inode_state (const inode_state&) = delete; // copy ctor
      inode_state& operator= (const inode_state&) = delete; // op=
      inode_state();
      const string& prompt();
      inode_ptr& getcwd() { return this->cwd; }
      inode_ptr& getroot() { return this->root; }
      void setprompt(string new_prompt) { this->prompt_ = new_prompt; }
};

// class inode -
// inode ctor -
//    Create a new inode of the given type.
// get_inode_nr -
//    Retrieves the serial number of the inode.  Inode numbers are
//    allocated in sequence by small integer.
// size -
//    Returns the size of an inode.  For a directory, this is the
//    number of dirents.  For a text file, the number of characters
//    when printed (the sum of the lengths of each word, plus the
//    number of words.
//

class inode {
   friend class inode_state;
   private:
      static int next_inode_nr;
      int inode_nr = 0;
      int size = 0;
      base_file_ptr contents;
      inode_ptr parent;
      string path;
   public:
      inode (file_type);
      int get_inode_nr() const;
      base_file_ptr getbase() { return contents; }
      void print() { cout << "printing from inode" << endl; }
      int get_node_number() { return inode_nr; }
      inode_ptr& getroot(inode_state& state) { state.getroot(); }
      inode_ptr& getcwd (inode_state& state) { state.getcwd(); }
      inode_ptr& get_parent_pointer() { return this->parent; }
      void set_parent_pointer(inode_ptr parent) {this->parent = parent; }
      void printPath() { cout << path; }
      string& getPath() { return path; }
      string dirname;

};

// class base_file -
// Just a base class at which an inode can point.  No data or
// functions.  Makes the synthesized members useable only from
// the derived classes.

class file_error: public runtime_error {
   public:
      explicit file_error (const string& what);
};

class base_file {
   protected:
      base_file() = default;
   public:
      virtual ~base_file() = default;
      base_file (const base_file&) = delete;
      base_file& operator= (const base_file&) = delete;
      virtual size_t size() const = 0;
      virtual void readfile(const string& input) const = 0;
      virtual void writefile (const wordvec& newdata) = 0;
      virtual void remove (const string& filename) = 0;
      virtual void mkdir (const string& dirname, inode_state&) = 0;
      virtual void mkfile (const string& filename) = 0;
      virtual void print() = 0;
      virtual void print_dirents() = 0;
      virtual map<string,inode_ptr>& get_dirents() = 0;
      virtual string& getType() = 0;
};

// class plain_file -
// Used to hold data.
// synthesized default ctor -
//    Default vector<string> is a an empty vector.
// readfile -
//    Returns a copy of the contents of the wordvec in the file.
// writefile -
//    Replaces the contents of a file with new contents.

class plain_file: public base_file {
   private:
      wordvec data;
      string type = "file";
   public:
      virtual size_t size() const override;
      virtual void readfile(const string& input) const override;
      virtual void writefile (const wordvec& newdata);
      virtual void remove (const string& filename) override;
      virtual void mkdir (const string& dirname, inode_state&) override;
      virtual void mkfile (const string& filename) override;
      virtual void print() { cout << "print from plain file class: " << data << endl; }
      virtual void print_dirents();
      virtual map<string,inode_ptr>& get_dirents();
      virtual string& getType() { return type; }
};

// class directory -
// Used to map filenames onto inode pointers.
// default ctor -
//    Creates a new map with keys "." and "..".
// remove -
//    Removes the file or subdirectory from the current inode.
//    Throws an file_error if this is not a directory, the file
//    does not exist, or the subdirectory is not empty.
//    Here empty means the only entries are dot (.) and dotdot (..).
// mkdir -
//    Creates a new directory under the current directory and
//    immediately adds the directories dot (.) and dotdot (..) to it.
//    Note that the parent (..) of / is / itself.  It is an error
//    if the entry already exists.
// mkfile -
//    Create a new empty text file with the given name.  Error if
//    a dirent with that name exists.

class directory: public base_file {
   private:
      // Must be a map, not unordered_map, so printing is lexicographic
      map<string,inode_ptr> dirents;
      string file_name = "";
      int directory_size = 0;
      string type = "directory";
   public:
      virtual size_t size() const override;
      virtual void readfile(const string& input) const override;
      virtual void writefile (const wordvec& newdata) override;
      virtual void remove (const string& filename) override;
      virtual void mkdir (const string& dirname, inode_state&) override;
      virtual void mkfile (const string& filename) override;
      virtual void print() { cout << "print from directory class" << endl; }
      virtual void print_dirents();
      virtual map<string,inode_ptr>& get_dirents();
      virtual string& getType() { return type; }
};

#endif

