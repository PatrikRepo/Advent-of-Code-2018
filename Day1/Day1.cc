#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

void parseInput(std::vector<int> &frequencies)
{
	std::string line;
	
	std::ifstream input("input.txt");
	if(input.is_open())
	{
		while(getline(input,line))
		{
			frequencies.push_back(std::stoi(line));
		}
	}
	input.close();
}

uint64_t calculateFrequency(const std::vector<int> &frequencies)
{
	uint64_t result = 0;
	
	for(unsigned int i=0; i<frequencies.size(); i++)
	{
		result += frequencies[i];
	}
	
	return result;
}

uint64_t calculateSecondFrequency(const std::vector<int> &frequencies)
{
	uint64_t result = 0;
	uint64_t totalFrequency = 0;
	unsigned int i = 0;
	std::unordered_map<int,int> foundFrequencies;
	bool found = false;
	
	while(!found)
	{
		totalFrequency += frequencies[i];
		if(foundFrequencies.count(totalFrequency))
		{
			result = totalFrequency;
			found = true;
			break;
		}
		else
		{
			foundFrequencies[totalFrequency] = 1;
		}
		i = (i+1)%frequencies.size();
	}
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	std::vector<int> frequencies;
	
	parseInput(frequencies);
	
	resultA = calculateFrequency(frequencies);
	
	resultB = calculateSecondFrequency(frequencies);
	
	std::cout << "resultA: " << resultA << '\n';
	std::cout << "resultB: " << resultB << std::endl;
	
	return 0;
}
