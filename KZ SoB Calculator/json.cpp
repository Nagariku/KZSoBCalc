#include "json.h"


json settings_json;


void CreateDefaultSettings() 
{
	settings_json["category"] = "All Stages";
	settings_json["run_type"] = "Action Only";
	settings_json["difficulty"] = "Normal";
	settings_json["show_settings"] = false;
	settings_json["game_store"] = "GOG";
}

void CreateSettingsFile() 
{
	CreateDefaultSettings();
	WriteSettingsFile();
}

void ReadSettingsFile() 
{
	std::ifstream file("settings.json");
	file >> settings_json;
	file.close();
}

void WriteSettingsFile() 
{
	std::ofstream file("settings.json");
	file << settings_json << std::endl;
	file.close();
}
