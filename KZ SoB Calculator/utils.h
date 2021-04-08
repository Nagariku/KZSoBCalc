#pragma once


#include <map>
#include <limits>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <filesystem>

#include <stdio.h>
#include <time.h>

#include <Windows.h>
#include <ShlObj.h>
#include "wx/wx.h"


namespace fs = std::filesystem;


#pragma push_macro("max")
#undef max
const long long max_size = std::numeric_limits<std::streamsize>::max();
#pragma pop_macro("max")


namespace utils
{
	// Folders
	bool GetKnownFolderWrapper(std::string& out, REFKNOWNFOLDERID rfid);

	std::string GetLocalAppdata();

	// Widget stuff
	wxBoxSizer* CenterControl(wxWindow* control, wxBoxSizer* sizer);

	std::vector<wxBitmap> GetMedalBitmaps();

	wxColour GetColor(std::string color);

	// String stuff
	std::string CreateTimeString(float timedelta);

	std::string zfill(std::string _string, int leading_zeros);

	std::string GetCurrentDatetimeString();

	// Backup loading/saving helpers
	bool CreateBackupDirectory();

	// Компьютер\HKEY_CURRENT_USER\SOFTWARE\Valve\Steam\ActiveProcess
	bool GetSteamId(int& str);
	// Компьютер\HKEY_CURRENT_USER\SOFTWARE\GOG.com\Galaxy\settings
	bool GetGalaxyId(std::string& str);

	void ShowErrorMessage(std::string message);
}
