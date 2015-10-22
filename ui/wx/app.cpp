
#include "wx/wx.h"

#include "window.hpp"

#ifdef WIN32
#pragma comment( linker, "/SUBSYSTEM:WINDOWS" )
#endif

class KovelApp : public wxApp{
	bool OnInit(){
		MainWindow* win=new MainWindow();
		win->Show();
		win->Update();
		return true;
	}
};

IMPLEMENT_APP(KovelApp)
