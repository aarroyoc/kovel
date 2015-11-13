#ifndef WINDOW_HPP
#define WINDOW_HPP

class MainWindow : public wxFrame{
	public:
		MainWindow();
		void LoadFile(wxString path);
};

#endif
