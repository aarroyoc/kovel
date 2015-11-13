#include "../../core/core.hpp"

#include "wx/wx.h"
#include <wx/clrpicker.h>
#include <wx/colordlg.h>
#include <wx/statline.h>
#include <wx/aboutdlg.h>

#include "window.hpp"
#include "preview.hpp"
#include "metadata.hpp"

#include "../../core/version.hpp"

wxColourPickerCtrl* picker;
wxString FILE_PATH;

class ToolPanelOld : public wxPanel{
	public:
		ToolPanelOld(wxWindow* parent) : wxPanel(parent,wxID_ANY){
			
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

class ToolPanel : public wxPanel{
	public:
		ToolPanel(wxWindow* parent) : wxPanel(parent,wxID_ANY)
		{
			wxBoxSizer* sizer=new wxBoxSizer(wxVERTICAL);
			wxStaticText* header=new wxStaticText(this,wxID_ANY,"Material");
			wxFont font=header->GetFont();
			font.SetPointSize(18);
			header->SetFont(font);
			sizer->Add(header,0,wxEXPAND | wxALL);
			picker=new wxColourPickerCtrl(this,wxID_ANY,*wxYELLOW);
			sizer->Add(picker,0,wxEXPAND | wxALL);
			picker->Bind(wxEVT_COLOURPICKER_CHANGED,[this](wxColourPickerEvent &)->void{
				wxColour color=picker->GetColour();
				Core* core=Core::Instance();
				float r=((float)color.Red())/255;
				float g=((float)color.Green())/255;
				float b=((float)color.Blue())/255;
				Material mat(color.GetAsString().ToStdString(),r,g,b);
				core->SetMaterial(mat);
			},wxID_ANY);
			
			wxStaticLine* line=new wxStaticLine(this);
			sizer->Add(line,0,wxEXPAND | wxALL);
			
			/* Texture selector will be here */
			
			SetSizer(sizer);
		}
};

class GridPanel : public wxPanel{
	public:
		unsigned short GRID_SIZE = 5; // 16 might be the maximum
		bool** GRID;
		short zCoord = 0;
		GridPanel(wxWindow* parent) : wxPanel(parent,wxID_ANY)
		{
			GRID=(bool**)malloc(sizeof(bool*) * GRID_SIZE);
			for(int i=0;i<GRID_SIZE;i++){
				GRID[i]=(bool*)malloc(sizeof(bool) * GRID_SIZE);
				for(int j=0;j<GRID_SIZE;j++){
					GRID[i][j]=false;
				}
			}
			SetSize(100,100);
			PaintNow();
			
			Bind(wxEVT_PAINT,&GridPanel::Paint,this,-1);
			Bind(wxEVT_LEFT_DOWN,&GridPanel::Click,this,-1);
		}
		~GridPanel(){
			free(GRID);
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
			
			Core* core=Core::Instance();
			if(GRID[xCoord][yCoord])
				core->UpdateGrid(1,xCoord,zCoord,yCoord);
			else
				core->UpdateGrid(0,xCoord,zCoord,yCoord);
			// Update preview
			
			this->PaintNow();
		}
		void Paint(wxPaintEvent& event){
			wxPaintDC dc(this);
			this->Render(dc);
		}
		void ReloadWorkGrid(unsigned short g){
			GRID_SIZE=g;
			Core* core=Core::Instance();
			
			/*for(int i=0;i<GRID_SIZE;i++){
				free(GRID[i]);
			}
			free(GRID);*/
			
			GRID=(bool**)malloc(sizeof(bool*) * GRID_SIZE);
			for(int i=0;i<GRID_SIZE;i++){
				GRID[i]=(bool*)malloc(sizeof(bool) * GRID_SIZE);
				for(int j=0;j<GRID_SIZE;j++){
					GRID[i][j]=core->geo->GetGrid(i,zCoord,j);
				}
			}
			this->PaintNow();
		}
		void ClearGrid(){
			Core* core=Core::Instance();
			for(int i=0;i<GRID_SIZE;i++){
				for(int j=0;j<GRID_SIZE;j++){
					core->UpdateGrid(0,i,zCoord,j);
				}
			}
			this->ReloadWorkGrid(GRID_SIZE);
		}
		void UpButton(wxCommandEvent& event){
			zCoord++;
			if(zCoord == GRID_SIZE)
				zCoord--;
			this->ReloadWorkGrid(GRID_SIZE);
		}
		void DownButton(wxCommandEvent& event){
			zCoord--;
			if(zCoord == -1)
				zCoord++;
			this->ReloadWorkGrid(GRID_SIZE);
		}
};

class WorkPanel : public wxPanel{
	public:
		GridPanel* workSide;
		WorkPanel(wxWindow* parent) : wxPanel(parent,wxID_ANY)
		{
			workSide=new GridPanel(this);
			wxPanel* workTools=new wxPanel(this,wxID_ANY);
			
			wxButton* up=new wxButton(workTools,wxID_ANY,"Up");
			up->Bind(wxEVT_BUTTON,&GridPanel::UpButton,workSide,-1);
			wxButton* down=new wxButton(workTools,wxID_ANY,"Down");
			down->Bind(wxEVT_BUTTON,&GridPanel::DownButton,workSide,-1);
			
			wxBoxSizer* sizer=new wxBoxSizer(wxVERTICAL);
			sizer->Add(workSide,3,wxALIGN_CENTER | wxSHAPED);
			sizer->Add(workTools,1,wxEXPAND | wxALL);
			SetSizer(sizer);
			
			wxBoxSizer* workToolsSizer=new wxBoxSizer(wxHORIZONTAL);
			workToolsSizer->Add(up,1,wxEXPAND | wxALL,5);
			workToolsSizer->Add(down,1,wxEXPAND | wxALL,5);
			workTools->SetSizer(workToolsSizer);
		}
		void CleanGrid(wxCommandEvent& evt){
			workSide->ClearGrid();
		}
		void UpdateGrid(unsigned short g){
			workSide->ReloadWorkGrid(g);
		}
};

WorkPanel* workTwo;

MainWindow::MainWindow() : wxFrame(NULL,-1,"(new file) -- Kovel - Voxel Editor",wxDefaultPosition,wxSize(800,600),wxDEFAULT_FRAME_STYLE)
{
	Core* core=Core::Instance();
	
	wxPanel* panel=new wxPanel(this,wxID_ANY);
	
	wxPanel* main=new wxPanel(panel,wxID_ANY);
	ToolPanel* tool=new ToolPanel(panel);
	
	wxPanel* work=new wxPanel(main,wxID_ANY);
	PreviewPanel* preview=new PreviewPanel(main);
	
	WorkPanel* workOne=new WorkPanel(work);
	workTwo=new WorkPanel(work);
	WorkPanel* workThree=new WorkPanel(work);
	
	wxBoxSizer* sizer=new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(main,3,wxEXPAND | wxALL);
	sizer->Add(tool,1,wxEXPAND | wxALL);
	panel->SetSizer(sizer);
	
	wxBoxSizer* mainSizer=new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(work,1,wxEXPAND | wxALL);
	mainSizer->Add(preview,1,wxEXPAND | wxALL);
	main->SetSizer(mainSizer);
	
	wxBoxSizer* workSizer=new wxBoxSizer(wxHORIZONTAL);
	//workSizer->Add(workOne,1,wxEXPAND | wxALL,5);
	workSizer->Add(workTwo,1,wxEXPAND | wxALL,5);
	//workSizer->Add(workThree,1,wxEXPAND | wxALL,5);
	work->SetSizer(workSizer);
	
	
	// COMPLETE OPTIONS BAR - Done
	
	// Grid Size - Done
	
	// Read metadata - Done
	
	// Command Line Tool - Done
	
	// TEMP FOLDER - Done
	
	// Widget showing Y level
	
	// Zoom indicator - Done
	
	// Zoom button - Done
	
	// FIX BUGS (First Material) - Done
	
	// Export as JSON - Done
	
	// Export as DAE - Done
	
	// Open with MIME TYPE Double Click
	
	// - Mime Type
	
	// - Command Line Interface - Done
	
	// Icon - WINDOWS
	
	// Capturas de pantalla
	
	// Marketing
	
	// homepage
	
	// DesdeLinux
	
	// Blog
	
	// Documentación man page
	
	// Packaging (Debian, DESKTOP file, Windows MSI, Reg for MIME TYPE and actions)
	
	// AND PUBLISH 1.0
	
	// In a future
	
	// (Three panels?)
	
	// Panel color
	
	// GLES 2.0 render
	
	// Haiku UI
	
	// Android UI
	
	// QML/Ubuntu UI
	
	// Textures
	
	// Custom mesh
	
	// MenuBar
	wxMenuBar* menuBar=new wxMenuBar;
	
	wxMenu* file=new wxMenu;
	wxMenuItem* newFile=new wxMenuItem(file,wxID_NEW,"&New file");
	file->Append(newFile);
	Bind(wxEVT_MENU,[core,workTwo,this](wxCommandEvent&)->void{
		MetadataDialog* metadata=new MetadataDialog(this,true);
		if(metadata->ShowModal() == wxID_CANCEL) return;
		//wxMessageBox(wxString::FromUTF8(metadata->author.c_str()));
		core->NewFile(metadata->gridSize);
		core->name=metadata->name;
		core->author=metadata->author;
		workTwo->UpdateGrid(core->grid);
		this->SetLabel("(new file) -- Kovel - Voxel Editor");
	},wxID_NEW);
	
	file->Append(wxID_OPEN,"&Open file");
	Bind(wxEVT_MENU,[this](wxCommandEvent&)->void{
		wxFileDialog* fileDlg=new wxFileDialog(this,"Open Kovel file","","","Kovel files (*.kvl)|*.kvl",wxFD_OPEN|wxFD_FILE_MUST_EXIST);
		if(fileDlg->ShowModal() == wxID_CANCEL)
			return;
		this->LoadFile(fileDlg->GetPath());
	},wxID_OPEN);
	
	wxMenuItem* saveFile=new wxMenuItem(file,wxID_SAVE,"&Save file");
	file->Append(saveFile);
	Bind(wxEVT_MENU,[core,this](wxCommandEvent &)->void{
		if(this->GetLabel().StartsWith("(new file)")){
			wxFileDialog* fileDlg=new wxFileDialog(this,"Save Kovel file","","","Kovel file (*.kvl)|*.kvl",wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
			if(fileDlg->ShowModal() == wxID_CANCEL)
				return;
			FILE_PATH=fileDlg->GetPath();
			if(!FILE_PATH.EndsWith(".kvl"))
				FILE_PATH.Append(".kvl");
			core->SaveFile(FILE_PATH.ToStdString());
			this->SetLabel(FILE_PATH + " -- Kovel - Voxel Editor");
		}else{
			core->SaveFile(FILE_PATH.ToStdString());
		}
	}, wxID_SAVE);
	
	file->Append(wxID_SAVEAS,"Save file as...");
	Bind(wxEVT_MENU,[core,this](wxCommandEvent&)->void{
		wxFileDialog* fileDlg=new wxFileDialog(this,"Save Kovel file","","","Kovel file (*.kvl)|*.kvl",wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
		if(fileDlg->ShowModal() == wxID_CANCEL)
			return;
		FILE_PATH=fileDlg->GetPath();
		if(!FILE_PATH.EndsWith(".kvl"))
			FILE_PATH.Append(".kvl");
		core->SaveFile(FILE_PATH.ToStdString());
		this->SetLabel(FILE_PATH + " -- Kovel - Voxel Editor");
	}, wxID_SAVEAS);
	
	file->AppendSeparator();
	
	/* EXPORT */
	
	file->Append(63,"Export as JSON...");
	Bind(wxEVT_MENU,[core,this](wxCommandEvent&)->void{
		wxFileDialog* fileDlg=new wxFileDialog(this,"Save JSON file","","","JSON file (*.json)|*.json",wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
		if(fileDlg->ShowModal() == wxID_CANCEL)
			return;
		wxString jsonPath=fileDlg->GetPath();
		if(!jsonPath.EndsWith(".json"))
			jsonPath.Append(".json");
		bool ret=core->ExportAsJSON(jsonPath.ToStdString());
		if(!ret)
			wxMessageBox("Please, save your file first","Error",wxOK | wxICON_ERROR);
	},63);
	
	file->Append(64,"Export as Collada DAE...");
	Bind(wxEVT_MENU,[core,this](wxCommandEvent&)->void{
		wxFileDialog* fileDlg=new wxFileDialog(this,"Save Collada DAE file","","","Collada DAE (*.dae)|*.dae",wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
		if(fileDlg->ShowModal() == wxID_CANCEL)
			return;
		wxString daePath=fileDlg->GetPath();
		if(!daePath.EndsWith(".dae"))
			daePath.Append(".dae");
		bool ret=core->ExportAsDAE(daePath.ToStdString());
		if(!ret)
			wxMessageBox("Please, save your file first","Error",wxOK | wxICON_ERROR);
	},64);
	
	file->AppendSeparator();
	
	wxMenuItem* exit=new wxMenuItem(file,wxID_EXIT,"&Exit");
	file->Append(exit);
	Bind(wxEVT_MENU, [this](wxCommandEvent &)->void{
            this->Close();
        }, wxID_EXIT);
	menuBar->Append(file,"&File");
	
	
	wxMenu* edit=new wxMenu;
	edit->Append(wxID_UNDO,"&Undo\tCtrl-Z");
	Bind(wxEVT_MENU,[workTwo,core](wxCommandEvent&)->void{
		core->Undo();
		workTwo->UpdateGrid(core->grid);
	},wxID_UNDO);
	
	edit->AppendSeparator();
	edit->Append(65,"Edit &metadata");
	Bind(wxEVT_MENU,[this,core](wxCommandEvent&)->void{
		MetadataDialog* metadata=new MetadataDialog(this,false);
		if(metadata->ShowModal() == wxID_CANCEL) return;
		core->name=metadata->name;
		core->author=metadata->author;
	},65);
	wxMenuItem* clearWorkGrid=new wxMenuItem(edit,66,"Clear work grid");
	edit->Append(clearWorkGrid);
	Bind(wxEVT_MENU,&WorkPanel::CleanGrid,workTwo,66);
	wxMenuItem* selectColor=new wxMenuItem(edit,67,"Select colour");
	edit->Append(selectColor);
	Bind(wxEVT_MENU,[this](wxCommandEvent &)->void{
		// Color picker
		wxColourDialog* clrDlg=new wxColourDialog(this);
		if(clrDlg->ShowModal() == wxID_OK){
			wxColourData& clrData= clrDlg->GetColourData(); // FILL THIS
			wxColour& color=clrData.GetColour();
			picker->SetColour(color);
			Core* core=Core::Instance();
			float r=((float)color.Red())/255;
			float g=((float)color.Green())/255;
			float b=((float)color.Blue())/255;
			Material mat(color.GetAsString().ToStdString(),r,g,b);
			core->SetMaterial(mat);
		}
	}, 67);
	menuBar->Append(edit,"&Edit");
	
	
	wxMenu* navigate=new wxMenu;
	navigate->Append(68,"Zoom &in");
	Bind(wxEVT_MENU,[preview](wxCommandEvent&)->void{
		preview->zoom+=1.0f;
	},68);
	navigate->Append(69,"Zoom &out");
	Bind(wxEVT_MENU,[preview](wxCommandEvent&)->void{
		preview->zoom-=1.0f;
	},69);
	navigate->AppendSeparator();
	navigate->Append(70,"Rotate left");
	Bind(wxEVT_MENU,[preview](wxCommandEvent&)->void{
		preview->rotation+=15;
	},70);
	navigate->Append(71,"Rotate right");
	Bind(wxEVT_MENU,[preview](wxCommandEvent&)->void{
		preview->rotation-=15;
	},71);
	menuBar->Append(navigate,"&Navigate");
	
	
	wxMenu* help=new wxMenu;
	help->Append(wxID_ANY,"Legacy OpenGL 1.0"); // Change to GL ES 2.0 in a future
	help->Append(100,"Visit author website");
	Bind(wxEVT_MENU,[this](wxCommandEvent&)->void{
		wxLaunchDefaultBrowser(AUTHOR_URL);
	},100);
	help->AppendSeparator();
	help->Append(wxID_ABOUT,"&About...");
	Bind(wxEVT_MENU,[this](wxCommandEvent&)->void{
		wxAboutDialogInfo aboutInfo;
		aboutInfo.SetName("Kovel");
		aboutInfo.SetVersion(KOVEL_VERSION);
		aboutInfo.SetDescription(DESCRIPTION);
		aboutInfo.SetCopyright(KOVEL_COPYRIGHT);
		aboutInfo.SetWebSite(KOVEL_URL);
		aboutInfo.AddDeveloper(wxString::FromUTF8(std::string("Adrián Arroyo Calle").c_str()));
		
		wxAboutBox(aboutInfo);
	},wxID_ABOUT);
	menuBar->Append(help,"&Help");
	SetMenuBar(menuBar);
	
	Centre();

}

void MainWindow::LoadFile(wxString path)
{
	Core* core=Core::Instance();
	FILE_PATH=path;
	core->LoadFile(path.ToStdString());
	workTwo->UpdateGrid(core->grid);
	this->SetLabel(path + " -- Kovel - Voxel Editor");
}
