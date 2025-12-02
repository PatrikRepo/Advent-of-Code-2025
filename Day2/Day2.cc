#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

void parseInput(std::vector<std::pair<uint64_t,uint64_t>> &ranges)
{
	std::string line;
	std::ifstream input("input.txt");

	if(input.is_open())
	{
		getline(input,line);
		size_t pos = 0;
		while(pos < line.size())
		{
			size_t endpos = line.find_first_of("-",pos);
			uint64_t from = std::stoll(line.substr(pos,endpos-pos));
			pos = endpos+1;
			endpos = line.find_first_of(",",pos);
			endpos = (endpos == std::string::npos) ? line.size() : endpos;
			uint64_t to = std::stoll(line.substr(pos,endpos-pos));
			ranges.emplace_back(from,to);
			pos = endpos+1;
		}
	}
	input.close();

}

uint64_t findInvalid(const std::vector<std::pair<uint64_t,uint64_t>> &ranges)
{
	uint64_t result = 0;
	
	for(unsigned i=0; i<ranges.size(); i++)
	{
		bool found = false;
		uint64_t half = 1;
		uint64_t invalid = 11;
		uint64_t factor = 10;
		while(!found)
		{
			if(invalid > ranges[i].second) found = true;
			if(found) break;
			if(invalid >= ranges[i].first) result += invalid;
			half++;
			if(half == factor) factor *= 10;
			invalid = (half*factor) + half;
		}
	}
	
	return result;
}

uint64_t moreInvalid(const std::vector<std::pair<uint64_t,uint64_t>> &ranges)
{
	uint64_t result = 0;
	
	for(unsigned i=0; i<ranges.size(); i++)
	{
		bool found = false;
		uint64_t part = 1;
		uint64_t invalid = 11;
		uint64_t factor = 10;
		std::vector<uint64_t> foundInvalid;
		while(!found)
		{
			if(invalid > ranges[i].second) break;
			uint64_t newFactor = factor;
			while(invalid <= ranges[i].second)
			{
				if(invalid >= ranges[i].first)
				{
					if(std::find(foundInvalid.begin(), foundInvalid.end(), invalid) == std::end(foundInvalid))
					{
						result += invalid;
						foundInvalid.push_back(invalid);
					}
				}
				newFactor *= factor;
				invalid += part*newFactor;
			}

			part++;
			if(part == factor) factor *= 10;
			invalid = (part*factor) + part;
		}
	}
	
	return result;
}


int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;

	std::vector<std::pair<uint64_t,uint64_t>> ranges;
	parseInput(ranges);
	resultA = findInvalid(ranges);
	resultB = moreInvalid(ranges);
	
	std::cout << "resultA: " << resultA << '\n' << "resultB: " << resultB << std::endl;

	return 0;
}

