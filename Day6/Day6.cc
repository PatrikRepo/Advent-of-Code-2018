#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

struct Point
{
	unsigned x = 0;
	unsigned y = 0;
	unsigned area = 0;
	bool outer = false;
	Point(unsigned xx, unsigned yy) : x(xx), y(yy) {}
};

void parseInput(std::vector<Point> &points)
{
	std::string line;
	std::ifstream input("input.txt");
	if(input.is_open())
	{
		while(getline(input,line))
		{
			size_t pos = line.find_first_of(",");
			unsigned x = std::stoi(line.substr(0,pos));
			unsigned y = std::stoi(line.substr(pos+2));
			points.push_back(Point(x,y));
		}
	}
	input.close();
}

std::pair<uint64_t,uint64_t> findArea(std::vector<Point> &points)
{
	std::pair<uint64_t,uint64_t> result = {0,0};
	unsigned maxX = 0;
	unsigned maxY = 0;
	for(unsigned i=0; i<points.size(); i++)
	{
		if(points[i].x > maxX)
		{
			maxX = points[i].x;
		}
		if(points[i].y > maxY)
		{
			maxY = points[i].y;
		}
	}
	
	for(unsigned i=0; i<=maxY; i++)
	{
		for(unsigned j=0; j<=maxX; j++)
		{
			uint64_t minDist = UINT64_MAX;
			unsigned index = 0;
			bool conflict = false;
			unsigned totalDist = 0;
			
			for(unsigned k=0; k<points.size(); k++)
			{
				unsigned newDist = abs(points[k].x - j) + abs(points[k].y - i);
				totalDist += newDist;
				if(minDist > newDist)
				{
					conflict = false;
					minDist = newDist;
					index = k;
				}
				else if(minDist == newDist)
				{
					conflict = true;
				}
			}
			if(!conflict && !points[index].outer)
			{
				if(i==0 || i==maxY || j==0 || j==maxX)
				{
					points[index].outer = true;
				}
				else
				{
					points[index].area += 1;
				}
			}
			if(totalDist < 10000)
			{
				result.second++;
			}
		}
	}
	
	for(auto it=points.begin(); it!=points.end(); it++)
	{
		if(!it->outer && result.first < it->area)
		{
			result.first = it->area;
		}
	}
	
	return result;
}

int main()
{
	std::pair<uint64_t,uint64_t> result;
	std::vector<Point> points;
	
	parseInput(points);

	result = findArea(points);
	
	std::cout << "result A: " << result.first << '\n';
	std::cout << "result B: " << result.second << std::endl;
	return 0;
}
