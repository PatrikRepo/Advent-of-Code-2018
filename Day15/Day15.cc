#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <queue>
#include <algorithm>

struct NPC
{
	char id = 0;
	unsigned x = 0;
	unsigned y = 0;
	char team = 0;
	int health = 200;
	unsigned power = 3;
};

struct orderNPCs
{
	bool operator()(const NPC* npc1, const NPC* npc2) const { if(npc1->y != npc2->y) return npc1->y > npc2->y; else return npc1->x > npc2->x; }
};

struct Node
{
	unsigned x = 0;
	unsigned y = 0;
	unsigned cost = 0;
	unsigned id = 0;
	unsigned parent = 0;
};

struct orderNodes
{
	bool operator()(const Node node1, const Node node2) const { if(node1.cost != node2.cost) return node1.cost < node2.cost; else if(node1.y != node2.y) return node1.y < node2.y; else return node1.x < node2.x; }
};

void parseInput(std::vector<std::string> &map, std::vector<NPC> &npcs)
{
	std::string line;
	std::ifstream input("input.txt");
	if(input.is_open())
	{
		unsigned y = 0;
		char id = 'A';
		while(getline(input,line))
		{
			size_t pos = line.find_first_of("EG");
			while(pos != std::string::npos)
			{
				NPC npc;
				npc.x = pos;
				npc.y = y;
				npc.id = id;
				id++;
				npc.team = line[pos];
				line[pos] = npc.id;
				npcs.push_back(npc);
				pos = line.find_first_of("EG",pos+1);
			}
			map.push_back(line);
			y++;
		}
	}
	input.close();
}

bool nextToEnemy(const std::vector<std::string> &map, const std::vector<NPC> &npcs, const size_t x, const size_t y, const char team)
{
	bool result = false;
	
	if(map[y-1][x] != '.' && map[y-1][x] != '#' && npcs[map[y-1][x]-'A'].team != team && npcs[map[y-1][x]-'A'].health > 0)
	{
		result = true;
	}
	else if(map[y+1][x] != '.' && map[y+1][x] != '#' && npcs[map[y+1][x]-'A'].team != team && npcs[map[y+1][x]-'A'].health > 0)
	{
		result = true;
	}
	else if(map[y][x-1] != '.' && map[y][x-1] != '#' && npcs[map[y][x-1]-'A'].team != team && npcs[map[y][x-1]-'A'].health > 0)
	{
		result = true;
	}
	else if(map[y][x+1] != '.' && map[y][x+1] != '#' && npcs[map[y][x+1]-'A'].team != team && npcs[map[y][x+1]-'A'].health > 0)
	{
		result = true;
	}
	
	return result;
}

void move(std::vector<std::string> &map, const std::vector<NPC> &npcs, NPC &npc)
{
	Node node;
	node.y = npc.y;
	node.x = npc.x;
	node.id = 32*node.y + node.x;
	unsigned rootId = node.id;
	
	std::deque<Node> openList;
	std::vector<Node> closedList;
	openList.push_back(node);
	
	while(!openList.empty())
	{
		node = openList.front();
		openList.pop_front();
		closedList.push_back(node);
		
		if(nextToEnemy(map, npcs, node.x, node.y, npc.team))
		{
			while(node.parent != rootId)
			{
				node = *(std::find_if(closedList.begin(), closedList.end(), [&node](Node parent) { return node.parent == parent.id; }));
			}
			
			map[npc.y][npc.x] = '.';
			npc.y = node.y;
			npc.x = node.x;
			map[npc.y][npc.x] = npc.id;
			
			break;
		}
		
		Node newNode;
		newNode.id = 32*(node.y-1) + node.x;
		
		if(map[node.y-1][node.x] == '.' && 
			std::find_if(openList.begin(), openList.end(), [&newNode](Node oldNode) { return newNode.id == oldNode.id; }) == openList.end() && 
			std::find_if(closedList.begin(), closedList.end(), [&newNode](Node oldNode) { return newNode.id == oldNode.id; }) == closedList.end())
		{
			newNode.y = node.y-1;
			newNode.x = node.x;
			newNode.cost = node.cost+1;
			newNode.parent = node.id;
			openList.push_back(newNode);
		}
		
		newNode.id = 32*(node.y+1) + node.x;
		if(map[node.y+1][node.x] == '.' && 
			std::find_if(openList.begin(), openList.end(), [&newNode](Node oldNode) { return newNode.id == oldNode.id; }) == std::end(openList) && 
			std::find_if(closedList.begin(), closedList.end(), [&newNode](Node oldNode) { return newNode.id == oldNode.id; }) == std::end(closedList))
		{
			newNode.y = node.y+1;
			newNode.x = node.x;
			newNode.cost = node.cost+1;
			newNode.parent = node.id;
			openList.push_back(newNode);
		}
		
		newNode.id = 32*node.y + node.x-1;
		if(map[node.y][node.x-1] == '.' && 
			std::find_if(openList.begin(), openList.end(), [&newNode](Node oldNode) { return newNode.id == oldNode.id; }) == std::end(openList) && 
			std::find_if(closedList.begin(), closedList.end(), [&newNode](Node oldNode) { return newNode.id == oldNode.id; }) == std::end(closedList))
		{
			newNode.y = node.y;
			newNode.x = node.x-1;
			newNode.cost = node.cost+1;
			newNode.parent = node.id;
			openList.push_back(newNode);
		}
		
		newNode.id = 32*node.y + node.x+1;
		if(map[node.y][node.x+1] == '.' && 
			std::find_if(openList.begin(), openList.end(), [&newNode](Node oldNode) { return newNode.id == oldNode.id; }) == std::end(openList) && 
			std::find_if(closedList.begin(), closedList.end(), [&newNode](Node oldNode) { return newNode.id == oldNode.id; }) == std::end(closedList))
		{
			newNode.y = node.y;
			newNode.x = node.x+1;
			newNode.cost = node.cost+1;
			newNode.parent = node.id;
			openList.push_back(newNode);
		}
		std::sort(openList.begin(), openList.end(), orderNodes());
	}
}

bool attack(std::vector<std::string> &map, std::vector<NPC> &npcs, NPC &npc)
{
	bool kill = false;
	int health = 201;
	NPC* enemy = nullptr;
	
	if(map[npc.y-1][npc.x] >= 'A' && npcs[map[npc.y-1][npc.x] - 'A'].team != npc.team && npcs[map[npc.y-1][npc.x] - 'A'].health < health)
	{
		enemy = &npcs[map[npc.y-1][npc.x] - 'A'];
		health = enemy->health;
	}
	if(map[npc.y][npc.x-1] >= 'A' && npcs[map[npc.y][npc.x-1] - 'A'].team != npc.team && npcs[map[npc.y][npc.x-1] - 'A'].health < health)
	{
		enemy = &npcs[map[npc.y][npc.x-1] - 'A'];
		health = enemy->health;
	}
	if(map[npc.y][npc.x+1] >= 'A' && npcs[map[npc.y][npc.x+1] - 'A'].team != npc.team && npcs[map[npc.y][npc.x+1] - 'A'].health < health)
	{
		enemy = &npcs[map[npc.y][npc.x+1] - 'A'];
		health = enemy->health;
	}
	if(map[npc.y+1][npc.x] >= 'A' && npcs[map[npc.y+1][npc.x] - 'A'].team != npc.team && npcs[map[npc.y+1][npc.x] - 'A'].health < health)
	{
		enemy = &npcs[map[npc.y+1][npc.x] - 'A'];
	}
	
	if(enemy)
	{
		enemy->health -= npc.power;
		if(enemy->health <= 0)
		{
			enemy->health = 0;
			map[enemy->y][enemy->x] = '.';
			kill = true;
		}
	}
	
	return kill;
}

uint64_t fight(std::vector<std::string> map, std::vector<NPC> npcs)
{
	uint64_t result = 0;
	unsigned elves = 0;
	unsigned goblins = 0;
	bool completeRound = false;
	
	std::priority_queue<NPC*, std::vector<NPC*>, orderNPCs> queue;
	
	for(unsigned i=0; i<npcs.size(); i++)
	{
		if(npcs[i].team == 'G')
		{
			goblins++;
		}
		else
		{
			elves++;
		}
	}
	
	while(goblins > 0 && elves > 0)
	{
		for(NPC& npc : npcs)
		{
			queue.push(&npc);
		}
		
		while(!queue.empty())
		{
			NPC* npc = queue.top();
			queue.pop();
			
			if(npcs[npc->id - 'A'].health > 0)
			{
				if(!nextToEnemy(map, npcs, npc->x, npc->y, npc->team))
				{
					move(map, npcs, *npc);
				}
				if(attack(map, npcs, *npc))
				{
					if(npc->team == 'E')
					{
						goblins--;
					}
					else
					{
						elves--;
					}
					if(goblins == 0 || elves == 0)
					{
						if(queue.empty())
						{
							completeRound = true;
						}
						break;
					}
				}
			}
		}
		result++;
	}
	
	if(!completeRound)
	{
		result--;
	}
	
	unsigned healthRemaining = 0;
	for(unsigned i=0; i<npcs.size(); i++)
	{
		healthRemaining += npcs[i].health;
	}
	
	result *= healthRemaining;
	
	return result;
}

void boostElves(std::vector<NPC> &npcs)
{
	for(NPC &npc : npcs)
	{
		if(npc.team == 'E')
		{
			npc.power++;
		}
	}
}

uint64_t strongerElves(std::vector<std::string> &baseMap, std::vector<NPC> &baseNpcs)
{
	uint64_t result = 0;
	unsigned elves = 0;
	unsigned goblins = 0;
	bool completeRound = false;
	bool done = false;
	bool failed = false;
	std::vector<std::string> map;
	std::vector<NPC> npcs;
	
	while(!done)
	{
		failed = false;
		result = 0;
		boostElves(baseNpcs);
		
		std::priority_queue<NPC*, std::vector<NPC*>, orderNPCs> queue;
		npcs = baseNpcs;
		map = baseMap;
		goblins = 0;
		elves = 0;
		
		for(unsigned i=0; i<npcs.size(); i++)
		{
			if(npcs[i].team == 'G')
			{
				goblins++;
			}
			else
			{
				elves++;
			}
		}
		
		while(!failed && goblins > 0 && elves > 0)
		{
			for(NPC& npc : npcs)
			{
				queue.push(&npc);
			}
			
			while(!queue.empty())
			{
				NPC* npc = queue.top();
				queue.pop();
				
				if(npcs[npc->id - 'A'].health > 0)
				{
					if(!nextToEnemy(map, npcs, npc->x, npc->y, npc->team))
					{
						move(map, npcs, *npc);
					}
					if(attack(map, npcs, *npc))
					{
						if(npc->team == 'E')
						{
							goblins--;
						}
						else
						{
							failed = true;
							break;
						}
						if(goblins == 0)
						{
							done = true;
							if(queue.empty())
							{
								completeRound = true;
							}
							break;
						}
					}
				}
			}
		
			result++;
		}	
	}
	
	if(!completeRound)
	{
		result--;
	}
	
	unsigned healthRemaining = 0;
	for(unsigned i=0; i<baseNpcs.size(); i++)
	{
		healthRemaining += npcs[i].health;
	}
	
	result *= healthRemaining;
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	std::vector<std::string> map;
	std::vector<NPC> npcs;
	
	parseInput(map, npcs);

	resultA = fight(map, npcs);
	resultB = strongerElves(map, npcs);
	
	std::cout << "result A: " << resultA << '\n';
	std::cout << "result B: " << resultB << std::endl;
	return 0;
}
