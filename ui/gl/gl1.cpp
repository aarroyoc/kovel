#include "gl1.hpp"

#include <GL/gl.h>

GlPreviewer::GlPreviewer()
{
	
}

void GlPreviewer::DrawCube(unsigned short x, unsigned short y, unsigned short z)
{
		glColor3f(1.0,1.0,0.0);
	glBegin(GL_TRIANGLES);
	
		/* First face */
		glVertex3f(0.0+x,0.0+y,0.0+z);
		glVertex3f(0.0+x,1.0+y,0.0+z);
		glVertex3f(0.0+x,1.0+y,1.0+z);
		
		glVertex3f(0.0+x,0.0+y,0.0+z);
		glVertex3f(0.0+x,1.0+y,1.0+z);
		glVertex3f(0.0+x,0.0+y,1.0+z);
		
		/* Second face */
		glVertex3f(0.0+x,0.0+y,1.0+z);
		glVertex3f(1.0+x,0.0+y,1.0+z);
		glVertex3f(1.0+x,1.0+y,1.0+z);
		
		glVertex3f(0.0+x,0.0+y,1.0+z);
		glVertex3f(0.0+x,1.0+y,1.0+z);
		glVertex3f(1.0+x,1.0+y,1.0+z);
		
		/* Third face */
		glVertex3f(1.0+x,0.0+y,1.0+z);
		glVertex3f(1.0+x,0.0+y,0.0+z);
		glVertex3f(1.0+x,1.0+y,0.0+z);
		
		glVertex3f(1.0+x,0.0+y,1.0+z);
		glVertex3f(1.0+x,1.0+y,1.0+z);
		glVertex3f(1.0+x,1.0+y,0.0+z);
		
		/* Forth face */
		glVertex3f(0.0+x,0.0+y,0.0+z);
		glVertex3f(0.0+x,1.0+y,0.0+z);
		glVertex3f(1.0+x,1.0+y,0.0+z);
		
		glVertex3f(0.0+x,0.0+y,0.0+z);
		glVertex3f(1.0+x,0.0+y,0.0+z);
		glVertex3f(1.0+x,1.0+y,0.0+z);
		
		/* Fifth face - Top */
		glVertex3f(0.0+x,1.0+y,0.0+z);
		glVertex3f(0.0+x,1.0+y,1.0+z);
		glVertex3f(1.0+x,1.0+y,1.0+z);
		
		glVertex3f(0.0+x,1.0+y,0.0+z);
		glVertex3f(1.0+x,1.0+y,0.0+z);
		glVertex3f(1.0+x,1.0+y,1.0+z);
		
		/* Sixth face - Bottom*/
		glVertex3f(0.0+x,0.0+y,0.0+z);
		glVertex3f(0.0+x,0.0+y,1.0+z);
		glVertex3f(1.0+x,0.0+y,1.0+z);
		
		glVertex3f(0.0+x,0.0+y,0.0+z);
		glVertex3f(1.0+x,0.0+y,0.0+z);
		glVertex3f(1.0+x,0.0+y,1.0+z);
	glEnd();
}

void GlPreviewer::Render(int width, int height, Geometry* geo)
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
	
	for(short i=0;i<geo->g;i++){
		for(short j=0;j<geo->g;j++){
			for(short k=0;k<geo->g;k++){
				if(geo->GetGrid(i,j,k)==1){
					DrawCube(i,j,k);
				}
			}
		}
	}
}
