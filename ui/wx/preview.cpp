#include "preview.hpp"

#include "wx/wx.h"
#include "wx/glcanvas.h"

#include "../gl/gl1.hpp"

PreviewPanel::PreviewPanel(wxWindow* parent) : wxPanel(parent,wxID_ANY,wxDefaultPosition,wxSize(100,100))
{	
	wxBoxSizer* box=new wxBoxSizer(wxHORIZONTAL);
	
	gl=new GlPreviewer();
	
	int attribList[]={
		WX_GL_RGBA, WX_GL_DOUBLEBUFFER,WX_GL_DEPTH_SIZE,32,0
	};
	
	canvas=new wxGLCanvas(this,wxID_ANY,attribList,wxDefaultPosition,wxSize(100,100));
	ctx=new wxGLContext(canvas);
	
	wxBoxSizer* control=new wxBoxSizer(wxVERTICAL);
	zoomLevel=new wxStaticText(this,wxID_ANY,"Zoom level: 0");
	control->Add(zoomLevel);
	
	rotationLevel=new wxStaticText(this,wxID_ANY,"Rotation level: 0");
	control->Add(rotationLevel);
	
	wxBoxSizer* zoomButton=new wxBoxSizer(wxHORIZONTAL);
	wxButton* zoomMore=new wxButton(this,wxID_ANY,"Zoom in");
	zoomMore->Bind(wxEVT_BUTTON,[this](wxCommandEvent&)->void{
		this->zoom+=1.0f;
	},-1);
	zoomButton->Add(zoomMore,1,wxEXPAND|wxALL,0);
	wxButton* zoomLess=new wxButton(this,wxID_ANY,"Zoom out");
	zoomLess->Bind(wxEVT_BUTTON,[this](wxCommandEvent&)->void{
		this->zoom-=1.0f;
	},-1);
	zoomButton->Add(zoomLess,1,wxEXPAND | wxALL, 0);
	
	control->Add(zoomButton);
	
	wxBoxSizer* rotationButton=new wxBoxSizer(wxHORIZONTAL);
	wxButton* rotationLeft=new wxButton(this,wxID_ANY,"Rotate left");
	rotationLeft->Bind(wxEVT_BUTTON,[this](wxCommandEvent&)->void{
		this->rotation+=15;
	},-1);
	rotationButton->Add(rotationLeft,1,wxEXPAND | wxALL, 0);
	wxButton* rotationRight=new wxButton(this,wxID_ANY,"Rotate right");
	rotationRight->Bind(wxEVT_BUTTON,[this](wxCommandEvent&)->void{
		this->rotation-=15;
	},-1);
	rotationButton->Add(rotationRight,1,wxEXPAND | wxALL, 0);
	
	control->Add(rotationButton);
	
	box->Add(canvas,2,wxALIGN_CENTER | wxSHAPED);
	box->Add(control,1,wxEXPAND | wxALL, 0);
	
	SetSizer(box);
	
	Bind(wxEVT_IDLE,&PreviewPanel::OnIdle,this,-1);
}

void PreviewPanel::OnIdle(wxIdleEvent& event)
{
	if(!canvas->IsShownOnScreen()) return;
	Core* core=Core::Instance();
	wxSize size=canvas->GetSize();
	wxClientDC(this);
	canvas->SetCurrent(*ctx);
	gl->Render(size.GetWidth(),size.GetHeight(),core->geo,core->mat,zoom,rotation);
	canvas->SwapBuffers();
	
	zoomLevel->SetLabel(wxString::Format("Zoom level: %g", zoom));
	rotationLevel->SetLabel(wxString::Format("Rotation level: %g",rotation));
}

void PreviewPanel::OnPaint(wxPaintEvent& event)
{
	if(!canvas->IsShownOnScreen()) return;
	Core* core=Core::Instance();
	wxSize size=canvas->GetSize();
	wxPaintDC(this);
	canvas->SetCurrent(*ctx);
	gl->Render(size.GetWidth(),size.GetHeight(),core->geo,core->mat,zoom,rotation);
	canvas->SwapBuffers();
}
