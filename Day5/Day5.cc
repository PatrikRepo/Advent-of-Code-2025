#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

void parseInput(std::vector<std::pair<uint64_t,uint64_t>> &ranges, std::vector<uint64_t> &ingredients)
{
	std::string line;
	std::ifstream input("input.txt");

	if(input.is_open())
	{
		getline(input,line);
		while(line != "")
		{
			size_t pos = line.find_first_of("-");
			ranges.emplace_back(std::stoll(line.substr(0,pos)),std::stoll(line.substr(pos+1)));
			getline(input,line);
		}
		while(getline(input,line))
		{
			ingredients.push_back(std::stoll(line));
		}
	}
	input.close();

	std::sort(ingredients.begin(), ingredients.end(), [](uint64_t a, uint64_t b){ return a < b;});
	std::sort(ranges.begin(), ranges.end(), [](std::pair<uint64_t,uint64_t> &a, std::pair<uint64_t,uint64_t> &b){ return a.first < b.first;});
}

uint64_t findFresh(const std::vector<std::pair<uint64_t,uint64_t>> &ranges, const std::vector<uint64_t> &ingredients)
{
	uint64_t result = 0;
	
	uint64_t rangeIndex = 0;
	for(uint64_t i=0; i<ingredients.size(); i++)
	{	
		for(uint64_t j=rangeIndex; j<ranges.size(); j++)
		{
			if(ingredients[i] >= ranges[j].first && ingredients[i] <= ranges[j].second)
			{
				rangeIndex = j;
				result++;
				break;
			}
		}
	}
	
	return result;
}

uint64_t findAllFresh(std::vector<std::pair<uint64_t,uint64_t>> &uRanges)
{
	uint64_t result = 0;

	bool overlap = true;
	while(overlap)
	{
		overlap = false;
		for(int64_t j=uRanges.size()-1; j>=0; j--)
		{

			for(uint64_t k=j+1; k<uRanges.size(); k++)
			{
				if((uRanges[j].first <= uRanges[k].first && uRanges[j].second >= uRanges[k].first) || 
						(uRanges[j].second >= uRanges[k].second && uRanges[j].first <= uRanges[k].second) ||
						(uRanges[k].first <= uRanges[j].first && uRanges[k].second >= uRanges[j].first) || 
						(uRanges[k].second >= uRanges[j].second && uRanges[k].first <= uRanges[j].second) ||
						(uRanges[j].first <= uRanges[k].first && uRanges[j].second >= uRanges[k].second) || 
						(uRanges[j].first >= uRanges[k].first && uRanges[j].second <= uRanges[k].second))
				{
					uRanges[j].first = std::min(uRanges[j].first, uRanges[k].first);
					uRanges[j].second = std::max(uRanges[j].second, uRanges[k].second);
					uRanges.erase(uRanges.begin()+k);
					k--;
					overlap = true;
				}
				if(uRanges[k].first > uRanges[j].second) break;
			}
		}
	}

	for(uint64_t i=0; i<uRanges.size(); i++)
	{
		result += uRanges[i].second - uRanges[i].first + 1;
	}
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::vector<std::pair<uint64_t,uint64_t>> ranges;
	std::vector<uint64_t> ingredients;
	parseInput(ranges, ingredients);
	resultB = findAllFresh(ranges);
	resultA = findFresh(ranges, ingredients);
	
	std::cout << "resultA: " << resultA << '\n' << "resultB: " << resultB << std::endl;

	return 0;
}

