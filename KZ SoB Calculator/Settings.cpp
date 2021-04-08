#include "Settings.h"

using utils::CenterControl;

// ----- Class declaration -----

Settings::Settings(wxWindow* parent, int id) :
	wxPanel(parent, id)
{
	this->CreateComboBoxes();

	// Title
	wxStaticText* text = new wxStaticText(this, wxID_ANY, "Settings:");
	wxBoxSizer* text_sizer = new wxBoxSizer(wxHORIZONTAL);
	text_sizer->Add(text, 0, wxALIGN_CENTER_VERTICAL);

	// Difficulty controls
	wxBoxSizer* difficulty = new wxBoxSizer(wxHORIZONTAL);
	difficulty->Add(new wxStaticText(this, wxID_ANY, "Difficulty: "), 1, wxEXPAND);
	difficulty->Add(this->difficulty, 2, wxEXPAND);

	// Run Type controls
	wxBoxSizer* run_type = new wxBoxSizer(wxHORIZONTAL);
	run_type->Add(new wxStaticText(this, wxID_ANY, "Run type: "), 1, wxEXPAND);
	run_type->Add(this->run_type, 2, wxEXPAND);

	// Category controls
	wxBoxSizer* category = new wxBoxSizer(wxHORIZONTAL);
	category->Add(new wxStaticText(this, wxID_ANY, "Category: "), 1, wxEXPAND);
	category->Add(this->category, 2, wxEXPAND);

	// Visibility checkbox
	this->settings_visibility = new wxCheckBox(this, IDs::Show_Settings_On_Launch, "Show settings on launch");
	this->settings_visibility->SetValue(settings_json["show_settings"].get<bool>());

	// Game store controls
	wxBoxSizer* game_store = new wxBoxSizer(wxHORIZONTAL);
	game_store->Add(new wxStaticText(this, wxID_ANY, "Game Store: "), 0);
	game_store->Add(this->game_store, 1, wxEXPAND);

	// Main sizer
	wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);

	main_sizer->Add(new wxStaticLine(this, wxID_ANY), 0, wxEXPAND | wxALL, 5);
	main_sizer->Add(text_sizer, 0, wxALIGN_CENTER_HORIZONTAL);
	main_sizer->Add(new wxStaticLine(this, wxID_ANY), 0, wxEXPAND | wxALL, 5);
	
	main_sizer->Add(CenterControl(new wxStaticText(this, wxID_ANY, "Times Selection"), main_sizer), 0, wxALIGN_CENTER_HORIZONTAL);
	main_sizer->Add(difficulty, 0, wxEXPAND);
	main_sizer->Add(run_type, 0, wxEXPAND);
	main_sizer->Add(category, 0, wxEXPAND);
	main_sizer->Add(this->settings_visibility, 0, wxEXPAND);

	main_sizer->Add(CenterControl(new wxStaticText(this, wxID_ANY, "Backups"), main_sizer), 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 15);
	main_sizer->Add(new wxButton(this, IDs::Save_Backup, "Save backup"), 0, wxEXPAND);
	main_sizer->Add(new wxButton(this, IDs::Load_Backup, "Load backup"), 0, wxEXPAND);
	main_sizer->Add(game_store, 0, wxEXPAND);

	main_sizer->AddStretchSpacer(1);
	main_sizer->Add(new wxButton(this, IDs::Save_Settings, "Save settings"), 0, wxEXPAND);

	this->SetSizer(main_sizer);
	this->Layout();
}

void Settings::CreateComboBoxes()
{
	wxString d_options[2] = { "Normal", "Hardmode" };
	this->difficulty = new wxComboBox(this, IDs::Difficulty_Box, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxArrayString(2, d_options), wxCB_READONLY);
	this->difficulty->SetSelection(0);

	this->run_type = new wxComboBox(this, IDs::RunType_Box, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxArrayString(3, RUN_TYPE_OPTIONS), wxCB_READONLY);
	this->run_type->SetSelection(0);

	wxString c_options[3] = { "All Stages", "Golden Ending", "Psychotherapy" };
	this->category = new wxComboBox(this, IDs::Category_Box, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxArrayString(3, c_options), wxCB_READONLY);
	this->category->SetSelection(0);

	this->game_store = new wxComboBox(this, IDs::Game_Store_Box, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxArrayString(1, {" "}), wxCB_READONLY);
}
