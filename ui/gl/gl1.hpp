#ifndef GL1_HPP
#define GL1_HPP

#include "../../core/core.hpp"

class GlPreviewer{
	public:
		GlPreviewer();
		void Render(int width, int height, Geometry* geo, Material3D mat); // Specify a FILE
		void DrawCube(unsigned short x, unsigned short y, unsigned short z, Material mat);
};

#endif
