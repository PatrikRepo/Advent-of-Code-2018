#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <utility>

void parseInput(std::string &plants, std::unordered_map<std::string,char> &conversions)
{
	std::string line;
	std::ifstream input("input.txt");
	if(input.is_open())
	{
		getline(input,line);
		size_t pos = line.find_first_of("#.");
		plants = line.substr(pos);
		getline(input,line);
		
		while(getline(input,line))
		{
			conversions[line.substr(0,5)] = line[9];
		}
	}
	input.close();
}

void addSoil(std::string &plants, int &offset)
{
	if(plants[0] == '#')
	{
		plants = '.' + plants;
		offset++;
	}	
	if(plants[1] == '#')
	{
		plants = '.' + plants;
		offset++;
	}
	if(plants[2] == '#')
	{
		plants = '.' + plants;
		offset++;
	}
	if(plants[plants.size()-1] == '#')
	{
		plants += '.';
	}	
	if(plants[plants.size()-2] == '#')
	{
		plants += '.';
	}
	if(plants[plants.size()-3] == '#')
	{
		plants += '.';
	}
}

std::pair<uint64_t,uint64_t> growPlants(const std::string &plants, std::unordered_map<std::string,char> &conversions)
{
	std::pair<uint64_t,uint64_t> result;
	int offset = 3;
	std::string plantsNow = "..." + plants + "...";
	std::string plantsFuture;
	
	for(unsigned i=0; i<20; i++)
	{
		addSoil(plantsNow, offset);
		plantsFuture = plantsNow;
		
		for(unsigned j=2; j<plantsFuture.length()-2; j++)
		{
			plantsFuture[j] = conversions[plantsNow.substr(j-2,5)];
		}

		plantsNow = plantsFuture;
	}
	
	for(int i=0; i<(int)plantsNow.length(); i++)
	{
		if(plantsNow[i] == '#')
		{
			result.first += i-offset;
		}
	}
	
	for(unsigned i=0; i<130; i++)
	{
		addSoil(plantsNow, offset);
		plantsFuture = plantsNow;
		
		for(unsigned j=2; j<plantsFuture.length()-2; j++)
		{
			plantsFuture[j] = conversions[plantsNow.substr(j-2,5)];
		}

		plantsNow = plantsFuture;
	}
	
	uint64_t plantOffset = 50000000000 - 150;
	
	for(int i=0; i<(int)plantsNow.length(); i++)
	{
		if(plantsNow[i] == '#')
		{
			result.second += i + plantOffset - offset;
		}
	}
	
	return result;
}

int main()
{
	std::pair<uint64_t,uint64_t> result;
	std::string plants;
	std::unordered_map<std::string,char> conversions;
	
	parseInput(plants, conversions);
	
	result = growPlants(plants, conversions);
	
	std::cout << "result A: " << result.first << '\n';
	std::cout << "result B: " << result.second << std::endl;
	return 0;
}
