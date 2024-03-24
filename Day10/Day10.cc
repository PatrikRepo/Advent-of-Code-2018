#include <iostream>
#include <fstream>
#include <vector>

struct Light
{
	int x = 0;
	int y = 0;
	int vX = 0;
	int vY = 0;
	Light(int xx, int yy, int vvX, int vvY) : x(xx), y(yy), vX(vvX), vY(vvY) {}
};

//Actually logic is in parseinput. Didn't really intend for this, but I was transforming data here anyway so why not just all the way?
void parseInput(std::vector<Light> &lights, uint64_t &time)
{
	std::string line;
	std::ifstream input("input.txt");
	if(input.is_open())
	{
		getline(input,line);
		
		size_t pos = line.find_first_of("-1234567890");
		size_t endpos = line.find_first_of(",", pos);
		int x = std::stoi(line.substr(pos,endpos-pos));
		
		pos = line.find_first_of("-1234567890",endpos);
		endpos = line.find_first_of(">", pos);
		int y = std::stoi(line.substr(pos,endpos-pos));
		
		pos = line.find_first_of("-1234567890",endpos);
		endpos = line.find_first_of(",", pos);
		int vX = std::stoi(line.substr(pos,endpos-pos));
		
		pos = line.find_first_of("-1234567890",endpos);
		endpos = line.find_first_of(">", pos);
		int vY = std::stoi(line.substr(pos,endpos-pos));
		
		while(abs(y) > 194)
		{
			y += vY;
			time++;
		}
		x += vX*time;
		
		lights.emplace_back(x,y,vX,vY);
		
		while(getline(input,line))
		{
			pos = line.find_first_of("-1234567890");
			endpos = line.find_first_of(",", pos);
			int x = std::stoi(line.substr(pos,endpos-pos));
			
			pos = line.find_first_of("-1234567890",endpos);
			endpos = line.find_first_of(">", pos);
			int y = std::stoi(line.substr(pos,endpos-pos));
			
			pos = line.find_first_of("-1234567890",endpos);
			endpos = line.find_first_of(",", pos);
			int vX = std::stoi(line.substr(pos,endpos-pos));
			
			pos = line.find_first_of("-1234567890",endpos);
			endpos = line.find_first_of(">", pos);
			int vY = std::stoi(line.substr(pos,endpos-pos));
			
			x += vX*time;
			y += vY*time;
			
			lights.emplace_back(x,y,vX,vY);
		}
	}
	input.close();
}

void printMessage(std::vector<Light> &lights)
{
	std::string line(62, ' ');
	std::vector<std::string> map;
	for(int i=0; i<10; i++)
	{
		map.push_back(line);
	}

	std::vector<std::string> newMap = map;
	for(unsigned i=0; i<lights.size(); i++)
	{
		newMap[lights[i].y-193][lights[i].x-126] = '#';
		lights[i].y += lights[i].vY;
		lights[i].x += lights[i].vX;
	}
	
	for(unsigned i=0; i<newMap.size(); i++)
	{
		std::cout << newMap[i] << '\n';
	}

}

int main()
{
	uint64_t time = 0;
	std::vector<Light> lights;
	
	parseInput(lights, time);
	
	std::cout << "result A: " << '\n';
	printMessage(lights);
	
	std::cout << "result B: " << time << std::endl;
	return 0;
}
