#include "wx/wx.h"
#include <wx/aui/framemanager.h>

#include "window.hpp"
#include "preview.hpp"

class ToolPanel : public wxPanel{
	public:
		ToolPanel(wxWindow* parent) : wxPanel(parent,wxID_ANY){
			
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

class GridPanel : public wxPanel{
	public:
		unsigned short GRID_SIZE = 16;
		bool GRID[16][16];
		GridPanel(wxWindow* parent) : wxPanel(parent,wxID_ANY)
		{
			for(int i=0;i<GRID_SIZE;i++){
				for(int j=0;j<GRID_SIZE;j++){
					GRID[i][j]=false;
				}
			}
			SetSize(100,100);
			PaintNow();
			
			Bind(wxEVT_PAINT,&GridPanel::Paint,this,-1);
			Bind(wxEVT_LEFT_DOWN,&GridPanel::Click,this,-1);
		}
		void PaintNow(){
			wxClientDC dc(this);
			this->Render(dc);
		}
		void Render(wxDC& dc){
			dc.SetPen(*wxTRANSPARENT_PEN);
			
			wxSize size=this->GetSize();
			int cellWidth=size.GetWidth()/GRID_SIZE;
			int cellHeight=size.GetHeight()/GRID_SIZE;
			for(int i=0;i<GRID_SIZE;i++){
				for(int j=0;j<GRID_SIZE;j++){
					dc.SetBrush(*wxBLUE_BRUSH);
					dc.DrawRectangle(i*cellWidth,j*cellHeight,cellWidth,cellHeight);
					if(GRID[i][j])
						dc.SetBrush(*wxCYAN_BRUSH);
					else
						dc.SetBrush(*wxWHITE_BRUSH);
					dc.DrawRectangle((i*cellWidth)+2,(j*cellHeight)+2,cellWidth-4,cellHeight-4);
				}
			}
		}
		void Click(wxMouseEvent& event){
			wxClientDC dc(this);
			
			wxPoint point=event.GetLogicalPosition(dc);
			
			wxSize size=this->GetSize();
			int cellWidth=size.GetWidth()/GRID_SIZE;
			int cellHeight=size.GetHeight()/GRID_SIZE;
			int xCoord=(point.x/cellWidth);
			int yCoord=(point.y/cellHeight);
			
			GRID[xCoord][yCoord]=!GRID[xCoord][yCoord];
			
			this->PaintNow();
		}
		void Paint(wxPaintEvent& event){
			wxPaintDC dc(this);
			this->Render(dc);
		}
};

class WorkPanel : public wxPanel{
	public:
		WorkPanel(wxWindow* parent) : wxPanel(parent,wxID_ANY)
		{
			GridPanel* workSide=new GridPanel(this);
			wxPanel* workTools=new wxPanel(this,wxID_ANY);
			
			wxButton* up=new wxButton(workTools,wxID_ANY,"Up");
			wxButton* down=new wxButton(workTools,wxID_ANY,"Down");
			
			wxBoxSizer* sizer=new wxBoxSizer(wxVERTICAL);
			sizer->Add(workSide,3,wxALIGN_CENTER | wxSHAPED);
			sizer->Add(workTools,1,wxEXPAND | wxALL);
			SetSizer(sizer);
			
			wxBoxSizer* workToolsSizer=new wxBoxSizer(wxHORIZONTAL);
			workToolsSizer->Add(up,1,wxEXPAND | wxALL,5);
			workToolsSizer->Add(down,1,wxEXPAND | wxALL,5);
			workTools->SetSizer(workToolsSizer);
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
	
	wxPanel* main=new wxPanel(panel,wxID_ANY);
	wxPanel* tool=new wxPanel(panel,wxID_ANY);
	
	wxPanel* work=new wxPanel(main,wxID_ANY);
	PreviewPanel* preview=new PreviewPanel(main);
	
	WorkPanel* workOne=new WorkPanel(work);
	WorkPanel* workTwo=new WorkPanel(work);
	WorkPanel* workThree=new WorkPanel(work);
	
	wxBoxSizer* sizer=new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(main,3,wxEXPAND | wxALL);
	sizer->Add(tool,1,wxEXPAND | wxALL);
	panel->SetSizer(sizer);
	
	wxBoxSizer* mainSizer=new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(work,1,wxEXPAND | wxALL);
	mainSizer->Add(preview,1,wxALIGN_CENTER | wxSHAPED);
	main->SetSizer(mainSizer);
	
	wxBoxSizer* workSizer=new wxBoxSizer(wxHORIZONTAL);
	workSizer->Add(workOne,1,wxEXPAND | wxALL,5);
	workSizer->Add(workTwo,1,wxEXPAND | wxALL,5);
	workSizer->Add(workThree,1,wxEXPAND | wxALL,5);
	work->SetSizer(workSizer);
	
	tool->SetBackgroundColour(green);
	
	Centre();

}
