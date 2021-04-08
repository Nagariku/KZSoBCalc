#pragma once

#include "wx/wx.h"
#include "wx/statline.h"

#include "json.h"
#include "globals.h"
#include "utils.h"


class Settings : public wxPanel {
public:
	Settings(wxWindow* parent, int id);

	wxComboBox* difficulty;
	wxComboBox* run_type;
	wxComboBox* category;
	wxComboBox* game_store;

	wxCheckBox* settings_visibility;
private:
	void CreateComboBoxes();
};

