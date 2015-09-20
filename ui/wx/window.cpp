#include "wx/wx.h"
#include <wx/aui/framemanager.h>

#include "window.hpp"
#include "preview.hpp"

class WorkPanel : public wxPanel{
	public:
		WorkPanel(wxWindow* parent) : wxPanel(parent,wxID_ANY){
			
			// Maybe wxGrid here for buttons with pictures?
			wxGridSizer* grid=new wxGridSizer(16,2,5,5);
			
			// GRID SIZER - TODO
			
			wxButton* newFile=new wxButton(this,wxID_ANY,"New file");
			wxButton* openFile=new wxButton(this,-1,"Open file");
			wxButton* saveFile=new wxButton(this,-1,"Save file");
			wxButton* saveFileAs=new wxButton(this,-1,"Save file as...");
			
			
			grid->Add(newFile,0,wxEXPAND);
			grid->Add(openFile,0,wxEXPAND);
			grid->Add(saveFile,0,wxEXPAND);
			grid->Add(saveFileAs,0,wxEXPAND);
			grid->Add(new wxStaticText(this,-1,""),0,wxEXPAND);
			grid->Add(new wxStaticText(this,-1,""),0,wxEXPAND);
			
			SetSizer(grid);
			SetMinSize(wxSize(100,400));
		}
};


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
	
	WorkPanel* workPanel=new WorkPanel(panel);
	wxPanel* centralPanel=new wxPanel(panel,wxID_ANY);
	PreviewPanel* previewPanel=new PreviewPanel(centralPanel);
	wxPanel* fillPanel=new wxPanel(centralPanel,wxID_ANY);
	wxPanel* texturePanel=new wxPanel(panel,wxID_ANY);
	fillPanel->SetBackgroundColour(blue);
	texturePanel->SetBackgroundColour(yellow);
	
	Bind(wxEVT_PAINT,&PreviewPanel::OnPaint,previewPanel,-1);
	
	sizer->Add(workPanel,1,wxEXPAND | wxALL, 0);
	sizer->Add(centralPanel,2,wxEXPAND | wxALL, 0);
	sizer->Add(texturePanel,1,wxEXPAND | wxALL, 0);
	
	centralSizer->Add(previewPanel,1,wxALIGN_CENTER | wxSHAPED, 0);
	centralSizer->Add(fillPanel,2,wxEXPAND | wxALL, 0);
	
	panel->SetSizer(sizer);
	centralPanel->SetSizer(centralSizer);
	
	Centre();

}
