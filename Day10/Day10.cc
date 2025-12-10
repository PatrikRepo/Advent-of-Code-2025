#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

void parseInput(std::vector<std::string> &lights, std::vector<std::vector<std::string>> &buttons, std::vector<std::vector<int>> &joltages)
{
	std::string line;
	std::ifstream input("input.txt");

	if(input.is_open())
	{
		while(getline(input,line))
		{
			size_t pos = 1;
			size_t endpos = line.find_first_of("]");
			lights.emplace_back(line.substr(pos,endpos-pos));
			
			buttons.push_back(std::vector<std::string>());
			pos = line.find_first_of("(",endpos);
			while(line[pos] == '(')
			{
				pos++;
				endpos = line.find_first_of(")",pos);
				std::string buttonString;
				while(pos < endpos)
				{
					buttonString += line[pos];
					pos += 2;
				}
				buttons.back().push_back(buttonString);
				pos++;
			}
			pos++;
			std::vector<int> joltage;
			while(pos < line.size())
			{
				endpos = line.find_first_of(",}",pos);
				joltage.push_back(std::stoi(line.substr(pos,endpos-pos)));
				pos = endpos+1;
			}
			joltages.push_back(joltage);
		}
	}
	input.close();
}

uint64_t turnOn(const std::string &goal, std::string current, const std::vector<std::string> &buttons, unsigned index, unsigned presses)
{
	uint64_t result = UINT64_MAX;
	
	std::string alt = current;
	for(unsigned i=0; i<buttons[index].size(); i++)
	{
		alt[buttons[index][i]-'0'] = (alt[buttons[index][i]-'0'] == '#') ? '.' : '#';
	}
	if(alt == goal)
	{
		return presses+1;
	}
	
	if(index+1 < buttons.size())
	{
		result = std::min(turnOn(goal, current, buttons, index+1, presses), turnOn(goal, alt, buttons, index+1, presses+1));
	}
	
	return result;
}

uint64_t turnOn(const std::vector<std::string> &lights, const std::vector<std::vector<std::string>> &buttons)
{
	uint64_t result = 0;
	
	for(unsigned i=0; i<lights.size(); i++)
	{
		std::string startLight = lights[i];
		for(auto &l:startLight) l = '.';
		result += turnOn(lights[i], startLight, buttons[i], 0, 0);
	}
	
	return result;
}

uint64_t setJoltages(const std::vector<std::vector<std::string>> &allButtons, const std::vector<std::vector<int>> &joltages)
{
	uint64_t result = 0;
	
	for(unsigned i=0; i<joltages.size(); i++)
	{
		std::vector<std::vector<int>> matrix;
		std::vector<int> joltage = joltages[i];
		for(unsigned j=0; j<allButtons[i].size(); j++)
		{
			std::vector<int> row;
			row.resize(joltage.size());
			for(auto &r:row) r = 0;
			for(unsigned k=0; k<allButtons[i][j].size(); k++)
			{
				row[allButtons[i][j][k]-'0'] = 1;
			}
			matrix.push_back(row);
		}
		{
			std::vector<std::vector<int>> newMatrix;
			for(unsigned j=0; j<matrix[0].size(); j++)
			{
				std::vector<int> newRow;
				for(unsigned k=0; k<matrix.size(); k++)
				{
					newRow.push_back(matrix[k][j]);
				}
				newRow.push_back(joltage[j]);
				newMatrix.push_back(newRow);
			}
			matrix = newMatrix;
		}
		
		int startRow = 0;
		std::vector<unsigned> pivotColumns;
		for(unsigned j=0; j<matrix[0].size()-1; j++)
		{
			int pivot = -1;
			for(unsigned k=startRow; k<matrix.size(); k++)
			{
				if(matrix[k][j] != 0)
				{
					pivot = k;
					break;
				}
			}
			if(pivot == -1) continue;
			pivotColumns.push_back(j);
			if(pivot != startRow)
			{
				auto temp = matrix[startRow];
				matrix[startRow] = matrix[pivot];
				matrix[pivot] = temp;
			}
		
			for(unsigned k=startRow+1; k<matrix.size(); k++)
			{
				if(matrix[k][j] != 0)
				{
					int a = matrix[startRow][j];
					int b = matrix[k][j];
					for(unsigned l=0; l<matrix[startRow].size(); l++)
					{
						matrix[k][l] = (matrix[k][l]*a) - (matrix[startRow][l]*b);
					}
				}
			}
			
			startRow++;
		}
		
		std::vector<unsigned> freeVars;
		
		for(unsigned j=0; j<matrix[0].size()-1; j++)
		{
			bool isPivot = false;
			for(unsigned k=0; k<pivotColumns.size(); k++)
			{
				if(pivotColumns[k] == j)
				{
					isPivot = true;
					break;
				}
			}
			if(!isPivot) freeVars.push_back(j);
		}
		
		uint64_t minResult = UINT64_MAX;
		unsigned limit = 0;
		for(unsigned j=0; j<joltage.size(); j++) limit = ((unsigned)joltage[j]+1 > limit) ? joltage[j]+1 : limit;
		
		std::vector<unsigned> freeVals = freeVars;
		for(auto &v:freeVals) v = 0;
		
		bool done = false;
		while(!done)
		{
			std::vector<int64_t> solution;
			solution.resize(matrix[0].size()-1);
			for(auto &n:solution) n = 0;
			for(unsigned j=0; j<freeVars.size(); j++)
			{
				solution[freeVars[j]] = freeVals[j];
			}
			
			bool valid = true;
			for(int j=matrix.size()-1; j>=0; j--)
			{
				if(j >= (int)pivotColumns.size()) continue;
				unsigned pivot = pivotColumns[j];
				int64_t val = matrix[j].back();
				for(unsigned k=pivot+1; k<matrix[j].size()-1; k++)
				{
					val -= ((int64_t)matrix[j][k])*solution[k];
				}
				if(matrix[j][pivot] == 0 || val%(matrix[j][pivot]) != 0)
				{
					valid = false;
				}
				else
				{
					solution[pivot] = val/matrix[j][pivot];
					if(solution[pivot] < 0) valid = false;
				}
				if(!valid) break;
			}
			if(valid)
			{
				uint64_t newResult = 0;
				for(unsigned j=0; j<solution.size(); j++)
				{
					newResult += solution[j];
				}
				minResult = (newResult < minResult) ? newResult : minResult;
			}
			unsigned pos = 0;
			while(pos < freeVals.size())
			{
				freeVals[pos] = (freeVals[pos]+1)%limit;
				if(freeVals[pos] == 0)
				{
					pos++;
				}
				else break;
			}
			if(pos >= freeVals.size())
			{
				result += minResult;
				done = true;
			}
		}
	}
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::vector<std::string> lights; 
	std::vector<std::vector<std::string>> buttons; 
	std::vector<std::vector<int>> joltages;
	parseInput(lights, buttons, joltages);
	resultA = turnOn(lights, buttons);
	resultB = setJoltages(buttons, joltages);
	std::cout << "resultA: " << resultA << '\n' << "resultB: " << resultB << std::endl;

	return 0;
}

