#pragma once

#include "wx/wx.h"
#include "wx/sizer.h"
#include "wx/statline.h"

#include "utils.h"
#include "globals.h"
#include "time_reader.h"

#include "LevelRow.h"


class Table : public wxPanel 
{
public:
	Table(wxWindow* parent, int id);

	wxBoxSizer* table;
	std::vector<LevelRow*> level_rows;
	std::vector<wxBitmap> medal_bitmaps;

	wxStaticText* TotalTime;
	wxStaticBitmap* TotalMedal;
	wxBoxSizer* total_time_sizer;

	int category;

	void ChangeCategory(int category);
private:
	void _Layout();
	void CreateSizer();
};

