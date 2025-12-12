#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

struct Tree
{
	unsigned x = 0;
	unsigned y = 0;
	std::array<unsigned,6> numbers = {0};
};

void parseInput(std::vector<std::vector<std::string>> &presents, std::vector<Tree> &trees)
{
	std::string line;
	std::ifstream input("input.txt");

	if(input.is_open())
	{
		unsigned count = 0;
		while(count < 6)
		{
			getline(input,line);
			std::vector<std::string> present;
			getline(input,line);
			present.push_back(line);
			getline(input,line);
			present.push_back(line);
			getline(input,line);
			present.push_back(line);
			presents.push_back(present);
			getline(input,line);
			count++;
		}
		while(getline(input,line))
		{
			Tree tree;
			size_t pos = 0;
			size_t endpos = line.find_first_of("x");
			tree.x = std::stoi(line.substr(pos,endpos-pos));
			pos = endpos+1;
			endpos = line.find_first_of(":",pos);
			tree.y = std::stoi(line.substr(pos,endpos-pos));
			count = 0;
			pos = endpos+2;
			while(count < 6)
			{
				endpos = pos+2;
				tree.numbers[count] = std::stoi(line.substr(pos,endpos-pos));
				pos = endpos+1;
				count++;
			}
			trees.push_back(tree);
		}
	}
	input.close();
}

uint64_t fitPresents(const std::vector<std::vector<std::string>> &presents, const std::vector<Tree> &trees)
{
	uint64_t result = 0;
	
	std::array<unsigned,6> sizes = {0};
	for(unsigned i=0; i<presents.size(); i++)
	{
		for(unsigned j=0; j<presents[i].size(); j++)
		{
			for(unsigned k=0; k<presents[i][j].size(); k++)
			{
				if(presents[i][j][k] == '#') sizes[i]++;
			}
		}
	}
	
	for(unsigned i=0; i<trees.size(); i++)
	{
		uint64_t area = trees[i].x*trees[i].y;
		uint64_t areaNeeded = 0;
		for(unsigned j=0; j<trees[i].numbers.size(); j++)
		{
			areaNeeded += trees[i].numbers[j]*sizes[j];
		}
		if(areaNeeded < area)
		{
			result++;
		}
	}
	
	return result;
}

int main()
{
	uint64_t result = 0;
	
	std::vector<std::vector<std::string>> presents;
	std::vector<Tree> trees;
	parseInput(presents, trees);
	result = fitPresents(presents, trees);
	std::cout << "result: " << result << '\n';

	return 0;
}

