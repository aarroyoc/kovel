#ifndef METADATA_HPP
#define METADATA_HPP

#include <AppKit.h>
#include <InterfaceKit.h>
#include <string>

class MetadataDialog : public BWindow{
	public:
		MetadataDialog(bool newFile);
		void MessageReceived(BMessage* msg);
		std::string name;
		std::string author;
		BTextControl* authorForm;
		BTextControl* nameForm;
		BSlider* gridForm;
		unsigned short gridSize=5;
		bool n;
};

#endif
