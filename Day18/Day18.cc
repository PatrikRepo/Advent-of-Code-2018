#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

void parseInput(std::vector<std::string> &map)
{
	std::string line;
	
	std::ifstream input("input.txt");
	if(input.is_open())
	{
		while(getline(input, line))
		{
			map.push_back(line);
		}
	}
	input.close();
}

unsigned countTrees(size_t x, size_t y, const std::vector<std::string> &map)
{
	unsigned result = 0;
	
	if(x > 0 && y > 0 && map[y-1][x-1] == '|')
	{
		result++;
	}	
	if(x > 0 && map[y][x-1] == '|')
	{
		result++;
	}	
	if(x > 0 && y+1 < map.size() && map[y+1][x-1] == '|')
	{
		result++;
	}
	if(y > 0 && map[y-1][x] == '|')
	{
		result++;
	}	
	if(y+1 < map.size() && map[y+1][x] == '|')
	{
		result++;
	}
	if(x+1 < map[0].size() && y > 0 && map[y-1][x+1] == '|')
	{
		result++;
	}	
	if(x+1 < map[0].size() && map[y][x+1] == '|')
	{
		result++;
	}
	if(x+1 < map[0].size() && y+1 < map.size() && map[y+1][x+1] == '|')
	{
		result++;
	}	
	
	return result;
}

unsigned countYards(size_t x, size_t y, const std::vector<std::string> &map)
{
	unsigned result = 0;
	
	if(x > 0 && y > 0 && map[y-1][x-1] == '#')
	{
		result++;
	}	
	if(x > 0 && map[y][x-1] == '#')
	{
		result++;
	}	
	if(x > 0 && y+1 < map.size() && map[y+1][x-1] == '#')
	{
		result++;
	}
	if(y > 0 && map[y-1][x] == '#')
	{
		result++;
	}	
	if(y+1 < map.size() && map[y+1][x] == '#')
	{
		result++;
	}
	if(x+1 < map[0].size() && y > 0 && map[y-1][x+1] == '#')
	{
		result++;
	}	
	if(x+1 < map[0].size() && map[y][x+1] == '#')
	{
		result++;
	}
	if(x+1 < map[0].size() && y+1 < map.size() && map[y+1][x+1] == '#')
	{
		result++;
	}	
	
	return result;
}

bool remainYard(size_t x, size_t y, const std::vector<std::string> &map)
{
	bool result = false;
	unsigned trees = 0;
	unsigned yards = 0;
	
	if(x > 0 && y > 0)
	{
		if(map[y-1][x-1] == '|')
		{
			trees++;
		}
		if(map[y-1][x-1] == '#')
		{
			yards++;
		}
	}	
	if(x > 0)
	{
		if(map[y][x-1] == '|')
		{
			trees++;
		}
		if(map[y][x-1] == '#')
		{
			yards++;
		}
	}	
	if(x > 0 && y+1 < map.size())
	{
		if(map[y+1][x-1] == '|')
		{
			trees++;
		}
		if(map[y+1][x-1] == '#')
		{
			yards++;
		}
	}
	if(y > 0)
	{
		if(map[y-1][x] == '|')
		{
			trees++;
		}
		if(map[y-1][x] == '#')
		{
			yards++;
		}
	}	
	if(y+1 < map.size())
	{
		if(map[y+1][x] == '|')
		{
			trees++;
		}
		if(map[y+1][x] == '#')
		{
			yards++;
		}
	}
	if(x+1 < map[0].size() && y > 0)
	{
		if(map[y-1][x+1] == '|')
		{
			trees++;
		}
		if(map[y-1][x+1] == '#')
		{
			yards++;
		}
	}	
	if(x+1 < map[0].size())
	{
		if(map[y][x+1] == '|')
		{
			trees++;
		}
		if(map[y][x+1] == '#')
		{
			yards++;
		}
	}
	if(x+1 < map[0].size() && y+1 < map.size())
	{
		if(map[y+1][x+1] == '|')
		{
			trees++;
		}
		if(map[y+1][x+1] == '#')
		{
			yards++;
		}
	}	
	
	if(trees >= 1 && yards >= 1)
	{
		result = true;
	}
	return result;
}

void transform(std::vector<std::string> &map)
{
	std::vector<std::string> newMap = map;
	
	for(unsigned i=0; i<newMap.size(); i++)
	{
		for(unsigned j=0; j<newMap[0].size(); j++)
		{
			if(newMap[i][j] == '.')
			{
				if(countTrees(j,i,map) >= 3)
				{
					newMap[i][j] = '|';
				}
			}
			else if(newMap[i][j] == '|')
			{
				if(countYards(j,i,map) >= 3)
				{
					newMap[i][j] = '#';
				}
			}
			else
			{
				if(!remainYard(j,i,map))
				{
					newMap[i][j] = '.';
				}
			}
		}
	}
	map = newMap;
}

uint64_t tenMinutes(std::vector<std::string> map)
{
	uint64_t result = 0;
	
	for(unsigned i=0; i<10; i++)
	{
		transform(map);
	}
	
	unsigned trees = 0;
	unsigned yards = 0;
	
	for(std::string &line : map)
	{
		for(char &element : line)
		{
			if(element == '|')
			{
				trees++;
			}
			else if(element == '#')
			{
				yards++;
			}
		}
	}
	
	result = trees*yards;
	return result;
}

uint64_t countResources(std::vector<std::string> &map, uint32_t iterations)
{
	uint64_t result = 0;
	
	std::unordered_map<std::string,unsigned> cache;
	std::string key;
	unsigned loopLength = 0;
	
	for(unsigned i=0; i<1000; i++)
	{
		transform(map);
	}
	
	cache.clear();
	
	for(unsigned i=0; i<iterations; i++)
	{
		transform(map);
		for(std::string &line : map)
		{
			key += line;
		}
		if(cache.count(key))
		{
			loopLength = i;
			break;
		}
		else
		{
			cache[key] = 1;
			key = "";
		}
	}
	
	iterations = (iterations - 1000 -1)%loopLength;
	
	for(unsigned i=1; i<=iterations; i++)
	{
		transform(map);
	}
	
	unsigned trees = 0;
	unsigned yards = 0;
	
	for(std::string &line : map)
	{
		for(char &element : line)
		{
			if(element == '|')
			{
				trees++;
			}
			else if(element == '#')
			{
				yards++;
			}
		}
	}
	
	result = trees*yards;
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	std::vector<std::string> map;

	parseInput(map);
	
	resultA = tenMinutes(map);

	resultB = countResources(map, 1000000000);

	std::cout << "result A: " << resultA << '\n';
	std::cout << "result B: " << resultB << std::endl;
	return 0;
}

