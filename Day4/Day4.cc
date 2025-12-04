#include <iostream>
#include <fstream>
#include <vector>

void parseInput(std::vector<std::string> &map)
{
	std::string line;
	std::ifstream input("input.txt");

	if(input.is_open())
	{
		while(getline(input,line))
		{
			map.push_back(line);
		}
	}
	input.close();

}

std::pair<uint64_t,uint64_t> removeRolls(std::vector<std::string> &map)
{
	std::pair<uint64_t,uint64_t> result = {0,0};
	
	bool first = false;
	bool done = false;
	while(!done)
	{
		done = true;
		auto newMap = map;
		for(unsigned i=0; i<map.size(); i++)
		{
			for(unsigned j=0; j<map[0].size(); j++)
			{
				if(map[i][j] == '@')
				{
					unsigned nextTo = 0;
					
					if(i>0 && map[i-1][j] == '@') nextTo++;
					if(i>0 && j+1<map[0].size() && map[i-1][j+1] == '@') nextTo++;
					if(j+1<map[0].size() && map[i][j+1] == '@') nextTo++;
					if(i+1<map.size() && j+1<map[0].size() && map[i+1][j+1] == '@') nextTo++;
					if(i+1<map.size() && map[i+1][j] == '@') nextTo++;
					if(i+1<map.size() && j>0 && map[i+1][j-1] == '@') nextTo++;
					if(j>0 && map[i][j-1] == '@') nextTo++;
					if(i>0 && j>0 && map[i-1][j-1] == '@') nextTo++;
					if(nextTo < 4)
					{
						done = false;
						if(!first) result.first++;
						result.second++;
						newMap[i][j] = '.';
					}
				}
			}
		}
		first = true;
		map = newMap;
	}
	
	return result;
}

int main()
{
	std::pair<uint64_t,uint64_t> result;

	std::vector<std::string> map;
	parseInput(map);
	result = removeRolls(map);
	
	std::cout << "resultA: " << result.first << '\n' << "resultB: " << result.second << std::endl;

	return 0;
}

