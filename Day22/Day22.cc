#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <unordered_map>
#include <deque>
#include <algorithm>

enum Tool
{
	NOTHING,
	TORCH,
	GEAR
};

struct Region
{
	size_t x = 0;
	size_t y = 0;
	int tool = 0;
	unsigned cost = 0;
	unsigned parentX = 0;
	unsigned parentY = 0;
};

struct orderRegions
{
	bool operator()(const Region region1, const Region region2) const { return region1.cost < region2.cost; }
};

void parseInput(unsigned &depth, std::pair<unsigned,unsigned> &target)
{
	std::string line;
	size_t pos = 0;
	size_t endpos = 0;
	std::ifstream input("input.txt");
	if(input.is_open())
	{
		getline(input, line);
		pos = line.find_first_of("1234567890");
		depth = std::stoi(line.substr(pos));
		
		getline(input, line);
		pos = line.find_first_of("1234567890");
		endpos = line.find_first_of(",", pos);
		target.first = std::stoi(line.substr(pos, endpos-pos));
		pos = line.find_first_of("1234567890", endpos);
		target.second = std::stoi(line.substr(pos));
	}
	input.close();
}

char determineType(unsigned erosion)
{
	char result = 0;
	
	switch(erosion%3)
	{
		case 0:
		{
			result = '0';
			break;
		}
		case 1:
		{
			result = '1';
			break;
		}
		case 2:
		{
			result = '2';
		}
	}
	
	return result;
}

void drawMap(unsigned depth, const std::pair<unsigned,unsigned> &target, std::vector<std::string> &map)
{
	std::string line;
	std::unordered_map<unsigned,unsigned> erosions;
	unsigned mod = 20183;
	
	for(unsigned i=0; i<=3*target.first; i++)
	{
		line += '#';
	}
	for(unsigned i=0; i<=(target.first*3+target.second); i++)
	{
		map.push_back(line);
	}
	
	erosions[10000*target.first + target.second] = depth%mod;
	map[target.second][target.first] = determineType(erosions[10000*target.first + target.second]);
	for(unsigned i=0; i<=3*target.first; i++)
	{
		erosions[i*10000] = (i*16807 + depth)%mod;
		map[0][i] = determineType(erosions[i*10000]);
	}
	for(unsigned i=1; i<=target.first*3+target.second; i++)
	{
		erosions[i] = (i*48271 + depth)%mod;
		map[i][0] = determineType(erosions[i]);
	}
	
	for(unsigned i=2; i<=(target.first+target.second)*2; i++)
	{
		for(unsigned j=1; j<i; j++)
		{
			if((j <= target.first*3) && (i-j <= target.first*2+target.second) && (j!=target.first || (i-j)!=target.second))
			{
				erosions[j*10000+(i-j)] = ((erosions[(j-1)*10000+(i-j)]*erosions[j*10000+(i-j-1)])+depth)%mod;
				map[i-j][j] = determineType(erosions[j*10000+(i-j)]);
			}
		}
	}
}

uint64_t calculateRisk(const std::vector<std::string> &map, const std::pair<unsigned,unsigned> &target)
{
	uint64_t result = 0;
	
	for(unsigned i=0; i<=target.first; i++)
	{
		for(unsigned j=0; j<=target.second; j++)
		{
			if(map[j][i] == '1')
			{
				result += 1;
			}
			else if(map[j][i] == '2')
			{
				result += 2;
			}
		}
	}
	return result;
}

void addNewRegion(Region newRegion, std::deque<Region> &openList, std::vector<Region> &closedList, const std::pair<unsigned,unsigned> &target, const std::vector<std::string> &map)
{
	if(newRegion.tool == map[newRegion.y][newRegion.x]-'0')
	{
		if(map[newRegion.parentY][newRegion.parentX] - '0' == (newRegion.tool+1)%3)
		{
			newRegion.tool = (newRegion.tool+2)%3;
		}
		else
		{
			newRegion.tool = (newRegion.tool+1)%3;
		}

		newRegion.cost += 8;
	}
	else
	{
		newRegion.cost += 1;
	}
	
	if(newRegion.x == target.first && newRegion.y == target.second && newRegion.tool == Tool::GEAR)
	{
		newRegion.cost += 7;
	}
	
	auto it = std::find_if(openList.begin(), openList.end(), 
			[&newRegion](Region listedRegion) { return newRegion.x == listedRegion.x && newRegion.y == listedRegion.y && newRegion.tool == listedRegion.tool; });
	if(it != std::end(openList))
	{
		if(newRegion.cost < it->cost)
		{
			(*it) = newRegion;
		}
	}
	else if(std::find_if(closedList.begin(), closedList.end(), 
			[&newRegion](Region searchedRegion) { return newRegion.x == searchedRegion.x && newRegion.y == searchedRegion.y && newRegion.tool == searchedRegion.tool; }) == std::end(closedList))
	{
		openList.push_back(newRegion);
	}		
}


uint64_t findPath(const std::vector<std::string> &map, const std::pair<unsigned,unsigned> &target)
{
	uint64_t result = 0;
	
	std::deque<Region> openList;
	std::vector<Region> closedList;
	Region region;
	region.tool = Tool::TORCH;
	openList.push_back(region);
	
	Region newRegion;

	while(!openList.empty())
	{
		region = openList.front();
		openList.pop_front();
		closedList.push_back(region);
		
		if(region.x == target.first && region.y == target.second)
		{
			result = region.cost;			
			break;
		}

		if(region.y > 0)
		{
			newRegion = region;
			newRegion.y -= 1;
			newRegion.parentX = region.x;
			newRegion.parentY = region.y;
			
			addNewRegion(newRegion, openList, closedList, target, map);	
		}
		
		if(region.y+1 < map.size())
		{
			newRegion = region;
			newRegion.y += 1;
			newRegion.parentX = region.x;
			newRegion.parentY = region.y;
			
			addNewRegion(newRegion, openList, closedList, target, map);	
		}
			
		if(region.x > 0)
		{
			newRegion = region;
			newRegion.x -= 1;
			newRegion.parentX = region.x;
			newRegion.parentY = region.y;
				
			addNewRegion(newRegion, openList, closedList, target, map);	
		}
			
		if(region.x+1 < map[0].size())
		{
			newRegion = region;
			newRegion.x += 1;
			newRegion.parentX = region.x;
			newRegion.parentY = region.y;
						
			addNewRegion(newRegion, openList, closedList, target, map);	
		}

		std::sort(openList.begin(), openList.end(), orderRegions());
	}
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;

	unsigned depth = 0;
	std::pair<unsigned,unsigned> target;
	std::vector<std::string> map;
	
	parseInput(depth, target);

	drawMap(depth, target, map);

	resultA = calculateRisk(map, target);

	resultB = findPath(map, target);

	std::cout << "result A: " << resultA << '\n';
	std::cout << "result B: " << resultB << std::endl;
	return 0;
}

