#include "SavedGames.h"
#include <algorithm>
#include <fstream>
#include <filesystem>

void SaveGamesToFile(const std::vector<Games>& gamesVec, const std::string fileName) {
	std::ofstream fout(fileName);
	fout << gamesVec.size();

	for (const Games& games : gamesVec) {
		std::string name = games.gameName;
		std::replace(name.begin(), name.end(), ' ', '_');
		double score = games.values[0];
		double visuals = games.values[1];
		double soundAndMusic = games.values[2];
		double coreLoop = games.values[3];
		double storyDesign = games.values[4];
		double UI = games.values[5];
		double competition = games.values[6];
		double realWorldSim = games.values[7];
		double rawPrice = games.values[8];
		double pricePaid = games.values[9];

		fout << '\n' << name << ' ' << score << ' ' << visuals << ' ' << soundAndMusic << ' ' << coreLoop << ' ' << storyDesign
			 << ' ' << UI << ' ' << competition << ' ' << realWorldSim << ' ' << rawPrice << ' ' << pricePaid;
	}
	fout.close();
}

std::vector<Games> LoadGamesFromFile(const std::string fileName) {
	if (!std::filesystem::exists(fileName))
		return std::vector<Games>();

	std::vector<Games> gamesVec;
	std::ifstream fin(fileName);

	int n;
	fin >> n;

	for (int i = 0; i < n; i++) {
		std::string name;
		double score, visuals, soundAndMusic, coreLoop, storyDesign, UI, competition, realWorldSim, rawPrice, pricePaid;

		fin >> name >> score >> visuals >> soundAndMusic >> coreLoop >> storyDesign >> UI >> competition >> realWorldSim >> rawPrice >> pricePaid;
		std::replace(name.begin(), name.end(), '_', ' ');
		gamesVec.push_back(Games{ name, score, visuals, soundAndMusic, coreLoop, storyDesign, UI, competition, realWorldSim, rawPrice, pricePaid });
	}
	fin.close();
	return gamesVec;
}
