#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

void parseInput(std::vector<std::string> &boxes)
{
	std::string line;
	
	std::ifstream input("input.txt");
	if(input.is_open())
	{
		while(getline(input,line))
		{
			boxes.push_back(line);
		}
	}
	input.close();
}

uint64_t calculateChecksum(const std::vector<std::string> &boxes)
{
	uint64_t result = 0;
	uint64_t two = 0;
	uint64_t three = 0;
	
	for(unsigned int i=0; i<boxes.size(); i++)
	{
		std::unordered_map<char,int> letters;
		for(unsigned int j=0; j<boxes[i].length(); j++)
		{
			letters[boxes[i][j]] += 1;
		}
		bool foundTwo = false;
		bool foundThree = false;
		
		for(auto it=letters.begin(); it!= letters.end(); it++)
		{
			if(!foundTwo && it->second == 2)
			{
				foundTwo = true;
				two += 1;
			}
			if(!foundThree && it->second == 3)
			{
				foundThree = true;
				three += 1;
			}
			if(foundTwo && foundThree)
			{
				break;
			}
		}
	}
	
	result = two*three;
	
	return result;
}

std::string findBoxes(const std::vector<std::string> &boxes)
{
	std::string result;
	
	for(unsigned int i=0; i<boxes.size()-1; i++)
	{
		for(unsigned int j=i+1; j<boxes.size(); j++)
		{	
			bool oneError = false;
			unsigned int index = 0;
			for(unsigned int k=0; k<boxes[0].length(); k++)
			{
				if(boxes[i][k] != boxes[j][k])
				{
					if(!oneError)
					{
						oneError = true;
						index = k;
					}
					else
					{
						oneError = false;
						break;
					}
				}
			} 
			if(oneError)
			{
				for(unsigned int k=0; k<boxes[0].length(); k++)
				{
					if(k!=index)
					{
						result += boxes[i][k];
					}
				}
			}
		}
	}
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	std::string resultB;
	std::vector<std::string> boxes;
	
	parseInput(boxes);
	
	resultA = calculateChecksum(boxes);
	
	resultB = findBoxes(boxes);
	
	std::cout << "resultA: " << resultA << '\n';
	std::cout << "resultB: " << resultB << std::endl;
	
	return 0;
}
