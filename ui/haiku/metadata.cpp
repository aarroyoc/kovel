#include "../../core/core.hpp"

#include <Layout.h>
#include <GroupLayout.h>

#include "metadata.hpp"

const uint32 OK = 300, CANCEL = 301;

MetadataDialog::MetadataDialog(bool newFile)
    : BWindow(BRect(100, 100, 500, 300), "Metadata", B_MODAL_WINDOW, 0) {
  n = newFile;
  BView* all = new BView(Bounds(), NULL, B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
  AddChild(all);
  Core* core = Core::Instance();
  BFont font;
  font.SetSize(14.0);

  all->SetViewColor(220, 220, 220);

  BGroupLayout* sizer = new BGroupLayout(B_VERTICAL);
  BGroupLayout* name = new BGroupLayout(B_HORIZONTAL);
  BGroupLayout* author = new BGroupLayout(B_HORIZONTAL);

  all->SetLayout(sizer);

  BStringView* nameLabel = new BStringView(NULL, "Name: ");
  nameLabel->SetFont(&font);

  this->name = core->name;
  nameForm = new BTextControl(NULL, "", "", NULL);
  nameForm->SetText((newFile) ? "" : this->name.c_str());
  nameForm->SetExplicitMinSize(BSize(200, 10));

  BStringView* authorLabel = new BStringView(NULL, "Author: ");
  authorLabel->SetFont(&font);

  this->author = core->author;
  authorForm = new BTextControl(NULL, "", "", NULL);
  authorForm->SetText((newFile) ? "" : this->author.c_str());
  authorForm->SetExplicitMinSize(BSize(200, 10));

  gridForm = new BSlider(BRect(0, 0, 50, 100), NULL, "Grid size", NULL, 1, 10);
  gridForm->SetValue(core->grid);
  if (!newFile)
	gridForm->SetEnabled(false);

  BButton* ok = new BButton("OK", new BMessage(OK));

  sizer->AddItem(name);
  sizer->AddItem(author);
  sizer->AddView(gridForm);
  sizer->AddView(ok);
  sizer->AddView(new BButton("Cancel", new BMessage(CANCEL)));

  name->AddView(nameLabel);
  name->AddView(nameForm);
  author->AddView(authorLabel);
  author->AddView(authorForm);

  SetDefaultButton(ok);

  /*sizer->AddItem(grid); - Falta BSlider o similar */
  /* sizer->AddView(buttons); - Faltan botones de OK y CANCEL */
}

void MetadataDialog::MessageReceived(BMessage* msg) {
  switch (msg->what) {
  case OK: {
	int32 count = be_app->CountWindows();
	for (int32 i = 0; i < count; i++) {
	  BMessage* mesg = new BMessage(666);
	  mesg->AddString("NAME", nameForm->Text());
	  mesg->AddString("AUTHOR", authorForm->Text());
	  mesg->AddInt32("GRID_SIZE", gridForm->Value());
	  mesg->AddBool("NEW_FILE", this->n);
	  be_app->WindowAt(i)->PostMessage(mesg);
	}

	Quit();
	break;
  }
  case CANCEL: {
	Quit();
	break;
  }
  default:
	return BWindow::MessageReceived(msg);
  }
}
