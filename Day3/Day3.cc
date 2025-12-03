#include <iostream>
#include <fstream>
#include <vector>

void parseInput(std::vector<std::string> &banks)
{
	std::string line;
	std::ifstream input("input.txt");

	if(input.is_open())
	{
		while(getline(input,line))
		{
			banks.push_back(line);
		}
	}
	input.close();

}

uint64_t maxJoltage(const std::vector<std::string> &banks, unsigned batteries)
{
	uint64_t result = 0;
	
	for(unsigned i=0; i<banks.size(); i++)
	{
		unsigned max = 0;
		int maxIndex = -1;
		uint64_t factor = 1;
		for(unsigned i=0; i<batteries-1; i++) factor *= 10;
		unsigned maxBattery = banks[i].size()-batteries;
		while(maxBattery < banks[i].size())
		{
			for(unsigned j=maxIndex+1; j<=maxBattery; j++)
			{
				if(banks[i][j] - '0' > max)
				{
					max = banks[i][j] - '0';
					maxIndex = j;
				}
			}
			result += max*factor;
			maxBattery++;
			factor /= 10;
			max = 0;
		}
	}
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;

	std::vector<std::string> banks;
	parseInput(banks);
	resultA = maxJoltage(banks, 2);
	resultB = maxJoltage(banks, 12);
	
	std::cout << "resultA: " << resultA << '\n' << "resultB: " << resultB << std::endl;

	return 0;
}

