#include "wx/wx.h"

#include "window.hpp"
#include "preview.hpp"


MainWindow::MainWindow() : wxFrame(NULL,-1,"(new file) -- Kovel - Voxel Editor",wxDefaultPosition,wxSize(800,600),wxDEFAULT_FRAME_STYLE)
{
	wxColourDatabase colorDb;
	wxColour red,blue,green,yellow;
	
	red=colorDb.Find("RED");
	blue=colorDb.Find("BLUE");
	green=colorDb.Find("GREEN");
	yellow=colorDb.Find("YELLOW");
	
	wxPanel* panel=new wxPanel(this,wxID_ANY);
	
	wxBoxSizer* sizer=new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* centralSizer=new wxBoxSizer(wxVERTICAL);
	
	wxPanel* workPanel=new wxPanel(panel,wxID_ANY);
	wxPanel* centralPanel=new wxPanel(panel,wxID_ANY);
	PreviewPanel* previewPanel=new PreviewPanel(centralPanel);
	wxPanel* fillPanel=new wxPanel(centralPanel,wxID_ANY);
	wxPanel* texturePanel=new wxPanel(panel,wxID_ANY);
	workPanel->SetBackgroundColour(red);
	fillPanel->SetBackgroundColour(blue);
	texturePanel->SetBackgroundColour(yellow);
	
	Bind(wxEVT_PAINT,&PreviewPanel::OnPaint,previewPanel,-1);
	
	sizer->Add(workPanel,1,wxEXPAND | wxALL, 0);
	sizer->Add(centralPanel,2,wxEXPAND | wxALL, 0);
	sizer->Add(texturePanel,1,wxEXPAND | wxALL, 0);
	
	centralSizer->Add(previewPanel,1,wxALIGN_CENTER | wxSHAPED, 0);
	//centralSizer->Add(previewPanel,1,wxEXPAND | wxALL, 0);
	centralSizer->Add(fillPanel,2,wxEXPAND | wxALL, 0);
	
	panel->SetSizer(sizer);
	centralPanel->SetSizer(centralSizer);
	
	Centre();
}
