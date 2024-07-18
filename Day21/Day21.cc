#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm>
#include <unordered_map>
#include <utility>

//Works with my input, not yours. Need to steer in line 218 - 227 depending on your input, and since I have no idea exactly how this may variate depending on input I can't really create something that definitively works with your input. 

typedef void(*opCode)(unsigned, unsigned, unsigned, std::array<unsigned, 6>&);

void parseInput(std::vector<std::array<unsigned, 4>> &program, unsigned &instructionPtr)
{
	std::string line;
	std::array<unsigned, 4> codeLine;
	std::string instruction;
	size_t pos = 0;
	size_t endpos = 0;
	std::ifstream input("input.txt");
	if(input.is_open())
	{
		getline(input, line);
		
		instructionPtr = line.back() - '0';

		while(getline(input, line))
		{
			instruction = line.substr(0,4);
			if(instruction == "addr")
			{
				codeLine[0] = 0;
			}
			else if(instruction == "addi")
			{
				codeLine[0] = 1;
			}
			else if(instruction == "mulr")
			{
				codeLine[0] = 2;
			}
			else if(instruction == "muli")
			{
				codeLine[0] = 3;
			}
			else if(instruction == "banr")
			{
				codeLine[0] = 4;
			}
			else if(instruction == "bani")
			{
				codeLine[0] = 5;
			}
			else if(instruction == "borr")
			{
				codeLine[0] = 6;
			}
			else if(instruction == "bori")
			{
				codeLine[0] = 7;
			}
			else if(instruction == "setr")
			{
				codeLine[0] = 8;
			}
			else if(instruction == "seti")
			{
				codeLine[0] = 9;
			}
			else if(instruction == "gtir")
			{
				codeLine[0] = 10;
			}
			else if(instruction == "gtri")
			{
				codeLine[0] = 11;
			}
			else if(instruction == "gtrr")
			{
				codeLine[0] = 12;
			}
			else if(instruction == "eqir")
			{
				codeLine[0] = 13;
			}
			else if(instruction == "eqri")
			{
				codeLine[0] = 14;
			}
			else if(instruction == "eqrr")
			{
				codeLine[0] = 15;
			}
			
			pos = line.find_first_of("1234567890");
			endpos = line.find_first_of(" ", pos);
			codeLine[1] = std::stoi(line.substr(pos,endpos-pos));
			pos = line.find_first_of("1234567890",endpos);
			endpos = line.find_first_of(" ", pos);
			codeLine[2] = std::stoi(line.substr(pos,endpos-pos));
			pos = line.find_first_of("1234567890",endpos);
			codeLine[3] = std::stoi(line.substr(pos));

			program.push_back(codeLine);
		}
	}
	input.close();
}

void addr(unsigned a, unsigned b, unsigned c, std::array<unsigned, 6> &registers)
{
  registers[c] = registers[a]+registers[b];
}

void addi(unsigned a, unsigned b, unsigned c, std::array<unsigned, 6> &registers)
{
  registers[c] = registers[a]+b;
}

void mulr(unsigned a, unsigned b, unsigned c, std::array<unsigned, 6> &registers)
{
  registers[c] = registers[a]*registers[b];
}

void muli(unsigned a, unsigned b, unsigned c, std::array<unsigned, 6> &registers)
{
  registers[c] = registers[a]*b;
}

void banr(unsigned a, unsigned b, unsigned c, std::array<unsigned, 6> &registers)
{
  registers[c] = registers[a]&registers[b];
}

void bani(unsigned a, unsigned b, unsigned c, std::array<unsigned, 6> &registers)
{
  registers[c] = registers[a]&b;
}

void borr(unsigned a, unsigned b, unsigned c, std::array<unsigned, 6> &registers)
{
  registers[c] = registers[a]|registers[b];
}

void bori(unsigned a, unsigned b, unsigned c, std::array<unsigned, 6> &registers)
{
  registers[c] = registers[a]|b;
}

void setr(unsigned a, unsigned b, unsigned c, std::array<unsigned, 6> &registers)
{
  registers[c] = registers[a];
}

void seti(unsigned a, unsigned b, unsigned c, std::array<unsigned, 6> &registers)
{
  registers[c] = a;
}

void gtir(unsigned a, unsigned b, unsigned c, std::array<unsigned, 6> &registers)
{
  registers[c] = (a > registers[b]) ? 1 : 0;
}

void gtri(unsigned a, unsigned b, unsigned c, std::array<unsigned, 6> &registers)
{
  registers[c] = (registers[a] > b) ? 1 : 0;
}

void gtrr(unsigned a, unsigned b, unsigned c, std::array<unsigned, 6> &registers)
{
  registers[c] = (registers[a] > registers[b]) ? 1 : 0;
}

void eqir(unsigned a, unsigned b, unsigned c, std::array<unsigned, 6> &registers)
{
  registers[c] = (a == registers[b]) ? 1 : 0;
}

void eqri(unsigned a, unsigned b, unsigned c, std::array<unsigned, 6> &registers)
{
  registers[c] = (registers[a] == b) ? 1 : 0;
}

void eqrr(unsigned a, unsigned b, unsigned c, std::array<unsigned, 6> &registers)
{
  registers[c] = (registers[a] == registers[b]) ? 1 : 0;
}

void initializeOpCodes(std::array<opCode,16> &opCodes)
{
	opCodes[0] = &addr;
	opCodes[1] = &addi;
	opCodes[2] = &mulr;
	opCodes[3] = &muli;
	opCodes[4] = &banr;
	opCodes[5] = &bani;
	opCodes[6] = &borr;
	opCodes[7] = &bori;
	opCodes[8] = &setr;
	opCodes[9] = &seti;
	opCodes[10] = &gtir;
	opCodes[11] = &gtri;
	opCodes[12] = &gtrr;
	opCodes[13] = &eqir;
	opCodes[14] = &eqri;
	opCodes[15] = &eqrr;
}

std::pair<uint64_t, uint64_t> runProgram(const std::array<opCode, 16> &opCodes, const std::vector<std::array<unsigned, 4>> &program, unsigned instructionPtr)
{
	std::pair<uint64_t, uint64_t> result = {0,0};
	std::array<unsigned,6> registers{0};
	std::unordered_map<unsigned,unsigned> cache;

	for(registers[instructionPtr]; registers[instructionPtr]>=0 && registers[instructionPtr]<program.size(); registers[instructionPtr]++)
	{
		if(registers[instructionPtr] == 28)
		{
			if(result.first == 0)
			{
				result.first = registers[3];
			}
			if(!cache.count(registers[3]))
			{
				cache[registers[3]] = 1;
				result.second = registers[3];
			}
			else
			{
				break;
			}
		}

		opCodes[program[registers[instructionPtr]][0]](program[registers[instructionPtr]][1],program[registers[instructionPtr]][2],program[registers[instructionPtr]][3], registers);
	}
	
	return result;
}

int main()
{
	std::pair<uint64_t, uint64_t> result;

	std::vector<std::array<unsigned, 4>> program;
	unsigned instructionPtr = 0;
	std::array<opCode,16> opCodes;

	parseInput(program, instructionPtr);

	initializeOpCodes(opCodes);

	result = runProgram(opCodes, program, instructionPtr);

	std::cout << "result A: " << result.first << '\n';
	std::cout << "result B: " << result.second << std::endl;
	return 0;
}

