// $Id: shape.cpp,v 1.1 2015-07-16 16:47:51-07 - - $

#include <typeinfo>
#include <unordered_map>
#include <cmath>
using namespace std;

#include "shape.h"
#include "util.h"
#include "graphics.h"

static unordered_map<void*,string> fontname {
   {GLUT_BITMAP_8_BY_13       , "Fixed-8x13"    },
   {GLUT_BITMAP_9_BY_15       , "Fixed-9x15"    },
   {GLUT_BITMAP_HELVETICA_10  , "Helvetica-10"  },
   {GLUT_BITMAP_HELVETICA_12  , "Helvetica-12"  },
   {GLUT_BITMAP_HELVETICA_18  , "Helvetica-18"  },
   {GLUT_BITMAP_TIMES_ROMAN_10, "Times-Roman-10"},
   {GLUT_BITMAP_TIMES_ROMAN_24, "Times-Roman-24"},
};

static unordered_map<string,void*> fontcode {
   {"Fixed-8x13"    , GLUT_BITMAP_8_BY_13       },
   {"Fixed-9x15"    , GLUT_BITMAP_9_BY_15       },
   {"Helvetica-10"  , GLUT_BITMAP_HELVETICA_10  },
   {"Helvetica-12"  , GLUT_BITMAP_HELVETICA_12  },
   {"Helvetica-18"  , GLUT_BITMAP_HELVETICA_18  },
   {"Times-Roman-10", GLUT_BITMAP_TIMES_ROMAN_10},
   {"Times-Roman-24", GLUT_BITMAP_TIMES_ROMAN_24},
};

ostream& operator<< (ostream& out, const vertex& where) {
   out << "(" << where.xpos << "," << where.ypos << ")";
   return out;
}

shape::shape() {
   DEBUGF ('c', this);
}

text::text (string glut_bitmap_font, const string& textdata):
      glut_bitmap_font(glut_bitmap_font), textdata(textdata) {
}

ellipse::ellipse (GLfloat width, GLfloat height):
dimension ({width, height}) {
   DEBUGF ('c', this);
}

circle::circle (GLfloat diameter): ellipse (diameter, diameter) {
   DEBUGF ('c', this);
}

polygon::polygon (const vertex_list& vertices): vertices(vertices) {
   DEBUGF ('c', this);
}

rectangle::rectangle (GLfloat width, GLfloat height) 
   : polygon({  {-width, height}, {width, height},
      {width, -height}, {-width, -height} }) {

}

diamond::diamond(const GLfloat width, const GLfloat height) 
   : polygon({ {height / 2, 0}, {0, width / 2},
      {-height / 2, 0}, {0, -width / 2} }) {

}

triangle::triangle(GLfloat width) 
   : polygon({ {-width, 0}, {width, 0},
      {0, width} }) {

}


square::square (GLfloat width): rectangle (width, width) {
   DEBUGF ('c', this);
}

void text::draw(const vertex& center, const rgbcolor& color) {

   auto u_str = reinterpret_cast<const GLubyte*>(textdata.c_str());
   GLfloat length_in_pixels;
   GLfloat height_in_pixels;

   auto it = fontcode.find(glut_bitmap_font);
   auto ptr = color.ubvec3();
   glColor3ubv(ptr);
   glRasterPos2f(center.xpos, center.ypos);
   glutBitmapString (it->second, u_str);

   auto font_ptr = it->second;
   auto new_font_ptr = const_cast<GLubyte*>
      (reinterpret_cast<const GLubyte*>(font_ptr));

   length_in_pixels = glutBitmapLength (new_font_ptr, u_str);
   height_in_pixels = glutBitmapHeight(new_font_ptr);

   //generate coordinates for border of text obj
   vertices.push_back({center.xpos, 
      (center.ypos - (height_in_pixels))});
   vertices.push_back({center.xpos, 
      (center.ypos + (height_in_pixels))});
   vertices.push_back({center.xpos + length_in_pixels,
      center.ypos + height_in_pixels});
   vertices.push_back({center.xpos + length_in_pixels,
      center.ypos - height_in_pixels});
}

void text::draw_border(const vertex& center, const rgbcolor& color) {

   vertex center_ = center;
   glClear (GL_COLOR_BUFFER_BIT);

   GLfloat x;
   GLfloat y;
   auto ptr = color.ubvec3();
 
   glLineWidth(window::border_width);
   glBegin(GL_LINE_LOOP);
   glPolygonMode(GL_BACK, GL_LINE);
   glColor3ubv(ptr);
   for (unsigned int num = 0; num < vertices.size(); num++) {
      x = vertices[num].xpos;
      y = vertices[num].ypos;
      glVertex2f(x, y);
   }
   glEnd();

   vertices.clear();
}


void ellipse::draw(const vertex& center, const rgbcolor& color) {
   
   auto ptr = color.ubvec3();

   glBegin(GL_POLYGON);
   glEnable(GL_LINE_SMOOTH);
   glColor3ubv(ptr);

   const float delta = 2 * M_PI / 32;

   for (float theta = 0; theta < 2 * M_PI; theta += delta) {
      //(window::width / 2); 
      float xpos = dimension.xpos * cos (theta) + center.xpos;
      //(window::height / 2); 
      float ypos = dimension.ypos * sin (theta) + center.ypos;
      glVertex2f (xpos, ypos);
   }

   glEnd();
}

void ellipse::draw_border(const vertex& center, const rgbcolor& color) {
   
   auto ptr = color.ubvec3();

   glLineWidth(window::border_width);
   glBegin(GL_LINE_LOOP);
   glEnable(GL_LINE_SMOOTH);
   glColor3ubv(ptr);

   const float delta = 2 * M_PI / 32;

   for (float theta = 0; theta < 2 * M_PI; theta += delta) {
      //(window::width / 2);
      float xpos = dimension.xpos * cos (theta) + center.xpos;
      //(window::height / 2); 
      float ypos = dimension.ypos * sin (theta) + center.ypos;
      glVertex2f (xpos, ypos);
   }
   
   glEnd();
}

void polygon::draw(const vertex& center, const rgbcolor& color) {

   auto ptr = color.ubvec3();
   vertex_list vertices_ = vertices;

   GLfloat x;
   GLfloat y;
   GLfloat x_average = 0.0;
   GLfloat y_average = 0.0;

   for (unsigned int x = 0; x < vertices.size(); x++) { 
      x_average += vertices_[x].xpos;
      y_average += vertices_[x].ypos;
   }

   x_average /= vertices_.size();
   y_average /= vertices_.size();
   
   glBegin(GL_POLYGON);
   glPolygonMode(GL_BACK, GL_LINE);
   glColor3ubv(ptr);
   for (unsigned int num = 0; num < vertices.size(); num++) {
      x = (vertices_[num].xpos - x_average) + center.xpos;
      y = (vertices_[num].ypos - y_average) + center.ypos;
      glVertex2f(x, y);
   }
   glEnd();
}

void polygon::draw_border(const vertex& center, const rgbcolor& color) {

   auto ptr = color.ubvec3();
   vertex_list vertices_ = vertices;

   GLfloat x;
   GLfloat y;
   GLfloat x_average = 0.0;
   GLfloat y_average = 0.0;

   for (unsigned int x = 0; x < vertices.size(); x++) { 
      x_average += vertices_[x].xpos;
      y_average += vertices_[x].ypos;
   }

   x_average /= vertices_.size();
   y_average /= vertices_.size();
   
   glLineWidth(window::border_width);
   glBegin(GL_LINE_LOOP);
   glPolygonMode(GL_BACK, GL_LINE);
   glColor3ubv(ptr);
   for (unsigned int num = 0; num < vertices.size(); num++) {
      x = (vertices_[num].xpos - x_average) + center.xpos;
      y = (vertices_[num].ypos - y_average) + center.ypos;
      glVertex2f(x, y);
   }
   glEnd();
}

/*** Useless functions**********/
void shape::show (ostream& out) const {
   out << this << "->" << demangle (*this) << ": ";
}
/*
void text::show (ostream& out) const {
   shape::show (out);
   out << glut_bitmap_font << "(" << fontname[glut_bitmap_font]
       << ") \"" << textdata << "\"";
}
*/
void ellipse::show (ostream& out) const {
   shape::show (out);
   out << "{" << dimension << "}";
}

void polygon::show (ostream& out) const {
   shape::show (out);
   out << "{" << vertices << "}";
}

ostream& operator<< (ostream& out, const shape& obj) {
   obj.show (out);
   return out;
}
