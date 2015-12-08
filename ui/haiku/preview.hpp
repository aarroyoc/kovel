#ifndef PREVIEW_HPP
#define PREVIEW_HPP

#include "../gl/gl1.hpp"

#include <GameKit.h>
#include <InterfaceKit.h>
#include <opengl/GLView.h>

class PreviewPanel : public BView{
	public:
		PreviewPanel();
		float zoom=0.0f;
		float rotation=0.0f;
		void Render();
		void Pulse();
	private:
		GlPreviewer* gl;
		BStringView* zoomLevel;
		BStringView* rotationLevel;
		BGLView* canvas;	
};

#endif
