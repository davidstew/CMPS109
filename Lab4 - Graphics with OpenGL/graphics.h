// $Id: graphics.h,v 1.1 2015-07-16 16:47:51-07 - - $

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <memory>
#include <vector>
using namespace std;

#include <GL/freeglut.h>

#include "rgbcolor.h"
#include "shape.h"
#include "interp.h"

struct window_dimensions {

static int window_width;
static int window_height;
static int pixels_move;
static rgbcolor border_color;

};

class object {
   private:
      shared_ptr<shape> pshape;
      shared_ptr<shape> obj_number;
      vertex center { 320, 240 };
      rgbcolor color;
      rgbcolor border_color;
      rgbcolor text_color {255, 255, 255};
      string name;
   public:
      // Default copier, movers, dtor all OK.
      object(string name_, shared_ptr<shape> ptr) 
         : pshape(ptr), name(name_) {}
      void set_new_center(vertex v) { 
         center.xpos = v.xpos; center.ypos = v.ypos; }
      vertex get_center() { return center; }
      void set_new_color(const rgbcolor& input) { 
         color = input; border_color = input; }
      void set_object_number(string num) { 
         obj_number = make_shared<text>("Fixed-8x13", num); }
      void draw_object_number() { 
         obj_number->draw(center, text_color); }
      string get_name() { return name; }
      void draw() { pshape->draw(center, color); }
      void draw_border() { 
         pshape->draw_border(center, 
            window_dimensions::border_color); }
      void clear_border() { border_color = color; }
      void move (GLfloat delta_x, GLfloat delta_y);
};

class mouse {
      friend class window;
   private:
      int xpos {0};
      int ypos {0};
      int entered {GLUT_LEFT};
      int left_state {GLUT_UP};
      int middle_state {GLUT_UP};
      int right_state {GLUT_UP};
   private:
      void set (int x, int y) { xpos = x; ypos = y; }
      void state (int button, int state);
      void draw();
};

class window {
      friend class mouse;
   private:
      static vector<object> objects;
      static size_t selected_obj;
      static mouse mus;
   private:
      static void close();
      static void entry (int mouse_entered);
      static void display();
      static void reshape (int width, int height);
      static void keyboard (GLubyte key, int, int);
      static void special (int key, int, int);
      static void motion (int x, int y);
      static void passivemotion (int x, int y);
      static void mousefn (int button, int state, int x, int y);
   public:
      static void set_new_center(string obj_name, vertex v);
      static void set_color(string obj_name, const rgbcolor& color);
      static void push_back (const object& obj) {
                  objects.push_back (obj); }
      static void setwidth (int width_) { width = width_; }
      static void setheight (int height_) { height = height_; }
      static void main();
      static int width;         // in pixels
      static int height;        // in pixels
      static int pixels_to_move_by;
      static rgbcolor color_for_border;
      static int border_width;
};

#endif
