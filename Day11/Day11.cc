#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

void parseInput(std::unordered_map<std::string,std::vector<std::string>> &map)
{
	std::string line;
	std::ifstream input("input.txt");

	if(input.is_open())
	{
		while(getline(input,line))
		{
			size_t pos = line.find_first_of(":");
			std::string from = line.substr(0,pos);
			std::vector<std::string> to;
			pos += 2;
			while(pos < line.size())
			{
				size_t endpos = line.find_first_of(" ",pos);
				endpos = (endpos == std::string::npos) ? line.size() : endpos;
				to.push_back(line.substr(pos,endpos-pos));
				pos = endpos+1;
			}
			map[from] = to;
		}
	}
	input.close();
}

uint64_t findPaths(const std::unordered_map<std::string,std::vector<std::string>> &map)
{
	uint64_t result = 0;
	
	std::vector<std::string> list;
	std::string from = "you";
	list.push_back(from);
	
	while(!list.empty())
	{
		from = list.back();
		list.pop_back();
		if(from == "out")
		{
			result++;
			continue;
		}
		for(unsigned i=0; i<map.at(from).size(); i++)
		{
			list.push_back(map.at(from)[i]);
		}
	}
	
	return result;
}

uint64_t countPaths(const std::unordered_map<std::string,std::vector<std::string>> &map, std::string from, std::string to, std::unordered_map<std::string,uint64_t> &cache)
{
	uint64_t result = 0;
	
	if(from == to) return 1;
	
	if(map.count(from) == 1)
	{
		for(unsigned i=0; i<map.at(from).size(); i++)
		{
			std::string step = map.at(from)[i];
			if(cache.count(step) == 0) cache[step] = countPaths(map, step, to, cache);
			result += cache[step];
		}
	}
	
	return result;
}

uint64_t fromServer(const std::unordered_map<std::string,std::vector<std::string>> &map)
{
	uint64_t result = 0;

	std::unordered_map<std::string,uint64_t> cache;
	uint64_t at_fft = countPaths(map, "svr", "fft", cache);
	cache.clear();
	uint64_t at_dac = at_fft*countPaths(map, "fft", "dac", cache);
	cache.clear();
	result = at_dac*countPaths(map, "dac", "out", cache);
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::unordered_map<std::string,std::vector<std::string>> map;
	parseInput(map);
	resultA = findPaths(map);
	resultB = fromServer(map);
	std::cout << "resultA: " << resultA << '\n' << "resultB: " << resultB << std::endl;

	return 0;
}

