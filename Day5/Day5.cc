#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cctype>
#include <utility>

void parseInput(std::string &polymer)
{
	std::ifstream input("input.txt");
	if(input.is_open())
	{
		getline(input,polymer);
	}
	input.close();
}

std::pair<uint64_t,uint64_t> react(std::string &polymer)
{
	std::pair<uint64_t,uint64_t> result;
	result.second = UINT64_MAX;
	
	for(int i=0; i<(int)polymer.size()-1; i++)
	{
		if(polymer[i] == polymer[i+1]+32 || polymer[i] == polymer[i+1]-32)
		{
			polymer.erase(i,2);
			i-=2;
		} 
	}
	result.first = polymer.length();

	std::string newPolymer;
	for(char c='A'; c<= 'Z'; c++)
	{
		newPolymer = polymer;
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
		if(result.second > newPolymer.length())
		{
			result.second = newPolymer.length();
		}
	}
	return result;
}

int main()
{
	std::pair<uint64_t,uint64_t> result;
	std::string polymer;
	parseInput(polymer);
	
	result = react(polymer);
	
	std::cout << "result A: " << result.first << '\n';
	std::cout << "result B: " << result.second << std::endl;
	return 0;
}
