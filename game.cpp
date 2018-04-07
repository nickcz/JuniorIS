#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include "GL/freeglut.h"
#endif

#include "stlReader.cpp"

// Name: Nicholas Czaban
// Assignment: Final Project 
// Title: CSG Asteroid Game
// Course: CS 200 
// Semester: Spring year
// Instructor: D. Byrnes
// Date: 2/21/18
// Sources consulted: any books consulted in writing the program
// Program description: A simple game where the user navigates a spaceship through an asteroid.
// Known bugs: 
// Creativity: 
// Instructions: Compile and run on the command line.

GLint window_width = 700;
GLint window_height = 700;
static GLfloat xpos = 0;
static GLfloat ypos = 0;
static GLfloat zpos = 0;
static GLfloat speed = 0.2;

GLfloat light_specular[] = {1, 1, 1, 1.0};
GLfloat light_position[] = {0.0, 3.0, -5.0, 0.0};

std::vector<Facet> asteroids = readFile("asteroid.stl");
std::vector<Facet> pipe = readFile("pipes.stl");
std::vector<Facet> spaceship = readFile("spaceship.stl");
std::vector<Facet> connector = readFile("connector.stl");

bool upKey, leftKey, rightKey, downKey;
float r1, r2, r3;

void flight(){
  // Change spaceship position
  zpos -= speed;
  if(zpos <= -100){
	r1 = float(rand())/RAND_MAX;
	r2 = float(rand())/RAND_MAX;
	r3 = float(rand())/RAND_MAX;
	zpos = 0;
	speed += 0.01;
  }
  if(leftKey){
	if(xpos > -10) xpos -= 0.2;
	else xpos = -10;
  }
  if(rightKey){
	if(xpos < 10) xpos += 0.2;
	else xpos = 10;
  }
  if(upKey){
	if(ypos < 10) ypos += 0.2;
	else ypos = 10;	
  }
  if(downKey){
	if(ypos > -10) ypos -= 0.2;
	else ypos = -10;
  }
  glutPostRedisplay();
}

void setup_scene(){
  glTranslatef(0,0,-10);
  gluLookAt(xpos, ypos+1, 5,    xpos, ypos, 0,     0,1,0);

  // Draw the spaceship
  glPushMatrix();
  glColor3f(0.4, 0.4, 0.4);
  glTranslatef(xpos, ypos, 1);
  if(leftKey) glRotatef(15, 0, 0, 1);
  if(rightKey) glRotatef(-15, 0, 0, 1);
  if(upKey) glRotatef(15, 1, 0, 0);
  if(downKey) glRotatef(-15, 1, 0, 0);
  
  glBegin(GL_TRIANGLES);
  for(int i = 0; i<spaceship.size(); i++){
	glNormal3f(spaceship[i].normal[0], spaceship[i].normal[1], spaceship[i].normal[2]);
	if((spaceship[i].normal[2] == -1) && (spaceship[i].vertices[0][2] < -1.5)) glColor3f(.4, .6, 1);
	for(int j = 2; j>=0; j--){
	  glVertex3f(spaceship[i].vertices[j][0], spaceship[i].vertices[j][1], -1*spaceship[i].vertices[j][2]);
	}
	glColor3f(0.4, 0.4, 0.4);
  }
  glEnd();
  glPopMatrix();

  // Draw a pipe
  glPushMatrix();
  glTranslatef((20*r3)-10, (20*r1)-10, -25-zpos);
  glRotatef(-1.5*zpos, r1, r2, r3);
  glColor3f(0.6, 0.6, 0.6);
  glBegin(GL_TRIANGLES);
  for(int i = 0; i<pipe.size(); i++){
	glNormal3f(-1*pipe[i].normal[0], pipe[i].normal[1], pipe[i].normal[2]);
	for(int j = 2; j>=0; j--){
	  glVertex3f(pipe[i].vertices[j][0], pipe[i].vertices[j][1], -1*pipe[i].vertices[j][2]);
	}
  }
  glEnd();
  glPopMatrix();

  // Draw an asteroid
  glPushMatrix();
  glTranslatef((20*r1)-10, (20*r2)-10, -50-zpos);
  glRotatef(-1.5*zpos, -r3, r1, r2);
  glColor3f(0.5, 0.25, 0.25);
  glBegin(GL_TRIANGLES);
  for(int i = 0; i<asteroids.size(); i++){
  	glNormal3f(-1*asteroids[i].normal[0], asteroids[i].normal[1], asteroids[i].normal[2]);
  	for(int j = 2; j>=0; j--){
  	  glVertex3f(asteroids[i].vertices[j][0], asteroids[i].vertices[j][1], -1*asteroids[i].vertices[j][2]);
  	}
  }
  glEnd();
  glPopMatrix();

  // Draw a hollow die
  glPushMatrix();
  glTranslatef((20*r2)-10, (20*r1)-10, -75-zpos);
  glRotatef(-1.5*zpos, r2, -r3, r1);
  glColor3f(0.25, 0.5, 0);
  glBegin(GL_TRIANGLES);
  for(int i = 0; i<connector.size(); i++){
  	glNormal3f(-1*connector[i].normal[0], connector[i].normal[1], connector[i].normal[2]);
  	for(int j = 2; j>=0; j--){
  	  glVertex3f(connector[i].vertices[j][0], connector[i].vertices[j][1], -1*connector[i].vertices[j][2]);
  	}
  }
  glEnd();
  glPopMatrix();
}

void display (void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	setup_scene();
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glutSwapBuffers();
}

void init(){
  r1 = float(rand())/RAND_MAX;
  r2 = float(rand())/RAND_MAX;
  r3 = float(rand())/RAND_MAX;

  glClearColor(0.1, 0.1, 0.1, 1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(85, window_width/window_height, 1, 50);
  
  glShadeModel(GL_SMOOTH);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
}

void reshape(int w, int h){
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(85, GLfloat(w)/GLfloat(h), 1, 50);

  glutPostRedisplay();
}

void key(unsigned char keystroke, int x, int y){
  switch(keystroke){
  case 'q':
	exit(0);
	break;
  case 'a':
	leftKey = true;
	break;
  case 'd':
	rightKey = true;
	break;
  case 'w':
	upKey = true;
	break;
  case 's':
	downKey = true;
	break;
  }
}
void keyUp(unsigned char keystroke, int x, int y){
  switch(keystroke){
  case 'a':
	leftKey = false;
	break;
  case 'd':
	rightKey = false;
	break;
  case 'w':
	upKey = false;
	break;
  case 's':
	downKey = false;
	break;
  }
}

int main(int argc, char **argv){
  srand(time(NULL));  
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(window_width, window_height);
  glutInitWindowPosition(10, 10);
  glutCreateWindow("Spaceship");
  init();

  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutIdleFunc(flight);
  glutKeyboardFunc(key);
  glutKeyboardUpFunc(keyUp);
  glutSetKeyRepeat(0);
  glutMainLoop();
  return 0;
}
