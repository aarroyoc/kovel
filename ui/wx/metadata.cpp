#include "../../core/core.hpp"

#include "metadata.hpp"
#include <wx/spinctrl.h>

MetadataDialog::MetadataDialog(wxWindow* parent, bool newFile)
    : wxDialog(parent, wxID_ANY, "Metadata") {
  Core* core = Core::Instance();
  wxFont font;
  font.SetPointSize(14);
  // DO UI
  wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

  wxBoxSizer* name = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* nameLabel = new wxStaticText(this, wxID_ANY, "Name:");
  nameLabel->SetFont(font);
  name->Add(nameLabel);
  this->name = core->name;
  wxTextCtrl* nameForm = new wxTextCtrl(
      this, wxID_ANY, (newFile) ? "" : wxString::FromUTF8(this->name.c_str()));
  nameForm->SetMinSize(wxSize(200, 10));
  nameForm->Bind(wxEVT_TEXT, [this, nameForm](wxCommandEvent&) -> void {
	this->name = std::string(nameForm->GetValue().ToUTF8());
  }, -1);
  name->Add(nameForm, 0, wxEXPAND | wxALL);

  wxBoxSizer* author = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* authorLabel = new wxStaticText(this, wxID_ANY, "Author:");
  authorLabel->SetFont(font);
  author->Add(authorLabel);
  this->author = core->author;
  wxTextCtrl* authorForm =
      new wxTextCtrl(this, wxID_ANY,
                     (newFile) ? "" : wxString::FromUTF8(this->author.c_str()));
  authorForm->SetMinSize(wxSize(200, 10));
  authorForm->Bind(wxEVT_TEXT, [this, authorForm](wxCommandEvent&) -> void {
	this->author = std::string(authorForm->GetValue().ToUTF8());
  }, -1);
  author->Add(authorForm, 0, wxEXPAND | wxALL);

  wxBoxSizer* grid = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* gridLabel = new wxStaticText(this, wxID_ANY, "Grid size: ");
  gridLabel->SetFont(font);
  grid->Add(gridLabel);
  wxSpinCtrl* gridForm = new wxSpinCtrl(this);
  gridForm->SetValue(core->grid);
  gridForm->Bind(wxEVT_SPINCTRL, [this, gridForm](wxCommandEvent&) -> void {
	this->gridSize = (unsigned short)gridForm->GetValue();
  }, -1);
  if (!newFile)
	gridForm->Enable(false);
  grid->Add(gridForm, 0, wxEXPAND | wxALL);

  wxSizer* buttons = this->CreateButtonSizer(wxOK | wxCANCEL);

  sizer->Add(name, 0, wxEXPAND | wxALL);
  sizer->Add(author, 0, wxEXPAND | wxALL);
  sizer->Add(grid, 0, wxEXPAND | wxALL);
  sizer->Add(buttons, 0, wxEXPAND | wxALL);

  SetSizer(sizer);
}

MetadataDialog::~MetadataDialog() {
}
