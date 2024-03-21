#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cctype>

void parseInput(std::string &polymer)
{
	std::ifstream input("input.txt");
	if(input.is_open())
	{
		getline(input,polymer);
	}
	input.close();
}

uint64_t react(std::string &polymer)
{
	uint64_t result = 0;
	
	for(int i=0; i<(int)polymer.size()-1; i++)
	{
		if(polymer[i] == polymer[i+1]+32 || polymer[i] == polymer[i+1]-32)
		{
			polymer.erase(i,2);
			i-=2;
		} 
	}
	result = polymer.length();

	return result;
}

uint64_t removeAndReact(std::string &polymerB)
{
	uint64_t result = UINT64_MAX;
	
	react(polymerB);
	
	std::string newPolymer;
	
	for(char c='A'; c<= 'Z'; c++)
	{
		newPolymer = polymerB;
		for(int i=0; i<(int)newPolymer.size()-1; i++)
		{
			if(newPolymer[i] == c || newPolymer[i] == c+32)
			{
				newPolymer.erase(i,1);
				i-=2;
			}
			else if(newPolymer[i] == newPolymer[i+1]+32 || newPolymer[i] == newPolymer[i+1]-32)
			{
				newPolymer.erase(i,2);
				i-=2;
			} 
		}
		if(result > newPolymer.length())
		{
			result = newPolymer.length();
		}
	}
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	std::string polymer;
	std::string polymerB;
	parseInput(polymer);
	polymerB = polymer;
	
	resultA = react(polymer);
	resultB = removeAndReact(polymerB);
	
	std::cout << "result A: " << resultA << '\n';
	std::cout << "result B: " << resultB << std::endl;
	return 0;
}
