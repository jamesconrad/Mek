#ifndef SAVEFUNCTIONS_H
#define SAVEFUNCTIONS_H

#include <fstream>
#include <vector>


inline void saveScoreBoard(std::vector<unsigned int> &_sndScores, std::vector<std::string> &_sndNames, std::vector<unsigned int> &_survivalScores, std::vector<std::string> &_survivalNames)
{
	std::ofstream myFile;
	myFile.open("ScoreTable.sav", fstream::out);
	
	for (unsigned int i = 0; i < _sndNames.size(); i++)
	{
		myFile << _sndNames[i].c_str() << " " << _sndScores[i] << std::endl;
	}

	myFile << "Surv Names:" << std::endl;

	for (unsigned int i = 0; i < _survivalNames.size(); i++)
	{
		myFile << _survivalNames[i].c_str() << " " << _survivalScores[i] << std::endl;
	}
	myFile.close();

}

inline void loadScoreBoard(std::vector<unsigned int> &_sndScores, std::vector<std::string> &_sndNames, std::vector<unsigned int> &_survivalScores, std::vector<std::string> &_survivalNames)
{
	FILE *myFile = fopen("ScoreTable.sav", "r");
	char buff[256];
	char secondBuff[64];
	std::string aName;
	float aScore;

	if (myFile == NULL)
	{
		return;
	}

	bool isSnD = true;

	while (fgets(buff, 256, myFile))
	{
		if (buff[3] == 'v')
		{
			isSnD = false;
			continue;
		}

		if (isSnD)
		{
			aName = strtok(buff, " \t\n\r");
			aScore = atof(strtok(NULL, " \t\n\r"));

			_sndNames.push_back(aName);
			_sndScores.push_back(aScore);
		}
		else if (!isSnD)
		{
			aName = strtok(buff, " \t\n\r");
			aScore = atof(strtok(NULL, " \t\n\r"));

			_survivalNames.push_back(aName);
			_survivalScores.push_back(aScore);
		}
	}

	return;
}

#endif