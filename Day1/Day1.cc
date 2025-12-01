#include <iostream>
#include <fstream>
#include <vector>

struct Instruction
{
	uint64_t steps = 0;
	char dir = 0;
};

void parseInput(std::vector<Instruction> &instructions)
{
	std::string line;
	std::ifstream input("input.txt");

	if(input.is_open())
	{
		while(getline(input,line))
		{
			Instruction instruction;
			instruction.dir = line[0];
			instruction.steps = std::stoll(line.substr(1));
			instructions.push_back(instruction);
		}
	}
	input.close();

}

uint64_t turnDial(const std::vector<Instruction> &instructions)
{
	uint64_t result = 0;
	
	uint64_t dial = 50;
	
	for(uint64_t i=0; i<instructions.size(); i++)
	{
		uint64_t steps = 0;
		switch(instructions[i].dir)
		{
			case 'R': steps = instructions[i].steps; break;
			case 'L': steps = instructions[i].steps*99; break;
		}
		dial = (dial + steps)%100;
		if(dial == 0) result++;
	}
	
	return result;
}

uint64_t turnPastZero(const std::vector<Instruction> &instructions)
{
	uint64_t result = 0;
	
	uint64_t dial = 50;
	
	for(uint64_t i=0; i<instructions.size(); i++)
	{
		switch(instructions[i].dir)
		{
			case 'R': if(dial + instructions[i].steps >= 100) result += (dial+instructions[i].steps)/100; dial = (dial + instructions[i].steps)%100; break;
			case 'L': if(instructions[i].steps >= dial && (dial > 0 || instructions[i].steps >= 100)) result += (dial > 0 ? 1 : 0) + ((instructions[i].steps-dial)/100); dial = (dial + instructions[i].steps*99)%100; break;
		}
	}
	
	return result;
}

int main()
{
	uint64_t result = 0;

	std::vector<Instruction> instructions;
	parseInput(instructions);
	result = turnDial(instructions);

	std::cout << "resultA: " << result << '\n';
	
	result = turnPastZero(instructions);
	std::cout << "resultB: " << result << std::endl;

	return 0;
}

