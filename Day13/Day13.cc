#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

enum Direction
{
	UP,
	RIGHT,
	DOWN,
	LEFT
};

struct Cart
{
	char id = 0;
	size_t x = 0;
	size_t y = 0;
	unsigned direction = 0;
	unsigned turn = Direction::LEFT;
	void updateTurn()
	{
		if(turn == Direction::LEFT)
		{
			turn = Direction::UP;
		}
		else if(turn == Direction::UP)
		{
			turn = Direction::RIGHT;
		}
		else if(turn == Direction::RIGHT)
		{
			turn = Direction::LEFT;
		}
	}
};

void parseInput(std::vector<std::string> &map, std::vector<Cart> &carts)
{
	std::string line;
	std::ifstream input("input.txt");
	if(input.is_open())
	{
		size_t pos = 0;
		unsigned y = 0;
		char id = 'A';
		while(getline(input,line))
		{
			pos = line.find_first_of("><v^");
			while(pos != std::string::npos)
			{
				Cart cart;
				cart.id = id;
				id++;
				cart.x = pos;
				cart.y = y;
				switch(line[pos])
				{
					case '^':
					{	
						cart.direction = Direction::UP;
						line[pos] = '|';
						break;
					}
					case '>':
					{	
						cart.direction = Direction::RIGHT;
						line[pos] = '-';
						break;
					}
					case 'v':
					{	
						cart.direction = Direction::DOWN;
						line[pos] = '|';
						break;
					}
					case '<':
					{	
						cart.direction = Direction::LEFT;
						line[pos] = '-';
						break;
					}
				}
				carts.push_back(cart);
				pos = line.find_first_of("><v^",pos+1);
			}
			map.push_back(line);
			y++;
		}
	}
	input.close();
}

std::pair<uint64_t,uint64_t> findCrash(const std::vector<std::string> map, std::vector<Cart> carts)
{
	std::pair<uint64_t,uint64_t> result;
	bool done = false;
	std::vector<std::string> cartMap;
	std::string cartRow (map[0].length(), ' ');
	for(unsigned i=0; i<map.size(); i++)
	{
		cartMap.push_back(cartRow);
	}
	
	for(unsigned i=0; i<carts.size(); i++)
	{
		cartMap[carts[i].y][carts[i].x] = carts[i].id;
	}
	
	std::vector<std::string> nextTick;
	while(!done)
	{
		nextTick = cartMap;
		for(unsigned i=0; i<cartMap.size(); i++)
		{
			for(unsigned j=0; j<cartMap[0].length(); j++)
			{
				if(cartMap[i][j] != ' ')
				{
					Cart* cart = &carts[cartMap[i][j] - 'A'];
					switch(cart->direction)
					{
						case Direction::UP:
						{
							cart->y--;
							if(map[cart->y][cart->x] == '/')
							{
								cart->direction = Direction::RIGHT;
							}
							else if(map[cart->y][cart->x] == '\\')
							{
								cart->direction = Direction::LEFT;
							}
							else if(map[cart->y][cart->x] == '+')
							{
								cart->direction = (cart->direction + cart->turn)%4;
								cart->updateTurn();
							}
							break;
						}
						case Direction::RIGHT:
						{
							cart->x++;
							if(map[cart->y][cart->x] == '/')
							{
								cart->direction = Direction::UP;
							}
							else if(map[cart->y][cart->x] == '\\')
							{
								cart->direction = Direction::DOWN;
							}
							else if(map[cart->y][cart->x] == '+')
							{
								cart->direction = (cart->direction + cart->turn)%4;
								cart->updateTurn();
							}
							break;
						}
						case Direction::DOWN:
						{
							cart->y++;
							if(map[cart->y][cart->x] == '/')
							{
								cart->direction = Direction::LEFT;
							}
							else if(map[cart->y][cart->x] == '\\')
							{
								cart->direction = Direction::RIGHT;
							}
							else if(map[cart->y][cart->x] == '+')
							{
								cart->direction = (cart->direction + cart->turn)%4;
								cart->updateTurn();
							}
							break;
						}
						case Direction::LEFT:
						{
							cart->x--;
							if(map[cart->y][cart->x] == '/')
							{
								cart->direction = Direction::DOWN;
							}
							else if(map[cart->y][cart->x] == '\\')
							{
								cart->direction = Direction::UP;
							}
							else if(map[cart->y][cart->x] == '+')
							{
								cart->direction = (cart->direction + cart->turn)%4;
								cart->updateTurn();
							}
							break;
						}
					}
					
					if(nextTick[cart->y][cart->x] == ' ' && cartMap[cart->y][cart->x] == ' ')
					{
						cartMap[i][j] = ' ';
						nextTick[i][j] = ' ';
						nextTick[cart->y][cart->x] = cart->id;
					}
					else
					{
						result.first = cart->x;
						result.second = cart->y;
						done = true;
						break;
					}
				}
			}
			if(done)
			{
				break;
			}
		}
		cartMap = nextTick;
	}
	
	return result;
}

std::pair<uint64_t,uint64_t> findLastSurvivor(const std::vector<std::string> map, std::vector<Cart> &carts)
{
	std::pair<uint64_t,uint64_t> result;
	bool done = false;
	unsigned cartsLeft = carts.size();
	std::vector<std::string> cartMap;
	std::string cartRow (map[0].length(), ' ');
	for(unsigned i=0; i<map.size(); i++)
	{
		cartMap.push_back(cartRow);
	}
	
	for(unsigned i=0; i<carts.size(); i++)
	{
		cartMap[carts[i].y][carts[i].x] = carts[i].id;
	}
	
	std::vector<std::string> nextTick;
	while(!done)
	{
		nextTick = cartMap;
		for(unsigned i=0; i<cartMap.size(); i++)
		{
			for(unsigned j=0; j<cartMap[0].length(); j++)
			{
				if(cartMap[i][j] != ' ')
				{
					Cart* cart = &carts[cartMap[i][j] - 'A'];
					switch(cart->direction)
					{
						case Direction::UP:
						{
							cart->y--;
							if(map[cart->y][cart->x] == '/')
							{
								cart->direction = Direction::RIGHT;
							}
							else if(map[cart->y][cart->x] == '\\')
							{
								cart->direction = Direction::LEFT;
							}
							else if(map[cart->y][cart->x] == '+')
							{
								cart->direction = (cart->direction + cart->turn)%4;
								cart->updateTurn();
							}
							break;
						}
						case Direction::RIGHT:
						{
							cart->x++;
							if(map[cart->y][cart->x] == '/')
							{
								cart->direction = Direction::UP;
							}
							else if(map[cart->y][cart->x] == '\\')
							{
								cart->direction = Direction::DOWN;
							}
							else if(map[cart->y][cart->x] == '+')
							{
								cart->direction = (cart->direction + cart->turn)%4;
								cart->updateTurn();
							}
							break;
						}
						case Direction::DOWN:
						{
							cart->y++;
							if(map[cart->y][cart->x] == '/')
							{
								cart->direction = Direction::LEFT;
							}
							else if(map[cart->y][cart->x] == '\\')
							{
								cart->direction = Direction::RIGHT;
							}
							else if(map[cart->y][cart->x] == '+')
							{
								cart->direction = (cart->direction + cart->turn)%4;
								cart->updateTurn();
							}
							break;
						}
						case Direction::LEFT:
						{
							cart->x--;
							if(map[cart->y][cart->x] == '/')
							{
								cart->direction = Direction::DOWN;
							}
							else if(map[cart->y][cart->x] == '\\')
							{
								cart->direction = Direction::UP;
							}
							else if(map[cart->y][cart->x] == '+')
							{
								cart->direction = (cart->direction + cart->turn)%4;
								cart->updateTurn();
							}
							break;
						}
					}
					
					cartMap[i][j] = ' ';
					nextTick[i][j] = ' ';
					if(nextTick[cart->y][cart->x] == ' ' && cartMap[cart->y][cart->x] == ' ')
					{

						nextTick[cart->y][cart->x] = cart->id;
					}
					else
					{
						nextTick[cart->y][cart->x] = ' ';
						cartMap[cart->y][cart->x] = ' ';
						cartsLeft -= 2;
					}
				}
			}
		}
		if(cartsLeft == 1)
		{
			done = true;
		}
		cartMap = nextTick;
	}
	
	bool found = false;
	
	for(unsigned i=0; i<nextTick.size(); i++)
	{	
		for(unsigned j=0; j<nextTick[0].length(); j++)
		{
			if(nextTick[i][j] != ' ')
			{
				result.first = carts[nextTick[i][j] - 'A'].x;
				result.second = carts[nextTick[i][j] - 'A'].y;
				break;
			}
		}
		if(found)
		{
			break;
		}
	}
	
	return result;
}


int main()
{
	std::pair<uint64_t,uint64_t> resultA;
	std::pair<uint64_t,uint64_t> resultB;
	std::vector<std::string> map;
	std::vector<Cart> carts;
	
	parseInput(map, carts);
	
	resultA = findCrash(map, carts);
	resultB = findLastSurvivor(map, carts);
	
	std::cout << "result A: " << resultA.first << "," << resultA.second << '\n';
	std::cout << "result B: " << resultB.first << "," << resultB.second << std::endl;
	return 0;
}
