
#include "wx/wx.h"

#include "window.hpp"

class KovelApp : public wxApp{
	bool OnInit(){
		MainWindow* win=new MainWindow();
		win->Show();
		win->Update();
		return true;
	}
};

IMPLEMENT_APP(KovelApp)
