#include "../../core/core.hpp"
#include "wx/wx.h"

#include "window.hpp"

#ifdef WIN32
#pragma comment( linker, "/SUBSYSTEM:WINDOWS" )
#endif

class KovelApp : public wxApp{
	bool OnInit(){
		wxInitAllImageHandlers();
		MainWindow* win=new MainWindow();
		win->Show();
		win->Update();
		
		if(this->argc == 2){
			win->LoadFile(this->argv[1]);
		}
		return true;
	}
};

IMPLEMENT_APP(KovelApp)
