#pragma once

#include "wx/wx.h"
#include "wx/sizer.h"
#include "wx/statline.h"

#include <map>
#include <string>
#include <filesystem>

#include "utils.h"
#include "time_reader.h"
#include "globals.h"


namespace fs = std::filesystem;


class LevelWidget : public wxPanel {
public:
	LevelWidget(wxWindow* parent, int id, std::string LevelName);

	void ImageToGrayscale();
	void ImageToNormal();
private:
	wxImage image;
	wxStaticBitmap* bitmap;
	wxStaticText* text;
};


class LevelRow : public wxPanel {
public:
	LevelRow(wxWindow* parent, int id, std::string LevelName);

	void update_widgets(Fields level_entry);
private:
	LevelWidget* level_name;
	wxStaticText* time_label;
	wxStaticBitmap* medal_bitmap;
};


class Table : public wxPanel {
public:
	Table(wxWindow* parent, int id);

	wxBoxSizer* table;
	std::vector<LevelRow*> level_rows;
	std::vector<wxBitmap> medal_bitmaps;

	wxStaticText* TotalTime;
	wxStaticBitmap* TotalMedal;

	int category;

	void SwitchCategory(int category);
private:
	void _Layout();
	void CreateSizer();
};


class Settings : public wxPanel {
public:
	Settings(wxWindow* parent, int id);

	wxComboBox* difficulty;
	wxComboBox* run_type;
	wxComboBox* category;
private:
	void CreateComboBoxes();
};
