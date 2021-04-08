#include "widgets.h"


// Classes

LevelWidget::LevelWidget(wxWindow* parent, int id, std::string LevelName) 
	: wxPanel(parent, id, wxDefaultPosition, wxDefaultSize) 
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

void LevelWidget::ImageToGrayscale()
{
	this->bitmap->SetBitmap(this->image.ConvertToGreyscale());
}

void LevelWidget::ImageToNormal()
{
	this->bitmap->SetBitmap(wxBitmap(this->image));
}



LevelRow::LevelRow(wxWindow* parent, int id, std::string LevelName)
	: wxPanel(parent, id) 
{
	this->level_name = new LevelWidget(this, wxID_ANY, LevelName);
	this->time_label = new wxStaticText(this, wxID_ANY, "No Entry");
	this->medal_bitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(1, 1));

	wxBoxSizer* time = new wxBoxSizer(wxVERTICAL);
	time->Add(this->time_label, 0, wxALIGN_RIGHT);

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	sizer->Add(this->level_name,   2, wxALIGN_CENTER_VERTICAL);
	sizer->Add(time,               1, wxALIGN_CENTER_VERTICAL);
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



Table::Table(wxWindow* parent, int id)
	: wxPanel(parent, id) 
{
	this->CreateSizer();
	this->SwitchCategory(Categories::ALL_STAGES);
}


void Table::CreateSizer() {
	// Column names
	wxBoxSizer* names = new wxBoxSizer(wxHORIZONTAL);

	names->Add(center_control(new wxStaticText(this, wxID_ANY, "Level"), names), 2, wxALIGN_CENTER_VERTICAL);
	names->Add(center_control(new wxStaticText(this, wxID_ANY, "Time"),  names), 1, wxALIGN_CENTER_VERTICAL);
	names->Add(center_control(new wxStaticText(this, wxID_ANY, "Rank"),  names), 1, wxALIGN_CENTER_VERTICAL);

	// Main Table
	this->table = new wxBoxSizer(wxVERTICAL);

	for (int i = 0; i < 12; i++) {
		LevelRow* level_row = new LevelRow(this, wxID_ANY, LevelNames[i]);
		this->level_rows.push_back(level_row);
		this->table->Add(level_row, 0, wxEXPAND);
	}
	this->table->SetMinSize(295, 0);

	/// Total row
	wxBoxSizer* total = new wxBoxSizer(wxHORIZONTAL);

	this->TotalTime = new wxStaticText(this, wxID_ANY, "");

	wxBoxSizer* time = new wxBoxSizer(wxVERTICAL);
	time->Add(this->TotalTime, 0, wxALIGN_RIGHT);

	this->TotalMedal = new wxStaticBitmap(this, wxID_ANY, wxBitmap(1, 1));

	total->Add(new wxStaticText(this, wxID_ANY, "Total:"), 2, wxLEFT | wxALIGN_CENTER_VERTICAL, 42);
	total->Add(time                                      , 1, wxALIGN_CENTER_VERTICAL);
	total->Add(this->TotalMedal                          , 1, wxALIGN_CENTER_VERTICAL);


	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(names, 0, wxEXPAND);
	sizer->Add(new wxStaticLine(this, wxID_ANY), 0, wxEXPAND | wxALL, 5);
	sizer->Add(this->table, wxEXPAND);
	sizer->Add(new wxStaticLine(this, wxID_ANY), 0, wxEXPAND | wxALL, 5);
	sizer->Add(total, 0, wxEXPAND);

	this->SetSizer(sizer);
	this->Layout();
}


void Table::_Layout() {
	this->GetSizer()->Layout();
	this->GetParent()->Layout();
	this->GetParent()->Fit();
}


void Table::SwitchCategory(int category) {
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

Settings::Settings(wxWindow* parent, int id) :
	wxPanel(parent, id)
{
	this->CreateComboBoxes();
	wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText* text = new wxStaticText(this, wxID_ANY, "Settings:");
	wxBoxSizer* text_sizer = new wxBoxSizer(wxHORIZONTAL);
	text_sizer->Add(text, 0, wxALIGN_CENTER_VERTICAL);

	wxBoxSizer* difficulty = new wxBoxSizer(wxHORIZONTAL);
	difficulty->Add(new wxStaticText(this, wxID_ANY, "Difficulty: "), 1, wxEXPAND);
	difficulty->Add(this->difficulty, 2, wxEXPAND);

	wxBoxSizer* run_type = new wxBoxSizer(wxHORIZONTAL);
	run_type->Add(new wxStaticText(this, wxID_ANY, "Run type: "), 1, wxEXPAND);
	run_type->Add(this->run_type, 2, wxEXPAND);

	wxBoxSizer* category = new wxBoxSizer(wxHORIZONTAL);
	category->Add(new wxStaticText(this, wxID_ANY, "Category: "), 1, wxEXPAND);
	category->Add(this->category, 2, wxEXPAND);

	main_sizer->Add(new wxStaticLine(this, wxID_ANY), 0, wxEXPAND | wxALL, 5);
	main_sizer->Add(text_sizer, 0, wxALIGN_CENTER_HORIZONTAL);
	main_sizer->Add(new wxStaticLine(this, wxID_ANY), 0, wxEXPAND | wxALL, 5);
	main_sizer->Add(difficulty, 0, wxEXPAND);
	main_sizer->Add(run_type, 0, wxEXPAND);
	main_sizer->Add(category, 0, wxEXPAND);

	this->SetSizer(main_sizer);
	this->Layout();
}

void Settings::CreateComboBoxes()
{
	wxString d_options[2] = {"Normal", "Hardmode"};
	this->difficulty = new wxComboBox(this, IDs::Difficulty_Box, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxArrayString(2, d_options));
	this->difficulty->SetSelection(0);

	wxString rt_options[3] = { "Action Only", "No Cutscenes", "Full run" };
	this->run_type = new wxComboBox(this, IDs::RunType_Box, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxArrayString(3, rt_options));
	this->run_type->SetSelection(0);

	wxString c_options[3] = { "All Stages", "Golden Ending", "Psychotherapy" };
	this->category = new wxComboBox(this, IDs::Category_Box, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxArrayString(3, c_options));
	this->category->SetSelection(0);
}
