#include "LevelName.h"

// ----- Class declaration -----

LevelNameWidget::LevelNameWidget(wxWindow* parent, int id, std::string LevelName)
	: wxPanel(parent, id)
{
	this->image = wxImage(wxString("images\\levels\\" + LevelName + ".png"), wxBITMAP_TYPE_PNG);
	this->image = this->image.Scale(LEVEL_IMAGE_SIZE, LEVEL_IMAGE_SIZE, wxIMAGE_QUALITY_HIGH);

	this->bitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(this->image));
	this->text = new wxStaticText(this, wxID_ANY, wxString(LevelName + ":"));

	wxBoxSizer* text = new wxBoxSizer(wxVERTICAL);
	text->Add(this->text, 0, wxLEFT | wxRIGHT, 10);

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(bitmap, 0, wxALIGN_LEFT);
	sizer->Add(text, 0, wxALIGN_CENTER_VERTICAL);

	sizer->FitInside(parent);
	this->SetSizer(sizer);
	this->Layout();
}


// ----- Image changers -----

void LevelNameWidget::ImageToGrayscale()
{
	this->bitmap->SetBitmap(this->image.ConvertToGreyscale());
}

void LevelNameWidget::ImageToNormal()
{
	this->bitmap->SetBitmap(wxBitmap(this->image));
}
