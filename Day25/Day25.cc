#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <array>

struct Point
{
	std::array<int,4> coordinates{0,0,0,0};
	unsigned root = 0;
};

void parseInput(std::vector<Point> &points)
{
  std::string line;
	unsigned root = 0;
	size_t pos = 0;
	size_t endpos = 0;
	
  std::ifstream input("input.txt");

  if(input.is_open())
  {
    while(getline(input,line))
    {
			Point point;
			point.root = root;
			root++;
			
			endpos = line.find_first_of(",");
			point.coordinates[0] = std::stoi(line.substr(0,endpos));
			
			pos = line.find_first_of("-1234567890", endpos);
			endpos = line.find_first_of(",", pos);
			point.coordinates[1] = std::stoi(line.substr(pos,endpos-pos));
			
			pos = line.find_first_of("-1234567890", endpos);
			endpos = line.find_first_of(",", pos);
			point.coordinates[2] = std::stoi(line.substr(pos,endpos-pos));
			
			pos = line.find_first_of("-1234567890", endpos);
			point.coordinates[3] = std::stoi(line.substr(pos));
			
			points.push_back(point);
    }
  }
  input.close();
}

bool sameConstellation(const Point &point1, const Point &point2)
{
	return (abs(point1.coordinates[0] - point2.coordinates[0]) + 
			abs(point1.coordinates[1] - point2.coordinates[1]) + 
			abs(point1.coordinates[2] - point2.coordinates[2]) + 
			abs(point1.coordinates[3] - point2.coordinates[3])) 
			<= 3 ? true : false;
}

uint64_t countConstellations(std::vector<Point> &points)
{
	uint64_t result = 0;
	
	std::unordered_map<unsigned,unsigned> constellations;
	bool stable = false;
	
	while(!stable)
	{
		stable = true;
		for(unsigned i=0; i<points.size()-1; i++)
		{
			for(unsigned j=i+1; j<points.size(); j++)
			{
				if(points[i].root != points[j].root && sameConstellation(points[i],points[j]))
				{
					stable = false;
					if(points[i].root < points[j].root)
					{
						points[j].root = points[i].root;
					}
					else
					{
						points[i].root = points[j].root;
					}
				}
			}
		}
	}

	
	for(Point &point : points)
	{
		constellations[point.root] = 1;
	}
	
	result = constellations.size();
	return result;
}

int main()
{
  uint64_t result = 0;
	std::vector<Point> points;
   
  parseInput(points);

 	result = countConstellations(points);
 
  std::cout << "result: " << result << std::endl;
  return 0;
}
