#include <iostream>
#include <fstream>
#include <vector>

void parseInput(unsigned &numberOfRecipees)
{
	std::string line;
	std::ifstream input("input.txt");
	if(input.is_open())
	{
		getline(input,line);
		numberOfRecipees = std::stoi(line);
	}
	input.close();
}

std::string scoreRecipees(const unsigned numberOfRecipees)
{
	std::string result;
	std::vector<unsigned> recipees;
	recipees.push_back(3);
	recipees.push_back(7);
	size_t index1 = 0;
	size_t index2 = 1;
	unsigned newScore = 0;
	while(recipees.size() < numberOfRecipees+10)
	{
		newScore = recipees[index1] + recipees[index2];
		if(newScore >= 10)
		{
			recipees.push_back(newScore/10);
			recipees.push_back(newScore%10);
		} 
		else
		{
			recipees.push_back(newScore);
		}
		index1 = (index1 + 1 + recipees[index1])%recipees.size();
		index2 = (index2 + 1 + recipees[index2])%recipees.size();
	}
	
	for(unsigned i=0; i<10; i++)
	{
		result += std::to_string(recipees[numberOfRecipees+i]);
	}
	
	return result;
}

uint64_t recipeesScore(const unsigned numberOfRecipees)
{
	uint64_t result = 0;
	unsigned scores1 = 0;
	unsigned scores2 = 0;
	unsigned scoreLength = std::to_string(numberOfRecipees).length();
	std::string finalScore = std::to_string(numberOfRecipees);
	std::vector<unsigned> recipees;
	recipees.push_back(3);
	recipees.push_back(7);
	size_t index1 = 0;
	size_t index2 = 1;
	unsigned newScore = 0;
	while(scores1 != numberOfRecipees && scores2 != numberOfRecipees)
	{
		newScore = recipees[index1] + recipees[index2];
		if(newScore >= 10)
		{
			recipees.push_back(newScore/10);
			recipees.push_back(newScore%10);
		} 
		else
		{
			recipees.push_back(newScore);
		}
		index1 = (index1 + 1 + recipees[index1])%recipees.size();
		index2 = (index2 + 1 + recipees[index2])%recipees.size();
		std::string scoreString;
		for(unsigned i=recipees.size()-scoreLength-1; i<recipees.size(); i++)
		{
			scoreString += recipees[i] + '0';
		}
		if(scoreString != "")
		{
			scores1 = std::stoi(scoreString.substr(0,scoreLength));
			scores2 = std::stoi(scoreString.substr(1,scoreLength));
		}
	}
	if(scores1 == numberOfRecipees)
	{
		result = recipees.size()-scoreLength-1;
	}
	else
	{
		result = recipees.size()-scoreLength;
	}

	return result;
}

int main()
{
	std::string resultA;
	uint64_t resultB = 0;
	unsigned numberOfRecipees = 0;
	
	parseInput(numberOfRecipees);

	resultA = scoreRecipees(numberOfRecipees);
	resultB = recipeesScore(numberOfRecipees);
	
	std::cout << "result A: " << resultA << '\n';
	std::cout << "result B: " << resultB << std::endl;
	return 0;
}
