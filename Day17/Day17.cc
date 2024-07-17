#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <queue>
#include <utility>

void parseInput(std::vector<std::array<unsigned, 4>> &scanData, std::array<unsigned, 4> &scanBoundaries)
{
	std::string line;
	std::array<unsigned, 4> data;
	size_t pos = 0;
	size_t endpos = 0;
	std::ifstream input("input.txt");
	if(input.is_open())
	{
		while(getline(input, line))
		{
			if(line[0] == 'x')
			{
				data[0] = 0;
				pos = line.find_first_of("1234567890");
				endpos = line.find_first_of(",", pos);
				data[1] = std::stoi(line.substr(pos, endpos-pos));
				if(scanBoundaries[0] > data[1])
				{
					scanBoundaries[0] = data[1];
				}
				if(scanBoundaries[1] < data[1])
				{
					scanBoundaries[1] = data[1];
				}
				
				pos = line.find_first_of("1234567890", endpos);
				endpos = line.find_first_of(".", pos);
				data[2] = std::stoi(line.substr(pos, endpos-pos));
				if(scanBoundaries[2] > data[2])
				{
					scanBoundaries[2] = data[2];
				}
				
				pos = endpos+2;
				data[3] = std::stoi(line.substr(pos));
				if(scanBoundaries[3] < data[3])
				{
					scanBoundaries[3] = data[3];
				}
			}
			else
			{
				data[0] = 1;
				pos = line.find_first_of("1234567890");
				endpos = line.find_first_of(",", pos);
				data[1] = std::stoi(line.substr(pos, endpos-pos));
				if(scanBoundaries[2] > data[1])
				{
					scanBoundaries[2] = data[1];
				}
				if(scanBoundaries[3] < data[1])
				{
					scanBoundaries[3] = data[1];
				}
				
				pos = line.find_first_of("1234567890", endpos);
				endpos = line.find_first_of(".", pos);
				data[2] = std::stoi(line.substr(pos, endpos-pos));
				if(scanBoundaries[0] > data[2])
				{
					scanBoundaries[0] = data[2];
				}
				
				pos = endpos+2;
				data[3] = std::stoi(line.substr(pos));
				if(scanBoundaries[1] < data[3])
				{
					scanBoundaries[1] = data[3];
				}
			}
			scanData.push_back(data);
		}
	}
	input.close();
}

void drawMap(const std::vector<std::array<unsigned, 4>> &scanData, const std::array<unsigned, 4> &scanBoundaries, std::vector<std::string> &map)
{
	unsigned minX = scanBoundaries[0] - 2;
	unsigned maxX = scanBoundaries[1] + 3;
	unsigned xRange = maxX - minX;
	unsigned minY = scanBoundaries[2];
	unsigned maxY = scanBoundaries[3] + 1;
	unsigned yRange = maxY - minY;
	
	std::string line;
	line.reserve(xRange);
	map.reserve(yRange);
	
	for(unsigned i=0; i<xRange; i++)
	{
		line += '.';
	}
	for(unsigned i=0; i<yRange; i++)
	{
		map.push_back(line);
	}
	
	for(auto data : scanData)
	{
		if(data[0] == 0)
		{
			for(unsigned i=data[2]; i<=data[3]; i++)
			{
				map[i-minY][data[1]-minX] = '#';
			}
		}
		else
		{
			for(unsigned i=data[2]; i<=data[3]; i++)
			{
				map[data[1]-minY][i-minX] = '#';
			}
		}
	}
	
	map[0][500-minX+1] = '|';

}

uint64_t fillWithWater(std::vector<std::string> &map)
{
	uint64_t result = 0;
	
	std::pair<unsigned, unsigned> water;
	std::queue<std::pair<unsigned,unsigned>> waterSources;
	waterSources.push(std::make_pair(map[0].find_first_of("|"), 0));
	size_t leftWallPos = 0;
	size_t rightWallPos = 0;
	size_t leftHolePos = 0;
	size_t rightHolePos = 0;
	size_t yLimit = map.size()-1;
	
	while(!waterSources.empty())
	{	
		water = waterSources.front();
		waterSources.pop();
		
		if(map[water.second][water.first] == '~' || (map[water.second][water.first-1] == '|' && map[water.second][water.first+1] == '|'))
		{
			continue;
		}
		while(water.second < yLimit && map[water.second+1][water.first] == '.')
		{
			map[water.second+1][water.first] = '|';
			water.second++;
		}
		if(water.second == yLimit)
		{
			continue;
		}

		if(map[water.second+1][water.first] != '|')
		{
			leftWallPos = map[water.second].find_last_of("#", water.first);
			if(leftWallPos == std::string::npos)
			{
				leftWallPos = 0;
			}
			rightWallPos = map[water.second].find_first_of("#", water.first);
			if(rightWallPos == std::string::npos)
			{
				rightWallPos = map[0].size();
			}
			leftHolePos = map[water.second+1].find_last_not_of("#~", water.first);
			rightHolePos = map[water.second+1].find_first_not_of("#~", water.first);
			
			bool leftHole = false;
			bool rightHole = false;
			
			if(leftWallPos < leftHolePos)
			{
				leftHole = true;
			}
			if(rightWallPos > rightHolePos)
			{
				rightHole = true;
			}
			if(leftHole || rightHole)
			{
				size_t leftLimit = leftWallPos < leftHolePos ? leftHolePos : leftWallPos+1;
				size_t rightLimit = rightWallPos > rightHolePos ? rightHolePos : rightWallPos-1;
				
				for(unsigned i=leftLimit; i<=rightLimit; i++)
				{
					map[water.second][i] = '|';
				}
				if(leftHole)
				{
					waterSources.push(std::make_pair(leftHolePos,water.second));
				}
				if(rightHole)
				{
					waterSources.push(std::make_pair(rightHolePos,water.second));
				}
			}
			else
			{
				bool filled = false;
				size_t newPos = 0;
				size_t tempLeftWallPos = leftWallPos;
				size_t tempRightWallPos = rightWallPos;
				
				while(!filled)
				{
					for(unsigned i=tempLeftWallPos+1; i<tempRightWallPos; i++)
					{
						map[water.second][i] = '~';
						unsigned temp = water.second+1;
						while(map[temp][i] == '.')
						{
							map[temp][i] = '~';
							temp++;
						}
					}
					
					newPos = map[water.second-1].find_last_of("#", water.first);
					if(newPos == std::string::npos)
					{
						newPos = 0;
					}
					if(newPos < leftWallPos)
					{
						filled = true;
					}
					else
					{
						tempLeftWallPos = newPos;
					}
					
					newPos = map[water.second-1].find_first_of("#", water.first);
					if(rightWallPos == std::string::npos)
					{
						newPos = map[0].size();
					}
					if(newPos > rightWallPos)
					{
						filled = true;
					}
					else
					{
						tempRightWallPos = newPos;
					}
					water.second--;
				}
				waterSources.push(std::make_pair(water.first,water.second));
			}
		}
	}
	
	for(std::string &line : map)
	{
		for(char &element : line)
		{
			if(element == '|' || element == '~')
			{
				result++;
			}
		}
	}
	
	return result;
}

uint64_t countStillWater(const std::vector<std::string> &map)
{
	uint64_t result = 0;
	for(const std::string &line : map)
	{
		for(const char &element : line)
		{
			if(element == '~')
			{
				result++;
			}
		}
	}
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	std::vector<std::array<unsigned, 4>> scanData;
	std::array<unsigned, 4> scanBoundaries = {2000,0,2000,0};
	std::vector<std::string> map;

	parseInput(scanData, scanBoundaries);
	drawMap(scanData, scanBoundaries, map);
	
	resultA = fillWithWater(map);

	resultB = countStillWater(map);

	std::cout << "result A: " << resultA << '\n';
	std::cout << "result B: " << resultB << std::endl;
	return 0;
}

