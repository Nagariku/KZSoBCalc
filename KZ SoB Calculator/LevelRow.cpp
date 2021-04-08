#include "LevelRow.h"

using utils::GetColor;
using utils::CreateTimeString;

// ----- Class declaration -----

LevelRow::LevelRow(wxWindow* parent, int id, std::string LevelName)
	: wxPanel(parent, id)
{
	this->level_name = new LevelNameWidget(this, wxID_ANY, LevelName);
	this->time_label = new wxStaticText(this, wxID_ANY, "No Entry");
	this->medal_bitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(1, 1));

	wxBoxSizer* time = new wxBoxSizer(wxVERTICAL);
	time->Add(this->time_label, 0, wxALIGN_RIGHT);

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	sizer->Add(this->level_name, 2, wxALIGN_CENTER_VERTICAL);
	sizer->Add(time, 1, wxALIGN_CENTER_VERTICAL);
	sizer->Add(this->medal_bitmap, 1, wxALIGN_CENTER_VERTICAL);

	this->SetSizer(sizer);
	this->Layout();
}


void LevelRow::update_widgets(Fields level_entry) {
	// If level entry doesn't exist, all values will equal -1, rank is changed to 0 for correct image to show
	int rank = std::any_cast<int>(level_entry["rank"]);
	if (rank == -1) rank = 0;

	float time = std::any_cast<float>(level_entry["time"]);
	if (time == float(-1)) {
		this->time_label->SetLabel("No Entry");
		this->time_label->SetForegroundColour(GetColor("Gray"));
	}
	else {
		this->time_label->SetLabel(CreateTimeString(time));
		this->time_label->SetForegroundColour(GetColor("Black"));
	}

	this->medal_bitmap->SetBitmap(MedalBitmaps[rank]);
	this->Layout();
}
