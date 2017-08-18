// $Id: listmap.tcc,v 1.9 2016-07-20 21:00:33-07 - - $

#include "listmap.h"
#include "trace.h"
#include <iostream>

//
/////////////////////////////////////////////////////////////////
// Operations on listmap::node.
/////////////////////////////////////////////////////////////////
//

//
// listmap::node::node (link*, link*, const value_type&)
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::node::node (node* next, node* prev, const value_type& value):
         link (next, prev), value (value) 
{}

//
/////////////////////////////////////////////////////////////////
// Operations on listmap.
/////////////////////////////////////////////////////////////////
//

//
// listmap::~listmap()
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::~listmap() {

  //cout << "destructor is invoked" << endl;

  if (this->size > 0) {
     //cout << "the size of the data structure is: " << size << endl;
     auto it = begin();
     while (it != end()) {
      this->erase(it);
      it = begin();
     }
   
    this->back->next = nullptr;
    this->back->prev = nullptr;
    delete back;
  }
}

//
// iterator listmap::insert (const value_type&)
//
template <typename Key, typename Value, class Less>
void listmap<Key,Value,Less>::insert (const value_type& pair)  {

   if(find(pair.first)) //overwrite pair.second if it already exists
   (*it).second = pair.second;
    
  else {
   
   if (size == 0) //currently appends before the "back" of the circular list
      {
         back = new node(nullptr, nullptr, pair); //uses dummy pair
         node* new_node = new node(back,back,pair);
         front = new_node;
         back->prev = new_node;
         back->next = new_node;
         size++;
      }
      else //append to list in lexographic order
      {
        node* ptr = front;

        while(less(ptr->value.first, pair.first) && ptr != back) 
         ptr = ptr->next;

         //inserting based off keys
         if (ptr == back) { //if the largest node in the list
         node* new_node = new node(back,back->prev,pair);
         back->prev->next = new_node;
         back->prev = new_node;
         //cout << "largest element in list" << endl;
        }
        else if (ptr->prev == back) { //if less than first node in the list
            node* new_node = new node(ptr,back, pair);
            ptr->prev = new_node;
            back->next = new_node;
            front = new_node;
            //cout << "smallest element in list" << endl;
            }

          else { //if somewhere in between 
            node* new_node = new node(ptr,ptr->prev,pair);
            ptr->prev->next = new_node;
            ptr->prev = new_node;
            //cout << "inserting between nodes" << endl;
            }

       size++;
      }
   }
}

template <typename Key, typename Value, class Less>
listmap<Key,Value,Less> listmap<Key,Value,Less>::search_for_keys(const key_type& value) {

   listmap out;
   node* ptr = front;
   for (int num = 0; num < size; num++)
   {
      if ((ptr->value).second == value)
      out.insert(ptr->value);
   
      ptr = ptr->next;

  }

  return out;

}

//
// listmap::find(const key_type&)
//
template <typename Key, typename Value, class Less> 
bool listmap<Key,Value,Less>::find(const key_type& that) {
   
   node* ptr = this->front;

   for (int num = 0; num < size; num++) {

   //cout << "comparing: " << that << " with  " <<  (ptr->value).first << endl;

      if ((ptr->value).first == that)
      {
         //cout << "Found a match!" << endl;
         it.init_where(ptr);
         return true;
      }

      ptr = ptr->next;
   }

   return false;
}

//
// iterator listmap::erase (iterator position)
//
template <typename Key, typename Value, class Less>
void listmap<Key,Value,Less>::erase (iterator position) {

      //cout << "The key to be deleted is: " << position.where->value << endl;
 
      auto begin = this->begin();
      node* temp;

      //cout << "the begin iterator is at: " << *begin << endl;

      if (position == begin) { //The node being deleted is in the front of list

        //cout << "deleting from front" << endl;
        temp = front;
        front = front->next;
        front->prev = back;
        back->next = front;
        temp->next = nullptr;
        temp->prev = nullptr;
        delete temp;
        size--;

      }

     else { //the key being deleted is in the middle somewhere not including anchor
      
      temp = position.where;
      temp->prev->next = temp->next;
      temp->next->prev = temp->prev;
      temp->next = nullptr;
      temp->prev = nullptr;
      delete temp;
      size--;
     }
   }  

//
/////////////////////////////////////////////////////////////////
// Operations on listmap::iterator.
/////////////////////////////////////////////////////////////////
//

//
// listmap::value_type& listmap::iterator::operator*()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type&
listmap<Key,Value,Less>::iterator::operator*() {

   return where->value;
}

//
// listmap::value_type* listmap::iterator::operator->()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type*
listmap<Key,Value,Less>::iterator::operator->() {
   TRACE ('l', where);
   return &(where->value);
}

//
// listmap::iterator& listmap::iterator::operator++()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator++() {
  
   where = where->next;

   //map refers to current invoking listmap object
   //if (map->back == where) //causes circling 
   //where = where->next; //make sure to circle around if necessary

   return *this;
}

//
// listmap::iterator& listmap::iterator::operator--()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator--() {
   
   if (map->back == where) //causes circling 
   where = where->prev;

   where = where->prev;
   return *this;
}

//
// bool listmap::iterator::operator== (const iterator&)
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator==
            (const iterator& that) const {

   return this->where == that.where;
}

//
// bool listmap::iterator::operator!= (const iterator&)
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator!=
            (const iterator& that) const {

   //if (that.where == map->back)
   //return this->where != that.where->prev; //back is a special exception
   
   //else
   return this->where != that.where;
}

template <typename Key, typename Value, class Less>
void listmap<Key,Value,Less>::iterator::init_where(node* input) {
   
   this->where = input;
}

template <typename Key, typename Value, class Less>
void listmap<Key,Value,Less>::print_list() {

  for (auto it = this->begin(); it != this->end(); ++it)
  {
     cout << *it << endl;
  }
}

template <typename Key, typename Value, class Less>
void listmap<Key,Value,Less>::print_iterator() {

     cout << *it << endl;
}
