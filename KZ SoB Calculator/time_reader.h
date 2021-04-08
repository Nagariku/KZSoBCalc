#pragma once


#include <map>
#include <any>
#include <string>
#include <vector>

#include <fstream>
#include <iostream>

#include "globals.h"


typedef std::map<std::string, std::any> Fields;
typedef std::map<int, Fields> LevelDifficulty;
typedef std::map<int, LevelDifficulty> Level;


namespace time_reader 
{
	std::vector<Level> GetTimes(std::string id);
	std::vector<Fields> ReadFieldsFromFile(std::string id);
}
