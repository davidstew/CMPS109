// $Id: commands.cpp,v 1.16 2016-01-14 16:10:40-08 - - $

//By: David Stewart (daastewa@ucsc.edu)
//By: Christopher Hahn (cnhahn@ucsc.edu)

#include "commands.h"
#include "debug.h"
#include <map>
#include <sstream>

command_hash cmd_hash {
   {"cat"   , fn_cat   },
   {"cd"    , fn_cd    },
   {"echo"  , fn_echo  },
   {"exit"  , fn_exit  },
   {"ls"    , fn_ls    },
   {"lsr"   , fn_lsr   },
   {"make"  , fn_make  },
   {"mkdir" , fn_mkdir },
   {"prompt", fn_prompt},
   {"pwd"   , fn_pwd   },
   {"rm"    , fn_rm    },
   {"rmr"   , fn_rmr   },
   {"#"     , fn_ignore}
};

command_fn find_command_fn (const string& cmd) {
   // Note: value_type is pair<const key_type, mapped_type>
   // So: iterator->first is key_type (string)
   // So: iterator->second is mapped_type (command_fn)
   const auto result = cmd_hash.find (cmd);
   if (result == cmd_hash.end()) {
      throw command_error (cmd + ": no such function");

   }
   return result->second;
}

command_error::command_error (const string& what):
      runtime_error (what) {
}

int exit_status_message() {
   int exit_status = exit_status::get();
   cout << execname() << ": exit(" << exit_status << ")" << endl;
   return exit_status;
}

void fn_cat (inode_state& state, const wordvec& words){

   inode_ptr tcwd = state.getcwd();

   for (unsigned int num = 1; num < words.size(); num++)
   {
      tcwd->getbase()->readfile(words[num]);
   }
}

void fn_cd (inode_state& state, const wordvec& words){

   unsigned int num1;

   //stuff is for lsr command
   if(words[0] != "cd" && words[0] != "")
      num1 = 0;
   else
      num1 = 1;


   string contents_, input, error;
   wordvec contents;
   bool found_directory = false;


   for (; num1 < words.size(); num1++)
   {
      if (num1 == words.size() - 1)
      contents_ += words[num1];
      else
      contents_ += words[num1] + " ";
   }

   //cout << "contents_: " << contents_ << endl;

   if (contents_[0] != '/')
   {
      //cout << "prepended slash" << endl;
      contents_ = "/" + contents_;
   }

   std::istringstream ss(contents_);
   std::string token;

   while(std::getline(ss, token, '/'))
   {
      //cout << "pushing back: " << token << endl;
      contents.push_back(token);
      //std::cout << token << '\n';
   }

   //for (int size = 0; size < contents.size(); size++)
      //cout << "input: " << contents[size] << endl;

   if (contents.size() == 1) //if nothing
   {
      if (contents[0] == "" or contents[0] == "/")
      {
         state.getcwd() = state.getroot();
      }
   }
   //if a path longer than 1 directory is 
   //specified or if . or .. is the path
   else 
   {
      //cout << "contents size is: " << contents.size() << endl;
      for (unsigned int num = 1; num < contents.size(); ++num)
      {
         input = contents[num];
         error = error + input + "/";
         for(auto it = state.getcwd()->getbase()->get_dirents().begin();
               it !=  state.getcwd()->getbase()->get_dirents().end();
                     ++it)
         {
            if (it->first == input)
            {
               found_directory = true;
               if (it->second->getbase()->getType() == "directory")
               {
                  state.getcwd() = it->second;
                  break;
               }
               else
               {
                  throw command_error (error + 
                     ": Not a directory" );
                  break;
               }
            }
         }
      }
      if (found_directory == false)
      {
         throw command_error (error + 
               ": No such file or directory" );
      }
      //cout << "cd: " << error << 
      //   " No such file or directory" << endl;
   }
}

void fn_echo (inode_state& state, const wordvec& words)
{
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   cout << word_range (words.cbegin() + 1, words.cend()) << endl;
}

void fn_exit (inode_state& state, const wordvec& words)
{
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   exit_status exitRef;
   exitRef.exit(words);
   throw ysh_exit();
}

void fn_ls (inode_state& state, const wordvec& words)
{
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   inode_ptr tcwd = state.getcwd();
   wordvec cd_to_parent, cd_to_root;
   cd_to_parent.push_back("..");
   cd_to_root.push_back(".");

   //if user enters nothing following ls
   if (words.size() == 1)
   {
      tcwd->printPath();
      cout << ":" << endl;
      tcwd->getbase()->print_dirents();
   }
   else
   {
      //prints root and changes back to directory which called this
      if (words[1] == "/") 
      {
         fn_cd(state, cd_to_root);
         cout << "/:" << endl;
         state.getcwd()->getbase()->print_dirents();
         //change back to current directory
         state.getcwd() = tcwd; 
      }
      //prints current directory
      else if (words[1] == ".") 
      {
         cout << ".:" << endl;
         tcwd->getbase()->print_dirents();
      }
      //prints the contents of the parent directory
      else if (words[1] == "..")
      {
         fn_cd(state, cd_to_parent);
         cout << "..:" << endl;
         state.getcwd()->getbase()->print_dirents();
         //change back to current directory
         state.getcwd() = tcwd; 
      }
      //nothing specified, prints current directory
      else
      {
         tcwd->printPath();
         cout << ":" << endl;
         tcwd->getbase()->print_dirents();
      }
   }
}

void fn_lsr (inode_state& state, const wordvec& words)
{
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   wordvec send_to_lsr;
   inode_ptr tcwd = state.getcwd();

   for (unsigned int size = 1; size < words.size(); size++)
   {
      send_to_lsr.push_back(words[size]);
      fn_lsr_(state, send_to_lsr);
      send_to_lsr.clear();
      state.getcwd() = tcwd;
   }

   //cout << "end recursion=== " << endl;
   //cout << "current path: " << state.getcwd()->getPath() << endl;
   //resets from invoking directory 
   //<---- might need this to get full recursive functionality
   //state.getcwd() = tcwd; 
   //cout << "reset path: " << state.getcwd()->getPath() << endl;
}

void fn_make (inode_state& state, const wordvec& words)
{
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   //name of file
   string filename = words[1];
   string contents_ = "";
   inode_ptr tcwd = state.getcwd();

   if(words.size() == 1)
   {
      throw command_error("make: missing operand");
   }
   else
   {
      //captures the message being inputted
      for (unsigned int num = 2; num < words.size(); num++)
      {
         if (num == words.size() - 1)
            contents_ += words[num];
         else
            contents_ += words[num] + " ";
      }

      wordvec temp_contents, check_name, to_cd;
      temp_contents.push_back(contents_);
      to_cd.push_back(filename);

      //bool valid = true;
      std::istringstream ss(filename);
      std::string token;

      while(std::getline(ss, token, '/'))
      {
         check_name.push_back(token);
      }

      if (check_name.size() > 1)
      {
         fn_cd(state, to_cd);
         //if filename matches name of directory path
         if (state.getcwd()->getPath() == "/" + filename) 
         {
            state.getcwd() = tcwd; //reset cwd
            throw command_error 
               ("make: cannot create file: directory exists" );
         }
         else
         {
            state.getcwd()->getbase()->mkfile
               (check_name[check_name.size() - 1]);
            state.getcwd()->getbase()->writefile(temp_contents);
         }
      //cout << "make get path: " << state.getcwd()->getPath() << endl;
      //reset cwd
      state.getcwd() = tcwd; 
      }
      else
      {
         for(auto it = tcwd->getbase()->get_dirents().begin();
             it !=  tcwd->getbase()->get_dirents().end(); ++it)
         {
            //if file name matches directory name, error occurs
            if (it->first == 
                  filename && it->second->getbase()->getType()
                      == "directory")
            {
               //reset cwd
               state.getcwd() = tcwd; 
               throw command_error
               ("mkdir: cannot create directory" 
                  + filename + ": File exists");
            }
            //overwriting file it already exits
            else if (it->first == filename)
            {
               it->second->getbase()->writefile(temp_contents);
               return;
            }
         }
      //if file name is not used yet
      tcwd->getbase()->mkfile(filename);
      tcwd->getbase()->writefile(temp_contents);
      state.getcwd() = tcwd; //reset cwd
      }
   }
}

void fn_mkdir (inode_state& state, const wordvec& words)
{
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   inode_ptr tcwd = state.getcwd();

   string contents_;
   wordvec contents;
   wordvec to_cd;

   for (unsigned int num = 1; num < words.size(); num++)
   {
      if (num == words.size() - 1)
         contents_ += words[num];
      else
         contents_ += words[num] + "/";
   }

   //cout << "contents is: " << contents_ << endl;

   std::istringstream ss(contents_);
   std::string token;

   while(std::getline(ss, token, '/'))
   {
      contents.push_back(token);
   }

   //bool valid = true;
   string comparator = "/";
   //cout << "contents: " << contents_ << endl;

   //if path is longer than one directory, we must do
   // a series of directory changes
   if (contents.size() > 1)
   {
      for (unsigned int num = 0; num < (contents.size()); num++)
      {
         if (num == 0)
            comparator = comparator + contents[num];
         else
         {
            comparator = comparator + "/";
            comparator = comparator + contents[num];
         }
         //to_cd.push_back(contents[num]);
         //fn_cd(state, to_cd);
         //to_cd.clear();
      }

      to_cd.push_back(comparator);
      //cout << "cd list is: " << to_cd << endl;
      //cout << "comparator: " << comparator << endl;
      fn_cd(state, to_cd);

      //cout << state.getcwd()->getPath() << endl;
      //state.getcwd()->getbase()->
      //mkdir(contents[contents.size() - 1], state);
      //cout << "comparing " << state.getcwd()->getPath()
      // << " and " << comparator << endl;
      //if (state.getcwd()->getPath() == comparator)
      //{
      state.getcwd()->getbase()->
         mkdir(contents[contents.size() - 1], state);
      //}
      /*
      else
      {
         cout << "EEEEEEEEEEEEEEEEEEEEEEEE" << endl;
      }
      */
   }
   else
   {
      tcwd->getbase()->mkdir(contents_, state);
   }
   //restore current directory position   
   state.getcwd() = tcwd; 
}

void fn_prompt (inode_state& state, const wordvec& words)
{
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   //cout << "Entered fn_prompt" << endl;
   string tprompt;

   if(words.size() == 1)
   {
      throw command_error("prompt : missing operand");
      //cout << "words.size() == 1" << endl;
      tprompt = state.inode_state::prompt();
   }
   else
   {
      //cout << "words.size() != 1" << endl;
      for(unsigned int i = 1; i < words.size(); i++)
      {
         tprompt += words[i] + " ";
      }
   }
   //cout << "Exit fn_prompt" << endl;
   state.inode_state::setprompt(tprompt);
}


void fn_pwd (inode_state& state, const wordvec& words)
{
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   inode_ptr tcwd = state.getcwd();
   tcwd->printPath();
   cout << endl;
}

void fn_rm (inode_state& state, const wordvec& words)
{
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   //cout << "words is: " << words << endl;
   //cout << "size of words in rm: " << words.size() << endl;
   inode_ptr tcwd = state.getcwd();
   string contents_;
   unsigned int num;
   if(words[0] == "rm")
   {
      num =1;
   }
   else 
   {
      num = 0;
   }
   //cout << "contents: " << contents_ << endl;
   for (unsigned int num = 0; num < words.size(); num++)
   {
      tcwd->getbase()->remove(words[num]);
      tcwd->getbase()->decrement_directory_size();
   }
   //cout << "its now gonna return bruh " << endl;
}

void fn_rmr (inode_state& state, const wordvec& words)
{

   string contents_;
   wordvec vector_to_cd;

   if (words[0] == "rmr")
   contents_ = words[1];
   else
   contents_ = words[0];

   // cout << "contents_ is: " << contents_ << endl;

   if (contents_ == "/" or contents_ == "")
   cout << "error: cannot remove root directory" << endl;

   vector_to_cd.push_back(contents_);
   fn_cd(state, vector_to_cd);
   //fn_ls(state, vector_to_cd);

   //invokes recursion
   for(auto it = state.getcwd()->getbase()->get_dirents().begin(); 
      it !=  state.getcwd()->getbase()->get_dirents().end(); ++it)
   {
      //if file name matches directory name, error occurs
      if (it->second->getbase()->getType() == "directory" 
         && it->first != "." && it->first != "..") 
      {
         //cout << "found new directory to
         // invoke recursion: " << it->first << endl;
         vector_to_cd.clear();
         vector_to_cd.push_back(it->first);
         fn_rmr(state, vector_to_cd);
         break;
      }
   }

   //actually does the deleting
   inode_ptr parent;
   wordvec delete_contents, parent_vec, root;
   wordvec current;
   root.push_back("/");
   current.push_back(state.getcwd()->getbase()->get_name());

   for(auto it = state.getcwd()->getbase()->get_dirents().begin();
      it !=  state.getcwd()->getbase()->get_dirents().end(); ++it)
   {
      if (it->first == "..")
      {
         parent = it->second;
         parent_vec.push_back(it->second->getPath());
      }

      delete_contents.push_back(it->first);
   }
   fn_rm(state, delete_contents);
   fn_cd(state,root);
   //move a directory up and remove that directory
   fn_cd(state, parent_vec);
   //remove the empty directory
   fn_rm(state, current);
}

void fn_ignore(inode_state& state, const wordvec& words)
{ 
}

void fn_lsr_(inode_state& state, const wordvec& words)
{
   //cout << "invoked " << endl;
   //cout << "word size: " << words.size() << endl;
   //cout << words[0] << endl;

   inode_ptr tcwd = state.getcwd();
   string contents_;
   wordvec contents;
   //if vector size is greater than 1,
   // initially calling the recursive function has a vector size of 2
   if (words.size() > 1) 
   {
      for (unsigned int num = 1; num < words.size(); num++)
      {
         //cout << "current word: " << words[num] << endl;

         if (num == words.size() - 1)
            contents_ += words[num];
         else
            contents_ += words[num] + "/";
      }
   }
   // if the words vector is a size 1, i.e during recursive calls
   else 
   {
      //cout << "contents_ = words[0]" << endl;
      contents_ = words[0];
   }

   if (contents_[0] != '/')
   {
      contents_ = "/" + contents_;
      //cout << "adding slash " << endl;
   }

   //cout << "string contents: " << contents_ << endl;

   std::istringstream ss(contents_);
   std::string token;
   string damn = "";
   string input;
   bool found_directory = false;
   wordvec vector_for_ls, cd_to_root;
   wordvec cd_to_parent;
   cd_to_parent.push_back("..");
   cd_to_root.push_back("/");

   vector_for_ls.clear();

   if (contents_ == "/")
   {
      //this is where it starts
      vector_for_ls.push_back("/"); 
      fn_cd(state, vector_for_ls);
      fn_ls(state,vector_for_ls);
      //cout << "root " << endl;
   }
   else
   {
      vector_for_ls.push_back(contents_);
      fn_cd(state, vector_for_ls);
      fn_ls(state,vector_for_ls);
   }

   for(auto it = state.getcwd()->getbase()->get_dirents().begin();
      it !=  state.getcwd()->getbase()->get_dirents().end(); ++it)
   {
      //cout << "looking at: " << it->first << endl;
      if (it->second->getbase()->getType() == "directory" 
         and it->first != "." and it->first!= "..")
      {
         vector_for_ls.clear();
         //needed since parsing loop starts at index 1
         //vector_for_ls.push_back("");
         found_directory = true;
         vector_for_ls.push_back(it->first);
         //cout << "current vector: " << vector_for_ls << endl;
         //fn_cd(state, vector_for_ls);
         //cout << "just changed directories " << endl;
         fn_lsr_(state, vector_for_ls);
         //vector_for_ls.clear();
         fn_cd(state, cd_to_parent);
         //fn_lsr(state, vector_for_ls);
         //break;
      }
      //might need to recursively call this function 
      //to keep printing out directories
      else
      {
      continue;
      }
   }

   if (found_directory == false)
   {
      //cout << "end recursion" << endl;
      //fn_cd(state, cd_to_parent);
      //cout << "current working directory: " 
      //<< state.getcwd()->getPath() << endl;
      return;
   }
}
