#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

void parseInput(std::vector<std::vector<uint64_t>> &problems, std::vector<char> &operations)
{
	std::string line;
	std::ifstream input("input.txt");

	if(input.is_open())
	{
		getline(input,line);
		size_t pos = line.find_first_of("1234567890");
		while(pos != std::string::npos)
		{
			size_t endpos = line.find_first_of(" ", pos);
			endpos = (endpos == std::string::npos) ? line.size() : endpos;
			problems.push_back(std::vector<uint64_t>());
			problems.back().push_back(std::stoll(line.substr(pos,endpos-pos)));
			pos = line.find_first_of("1234567890",endpos);
		}
		getline(input,line);
		pos = line.find_first_of("1234567890");
		while(pos != std::string::npos)
		{
			unsigned row = 0;
			while(pos != std::string::npos)
			{
				size_t endpos = line.find_first_of(" ", pos);
				endpos = (endpos == std::string::npos) ? line.size() : endpos;
				problems[row].push_back(std::stoll(line.substr(pos,endpos-pos)));
				pos = line.find_first_of("1234567890",endpos);
				row++;
			}
			getline(input,line);
			pos = line.find_first_of("1234567890");
		}
		pos = 0;
		while(pos != std::string::npos)
		{
			operations.push_back(line[pos]);
			pos = line.find_first_of("+*",pos+1);
		}
		
	}
	input.close();
}

void realParse(std::vector<std::vector<uint64_t>> &problems)
{
	std::string line;
	std::ifstream input("input.txt");

	if(input.is_open())
	{
		std::vector<std::string> rawProblems;
		while(getline(input,line))
		{
			rawProblems.push_back(line);
		}
		std::string operations = std::move(rawProblems.back());
		rawProblems.pop_back();
		
		size_t pos = 0;
		while(pos < operations.size())
		{
			problems.push_back(std::vector<uint64_t>());
			size_t endpos = operations.find_first_of("+*",pos+1);
			endpos = (endpos == std::string::npos) ? operations.size() : endpos-1;
			for(unsigned j=pos; j<endpos; j++)
			{
				unsigned factor = 1;
				uint64_t num = 0;
				for(int i=rawProblems.size()-1; i>=0; i--)
				{
					if(rawProblems[i][j] != ' ')
					{
						num += (rawProblems[i][j]-'0')*factor;
						factor *= 10;
					}
				}
				problems.back().push_back(num);
			}
			pos = endpos+1;
		}
	}
	input.close();
}

uint64_t solve(const std::vector<std::vector<uint64_t>> &problems, const std::vector<char> &operations)
{
	uint64_t result = 0;
	
	for(unsigned i=0; i<problems.size(); i++)
	{
		uint64_t newResult = (operations[i] == '+') ? 0 : 1;
		for(unsigned j=0; j<problems[i].size(); j++)
		{
			if(operations[i] == '+') newResult += problems[i][j];
			else newResult *= problems[i][j];
		}
		result += newResult;
	}
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::vector<std::vector<uint64_t>> problems;
	std::vector<char> operations;
	parseInput(problems, operations);
	resultA = solve(problems, operations);
	problems.clear();
	realParse(problems);
	resultB = solve(problems, operations);
	std::cout << "resultA: " << resultA << '\n' << "resultB: " << resultB << std::endl;

	return 0;
}

