#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm>

typedef void(*opCode)(unsigned, unsigned, unsigned, std::array<unsigned, 4>&);

void parseInput(std::vector<std::array<unsigned, 12>> &samples, std::vector<std::array<unsigned, 4>> &program)
{
	std::string line;
	std::array<unsigned, 12> sample;
	std::array<unsigned, 4> codeLine;
	size_t pos = 0;
	size_t endpos = 0;
	std::ifstream input("input.txt");
	if(input.is_open())
	{
		getline(input, line);
		while(line != "")
		{
			pos = line.find_first_of("1234567890");
			sample[0] = line[pos] - '0';
			pos = line.find_first_of("1234567890",pos+1);
			sample[1] = line[pos] - '0';
			pos = line.find_first_of("1234567890",pos+1);
			sample[2] = line[pos] - '0';
			pos = line.find_first_of("1234567890",pos+1);
			sample[3] = line[pos] - '0';

			getline(input, line);
			pos = line.find_first_of("1234567890");
			endpos = line.find_first_of(" ", pos);
			sample[4] = std::stoi(line.substr(pos, endpos-pos));
			pos = line.find_first_of("1234567890",endpos);
			sample[5] = line[pos] - '0';
			pos = line.find_first_of("1234567890",pos+1);
			sample[6] = line[pos] - '0';
			pos = line.find_first_of("1234567890",pos+1);
			sample[7] = line[pos] - '0';

			getline(input, line);
			pos = line.find_first_of("1234567890");
			sample[8] = line[pos] - '0';
			pos = line.find_first_of("1234567890",pos+1);
			sample[9] = line[pos] - '0';
			pos = line.find_first_of("1234567890",pos+1);
			sample[10] = line[pos] - '0';
			pos = line.find_first_of("1234567890",pos+1);
			sample[11] = line[pos] - '0';

			samples.push_back(sample);
			getline(input, line);
			getline(input, line);
		}

		getline(input, line);

		while(getline(input, line))
		{
			pos = line.find_first_of("1234567890");
			endpos = line.find_first_of(" ", pos);
			codeLine[0] = std::stoi(line.substr(pos, endpos-pos));
			pos = line.find_first_of("1234567890",endpos);
			codeLine[1] = line[pos] - '0';
			pos = line.find_first_of("1234567890",pos+1);
			codeLine[2] = line[pos] - '0';
			pos = line.find_first_of("1234567890",pos+1);
			codeLine[3] = line[pos] - '0';

			program.push_back(codeLine);
		}
	}
	input.close();
}

void addr(unsigned a, unsigned b, unsigned c, std::array<unsigned, 4> &registers)
{
  registers[c] = registers[a]+registers[b];
}

void addi(unsigned a, unsigned b, unsigned c, std::array<unsigned, 4> &registers)
{
  registers[c] = registers[a]+b;
}

void mulr(unsigned a, unsigned b, unsigned c, std::array<unsigned, 4> &registers)
{
  registers[c] = registers[a]*registers[b];
}

void muli(unsigned a, unsigned b, unsigned c, std::array<unsigned, 4> &registers)
{
  registers[c] = registers[a]*b;
}

void banr(unsigned a, unsigned b, unsigned c, std::array<unsigned, 4> &registers)
{
  registers[c] = registers[a]&registers[b];
}

void bani(unsigned a, unsigned b, unsigned c, std::array<unsigned, 4> &registers)
{
  registers[c] = registers[a]&b;
}

void borr(unsigned a, unsigned b, unsigned c, std::array<unsigned, 4> &registers)
{
  registers[c] = registers[a]|registers[b];
}

void bori(unsigned a, unsigned b, unsigned c, std::array<unsigned, 4> &registers)
{
  registers[c] = registers[a]|b;
}

void setr(unsigned a, unsigned b, unsigned c, std::array<unsigned, 4> &registers)
{
  registers[c] = registers[a];
}

void seti(unsigned a, unsigned b, unsigned c, std::array<unsigned, 4> &registers)
{
  registers[c] = a;
}

void gtir(unsigned a, unsigned b, unsigned c, std::array<unsigned, 4> &registers)
{
  registers[c] = (a > registers[b]) ? 1 : 0;
}

void gtri(unsigned a, unsigned b, unsigned c, std::array<unsigned, 4> &registers)
{
  registers[c] = (registers[a] > b) ? 1 : 0;
}

void gtrr(unsigned a, unsigned b, unsigned c, std::array<unsigned, 4> &registers)
{
  registers[c] = (registers[a] > registers[b]) ? 1 : 0;
}

void eqir(unsigned a, unsigned b, unsigned c, std::array<unsigned, 4> &registers)
{
  registers[c] = (a == registers[b]) ? 1 : 0;
}

void eqri(unsigned a, unsigned b, unsigned c, std::array<unsigned, 4> &registers)
{
  registers[c] = (registers[a] == b) ? 1 : 0;
}

void eqrr(unsigned a, unsigned b, unsigned c, std::array<unsigned, 4> &registers)
{
  registers[c] = (registers[a] == registers[b]) ? 1 : 0;
}

uint64_t calculateOpcodes(const std::vector<std::array<unsigned, 12>> &samples, std::array<opCode, 16> &opCodes)
{
  uint64_t result = 0;
  std::array<unsigned, 4> registers = {0,0,0,0};
  std::array<unsigned, 4> preRegisters = registers;
  std::array<unsigned, 4> resultRegisters = registers;
 
 	std::array<std::array<unsigned,16>,16> opCodeHistogram{0};
 
  for(std::array<unsigned, 12> sample : samples)
  {
    unsigned counter = 0;
    for(int i=0; i<4; i++)
    {
      registers[i] = sample[i];
      resultRegisters[i] = sample[i+8];
    }
   
    preRegisters = registers;
    addr(sample[5], sample[6], sample[7], preRegisters);
    if(preRegisters == resultRegisters)
    {
      counter++;
      opCodeHistogram[sample[4]][0]++;
    }
    preRegisters = registers;
    addi(sample[5], sample[6], sample[7], preRegisters);
    if(preRegisters == resultRegisters)
    {
      counter++;
      opCodeHistogram[sample[4]][1]++;
    }
    preRegisters = registers;
    mulr(sample[5], sample[6], sample[7], preRegisters);
    if(preRegisters == resultRegisters)
    {
      counter++;
      opCodeHistogram[sample[4]][2]++;

    }
    preRegisters = registers;
    muli(sample[5], sample[6], sample[7], preRegisters);
    if(preRegisters == resultRegisters)
    {
      counter++;
      opCodeHistogram[sample[4]][3]++;

    }
    preRegisters = registers;
    banr(sample[5], sample[6], sample[7], preRegisters);
    if(preRegisters == resultRegisters)
    {
      counter++;
      opCodeHistogram[sample[4]][4]++;

    }
    preRegisters = registers;
    bani(sample[5], sample[6], sample[7], preRegisters);
    if(preRegisters == resultRegisters)
    {
      counter++;
      opCodeHistogram[sample[4]][5]++;

    }
    preRegisters = registers;
    borr(sample[5], sample[6], sample[7], preRegisters);
    if(preRegisters == resultRegisters)
    {
      counter++;
      opCodeHistogram[sample[4]][6]++;

    }
    preRegisters = registers;
    bori(sample[5], sample[6], sample[7], preRegisters);
    if(preRegisters == resultRegisters)
    {
      counter++;
      opCodeHistogram[sample[4]][7]++;

    }
    preRegisters = registers;
    setr(sample[5], sample[6], sample[7], preRegisters);
    if(preRegisters == resultRegisters)
    {
      counter++;
      opCodeHistogram[sample[4]][8]++;

    }
    preRegisters = registers;
    seti(sample[5], sample[6], sample[7], preRegisters);
    if(preRegisters == resultRegisters)
    {
      counter++;
      opCodeHistogram[sample[4]][9]++;

    }
    preRegisters = registers;
    gtir(sample[5], sample[6], sample[7], preRegisters);
    if(preRegisters == resultRegisters)
    {
      counter++;
      opCodeHistogram[sample[4]][10]++;

    }
    preRegisters = registers;
    gtri(sample[5], sample[6], sample[7], preRegisters);
    if(preRegisters == resultRegisters)
    {
      counter++;
      opCodeHistogram[sample[4]][11]++;

    }
    preRegisters = registers;
    gtrr(sample[5], sample[6], sample[7], preRegisters);
    if(preRegisters == resultRegisters)
    {
      counter++;
      opCodeHistogram[sample[4]][12]++;

    }
    preRegisters = registers;
    eqir(sample[5], sample[6], sample[7], preRegisters);
    if(preRegisters == resultRegisters)
    {
      counter++;
      opCodeHistogram[sample[4]][13]++;

    }
    preRegisters = registers;
    eqri(sample[5], sample[6], sample[7], preRegisters);
    if(preRegisters == resultRegisters)
    {
      counter++;
      opCodeHistogram[sample[4]][14]++;

    }
    preRegisters = registers;
    eqrr(sample[5], sample[6], sample[7], preRegisters);
    if(preRegisters == resultRegisters)
    {
      counter++;
      opCodeHistogram[sample[4]][15]++;

    }
    if(counter >= 3)
    {
      result++;
	  }
  }

	for(std::array<unsigned, 16> &array : opCodeHistogram)
	{
		unsigned max = *std::max_element(array.begin(), array.end());
		for(unsigned &element : array)
		{
			if(element < max)
			{
				element = 0;
			}
		}
	}
	
	unsigned opCodesLeft = opCodes.size();
	
	while(opCodesLeft > 0)
	{
		for(unsigned j=0; j<opCodeHistogram.size(); j++)
		{
			unsigned countNonZero = 0;
			unsigned index = 0;
			for(unsigned i=0; i<opCodeHistogram[j].size(); i++)
			{
				if(opCodeHistogram[j][i] > 0)
				{
					countNonZero++;
					index = i;
				}
			}
			if(countNonZero == 1 && opCodes[j] == nullptr)
			{
				for(unsigned i=0; i<opCodeHistogram.size(); i++)
				{
					if(opCodeHistogram[i] != opCodeHistogram[j])
					{
						opCodeHistogram[i][index] = 0;
					}
				}
				switch(index)
				{
					case 0:
					{
						opCodes[j] = &addr;
						break;
					}
					case 1:
					{
						opCodes[j] = &addi;
						break;
					}
					case 2:
					{
						opCodes[j] = &mulr;
						break;
					}
					case 3:
					{
						opCodes[j] = &muli;
						break;
					}
					case 4:
					{
						opCodes[j] = &banr;
						break;
					}
					case 5:
					{
						opCodes[j] = &bani;
						break;
					}
					case 6:
					{
						opCodes[j] = &borr;
						break;
					}
					case 7:
					{
						opCodes[j] = &bori;
						break;
					}
					case 8:
					{
						opCodes[j] = &setr;
						break;
					}
					case 9:
					{
						opCodes[j] = &seti;
						break;
					}
					case 10:
					{
						opCodes[j] = &gtir;
						break;
					}
					case 11:
					{
						opCodes[j] = &gtri;
						break;
					}
					case 12:
					{
						opCodes[j] = &gtrr;
						break;
					}
					case 13:
					{
						opCodes[j] = &eqir;
						break;
					}
					case 14:
					{
						opCodes[j] = &eqri;
						break;
					}
					case 15:
					{
						opCodes[j] = &eqrr;
					}
				}
				opCodesLeft--;
			}
		}
	}

  return result;
}

uint64_t runProgram(const std::array<opCode, 16> &opCodes, const std::vector<std::array<unsigned, 4>> &program)
{
	uint64_t result = 0;
	
	std::array<unsigned, 4> registers{0};
	
	for(auto line : program)
	{
		opCodes[line[0]](line[1],line[2],line[3],registers);
	}
	
	result = registers[0];
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	std::vector<std::array<unsigned, 12>> samples;
	std::vector<std::array<unsigned, 4>> program;
	std::array<opCode, 16> opCodes{nullptr};

	parseInput(samples, program);

	resultA = calculateOpcodes(samples, opCodes);

	resultB = runProgram(opCodes, program);

	std::cout << "result A: " << resultA << '\n';
	std::cout << "result B: " << resultB << std::endl;
	return 0;
}

