#ifndef PREVIEW_HPP
#define PREVIEW_HPP

#include "../gl/gl1.hpp"

#include "wx/wx.h"
#include "wx/glcanvas.h"

class PreviewPanel : public wxPanel{
	public:
		PreviewPanel(wxWindow* parent);
		void OnPaint(wxPaintEvent& event);
		void OnIdle(wxIdleEvent& event);
		float zoom=0.0f;
	private:
		wxGLCanvas* canvas;
		wxGLContext* ctx;
		GlPreviewer* gl;
};

#endif
