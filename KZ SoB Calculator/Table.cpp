#include "Table.h"

using utils::CenterControl;

// ----- Class declaration -----

Table::Table(wxWindow* parent, int id)
	: wxPanel(parent, id)
{
	this->CreateSizer();
	this->ChangeCategory(Categories::ALL_STAGES);
}

void Table::_Layout() 
{
	this->GetSizer()->Layout();
	this->GetParent()->Layout();
	this->GetParent()->Fit();
}

// ----- Sizer creation -----

void Table::CreateSizer() 
{
	// Column names
	wxBoxSizer* names = new wxBoxSizer(wxHORIZONTAL);

	names->Add(CenterControl(new wxStaticText(this, wxID_ANY, "Level"), names), 2, wxALIGN_CENTER_VERTICAL);
	names->Add(CenterControl(new wxStaticText(this, wxID_ANY, "Time"), names), 1, wxALIGN_CENTER_VERTICAL);
	names->Add(CenterControl(new wxStaticText(this, wxID_ANY, "Rank"), names), 1, wxALIGN_CENTER_VERTICAL);

	// Main Table
	this->table = new wxBoxSizer(wxVERTICAL);

	for (int i = 0; i < 12; i++) {
		LevelRow* level_row = new LevelRow(this, wxID_ANY, LevelNames[i]);
		this->level_rows.push_back(level_row);
		this->table->Add(level_row, 0, wxEXPAND);
	}
	this->table->SetMinSize(295, 0);

	/// Total row
	this->total_time_sizer = new wxBoxSizer(wxHORIZONTAL);

	this->TotalTime = new wxStaticText(this, wxID_ANY, "");

	wxBoxSizer* time = new wxBoxSizer(wxVERTICAL);
	time->Add(this->TotalTime, 0, wxALIGN_RIGHT);

	this->TotalMedal = new wxStaticBitmap(this, wxID_ANY, wxBitmap(1, 1));

	this->total_time_sizer->Add(new wxStaticText(this, wxID_ANY, "Total:"), 2, wxLEFT | wxALIGN_CENTER_VERTICAL, 42);
	this->total_time_sizer->Add(time, 1, wxALIGN_CENTER_VERTICAL);
	this->total_time_sizer->Add(this->TotalMedal, 1, wxALIGN_CENTER_VERTICAL);


	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(names, 0, wxEXPAND);
	sizer->Add(new wxStaticLine(this, wxID_ANY), 0, wxEXPAND | wxALL, 5);
	sizer->Add(this->table, wxEXPAND);
	sizer->Add(new wxStaticLine(this, wxID_ANY), 0, wxEXPAND | wxALL, 5);
	sizer->Add(this->total_time_sizer, 0, wxEXPAND);

	this->SetSizer(sizer);
	this->Layout();
}


void Table::ChangeCategory(int category) 
{
	/*
		Each category has a different amount of levels needed for the run to be completed
		- All Stages requires first 11 levels to be completed,
		- Golden Ending requires first 7 levels and then choosing Golden Mask,
		- Psychotherapy requires all 12 levels. (Base game + Psychotherapy bossfight)
	*/
	this->category = category;

	if (category == Categories::ALL_STAGES) {
		for (int i = 7; i < 11; i++) {
			LevelRow* item = this->level_rows[i];
			item->Show();
		}
		this->level_rows[11]->Hide();
	}
	else if (category == Categories::GOLDEN_ENDING) {
		for (int i = 7; i < 12; i++) {
			LevelRow* item = this->level_rows[i];
			item->Hide();
		}
	}
	else if (category == Categories::PSYCHOTHERAPY) {
		for (int i = 7; i < 12; i++) {
			LevelRow* item = this->level_rows[i];
			item->Show();
		}
	}

	this->_Layout();
}