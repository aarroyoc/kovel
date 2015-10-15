#include "gl1.hpp"

#include <GL/gl.h>

GlPreviewer::GlPreviewer()
{
	
}

void GlPreviewer::Render(int width, int height)
{
	// USE OLD OPENGL 1 code
	glClearColor(0.0,0.0,0.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glViewport(0, 0, width, height);
	
	// Isometric
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(35.264f, 1.0f, 0.0f, 0.0f);
	glRotatef(-45.0f, 0.0f, 1.0f, 0.0f);
	
	// Draw AXIS
	glColor3f(1.0,0.0,0.0);
	
	glBegin(GL_LINES);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(100.0,0.0,0.0);
	glEnd();
	
	glColor3f(0.0,1.0,0.0);
	
	glBegin(GL_LINES);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(0.0,100.0,0.0);
	glEnd();
	
	glColor3f(0.0,0.0,1.0);
	
	glBegin(GL_LINES);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(0.0,0.0,100.0);
	glEnd();
}
