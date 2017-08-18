// $Id: listmap.h,v 1.13 2017-07-17 15:16:35-07 - - $

#ifndef __LISTMAP_H__
#define __LISTMAP_H__

#include "xless.h"
#include "xpair.h"

template <typename Key, typename Value, class Less=xless<Key>>
class listmap {
   public:
      using key_type = Key;
      using mapped_type = Value;
      using value_type = xpair<const key_type, mapped_type>;
      class iterator;
   private:
      iterator it;
      Less less;
      int size {};
      struct node;
      node* front;
      node* back;
      struct link {
         node* next{};
         node* prev{};
         link (node* next, node* prev): next(next), prev(prev){}
      };
      struct node : link {  
         node (node* next, node* prev, const value_type&); 
         value_type value{};
      };

   public:
      listmap(){};
      listmap (const listmap&);
      listmap& operator= (const listmap&);
      ~listmap();
      void insert (const value_type&) ; //used to return iterator
      bool find (const key_type&) ; //used to return iterator
      void erase (iterator position);
      iterator begin() { iterator it(this, front); return it; } 
      iterator end() {iterator it(this, back); return it; }
      bool empty() const { return begin() == end(); }
      void print_list();
      void print_iterator();
      listmap search_for_keys(const key_type&);
      listmap search_for_values(const value_type&);
      iterator get_iterator() {return it; }
};

template <typename Key, typename Value, class Less>
class listmap<Key,Value,Less>::iterator {
   private:
      friend class listmap<Key,Value>;
      listmap<Key,Value,Less>::node* where {nullptr};
      listmap* map {nullptr};
      iterator (listmap* map, node* where): where(where), map(map){};
   public:
      iterator(){}
      value_type& operator*();
      value_type* operator->();
      iterator& operator++(); //++itor
      iterator& operator--(); //--itor
      void erase();
      bool operator== (const iterator&) const;
      bool operator!= (const iterator&) const;
      void init_where(node* input);
};

#include "listmap.tcc"
#endif
