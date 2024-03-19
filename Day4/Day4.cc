#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

void parseInput(std::vector<std::string> &notes)
{
	std::string line;
	
	std::ifstream input("input.txt");
	if(input.is_open())
	{
		while(getline(input,line))
		{
			notes.push_back(line);
		}
	}
	std::sort(notes.begin(), notes.end());
	
	input.close();
}

uint64_t mostAsleep(std::vector<std::string> &notes)
{
	uint64_t result = 0;
	std::unordered_map<unsigned int, unsigned int> guards;
	unsigned int guard;
	unsigned int minute = 0;
	unsigned int sleeping = 0;
	size_t pos = 0;
	size_t endpos = 0;
	
	for(unsigned int i=0; i<notes.size(); i++)
	{
		pos = notes[i].find_first_of("#");
		if(pos != std::string::npos)
		{
			sleeping = 0;
			endpos = notes[i].find_first_of(" ", pos);
			guard = std::stoi(notes[i].substr(pos+1, endpos-pos-1));
		}
		else
		{
			sleeping = (sleeping+1)%2;
			pos = notes[i].find_first_of(":")+1;
			if(sleeping == 1)
			{	
				minute = std::stoi(notes[i].substr(pos, 2));
			}
			else
			{
				guards[guard] += std::stoi(notes[i].substr(pos,2)) - minute;
			}
		}
	}

	uint64_t maxSleep = 0;
	for(auto it=guards.begin(); it!=guards.end(); it++)
	{
		if(maxSleep < it->second)
		{
			guard = it->first;
			maxSleep = it->second;
		}
	}
	
	std::unordered_map<unsigned int,unsigned int> minuteSleep;
	bool sleepyGuard = false;
	unsigned int minutesSleeping = 0;
	for(unsigned int i=0; i<notes.size(); i++)
	{
		pos = notes[i].find_first_of("#");
		if(pos != std::string::npos)
		{
			sleeping = 0;
			endpos = notes[i].find_first_of(" ", pos);
			if(guard == (unsigned int)std::stoi(notes[i].substr(pos+1, endpos-pos-1)))
			{
				sleepyGuard = true;
			}
			else
			{
				sleepyGuard = false;
			}
		}
		else if(sleepyGuard)
		{
			sleeping = (sleeping+1)%2;
			pos = notes[i].find_first_of(":")+1;
			if(sleeping == 1)
			{	
				minute = std::stoi(notes[i].substr(pos, 2));
			}
			else
			{
				minutesSleeping = std::stoi(notes[i].substr(pos,2)) - minute;
				for(unsigned int j=0; j<minutesSleeping; j++)
				{
					minuteSleep[j+minute] += 1;
				}
			}
		}
	}
	
	uint64_t maxMinutes = 0;
	uint64_t maxMinuteAsleep = 0;
	for(auto it=minuteSleep.begin(); it!=minuteSleep.end(); it++)
	{
		if(maxMinutes < it->second)
		{
			maxMinutes = it->second;
			maxMinuteAsleep = it->first;
		}
	}
	
	result = maxMinuteAsleep*guard;
	
	return result;
}

uint64_t mostAsleepAtMinute(std::vector<std::string> &notes)
{
	uint64_t result = 0;
	std::unordered_map<unsigned int, unsigned int[60]> guards;
	unsigned int guard = 0;
	unsigned int minute = 0;
	unsigned int sleeping = 0;
	unsigned int minutesSleeping = 0;
	size_t pos = 0;
	size_t endpos = 0;
	
	for(unsigned int i=0; i<notes.size(); i++)
	{
		pos = notes[i].find_first_of("#");
		if(pos != std::string::npos)
		{
			sleeping = 0;
			endpos = notes[i].find_first_of(" ", pos);
			guard = std::stoi(notes[i].substr(pos+1, endpos-pos-1));
		}
		else
		{
			sleeping = (sleeping+1)%2;
			pos = notes[i].find_first_of(":")+1;
			if(sleeping == 1)
			{	
				minute = std::stoi(notes[i].substr(pos, 2));
			}
			else
			{
				minutesSleeping = std::stoi(notes[i].substr(pos,2)) - minute;
				for(unsigned int j=0; j<minutesSleeping; j++)
				{
					(guards[guard])[j+minute] += 1;
				}
			}
		}
	}

	uint64_t maxSleep = 0;

	for(auto it=guards.begin(); it!=guards.end(); it++)
	{
		uint64_t newMaxSleep = 0;
		uint64_t maxMinutesAsleep = 0;
		for(unsigned int i=0; i<60; i++)
		{
			if(maxMinutesAsleep < it->second[i])
			{
				maxMinutesAsleep = it->second[i];
				newMaxSleep = i;
			}
		}
		if(maxSleep < newMaxSleep)
		{
			guard = it->first;
			maxSleep = newMaxSleep;
		}
	}
	
	result = maxSleep*guard;
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	std::vector<std::string> notes;
	
	parseInput(notes);
	
	resultA = mostAsleep(notes);
	resultB = mostAsleepAtMinute(notes);
	
	std::cout << "result A: " << resultA << '\n';
	std::cout << "result B: " << resultB << std::endl;
	return 0;
}
