#include "wx/wx.h"
#include "wx/glcanvas.h"

#include "preview.hpp"

#include "../gl/gl1.hpp"

PreviewPanel::PreviewPanel(wxWindow* parent) : wxPanel(parent,wxID_ANY,wxDefaultPosition,wxSize(100,100))
{
	wxColour green;
	green.Set("#00ff00");
	SetBackgroundColour(green);
	
	wxBoxSizer* box=new wxBoxSizer(wxVERTICAL);
	
	gl=new GlPreviewer();
	
	canvas=new wxGLCanvas(this,wxID_ANY,NULL,wxDefaultPosition,wxSize(100,100));
	ctx=new wxGLContext(canvas);
	
	
	box->Add(canvas,1,wxEXPAND | wxALL, 0);
	
	SetSizer(box);
	
	Bind(wxEVT_IDLE,&PreviewPanel::OnIdle,this,-1);
}

void PreviewPanel::OnIdle(wxIdleEvent& event)
{
	Core* core=Core::Instance();
	wxSize size=this->GetSize();
	wxClientDC(this);
	canvas->SetCurrent(*ctx);
	gl->Render(size.GetWidth(),size.GetHeight(),core->geo,core->mat);
	canvas->SwapBuffers();
}

void PreviewPanel::OnPaint(wxPaintEvent& event)
{
	Core* core=Core::Instance();
	wxSize size=this->GetSize();
	wxPaintDC(this);
	canvas->SetCurrent(*ctx);
	gl->Render(size.GetWidth(),size.GetHeight(),core->geo,core->mat);
	canvas->SwapBuffers();
}
