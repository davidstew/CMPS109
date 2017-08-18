// $Id: commands.cpp,v 1.16 2016-01-14 16:10:40-08 - - $

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
   {"#"     , fn_ignore}
};

command_fn find_command_fn (const string& cmd) {
   // Note: value_type is pair<const key_type, mapped_type>
   // So: iterator->first is key_type (string)
   // So: iterator->second is mapped_type (command_fn)
   const auto result = cmd_hash.find (cmd);
   if (result == cmd_hash.end()) {
      throw command_error ("yshell: " + cmd + ": no such function");

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
   //DEBUGF ('c', state);
   //DEBUGF ('c', words);
   inode_ptr tcwd = state.getcwd();

   for (unsigned int num = 1; num < words.size(); num++)
        tcwd->getbase()->readfile(words[num]);
}

void fn_cd (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   int num1;

   //stuff is for lsr command
   if(words[0] != "cd" && words[0] != "")
    num1 = 0;
   else
    num1 = 1;

    cout << "directory: " << words << endl;

    //inode_ptr tcwd = state.getcwd();
    cout << "current directory: ";
    state.getcwd()->printPath();
    cout << endl;

    string contents_, input;
    wordvec contents;
    bool found_directory = false;


    for (; num1 < words.size(); num1++)
    {
        if (num1 == words.size() - 1)
        contents_ += words[num1];
        else
        contents_ += words[num1] + " ";
    }

    if (contents_[0] != '/')
    {
        //cout << "prepended slash" << endl;
        contents_ = "/" + contents_;
    }

    //if (contents_ == "/..")
    //cout << "changing to parent directory" << endl;

    //cout << "stuff: " << contents_ << endl;

    std::istringstream ss(contents_);
	std::string token;

	while(std::getline(ss, token, '/')) {
        //cout << "pushing back: " << token << endl;
		contents.push_back(token);
		//std::cout << token << '\n';
	}

    //cout << "stuff: " << contents << endl;

    for (int num = 1; num < contents.size(); ++num)
    {
        //cout << "inside outer loop" << endl;
        if (contents[num] == "")
        input = ".";
        else
        input = contents[num];
        //cout << "current input to find: "  << input << endl;

        //looking for specific path located in directory's map structure
        for(auto it = state.getcwd()->getbase()->get_dirents().begin(); it !=  state.getcwd()->getbase()->get_dirents().end(); ++it)
        {
            //cout << "checking map: " << it->first << endl;

            if (it->first == input)
            {
                found_directory = true;
                if (it->second->getbase()->getType() == "directory")
                {
                //cout << "found " << it->first << endl;
                //cout << "current it->second: " << it->second->getPath() << endl;
                state.getcwd() = it->second;
                //cout << "changed directory to: " << state.getcwd()->getPath() << endl;
                break;
                }
                else
                {
                cout << "not a directory" << endl;
                break;
                }
            }
        }

        if (found_directory == false)
        cout << "cd: " << contents_ << " No such file or directory" << endl;
    }

    cout << "new directory: ";
    state.getcwd()->printPath();
    cout << endl;
}

void fn_echo (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   cout << word_range (words.cbegin() + 1, words.cend()) << endl;
}

void fn_exit (inode_state& state, const wordvec& words){
   //DEBUGF ('c', state);
   //DEBUGF ('c', words);
   //throw ysh_exit();


}

void fn_ls (inode_state& state, const wordvec& words){
   //DEBUGF ('c', state);
   //DEBUGF ('c', words);
   inode_ptr tcwd = state.getcwd();

   //if user enters nothing following ls
   if (words.size() == 1)
   {
     tcwd->printPath();
     cout << ":" << endl;
     tcwd->getbase()->print_dirents();
   }

   //need to figure out if / implies a fn_cd to root directory
   else
   {
       if (words[1] == "/")
       {
           cout << "/:" << endl;
       }
       else if (words[1] == ".")
        cout << ".:" << endl;
       else
       {
       cout << "inside ls" << endl;
       tcwd->printPath();
       cout << ":" << endl;
       }

    tcwd->getbase()->print_dirents();
   }

}

void fn_lsr (inode_state& state, const wordvec& words){
   //DEBUGF ('c', state);
   //DEBUGF ('c', words);
   cout << "invoked " << endl;

   inode_ptr tcwd = state.getcwd();
   string contents_;
   wordvec contents;

    for (int num = 1; num < words.size(); num++)
    {
        if (num == words.size() - 1)
        contents_ += words[num];
        //else
        //contents_ += words[num] + " ";
    }

    if (contents_[0] != '/')
    {
        contents_ = "/" + contents_;
        //cout << "adding slash " << endl;
    }

    std::istringstream ss(contents_);
	std::string token;

	while(std::getline(ss, token, '/')) {
        //cout << "pushing back: " << token << endl;
		contents.push_back(token);
		std::cout << token << '\n';
	}
/*
	cout << "contents: " << contents_ << endl;
	cout << "path: " << tcwd->getPath() << endl;

	  if (tcwd->getPath() != contents_)
      {
          cout << "FFFFFFFFFFFF" << endl;
          fn_cd(state, contents);
          tcwd = state.getcwd();
      }
*/
    wordvec vector_for_ls;
    wordvec cd_to_parent;
    cd_to_parent.push_back("..");
    //cd_to_parent.push_back(".."); //must push back twice
    string input;
    bool found_directory;
    int num;



     for (int num = 0; num < contents.size(); num++)
    {
        input = contents[num];
        vector_for_ls.push_back(""); //this is where it starts
        fn_ls(state,vector_for_ls);

        for(auto it = tcwd->getbase()->get_dirents().begin(); it !=  tcwd->getbase()->get_dirents().end(); ++it)
        {
            if (it->second->getbase()->getType() == "directory" and it->first != "." and it->first!= "..")
            {

                vector_for_ls.clear();
                cout << "loop directory: " << it->first << endl;
                found_directory = true;
                vector_for_ls.push_back(it->first);
                //cout << "current vector: " << vector_for_ls << endl;
                fn_cd(state, vector_for_ls);
                //cout << "just changed directories " << endl;
                fn_lsr(state, vector_for_ls);
                //vector_for_ls.clear();
                fn_cd(state, cd_to_parent);
                //fn_lsr(state, vector_for_ls);
            }
            //might need to recursively call this function to keep printing out directories
            else
            {
                continue;
            }
        }

        if (found_directory == false)
        {
        vector_for_ls.push_back("");
        fn_cd(state, cd_to_parent);
        return;
        }
    }
}

void fn_make (inode_state& state, const wordvec& words){
   //DEBUGF ('c', state);
   //DEBUGF ('c', words);
   string filename = words[1]; //name of file
   string contents_ = "";

   for (int num = 2; num < words.size(); num++)
   {
        if (num == words.size() - 1)
        contents_ += words[num];
        else
        contents_ += words[num] + " ";
   }

   wordvec temp_contents;
   temp_contents.push_back(contents_);

   inode_ptr tcwd = state.getcwd();
   inode_ptr file_ptr = nullptr;

   tcwd->getbase()->mkfile(filename);
   tcwd->getbase()->writefile(temp_contents);
}

void fn_mkdir (inode_state& state, const wordvec& words){

   //DEBUGF ('c', state);
   //DEBUGF ('c', words);

    inode_ptr tcwd = state.getcwd();

    string contents_;
    wordvec contents;
    wordvec to_cd;

    for (int num = 1; num < words.size(); num++)
    {
        if (num == words.size() - 1)
        contents_ += words[num];
        else
        contents_ += words[num] + " ";
    }

    std::istringstream ss(contents_);
	std::string token;

    while(std::getline(ss, token, '/')) {
		contents.push_back(token);
	}

	//cout << "contents: " << contents << endl;

	//if path is longer than one directory, we must do a series of directory changes
	if (contents.size() > 1)
    {
        for (int num = 0; num < (contents.size() - 1); num++)
        {
             //cout << "vector contents: " << contents[num] << endl;
             to_cd.push_back(contents[num]);
             fn_cd(state, to_cd);
             to_cd.clear();
             cout << "changed directory to: " << state.getcwd()->getPath() << endl;
        }

        tcwd->getbase()->mkdir(contents[contents.size() - 1], state);
    }
    else
    {
      tcwd->getbase()->mkdir(contents_, state);
    }

	state.getcwd() = tcwd; //restore current directory position
}

void fn_prompt (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   //cout << "Entered fn_prompt" << endl;
   string tprompt;

   if(words.size() == 1)
   {
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


void fn_pwd (inode_state& state, const wordvec& words){
   //DEBUGF ('c', state);
   //DEBUGF ('c', words);
     inode_ptr tcwd = state.getcwd();
     tcwd->printPath();
     cout << endl;
}

void fn_rm (inode_state& state, const wordvec& words){
   //DEBUGF ('c', state);
   //DEBUGF ('c', words);
    inode_ptr tcwd = state.getcwd();
    string contents_;

    for (int num = 1; num < words.size(); num++)
    {
        if (num == words.size() - 1)
        contents_ += words[num];
        else
        contents_ += words[num] + " ";
    }

    tcwd->getbase()->remove(contents_);
}

void fn_rmr (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_ignore(inode_state& state, const wordvec& words) {}

