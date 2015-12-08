#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <AppKit.h>
#include <InterfaceKit.h>

class KovelWindow : public BWindow{
	public:
		KovelWindow();
		void LoadFile(BString path);
		bool QuitRequested();
		void MessageReceived(BMessage* msg);
		BMenuBar* CreateMenuBar();	
};

#endif
