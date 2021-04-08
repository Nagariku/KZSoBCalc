#pragma once

#include <iostream>
#include <fstream>

#include <nlohmann/json.hpp>

using json = nlohmann::json;


extern json settings_json;


void CreateDefaultSettings();

void CreateSettingsFile();

void ReadSettingsFile();

void WriteSettingsFile();

