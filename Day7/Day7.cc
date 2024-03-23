#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <map>
#include <queue>

struct Step
{
	unsigned dependencies = 0;
	std::vector<char> after;
};

void parseInput(std::map<char,Step> &steps)
{
	std::string line;
	std::ifstream input("input.txt");
	if(input.is_open())
	{
		while(getline(input,line))
		{
			char before = line[5];
			char after = line[36];
			if(!steps.count(before))
			{
				steps[before] = Step();
			}
			if(!steps.count(after))
			{
				steps[after] = Step();
			}
			steps[before].after.push_back(after);
			steps[after].dependencies += 1;
		}
	}
	input.close();
}

std::string printInstructions(std::map<char,Step> steps)
{
	std::string result;
	std::priority_queue<char, std::vector<char>, std::greater<char>> queue;
	char next = 0;
	
	for(auto it=steps.begin(); it!=steps.end(); it++)
	{
		if(it->second.dependencies == 0)
		{
			queue.push(it->first);
		}
	}
	
	while(!queue.empty())
	{
		next = queue.top();
		result += next;
		queue.pop();
		
		for(unsigned i=0; i<steps[next].after.size(); i++)
		{
			steps[steps[next].after[i]].dependencies -= 1;
			if(steps[steps[next].after[i]].dependencies == 0)
			{
				queue.push(steps[next].after[i]);
			}
		}
	}
	
	return result;
}

uint64_t multiThreading(std::map<char,Step> &steps)
{
	uint64_t result = 0;
	unsigned freeWorkers = 5;
	std::priority_queue<char, std::vector<char>, std::greater<char>> queue;
	std::pair<char,unsigned> time[5];
	char next = 0;
	
	for(auto it=steps.begin(); it!=steps.end(); it++)
	{
		if(it->second.dependencies == 0)
		{
			queue.push(it->first);
		}
	}
	
	while(!queue.empty() || freeWorkers < 5)
	{
		while(!queue.empty() && freeWorkers > 0)
		{
			freeWorkers--;
			next = queue.top();
			queue.pop();
			for(unsigned i=0; i<5; i++)
			{
				if(time[i].second == 0)
				{
					time[i].second = 61 + next - 'A';
					time[i].first = next;
					break;
				}
			}
		}
		
		unsigned min = 100;
		for(unsigned i=0; i<5; i++)
		{
			if(min > time[i].second && time[i].second != 0)
			{
				min = time[i].second;
			}
		}
		std::priority_queue<char, std::vector<char>, std::greater<char>> innerQueue;
		for(unsigned i=0; i<5; i++)
		{
			if(time[i].second > 0)
			{
				time[i].second -= min;
				if(time[i].second == 0)
				{
					innerQueue.push(time[i].first);
				}
			}
		}
		result += min;
		while(!innerQueue.empty())
		{
			freeWorkers++;
			next = innerQueue.top();
			innerQueue.pop();
			for(unsigned i=0; i<steps[next].after.size(); i++)
			{
				steps[steps[next].after[i]].dependencies -= 1;
				if(steps[steps[next].after[i]].dependencies == 0)
				{
					queue.push(steps[next].after[i]);
				}
			}
		}
	}
	return result;
}

int main()
{
	std::string resultA;
	uint64_t resultB = 0;
	std::map<char,Step> steps;
	
	parseInput(steps);

	resultA = printInstructions(steps);
	resultB = multiThreading(steps);
	
	std::cout << "result A: " << resultA << '\n';
	std::cout << "result B: " << resultB << std::endl;
	return 0;
}
