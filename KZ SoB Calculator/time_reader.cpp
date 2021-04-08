#include "time_reader.h"


std::vector<Level> time_reader::GetTimes(std::string id) 
{
	std::vector<Level> levels_vector;
	std::vector<Fields> fields_vector = ReadFieldsFromFile(id);

	for (int i = 0; i != 12*6; i += 6) {
		LevelDifficulty normal = {
			{RunType::FULL_RUN, fields_vector[i]},
			{RunType::NO_CUTSCENES, fields_vector[i + 1]},
			{RunType::ACTION_ONLY, fields_vector[i + 2]}
		};
		LevelDifficulty hard = {
			{RunType::FULL_RUN, fields_vector[i + 3]},
			{RunType::NO_CUTSCENES, fields_vector[i + 4]},
			{RunType::ACTION_ONLY, fields_vector[i + 5]}
		};
		Level level = {
			{Difficulty::NORMAL, normal},
			{Difficulty::HARDMODE, hard}
		};

		levels_vector.push_back(level);
	}

	return levels_vector;
}


std::vector<Fields> time_reader::ReadFieldsFromFile(std::string id) 
{
	std::vector<Fields> fields_vector;

	std::string path = LocalAppdata + std::string("\\Katana_ZERO\\KatanaSpeedrunOpt" + id + ".zero");
	std::ifstream file_stream(path);

	if (file_stream.is_open()) {
		for (int i = 0; i < 32; i++) file_stream.ignore(max_size, '\n');

		std::string value = "";
		for (int i = 0; i < 12*6; i++) {
			std::getline(file_stream, value);
			float time = std::stof(value);

			std::getline(file_stream, value);
			int deaths = std::stoi(value);

			std::getline(file_stream, value);
			int rank = std::stoi(value);

			Fields fields = { {"time", time}, {"deaths", deaths}, {"rank", rank} };
			fields_vector.push_back(fields);
		}

		return fields_vector;
	}
	else {
		throw std::runtime_error("Couldn't read the file, have you run KZ on this machine yet?");
	}
}

