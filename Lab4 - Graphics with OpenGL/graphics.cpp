// $Id: graphics.cpp,v 1.3 2016-07-20 21:33:16-07 - - $

#include <iostream>
using namespace std;

#include <GL/freeglut.h>

#include "graphics.h"
#include "util.h"

int window::width = 640; // in pixels
int window::height = 480; // in pixels
int window_dimensions::window_height = window::height;
int window_dimensions::window_width = window::width;
int window_dimensions::pixels_move = window::pixels_to_move_by;
rgbcolor window_dimensions::border_color {255,   0,   0};
vector<object> window::objects;
size_t window::selected_obj = 0;
int window::pixels_to_move_by = 4;
int window::border_width = 4;
mouse window::mus;

// Executed when window system signals to shut down.
void window::close() {
   DEBUGF ('g', sys_info::execname() << ": exit ("
           << sys_info::exit_status() << ")");
   exit (sys_info::exit_status());
}

// Executed when mouse enters or leaves window.
void window::entry (int mouse_entered) {
   DEBUGF ('g', "mouse_entered=" << mouse_entered);
   window::mus.entered = mouse_entered;
   if (window::mus.entered == GLUT_ENTERED) {
      DEBUGF ('g', sys_info::execname() << ": width=" << window::width
           << ", height=" << window::height);
   }
   glutPostRedisplay();
}

// Called to display the objects in the window.
void window::display() {
   glClear (GL_COLOR_BUFFER_BIT);
   int i = 0;
   string number;
   vertex bounds_vertex;
   //NEEDS TO BE HERE FOR TEXT OBJECT BORDER TO RENDER
   objects[selected_obj].draw();
   //Sets the border around current selected object
   objects[selected_obj].draw_border();
   //cout << "Drawing selected object's border " << endl;
   for (auto& object: window::objects) {
      bounds_vertex = object.get_center();
      object.draw();
      number = std::to_string(i);
      object.set_object_number(number);
      object.draw_object_number();
      ++i;
   }
   glutSwapBuffers();
}

// Called when window is opened and when resized.
void window::reshape (int width, int height) {
   DEBUGF ('g', "width=" << width << ", height=" << height);
   window::width = width;
   window::height = height;
   window_dimensions::window_width = width;
   window_dimensions::window_height = height;
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D (0, window::width, 0, window::height);
   glMatrixMode (GL_MODELVIEW);
   glViewport (0, 0, window::width, window::height);
   glClearColor (0.25, 0.25, 0.25, 1.0);
   glutPostRedisplay();
}

// Executed when a regular keyboard key is pressed.
void window::keyboard (GLubyte key, int x, int y) {

   //cout << "moving by: " << pixels_to_move_by << " pixels" << endl;
   //objects[selected_obj].draw_border();
   enum {BS = 8, TAB = 9, ESC = 27, SPACE = 32, DEL = 127};
   DEBUGF ('g', "key=" << unsigned (key) << ", x=" << x << ", y=" << y);
   window::mus.set (x, y);
   switch (key) {
      case 'Q': case 'q': case ESC:
            window::close();
            break;
      case 'H': case 'h':
         {
            objects[selected_obj].move(-1 * pixels_to_move_by, 0.0);
            //objects[selected_obj].draw_border();
            break;
         }
      case 'J': case 'j':
         {
            objects[selected_obj].move(0.0, -1 * pixels_to_move_by);
            //objects[selected_obj].draw_border();
            break;
         }
      case 'K': case 'k':
         {
            objects[selected_obj].move(0.0, pixels_to_move_by);
            //objects[selected_obj].draw_border();
            break;
         }
      case 'L': case 'l':
         {
            objects[selected_obj].move(pixels_to_move_by, 0.0);
            break;
         }
      case 'N': case 'n': case SPACE: case TAB:
         {
            ++selected_obj;
            if (selected_obj == objects.size())
               selected_obj = 0;
            break;
         }
      case 'P': case 'p': case BS:
         {
            unsigned int check = -1;
            --selected_obj;
            if (selected_obj == check)
               selected_obj = objects.size() - 1;
            break;
         }
      case '0'...'9':
         {
            size_t temp_selected_obj = key - '0';
            //cout << "setting control to index: " << temp_selected_obj 
            //<< endl << "objects size: " << objects.size() << endl;
           
            if(temp_selected_obj < objects.size())
            {
               selected_obj = temp_selected_obj;
            }
            else
            {
               cerr << unsigned (key - '0') 
               << ": invalid index selection" << endl;
            }
            break;
         }
      default:
         cerr << unsigned (key) << ": invalid keystroke" << endl;
         break;
   }
   //cout << "refreshing screen" << endl;
   glClear(GL_DEPTH_BUFFER_BIT); 
   glutPostRedisplay();
}

// Executed when a special function key is pressed.
void window::special (int key, int x, int y) {
   DEBUGF ('g', "key=" << key << ", x=" << x << ", y=" << y);
   window::mus.set (x, y);
   switch (key) {
      case GLUT_KEY_LEFT: //move_selected_object (-1, 0); break;
         {
            objects[selected_obj].move(-1 * pixels_to_move_by, 0.0);
            //objects[selected_obj].draw_border();
            break;
         }
      case GLUT_KEY_DOWN: //move_selected_object (0, -1); break;
         {
            objects[selected_obj].move(0.0, -1 * pixels_to_move_by);
            //objects[selected_obj].draw_border();
            break;
         }
      case GLUT_KEY_UP: //move_selected_object (0, +1); break;
         {
            objects[selected_obj].move(0.0, pixels_to_move_by);
            //objects[selected_obj].draw_border();
            break;
         }
      case GLUT_KEY_RIGHT: //move_selected_object (+1, 0); break;
         {
            objects[selected_obj].move(pixels_to_move_by, 0.0);
            break;
         }
      case GLUT_KEY_F1: //select_object (1); break;
         {
            size_t temp_selected_obj = 1;
            //cout << "setting control to index: " << temp_selected_obj 
            //<< endl << "objects size: " << objects.size() << endl;
           
            if(temp_selected_obj <= objects.size())
            {
               selected_obj = 0;
            }
            else
            {
               cerr << "Error: invalid index selection" << endl;
            }           
            break;
         }
      case GLUT_KEY_F2: //select_object (2); break;
         {
            size_t temp_selected_obj = 2;
            //cout << "setting control to index: " << temp_selected_obj 
            //<< endl << "objects size: " << objects.size() << endl;
           
            if(temp_selected_obj <= objects.size())
            {
               selected_obj = 1;
            }
            else
            {
               cerr << "Error: invalid index selection" << endl;
            }           
            break;
         }
      case GLUT_KEY_F3: //select_object (3); break;
         {
            size_t temp_selected_obj = 3;
            //cout << "setting control to index: " << temp_selected_obj 
            //<< endl << "objects size: " << objects.size() << endl;
           
            if(temp_selected_obj <= objects.size())
            {
               selected_obj = 2;
            }
            else
            {
               cerr << "Error: invalid index selection" << endl;
            }           
            break;
         }
      case GLUT_KEY_F4: //select_object (4); break;
         {
            size_t temp_selected_obj = 4;
            //cout << "setting control to index: " << temp_selected_obj 
            //<< endl << "objects size: " << objects.size() << endl;
           
            if(temp_selected_obj <= objects.size())
            {
               selected_obj = 3;
            }
            else
            {
               cerr << "Error: invalid index selection" << endl;
            }           
            break;
         }
      case GLUT_KEY_F5: //select_object (5); break;
         {
            size_t temp_selected_obj = 5;
            //cout << "setting control to index: " << temp_selected_obj 
            //<< endl << "objects size: " << objects.size() << endl;
           
            if(temp_selected_obj <= objects.size())
            {
               selected_obj = 4;
            }
            else
            {
               cerr << "Error: invalid index selection" << endl;
            }           
            break;
         }
      case GLUT_KEY_F6: //select_object (6); break;
         {
            size_t temp_selected_obj = 6;
            //cout << "setting control to index: " << temp_selected_obj 
            //<< endl << "objects size: " << objects.size() << endl;
           
            if(temp_selected_obj <= objects.size())
            {
               selected_obj = 5;
            }
            else
            {
               cerr << "Error: invalid index selection" << endl;
            }           
            break;
         }
      case GLUT_KEY_F7: //select_object (7); break;
         {
            size_t temp_selected_obj = 7;
            //cout << "setting control to index: " << temp_selected_obj 
            //<< endl << "objects size: " << objects.size() << endl;
           
            if(temp_selected_obj <= objects.size())
            {
               selected_obj = 6;
            }
            else
            {
               cerr << "Error: invalid index selection" << endl;
            }           
            break;
         }
      case GLUT_KEY_F8: //select_object (8); break;
         {
            size_t temp_selected_obj = 8;
            //cout << "setting control to index: " << temp_selected_obj 
            //<< endl << "objects size: " << objects.size() << endl;
           
            if(temp_selected_obj <= objects.size())
            {
               selected_obj = 7;
            }
            else
            {
               cerr << "Error: invalid index selection" << endl;
            }           
            break;
         }
      case GLUT_KEY_F9: //select_object (9); break;
         {
            size_t temp_selected_obj = 9;
            //cout << "setting control to index: " << temp_selected_obj 
            //<< endl << "objects size: " << objects.size() << endl;
           
            if(temp_selected_obj <= objects.size())
            {
               selected_obj = 8;
            }
            else
            {
               cerr << "Error: invalid index selection" << endl;
            }           
            break;
         }
      case GLUT_KEY_F10: //select_object (10); break;
         {
            size_t temp_selected_obj = 10;
            //cout << "setting control to index: " << temp_selected_obj 
            //<< endl << "objects size: " << objects.size() << endl;
           
            if(temp_selected_obj <= objects.size())
            {
               selected_obj = 9;
            }
            else
            {
               cerr << "Error: invalid index selection" << endl;
            }           
            break;
         }
      case GLUT_KEY_F11: //select_object (11); break;
         {
            size_t temp_selected_obj = 11;
            //cout << "setting control to index: " << temp_selected_obj 
            //<< endl << "objects size: " << objects.size() << endl;
           
            if(temp_selected_obj <= objects.size())
            {
               selected_obj = 10;
            }
            else
            {
               cerr << "Error: invalid index selection" << endl;
            }           
            break;
         }
      case GLUT_KEY_F12: //select_object (12); break;
         {
            size_t temp_selected_obj = 12;
            //cout << "setting control to index: " << temp_selected_obj 
            //<< endl << "objects size: " << objects.size() << endl;
           
            if(temp_selected_obj <= objects.size())
            {
               selected_obj = 11;
            }
            else
            {
               cerr << "Error: invalid index selection" << endl;
            }           
            break;
         }
      default:
         cerr << unsigned (key) << ": invalid function key" << endl;
         break;
   }
   glutPostRedisplay();
}

void window::motion (int x, int y) {
   DEBUGF ('g', "x=" << x << ", y=" << y);
   window::mus.set (x, y);
   glutPostRedisplay();
}

void window::passivemotion (int x, int y) {
   DEBUGF ('g', "x=" << x << ", y=" << y);
   window::mus.set (x, y);
   glutPostRedisplay();
}

void window::mousefn (int button, int state, int x, int y) {
   DEBUGF ('g', "button=" << button << ", state=" << state
           << ", x=" << x << ", y=" << y);
   window::mus.state (button, state);
   window::mus.set (x, y);
   glutPostRedisplay();
}

void window::set_new_center(string obj_name, vertex v) {

   for (unsigned int num = 0; num < objects.size(); num++) {
      
      if (objects[num].get_name() == obj_name)
      {
         objects[num].set_new_center(v);
      }
  }
}

void window::set_color(string obj_name, const rgbcolor& color) {

   for (unsigned int num = 0; num < objects.size(); num++) {
      
   if (objects[num].get_name() == obj_name)
   {
        objects[num].set_new_color(color);
   }
  }
}

void window::main () {
   //cout << "CALLING WINDOW::MAIN" << endl;
   static int argc = 0;
   glutInit (&argc, nullptr);
   glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE);
   glutInitWindowSize (window::width, window::height);
   glutInitWindowPosition (128, 128);
   glutCreateWindow (sys_info::execname().c_str());
   //glutCloseFunc (window::close);
   //glutEntryFunc (window::entry);
   glutSpecialFunc (window::special);
   glutKeyboardFunc (window::keyboard);
   glutDisplayFunc (window::display);
   glutReshapeFunc (window::reshape);
   //glutKeyboardFunc (window::keyboard);
   //glutSpecialFunc (window::special);
   //glutMotionFunc (window::motion);
   //glutPassiveMotionFunc (window::passivemotion);
   //glutMouseFunc (window::mousefn);
   DEBUGF ('g', "Calling glutMainLoop()");
   glutMainLoop();
}

void mouse::state (int button, int state) {
   switch (button) {
      case GLUT_LEFT_BUTTON: left_state = state; break;
      case GLUT_MIDDLE_BUTTON: middle_state = state; break;
      case GLUT_RIGHT_BUTTON: right_state = state; break;
   }
}

void mouse::draw() {
   static rgbcolor color ("green");
   ostringstream text;
   text << "(" << xpos << "," << window::height - ypos << ")";
   if (left_state == GLUT_DOWN) text << "L"; 
   if (middle_state == GLUT_DOWN) text << "M"; 
   if (right_state == GLUT_DOWN) text << "R"; 
   if (entered == GLUT_ENTERED) {
      void* font = GLUT_BITMAP_HELVETICA_18;
      glColor3ubv (color.ubvec);
      glRasterPos2i (10, 10);
      auto ubytes = reinterpret_cast<const GLubyte*>
                    (text.str().c_str());
      glutBitmapString (font, ubytes);
   }
}

void object::move (GLfloat delta_x, GLfloat delta_y) {
   center.xpos += delta_x;
   center.ypos += delta_y;
   if (center.xpos == window_dimensions::window_width)
      center.xpos = window_dimensions::pixels_move;
   else if (center.xpos == 0) 
      center.xpos = 
         window_dimensions::window_width
            - window_dimensions::pixels_move;
   else if (center.ypos == window_dimensions::window_height) 
      center.ypos = window_dimensions::pixels_move;
   else if (center.ypos == 0)
      center.ypos = 
         window_dimensions::window_height
            - window_dimensions::pixels_move;
}
