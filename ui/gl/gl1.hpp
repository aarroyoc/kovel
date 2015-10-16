#ifndef GL1_HPP
#define GL1_HPP

#include "../../core/core.hpp"

class GlPreviewer{
	public:
		GlPreviewer();
		void Render(int width, int height, Geometry* geo); // Specify a FILE
		void DrawCube(unsigned short x, unsigned short y, unsigned short z);
};

#endif
