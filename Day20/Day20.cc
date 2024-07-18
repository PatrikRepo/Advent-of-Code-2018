#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <stack>
#include <utility>
#include <deque>
#include <algorithm>

struct Room
{
	size_t x = 0;
	size_t y = 0;
	unsigned steps = 0;
};

struct orderRooms
{
	bool operator()(const Room room1, const Room room2) const { return room1.steps < room2.steps; }
};

void parseInput(std::string &directions)
{
	std::ifstream input("input.txt");
	if(input.is_open())
	{
		getline(input,directions);
	}
	input.close();
}

void drawMap(const std::string &instructions, std::array<std::string, 201> &map)
{
	std::string mapLine;
	for(unsigned i=0; i<201; i++)
	{
		mapLine += '#';
	}
	for(unsigned i=0; i<map.size(); i++)
	{
		map[i] = mapLine;
	}

	size_t x = 99;
	size_t y = 103;
	
	std::stack<std::pair<unsigned,unsigned>> branchPoints;
	std::pair<unsigned,unsigned> branchPoint;
	
	for(unsigned i=0; i<instructions.size(); i++)
	{
		switch(instructions[i])
		{
			case 'N':
			{
				map[y-1][x] = '_';
				y -= 2;
				map[y][x] = '.';
				break;
			}
			case 'E':
			{
				map[y][x+1] = '|';
				x += 2;
				map[y][x] = '.';
				break;
			}
			case 'S':
			{
				map[y+1][x] = '_';
				y += 2;
				map[y][x] = '.';
				break;
			}
			case 'W':
			{
				map[y][x-1] = '|';
				x -= 2;
				map[y][x] = '.';
				break;
			}
			case '(':
			{
				branchPoint = {x,y};
				branchPoints.push(branchPoint);
				break;
			}
			case '|':
			{
				branchPoint = branchPoints.top();
				x=branchPoint.first;
				y=branchPoint.second;
				break;
			}
			case ')':
			{
				branchPoints.pop();
			}
		}
	}
}

std::pair<uint64_t,uint64_t> findLongestPath(std::array<std::string, 201> &map)
{
	std::pair<uint64_t,uint64_t> result;
	
	std::deque<Room> openList;
	std::vector<Room> closedList;
	
	Room room;
	Room newRoom;
	room.x = 99;
	room.y = 103;
	openList.push_back(room);
	
	while(!openList.empty())
	{
		room = openList.front();
		openList.pop_front();
		closedList.push_back(room);
		
		map[room.y][room.x] = 'X';
		
		newRoom.steps = room.steps+1;
		if(room.steps > result.first)
		{
			result.first = room.steps;
		}
		if(room.steps >= 1000)
		{
			result.second++;
		}
		
		newRoom.x = room.x;
		newRoom.y = room.y-2;
		if(map[room.y-1][room.x] != '#' && 
			std::find_if(openList.begin(), openList.end(), [&newRoom](Room listedRoom) { return (newRoom.x == listedRoom.x) && (newRoom.y == listedRoom.y); }) == std::end(openList) && 
			std::find_if(closedList.begin(), closedList.end(), [&newRoom](Room searchedRoom) { return newRoom.x == searchedRoom.x && (newRoom.y == searchedRoom.y); }) == std::end(closedList))
		{
			openList.push_back(newRoom);
		}
		
		newRoom.x = room.x;
		newRoom.y = room.y+2;
		if(map[room.y+1][room.x] != '#' && 
			std::find_if(openList.begin(), openList.end(), [&newRoom](Room listedRoom) { return (newRoom.x == listedRoom.x) && (newRoom.y == listedRoom.y); }) == std::end(openList) && 
			std::find_if(closedList.begin(), closedList.end(), [&newRoom](Room searchedRoom) { return newRoom.x == searchedRoom.x && (newRoom.y == searchedRoom.y); }) == std::end(closedList))
		{
			openList.push_back(newRoom);
		}

		newRoom.x = room.x-2;
		newRoom.y = room.y;
		if(map[room.y][room.x-1] != '#' && 
			std::find_if(openList.begin(), openList.end(), [&newRoom](Room listedRoom) { return (newRoom.x == listedRoom.x) && (newRoom.y == listedRoom.y); }) == std::end(openList) && 
			std::find_if(closedList.begin(), closedList.end(), [&newRoom](Room searchedRoom) { return newRoom.x == searchedRoom.x && (newRoom.y == searchedRoom.y); }) == std::end(closedList))
		{
			openList.push_back(newRoom);
		}

		newRoom.x = room.x+2;
		newRoom.y = room.y;
		if(map[room.y][room.x+1] != '#' && 
			std::find_if(openList.begin(), openList.end(), [&newRoom](Room listedRoom) { return (newRoom.x == listedRoom.x) && (newRoom.y == listedRoom.y); }) == std::end(openList) && 
			std::find_if(closedList.begin(), closedList.end(), [&newRoom](Room searchedRoom) { return newRoom.x == searchedRoom.x && (newRoom.y == searchedRoom.y); }) == std::end(closedList))
		{
			openList.push_back(newRoom);
		}
		std::sort(openList.begin(), openList.end(), orderRooms());
	}
	return result;
}

int main()
{
	std::pair<uint64_t,uint64_t> result;
	std::string instructions;
	std::array<std::string, 201> map;
	
	parseInput(instructions);
	
	drawMap(instructions, map);

	result = findLongestPath(map);
	
	std::cout << "resultA: " << result.first << '\n';
	std::cout << "resultB: " << result.second << std::endl;
	
	return 0;
}
