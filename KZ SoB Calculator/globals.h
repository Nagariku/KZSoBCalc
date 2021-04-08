#pragma once


#include <vector>

#include <wx/wx.h>

#include "utils.h"


// Medal Bitmaps for Level rank Image widgets
extern std::vector<wxBitmap> MedalBitmaps;

// Font for the app
extern wxFont Font;

// All Level Names
const std::string LevelNames[12] = {
	"Factory",
	"Hotel",
	"Club",
	"Prison",
	"Studio",
	"Mansion",
	"Chinatown",
	"Dragon",
	"Slaughterhouse",
	"Bunker",
	"Bunker2",
	"Psychotherapy"
};

// Getting LocalAppdata so we don't call the function too much
const std::string LocalAppdata = utils::GetLocalAppdata();
const std::string BackupsFolder = LocalAppdata + "\\Katana_ZERO\\KZ SoB Backups";

// Image sizes
const int LEVEL_IMAGE_SIZE = 30;

const int SETTINGS_IMAGE_SIZE = 30;

// Strings for Run Type Combobox
const wxString GE_RUN_TYPE_OPTIONS[2] = { "No Cutscenes", "Full run" }; // GE is Golden Ending 
const wxString RUN_TYPE_OPTIONS[3] = { "Action Only", "No Cutscenes", "Full run" };

// Enums
enum IDs {
	Reload_Times = 1001,
	Open_Settings = 1002,
	Difficulty_Box = 1005,
	RunType_Box = 1006,
	Category_Box = 1007,
	Save_Settings = 1008,
	Show_Settings_On_Launch = 1009,
	Save_Backup = 1010,
	Load_Backup = 1011,
	Game_Store_Box = 1012
};


enum Categories {
	ALL_STAGES = 0,
	GOLDEN_ENDING = 1,
	PSYCHOTHERAPY = 2
};

enum Difficulty {
	NORMAL = 0,
	HARDMODE = 1
};

enum RunType {
	ACTION_ONLY = 0,
	NO_CUTSCENES = 1,
	FULL_RUN = 2
};
