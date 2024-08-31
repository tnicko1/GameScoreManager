#pragma once
#include <vector>
#include <string>

struct Games {
	std::string gameName;
	int values[10];
};

void SaveGamesToFile(const std::vector<Games>& gamesVec, const std::string fileName);
std::vector<Games> LoadGamesFromFile(const std::string fileName);