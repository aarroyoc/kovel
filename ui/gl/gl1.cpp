#include "gl1.hpp"

#include <GL/gl.h>

GlPreviewer::GlPreviewer()
{
	
}

void GlPreviewer::Render()
{
	// USE OLD OPENGL 1 code
	glClearColor(0.0,0.0,0.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
