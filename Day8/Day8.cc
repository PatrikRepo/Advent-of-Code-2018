#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

void parseInput(std::string &data)
{
	std::ifstream input("input.txt");
	if(input.is_open())
	{
		getline(input,data);
	}
	input.close();
}

uint64_t sumMetadata(const std::string &data, size_t &pos)
{
	uint64_t result = 0;
	size_t endpos = 0;
	unsigned children = 0; 
	unsigned metaData = 0;
	
	endpos = data.find_first_of(" ", pos); 
	children = std::stoi(data.substr(pos, endpos-pos));
	pos = endpos+1;
	endpos = data.find_first_of(" ", pos);
	metaData = std::stoi(data.substr(pos, endpos-pos));
	pos = endpos+1;
	
	for(unsigned i=0; i<children; i++)
	{
		result += sumMetadata(data, pos);
	}
	for(unsigned i=0; i<metaData; i++)
	{
		endpos = data.find_first_of(" ", pos); 
		if(endpos != std::string::npos)
		{
			result += std::stoi(data.substr(pos, endpos-pos));
			pos = endpos+1;
		}
		else
		{
			result += std::stoi(data.substr(pos));
			pos = endpos;
		}
	}
	
	return result;
}

uint64_t sumMetadata(const std::string &data)
{
	uint64_t result = 0;
	size_t pos = 0;
	size_t endpos = 0;
	unsigned children = 0; 
	unsigned metaData = 0;
	
	while(pos < data.length())
	{
		endpos = data.find_first_of(" ", pos); 
		children = std::stoi(data.substr(pos, endpos-pos));
		pos = endpos+1;
		endpos = data.find_first_of(" ", pos);
		metaData = std::stoi(data.substr(pos, endpos-pos));
		pos = endpos+1;
		
		for(unsigned i=0; i<children; i++)
		{
			result += sumMetadata(data, pos);
		}
		for(unsigned i=0; i<metaData; i++)
		{
			endpos = data.find_first_of(" ", pos); 
			if(endpos != std::string::npos)
			{
				result += std::stoi(data.substr(pos, endpos-pos));
				pos = endpos+1;
			}
			else
			{
				result += std::stoi(data.substr(pos));
				pos = endpos;
			}
		}
	}

	return result;
}

uint64_t calculateRootNode(const std::string &data, size_t &pos)
{
	uint64_t result = 0;
	std::vector<uint64_t> childrensResult;
	size_t endpos = 0;
	unsigned children = 0; 
	unsigned metaData = 0;
	
	endpos = data.find_first_of(" ", pos); 
	children = std::stoi(data.substr(pos, endpos-pos));
	pos = endpos+1;
	endpos = data.find_first_of(" ", pos);
	metaData = std::stoi(data.substr(pos, endpos-pos));
	pos = endpos+1;
	
	for(unsigned i=0; i<children; i++)
	{
		childrensResult.push_back(calculateRootNode(data, pos));
	}
	for(unsigned i=0; i<metaData; i++)
	{
		endpos = data.find_first_of(" ", pos); 
		if(children == 0)
		{
			result += std::stoi(data.substr(pos, endpos-pos));
		}
		else
		{
			unsigned mdata = std::stoi(data.substr(pos, endpos-pos));
			if(mdata > 0 && mdata <= children)
			{
				result += childrensResult[mdata-1];
			}
		}
		pos = endpos+1;
	}
	
	return result;
}

uint64_t calculateRootNode(const std::string &data)
{
	size_t pos = 0;
	return calculateRootNode(data, pos);
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	std::string data;
	
	parseInput(data);

	resultA = sumMetadata(data);
	resultB = calculateRootNode(data);
	
	std::cout << "result A: " << resultA << '\n';
	std::cout << "result B: " << resultB << std::endl;
	return 0;
}
