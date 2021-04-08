#pragma once

#include "wx/wx.h"
#include "wx/sizer.h"

#include "globals.h"
#include "time_reader.h"


class LevelNameWidget : public wxPanel {
public:
	LevelNameWidget(wxWindow* parent, int id, std::string LevelName);

	void ImageToGrayscale();
	void ImageToNormal();
private:
	wxImage image;
	wxStaticBitmap* bitmap;
	wxStaticText* text;
};

