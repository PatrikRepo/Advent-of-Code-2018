#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <algorithm>
#include <utility>

struct Group
{
	unsigned id = 0;
	unsigned target = 0;
	bool targeted = false;
	int units = 0;
	unsigned HP = 0;
	char team = 0;
	unsigned power = 0;
	unsigned initiative = 0;
	//Radiation = 1, Cold = 2, Fire = 4, Bludgeoning = 8, Slashing = 16
	unsigned attackType = 0;
	unsigned immunities = 0;
	unsigned weaknesses = 0;
};

struct selectionOrdering
{
	bool operator()(const Group* group1, const Group* group2) const { 
		if(group1->units*group1->power != group2->units*group2->power) 
			return group1->units*group1->power > group2->units*group2->power; 
		else 
			return group1->initiative > group2->initiative; }
};

struct attackOrdering
{
	bool operator()(const Group* group1, const Group* group2) const { return group1->initiative > group2->initiative; }
};

unsigned convertElement(char element)
{
	unsigned result = 0;
	
	switch(element)
	{
		case 'r':
		{
			result = 1;
			break;
		}
		case 'c':
		{
			result = 2;
			break;
		}
		case 'f':
		{
			result = 4;
			break;
		}
		case 'b':
		{
			result = 8;
			break;
		}
		case 's':
		{
			result = 16;
		}
	}
	
	return result;
}

void buildGroup(Group &group, std::string &line)
{
	size_t pos = 0;
  size_t endpos = 0;
  
	endpos = line.find_first_of(" ");
	group.units = std::stoi(line.substr(0, endpos));

	pos = line.find_first_of("1234567890", endpos);
	endpos = line.find_first_of(" ", pos);
	group.HP = std::stoi(line.substr(pos, endpos-pos));

	pos = line.find_first_of("(", endpos);
	if(pos != std::string::npos)
	{
		pos++;
		if(line[pos] == 'i')
		{
			pos += 8;
			while(line[pos] != ')' && line[pos] != ';')
			{
				pos += 2;
				group.immunities += convertElement(line[pos]);
				pos = line.find_first_of(",);", pos);
			}
			pos += 2;
		}
		if(line[pos+1] == 'e')
		{
			pos += 6;
			while(line[pos] != ')')
			{
				pos += 2;
				group.weaknesses += convertElement(line[pos]);
				pos = line.find_first_of(",)", pos);
			}
			pos += 2;
		}
	}

	pos = line.find_first_of("1234567890", endpos);
	endpos = line.find_first_of(" ", pos);
	group.power = std::stoi(line.substr(pos, endpos-pos));

	group.attackType = convertElement(line[endpos+1]);

	pos = line.find_first_of("1234567890", endpos);
	group.initiative = std::stoi(line.substr(pos));
}

void parseInput(std::vector<Group> &groups)
{
  std::string line;
	unsigned id = 1;
	
  std::ifstream input("input.txt");

  if(input.is_open())
  {
  	getline(input,line);
  	getline(input,line);
  	
    while(line != "")
    {
    	Group group;
    	group.id = id;
    	id++;
    	group.team = 'G';
    	buildGroup(group, line);
    	
    	groups.push_back(group);
    	
    	getline(input,line);
    }
    
    getline(input,line);
    
    while(getline(input,line))
    {
    	Group group;
     	group.id = id;
    	id++;
    	group.team = 'E';
    	buildGroup(group, line);
    	
    	groups.push_back(group);
    }
  }
  input.close();
}

uint64_t fight(std::vector<Group> &groups)
{
	uint64_t result = 0;
	
	unsigned numberOfGood = 0;
	unsigned numberOfEvil = 0;
	
	for(Group &group:groups)
	{
		if(group.team == 'G')
		{
			numberOfGood++;
		}
		else
		{
			numberOfEvil++;
		}
	}
	
	while(numberOfGood > 0 && numberOfEvil > 0)
	{
		bool stalemate = true;											
		std::deque<Group*> selectOrder;
		std::deque<Group*> attackOrder;

		for(Group &group:groups)
		{
			selectOrder.push_back(&group);
			attackOrder.push_back(&group);
		}
		std::sort(selectOrder.begin(), selectOrder.end(), selectionOrdering());
		std::sort(attackOrder.begin(), attackOrder.end(), attackOrdering());

		for(Group* group:selectOrder)
		{
			unsigned target = 0;
			unsigned damage = 0;
			unsigned enemyPower = 0;
			unsigned enemyUnits = 0;
			unsigned enemyInitiative = 0;
			for(Group &enemy:groups)
			{
				if(!enemy.targeted && enemy.team != group->team)
				{
					if((enemy.immunities & group->attackType) == 0)
					{
						unsigned tempDamage = group->power*group->units;
						if((enemy.weaknesses & group->attackType) > 0)
						{
							tempDamage *= 2;
						}
						if(tempDamage > damage || (tempDamage == damage && (enemy.power*enemy.units > enemyPower*enemyUnits || (enemy.power*enemy.units == enemyPower*enemyUnits && enemy.initiative > enemyInitiative))))
						{
							target = enemy.id;
							damage = tempDamage;
							enemyPower = enemy.power;
							enemyUnits = enemy.units;
							enemyInitiative = enemy.initiative;
						}
					}
				}
			}
			
			group->target = target;
			for(Group &enemy:groups)
			{
				if(enemy.id == target)
				{
					enemy.targeted = true;
					break;
				}
			}
		}

		for(Group* group:attackOrder)
		{
			if(group->target > 0 && group->units > 0)
			{
				Group* enemyTarget = nullptr;
				for(Group &enemy:groups)
				{
					if(enemy.id == group->target)
					{
						enemyTarget = &enemy;
						break;
					}
				}
				unsigned attackDamage = group->power*group->units;
				
				if((group->attackType & enemyTarget->weaknesses) > 0)
				{
					attackDamage *= 2;
				}
				
				if(enemyTarget != nullptr)
				{
					enemyTarget->units -= attackDamage/enemyTarget->HP;
					if(stalemate && attackDamage/enemyTarget->HP > 0)
					{
						stalemate = false;
					}
					if(enemyTarget->units <= 0)
					{
						if(enemyTarget->team == 'G')
						{
							numberOfGood--;
						}
						else
						{
							numberOfEvil--;
						}
					}
				}

			}
		}
		
		for(unsigned i=0; i<groups.size();)
		{
			if(groups[i].units <= 0)
			{
				groups.erase(groups.begin()+i);
			}
			else
			{
				groups[i].targeted = false;
				groups[i].target = 0;
				i++;
			}
		}
		if(stalemate)
		{
			break;
		}
	}

	for(Group &group:groups)
	{
		result += group.units;
	}

	return result;
}

std::pair<uint64_t,uint64_t> boostImmuneSystem(std::vector<Group> &baseGroups)
{
	std::pair<uint64_t,uint64_t> result;
	auto groups = baseGroups;
	result.first = fight(groups);
	
	unsigned boost = 10;
	bool done = false;

	while(!done)
	{
		groups = baseGroups;
		for(Group &group:groups)
		{
			if(group.team == 'G')
			{
				group.power += boost;
			}
		}
		fight(groups);
		if(groups[0].team == 'G')
		{
			break;
		}
		boost += 10;
	}
	
	boost -= 9;
	
	while(!done)
	{
		groups = baseGroups;
		for(Group &group:groups)
		{
			if(group.team == 'G')
			{
				group.power += boost;
			}
		}
		fight(groups);
		if(groups.back().team == 'G')
		{
			done = true;
			for(Group &group:groups)
			{
				result.second += group.units;
			}
			break;
		}
		boost += 1;
	}
	return result;
}

int main()
{
  std::pair<uint64_t,uint64_t> result;
	std::vector<Group> groups;
   
  parseInput(groups);

 	result = boostImmuneSystem(groups);
 
  std::cout << "result A: " << result.first << '\n';
  std::cout << "result B: " << result.second << std::endl;
  return 0;
}
