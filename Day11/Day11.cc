#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>

void parseInput(unsigned &serialNumber)
{
	std::string line;
	std::ifstream input("input.txt");
	if(input.is_open())
	{
		getline(input,line);
		serialNumber = std::stoi(line);
	}
	input.close();
}

std::pair<unsigned,unsigned> getCoordinates(const unsigned serialNumber)
{
	std::pair<unsigned,unsigned> result;
	int map[300][300];
	
	for(unsigned i=0; i<300; i++)
	{
		for(unsigned j=0; j<300; j++)
		{
			unsigned temp = ((j+1+10)*(i+1)+serialNumber)*(j+1+10);
			temp %= 1000;
			temp /= 100;
			map[i][j] = temp-5;
		} 
	}
	
	int mostFuel = 0;
	for(unsigned i=0; i<300-2; i++)
	{
		for(unsigned j=0; j<300-2; j++)
		{
			int newMostFuel = 0;
			newMostFuel += map[i][j];
			newMostFuel += map[i][j+1];
			newMostFuel += map[i][j+2];
			newMostFuel += map[i+1][j];
			newMostFuel += map[i+1][j+1];
			newMostFuel += map[i+1][j+2];
			newMostFuel += map[i+2][j];
			newMostFuel += map[i+2][j+1];
			newMostFuel += map[i+2][j+2];
			if(mostFuel < newMostFuel)
			{
				mostFuel = newMostFuel;
				result.first = j+1;
				result.second = i+1;
			}
		} 
	}
	
	return result;
}

std::tuple<unsigned,unsigned,unsigned> getLargestSquare(const unsigned serialNumber)
{
	std::tuple<unsigned,unsigned,unsigned> result;
	int map[300][300];
	
	for(unsigned i=0; i<300; i++)
	{
		for(unsigned j=0; j<300; j++)
		{
			unsigned temp = ((j+1+10)*(i+1)+serialNumber)*(j+1+10);
			temp %= 1000;
			temp /= 100;
			map[i][j] = temp-5;
		} 
	}
	
	int mostFuel = 0;
	for(unsigned i=0; i<300; i++)
	{
		for(unsigned j=0; j<300; j++)
		{
			unsigned n = i>j ? i:j;
			for(unsigned k=1; k<300-n; k++)
			{
				int newMostFuel = 0;
				for(unsigned y=i; y<i+k; y++)
				{
					for(unsigned x=j; x<j+k; x++)
					{
						newMostFuel += map[y][x];
					}
				}
				if(mostFuel < newMostFuel)
				{
					mostFuel = newMostFuel;
					std::get<0>(result) = j+1;
					std::get<1>(result) = i+1;
					std::get<2>(result) = k;
				}
			}
		} 
	}
	
	return result;
}

int main()
{
	std::pair<unsigned,unsigned> resultA;
	std::tuple<unsigned,unsigned,unsigned> resultB;
	unsigned serialNumber = 0;
	
	parseInput(serialNumber);
	
	resultA = getCoordinates(serialNumber);
	resultB = getLargestSquare(serialNumber);
	
	std::cout << "result A: " << resultA.first << "," << resultA.second << '\n';
	std::cout << "result B: " << std::get<0>(resultB) << "," << std::get<1>(resultB) << "," << std::get<2>(resultB) << std::endl;
	return 0;
}
