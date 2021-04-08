#pragma once

#include "wx/wx.h"
#include "wx/sizer.h"

#include "globals.h"
#include "time_reader.h"

#include "LevelName.h"


class LevelRow : public wxPanel {
public:
	LevelRow(wxWindow* parent, int id, std::string LevelName);

	void update_widgets(Fields level_entry);
private:
	LevelNameWidget* level_name;
	wxStaticText* time_label;
	wxStaticBitmap* medal_bitmap;
};
