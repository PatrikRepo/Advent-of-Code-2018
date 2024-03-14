#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

struct Cut
{
	unsigned int x = 0;
	unsigned int width = 0;
	unsigned int y = 0;
	unsigned int height = 0;
};

void parseInput(std::vector<Cut> &cuts)
{
	std::string line;
	
	std::ifstream input("input.txt");
	if(input.is_open())
	{
		while(getline(input,line))
		{
			size_t pos = line.find_first_of("@");
			pos = line.find_first_of("1234567890", pos);
			size_t endpos = line.find_first_of(",", pos);
			Cut cut;
			
			cut.x = std::stoi(line.substr(pos,endpos-pos));
			
			pos = endpos+1;
			endpos = line.find_first_of(":", pos);
			cut.y = std::stoi(line.substr(pos,endpos-pos));
			
			pos = endpos+2;
			endpos = line.find_first_of("x",pos);
			cut.width = std::stoi(line.substr(pos,endpos-pos));
			
			pos = endpos+1;
			cut.height = std::stoi(line.substr(pos));
			
			cuts.push_back(cut);
		}
	}
	input.close();
}

std::pair<uint64_t,uint64_t> findOverlap(std::vector<Cut> &cuts)
{
	std::pair<uint64_t,uint64_t> result;
	std::vector<std::string> map;
	std::string row;
	for(int i=0; i<1000; i++)
	{
		row += '.';
	}
	for(int i=0; i<1000; i++)
	{
		map.push_back(row);
	}
	
	for(unsigned int i=0; i<cuts.size(); i++)
	{
		for(unsigned int j=cuts[i].y; j<cuts[i].y+cuts[i].height; j++)
		{
			for(unsigned int k=cuts[i].x; k<cuts[i].x+cuts[i].width; k++)
			{
				if(map[j][k] == '.')
				{
					map[j][k] = '#';
				}
				else if(map[j][k] == '#')
				{
					map[j][k] = 'X';
					result.first++;
				}
			}
		}
	}
	
	for(unsigned int i=0; i<cuts.size(); i++)
	{
		bool found = true;
		for(unsigned int j=cuts[i].y; j<cuts[i].y+cuts[i].height; j++)
		{
			for(unsigned int k=cuts[i].x; k<cuts[i].x+cuts[i].width; k++)
			{
				if(map[j][k] == 'X')
				{
					found = false;
					break;
				}
			}
			if(!found)
			{
				break;
			}
		}
		if(found)
		{
			result.second = i+1;
			break;
		}
	}
	
	return result;
}

int main()
{
	std::pair<uint64_t,uint64_t> result;
	std::vector<Cut> cuts;
	
	parseInput(cuts);
	
	result = findOverlap(cuts);
	
	std::cout << "resultA: " << result.first << '\n';
	std::cout << "resultB: " << result.second << std::endl;
	
	return 0;
}
