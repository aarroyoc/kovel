#include "../../core/core.hpp"

#include <AppKit.h>
#include <InterfaceKit.h>
#include <Layout.h>
#include <GroupLayout.h>
#include <StorageKit.h>

#include "window.hpp"
#include "preview.hpp"
#include "metadata.hpp"

#include "../../core/version.hpp"

BString FILE_PATH;

uint32 BVIEW_FLAGS= B_WILL_DRAW | B_FRAME_EVENTS | B_NAVIGABLE;

const uint32 NEW_FILE=0, OPEN_FILE=1, SAVE_FILE=2, SAVE_FILEAS=3, UNDO=4, CLEARGRID=5, SELECT_COLOR=6, UP=7, DOWN=8, RLEFT=9, RRIGHT=10;
const uint32 ZOOM_IN=11, ZOOM_OUT=12, META=13, READ_FILE=14, SAVE_FILE_DO=15;
const uint32 EXPORT_DAE=16, EXPORT_DAE_DO=17, EXPORT_JSON=18, EXPORT_JSON_DO=19;

class ToolPanel : public BView{
	public:
		ToolPanel() : BView(NULL,B_WILL_DRAW)
		{
			SetViewColor(220,220,220);
			BGroupLayout* sizer=new BGroupLayout(B_VERTICAL);
			SetLayout(sizer);
			BFont font;
			font.SetSize(18.0);
			BTextView* header=new BTextView(NULL,B_WILL_DRAW | B_PULSE_NEEDED);
			header->SetFont(&font);
			sizer->AddView(header);
			
			SetExplicitMaxSize(BSize(100,100));
			
			/* Colour picker */
			/* BColorControl */
			
			
		}
};


class GridPanel : public BView{
	public:
		unsigned short GRID_SIZE=5;
		std::vector<std::vector<bool>> GRID;
		short zCoord=0;
		GridPanel() : BView(Bounds(),NULL,B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE | B_FRAME_EVENTS | B_DRAW_ON_CHILDREN,0)
		{
			SetViewColor(220,220,220);
			
			GRID.resize(GRID_SIZE);
			for(short u=0;u<GRID_SIZE;u++){
				GRID.at(u).resize(GRID_SIZE);
				for(short j=0;j<GRID_SIZE;j++){
					GRID[u][j]=false;
				}
			}
			SetExplicitMinSize(BSize(100,100));
			PaintNow();
		}
		void PaintNow(){
			this->Draw(Bounds());
		}
		void Draw(BRect rect){
			SetDrawingMode(B_OP_COPY);
			BSize size=rect.Size();
			int cellWidth=size.Width()/GRID_SIZE;
			int cellHeight=size.Height()/GRID_SIZE;
			for(int i=0;i<GRID_SIZE;i++){
				for(int j=0;j<GRID_SIZE;j++){
					this->SetHighColor(0,0,255);
					this->StrokeRect(BRect(i*cellWidth,j*cellHeight,i*cellWidth+cellWidth,j*cellHeight+cellHeight));
					if(GRID[i][j])
						this->SetHighColor(0,220,220);
					else
						this->SetHighColor(255,255,255);
					this->FillRect(BRect((i*cellWidth)+4,(j*cellHeight)+4,(i*cellWidth)+4+cellWidth-8,(j*cellHeight)+4+cellHeight-8));
				}
			}
		}
		void MouseDown(BPoint point){
			BSize size=this->Bounds().Size();
			int cellWidth=size.Width()/GRID_SIZE;
			int cellHeight=size.Height()/GRID_SIZE;
			int xCoord=(point.x/cellWidth);
			int yCoord=(point.y/cellHeight);
			
			GRID[xCoord][yCoord]=!GRID[xCoord][yCoord];
			
			Core* core=Core::Instance();
			if(GRID[xCoord][yCoord])
				core->UpdateGrid(1,xCoord,zCoord,yCoord);
			else
				core->UpdateGrid(0,xCoord,zCoord,yCoord);
			this->PaintNow();
		}
		void ReloadWorkGrid(unsigned short g){
			GRID_SIZE=g;
			Core* core=Core::Instance();
			
			for(int i=0;i<GRID_SIZE;i++){
				GRID[i].clear();
			}
			
			GRID.resize(GRID_SIZE);
			for(short i=0;i<GRID_SIZE;i++){
				GRID.at(i).resize(GRID_SIZE);
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
		void UpButton(){
			zCoord++;
			if(zCoord == GRID_SIZE)
				zCoord--;
			this->ReloadWorkGrid(GRID_SIZE);
		}
		void DownButton(){
			zCoord--;
			if(zCoord == -1)
				zCoord++;
			this->ReloadWorkGrid(GRID_SIZE);
		}
};

GridPanel* workSide;

class WorkPanel : public BView{
	
	public:
		WorkPanel() : BView(NULL,B_WILL_DRAW)
		{
			SetViewColor(220,220,220);
			
			BGroupLayout* sizer=new BGroupLayout(B_VERTICAL);
			BGroupLayout* workToolsSizer=new BGroupLayout(B_HORIZONTAL);
			SetLayout(sizer);
			
			workSide=new GridPanel();
			BView* workTools=new BView(NULL,B_WILL_DRAW);
			workTools->SetViewColor(220,220,220);
			workTools->SetLayout(workToolsSizer);
			
			BButton* up=new BButton("Up",new BMessage(UP));
			BButton* down=new BButton("Down",new BMessage(DOWN));
			
			
			sizer->AddView(workSide,3);
			sizer->AddView(workTools,1);	
			workToolsSizer->AddView(up,1);
			workToolsSizer->AddView(down,1);
			
		}
		void CleanGrid(){
			workSide->ClearGrid();
		}
		void UpdateGrid(unsigned short g){
			workSide->ReloadWorkGrid(g);
		}
};

WorkPanel* workTwo;
PreviewPanel* preview;
ToolPanel* tool;

/* All options */
/* mime type */
/* icono */
/* Graphical glitches */

KovelWindow::KovelWindow() : BWindow(BRect(30,30,530,530),"(new file) -- Kovel - Voxel Editor",B_TITLED_WINDOW,0)
{
	Core* core=Core::Instance();
	
	BGroupLayout* sizer=new BGroupLayout(B_HORIZONTAL);
	BGroupLayout* mainSizer=new BGroupLayout(B_VERTICAL);
	BGroupLayout* workSizer=new BGroupLayout(B_HORIZONTAL);
	
	BView* panel=new BView(Bounds(),NULL,B_FOLLOW_ALL_SIDES,B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE | B_FRAME_EVENTS | B_DRAW_ON_CHILDREN);
	panel->SetViewColor(220,220,220);
	panel->SetLayout(sizer);
	
	BView* main=new BView(NULL,B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE | B_FRAME_EVENTS | B_DRAW_ON_CHILDREN);
	main->SetViewColor(220,220,220);
	main->SetLayout(mainSizer);
	tool=new ToolPanel();
	
	BView* work=new BView(NULL,B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE | B_FRAME_EVENTS | B_DRAW_ON_CHILDREN);
	work->SetLayout(workSizer);
	preview=new PreviewPanel();
	
	workTwo=new WorkPanel();
	
	workSizer->AddView(workTwo,1);
	
	mainSizer->AddView(CreateMenuBar());
	mainSizer->AddView(work,1);
	mainSizer->AddView(preview,1);
	
	sizer->AddView(main,3);
	//sizer->AddView(tool,1);
	
	AddChild(panel);
}

BMenuBar*
KovelWindow::CreateMenuBar()
{
	/* MenuBar */
	BMenuBar* menuBar=new BMenuBar("MenuBar");
	BMenu* file=new BMenu("File");
	BMenu* edit=new BMenu("Edit");
	BMenu* help=new BMenu("Help");
	
	menuBar->AddItem(file);
	menuBar->AddItem(edit);
	menuBar->AddItem(help);
	
	BMenuItem* newFile=new BMenuItem("New file",new BMessage(NEW_FILE));
	newFile->SetShortcut('N',B_COMMAND_KEY);
	file->AddItem(newFile);
	
	BMenuItem* openFile=new BMenuItem("Open file...",new BMessage(OPEN_FILE));
	openFile->SetShortcut('O',B_COMMAND_KEY);
	file->AddItem(openFile);
	
	BMenuItem* saveFile=new BMenuItem("Save file...",new BMessage(SAVE_FILE));
	saveFile->SetShortcut('S',B_COMMAND_KEY);
	file->AddItem(saveFile);
	
	file->AddSeparatorItem();
	
	BMenuItem* exportDAE=new BMenuItem("Export as DAE...",new BMessage(EXPORT_DAE));
	file->AddItem(exportDAE);
	
	BMenuItem* exportJSON=new BMenuItem("Export as JSON...",new BMessage(EXPORT_JSON));
	file->AddItem(exportJSON);
	
	file->AddSeparatorItem();
	
	BMenuItem* quit=new BMenuItem("Quit",new BMessage(B_QUIT_REQUESTED));
	quit->SetShortcut('Q',B_COMMAND_KEY);
	file->AddItem(quit);
	
	
	BMenuItem* undo=new BMenuItem("Undo",new BMessage(UNDO));
	undo->SetShortcut('Z',B_COMMAND_KEY);
	edit->AddItem(undo);
	BMenuItem* meta=new BMenuItem("Edit metadata",new BMessage(META));
	edit->AddItem(meta);
	
	BMenuItem* clear=new BMenuItem("Clear current grid",new BMessage(CLEARGRID));
	edit->AddItem(clear);
	BMenuItem* select=new BMenuItem("Select colour",new BMessage(SELECT_COLOR));
	edit->AddItem(select);
	
	BMenuItem* about=new BMenuItem("About Kovel",new BMessage(B_ABOUT_REQUESTED));
	about->SetTarget(be_app);
	help->AddItem(about);
	
	return menuBar;	
}

void
KovelWindow::LoadFile(BString path)
{
	Core* core=Core::Instance();
	FILE_PATH=path;
	core->LoadFile(path.String());
	workTwo->UpdateGrid(core->grid);
	this->SetTitle(path.Append(" -- Kovel - Voxel Editor").String());
}

bool
KovelWindow::QuitRequested()
{
	be_app_messenger.SendMessage(B_QUIT_REQUESTED);
	return BWindow::QuitRequested();
}

void
KovelWindow::MessageReceived(BMessage* msg)
{
	Core* core=Core::Instance();
	switch(msg->what){
		case NEW_FILE: {
			MetadataDialog* metadata=new MetadataDialog(true);
			metadata->Show();
			break;
		}
		case 666: {
			if(msg->GetBool("NEW_FILE",false))
			{
				core->NewFile(msg->GetInt32("GRID_SIZE",5));
				this->SetTitle("(new file) -- Kovel - Voxel Editor");
			}
			core->name=msg->GetString("NAME","");
			core->author=msg->GetString("AUTHOR","");
			workTwo->UpdateGrid(core->grid);
			break;
		}
		case OPEN_FILE: {
			BFilePanel* filep=new BFilePanel(B_OPEN_PANEL,new BMessenger(this),NULL,B_FILE_NODE,false,new BMessage(READ_FILE));
			filep->Show();
			break;
		}
		case SAVE_FILE: {
			BMessage* mesg=new BMessage(B_SAVE_REQUESTED);
			mesg->AddString("TYPE","KVL");
			BFilePanel* filep=new BFilePanel(B_SAVE_PANEL,new BMessenger(this),NULL,B_FILE_NODE,false,mesg);
			filep->Show();
			break;
		}
		case EXPORT_DAE: {
			BMessage* mesg=new BMessage(B_SAVE_REQUESTED);
			mesg->AddString("TYPE","DAE");
			BFilePanel* filep=new BFilePanel(B_SAVE_PANEL,new BMessenger(this),NULL,B_FILE_NODE,false,mesg);
			filep->Show();
			break;
		}
		case EXPORT_JSON: {
			BMessage* mesg=new BMessage(B_SAVE_REQUESTED);
			mesg->AddString("TYPE","JSON");
			BFilePanel* filep=new BFilePanel(B_SAVE_PANEL,new BMessenger(this),NULL,B_FILE_NODE,false,mesg);
			filep->Show();
			break;
		}
		case B_SAVE_REQUESTED: {
			entry_ref ref;
			msg->FindRef("directory",0,&ref);
			BEntry entry(&ref,true);
			BPath path;
			entry.GetPath(&path);
			BString name=msg->FindString("name");
			BString filename=path.Path();
			filename.Append("/");
			filename.Append(name);
			BString type=msg->FindString("TYPE");
			if(type == BString("KVL")){
				if(!filename.EndsWith(".kvl")){
					filename.Append(".kvl");
				}
				FILE_PATH=filename;
				this->SetTitle(FILE_PATH.Append(" -- Kovel -Voxel Editor"));
				core->SaveFile(filename.String());
			}
			if(type == BString("DAE")){
				if(!filename.EndsWith(".dae")){
					filename.Append(".dae");
				}
				core->ExportAsDAE(filename.String());
			}
			if(type == BString("JSON")){
				if(!filename.EndsWith(".json")){
					filename.Append(".json");
				}
				core->ExportAsJSON(filename.String());
			}
			break;
		}
		case READ_FILE: {
			if(msg->HasRef("refs")){
				entry_ref ref;
				if(msg->FindRef("refs",0,&ref) == B_OK){
					BEntry entry(&ref,true);
					BPath path;
					entry.GetPath(&path);
					this->LoadFile(path.Path());
				}
			}
			break;
		}
		case UNDO: {
			core->Undo();
			workTwo->UpdateGrid(core->grid);
			break;
		}
		case UP: {
			workSide->UpButton();
			break;
		}
		case DOWN: {
			workSide->DownButton();
			break;
		}
		default:
			BWindow::MessageReceived(msg);
	}
}

/* Message Queue*/
