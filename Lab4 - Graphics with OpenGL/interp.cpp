// $Id: interp.cpp,v 1.2 2015-07-16 16:57:30-07 - - $

#include <memory>
#include <string>
#include <vector>
using namespace std;

#include <GL/freeglut.h>

#include "debug.h"
#include "interp.h"
#include "shape.h"
#include "util.h"

unordered_map<string,interpreter::interpreterfn>
interpreter::interp_map {
   {"define" , &interpreter::do_define },
   {"draw"   , &interpreter::do_draw   },
};

unordered_map<string,interpreter::factoryfn>
interpreter::factory_map {
   {"text"     , &interpreter::make_text     },
   {"ellipse"  , &interpreter::make_ellipse  },
   {"circle"   , &interpreter::make_circle   },
   {"polygon"  , &interpreter::make_polygon  },
   {"rectangle", &interpreter::make_rectangle},
   {"square"   , &interpreter::make_square   },
   {"triangle" , &interpreter::make_polygon  },
   {"diamond"  , &interpreter::make_diamond  },
   {"equilateral", &interpreter::make_triangle},
};

interpreter::shape_map interpreter::objmap;
static rgbcolor color_for_border;

interpreter::~interpreter() {
   for (const auto& itor: objmap) {
      cout << "objmap[" << itor.first << "] = "
           << *itor.second << endl;
   }
}

void interpreter::interpret (const parameters& params) {
   shape_ptr pointer_for_window;
   string obj_name = "";
   param begin = params.cbegin();
   string command = *begin;
   //cout << "The command is: " << command << endl;
   if (command == "moveby") {
      ++begin;
      window::pixels_to_move_by = std::strtof((*begin).c_str(), 0);
      return;
   }  
   if (command == "border") {
      rgbcolor new_color {begin[1]};
      //cout << "The inputted color is: " << new_color << endl;
      window_dimensions::border_color = new_color;
      //cout << "The color of the border will be: " 
      //<< window_dimensions::border_color << endl;
      window::border_width = std::strtof((begin[2]).c_str(), 0);
      return;
   } 
   auto itor = interp_map.find (command);
   if (itor == interp_map.end()) throw runtime_error ("syntax error");
   interpreterfn func = itor->second;
   func(++begin, params.cend());
}

void interpreter::do_define (param begin,param end) {
   DEBUGF ('f', range (begin, end));
   string name = *begin;
   shape_ptr pointer = make_shape(++begin, end);
   objmap.emplace(name, pointer);
   object new_object(name, pointer);
   window::push_back(new_object);
}

void interpreter::do_draw (param begin, param end) {

   if (end - begin != 4) throw runtime_error ("syntax error");
   string name = begin[1];
   shape_map::const_iterator itor = objmap.find (name);
   if (itor == objmap.end()) {
      cerr << name  << ": has not been defined yet!" << endl;
   }

   rgbcolor color {begin[0]}; 
   GLfloat begin_2 = std::strtof((begin[2]).c_str(), 0);
   GLfloat begin_3 = std::strtof((begin[3]).c_str(), 0);
   vertex where {begin_2, begin_3}; 

   window::set_new_center(name, where);
   window::set_color(name, color); 
   //itor->second->draw(where, color);
}

shape_ptr interpreter::make_shape (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   string type = *begin++;
   auto itor = factory_map.find(type);
   if (itor == factory_map.end()) {
      throw runtime_error (type + ": is not a shape!");
   }
   factoryfn func = itor->second;
   return func (begin, end);
}

shape_ptr interpreter::make_text (param begin, param end) {

   string text_field = "";
   string font_type = *begin;
   ++begin;

   while (begin != end) {
      text_field = text_field + (*begin);
      text_field += " ";
      ++begin;
   }

   return make_shared<text>(font_type, text_field);
}

shape_ptr interpreter::make_ellipse (param begin, param end) {

   
   GLfloat width = std::strtof((*begin).c_str(), 0);
   ++begin;
   GLfloat height = std::strtof((*begin).c_str(), 0);

   return make_shared<ellipse> (width, height);
}

shape_ptr interpreter::make_circle (param begin, param end) {
   
   GLfloat radius = std::strtof((*begin).c_str(), 0);

   if (++begin != end)
      throw runtime_error 
         ("Define failed: incorrect input for making a circle");

   return make_shared<circle> (radius);
}

shape_ptr interpreter::make_polygon (param begin, param end) {

   vertex_list list;
   GLfloat x_coordinate;
   GLfloat y_coodrinate; 

   while (begin != end) { 
      x_coordinate = std::strtof((*begin).c_str(), 0);
      ++begin;
      y_coodrinate = std::strtof((*begin).c_str(), 0);
      ++begin;
      list.push_back({x_coordinate, y_coodrinate});
   }

   return make_shared<polygon>(list);
}

shape_ptr interpreter::make_rectangle (param begin, param end) {
   
   GLfloat width = std::strtof((*begin).c_str(), 0);
   ++begin;
   GLfloat height = std::strtof((*begin).c_str(), 0);

   return make_shared<rectangle> (width, height);
}

shape_ptr interpreter::make_square (param begin, param end) {

   GLfloat side = std::strtof((*begin).c_str(), 0);

   if (++begin != end)
      throw runtime_error 
         ("Define failed: incorrect input for making a square");

   return make_shared<square>(side);
}

shape_ptr interpreter::make_diamond(param begin, param end) {

   GLfloat width = std::strtof((*begin).c_str(), 0);
   ++begin;
   GLfloat height = std::strtof((*begin).c_str(), 0);

   return make_shared<diamond> (width, height);

}

shape_ptr interpreter::make_triangle(param begin, param end) {

   GLfloat width = std::strtof((*begin).c_str(), 0);

   return make_shared<triangle>(width);
}
