#include <iostream>
#include <fstream>
#include <list>
#include <utility>
#include <unordered_map>

void parseInput(std::pair<unsigned, unsigned> &inputData)
{
	std::string line;
	std::ifstream input("input.txt");
	if(input.is_open())
	{
		getline(input,line);
		size_t pos = line.find_first_of(" ");
		unsigned numberOfPlayers = std::stoi(line.substr(0,pos));
		pos = line.find_first_of("1234567890", pos);
		size_t endpos = line.find_first_of(" ", pos);
		unsigned highestPoints = std::stoi(line.substr(pos,endpos-pos));
		inputData.first = numberOfPlayers;
		inputData.second = highestPoints;
	}
	input.close();
}

uint64_t marbleGame(const std::pair<unsigned, unsigned> &inputData, const unsigned scale)
{
	uint64_t result = 0;
	unsigned players = inputData.first;
	unsigned highestPoints = inputData.second*scale;
	unsigned player = 1;
	std::list<unsigned> circle;
	unsigned index = 1;
	int steps = 0;
	circle.push_back(0);
	circle.push_back(1);
	auto it = circle.begin();
	it = std::next(it,1);
	unsigned marbleNumber = 2;
	std::unordered_map<unsigned,unsigned> scores;
	
	while(marbleNumber <= highestPoints)
	{
		if(marbleNumber%23 != 0)
		{
			steps = (index+2)%circle.size() - index;
			index += steps;
			it = std::next(it,steps);
			it = circle.insert(it,marbleNumber);
		}
		else
		{
			steps = (index+circle.size()-7)%circle.size() - index;
			index += steps;
			it = std::next(it,steps);
			int newScore = marbleNumber + *it;
			it = circle.erase(it);
			scores[player] += newScore;
		}
		marbleNumber++;
		player = (player+1)%players;
	}	

	for(auto it=scores.begin(); it!=scores.end(); it++)
	{
		if(result < it->second)
		{
			result = it->second;
		}
	}
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	std::pair<unsigned,unsigned> inputData;
	
	parseInput(inputData);

	resultA = marbleGame(inputData,1);
	resultB = marbleGame(inputData,100);
	
	std::cout << "result A: " << resultA << '\n';
	std::cout << "result B: " << resultB << std::endl;
	return 0;
}
