#include <iostream>
#include <fstream>
#include <vector>

void parseInput(std::vector<std::pair<int64_t,int64_t>> &reds)
{
	std::string line;
	std::ifstream input("input.txt");

	if(input.is_open())
	{
		while(getline(input,line))
		{
			size_t pos = line.find_first_of(",");
			reds.emplace_back(std::stoll(line.substr(0,pos)),std::stoll(line.substr(pos+1)));
		}		
	}
	input.close();
}

uint64_t largestRectangle(const std::vector<std::pair<int64_t,int64_t>> &reds)
{
	uint64_t result = 0;
	
	for(unsigned i=0; i<reds.size()-1; i++)
	{
		for(unsigned j=i+1; j<reds.size(); j++)
		{
			uint64_t newArea = (std::max(reds[i].first,reds[j].first) - std::min(reds[i].first,reds[j].first) + 1)*(std::max(reds[i].second,reds[j].second) - std::min(reds[i].second,reds[j].second) + 1);
			result = (newArea > result) ? newArea : result;
		}	
	}
	
	return result;
}

uint64_t redGreenRectangle(const std::vector<std::pair<int64_t,int64_t>> &reds)
{
	uint64_t result = 0;
	
	for(unsigned i=0; i<reds.size()-1; i++)
	{
		for(unsigned j=i+1; j<reds.size(); j++)
		{
			uint64_t newArea = (std::max(reds[i].first,reds[j].first) - std::min(reds[i].first,reds[j].first) + 1)*(std::max(reds[i].second,reds[j].second) - std::min(reds[i].second,reds[j].second) + 1);

			if(newArea > result)
			{
				std::pair<uint64_t,uint64_t> NW = {std::min(reds[i].first,reds[j].first),std::min(reds[i].second,reds[j].second)};
				std::pair<uint64_t,uint64_t> NE = {std::max(reds[i].first,reds[j].first),std::min(reds[i].second,reds[j].second)};
				std::pair<uint64_t,uint64_t> SE = {std::max(reds[i].first,reds[j].first),std::max(reds[i].second,reds[j].second)};
				std::pair<uint64_t,uint64_t> SW = {std::min(reds[i].first,reds[j].first),std::max(reds[i].second,reds[j].second)};
				bool within = true;
				bool NWC = false;
				bool NEC = false;
				bool SEC = false;
				bool SWC = false;
				int NWP = 0;
				int NEP = 0;
				int SEP = 0;
				int SWP = 0;
				for(unsigned k=0; k<reds.size(); k++)
				{
					uint64_t minX = std::min(reds[k].first,reds[(k+1)%reds.size()].first);
					uint64_t maxX = std::max(reds[k].first,reds[(k+1)%reds.size()].first);
					uint64_t minY = std::min(reds[k].second,reds[(k+1)%reds.size()].second);
					uint64_t maxY = std::max(reds[k].second,reds[(k+1)%reds.size()].second);
					if(minX == maxX)
					{
						if((minX > NW.first && minX < NE.first && minY < NW.second && maxY > NW.second) || (minX > SW.first && minX < SE.first && minY < SW.second && maxY > SW.second))
						{
							within = false;
							break;
						}
						if(minX == NW.first && minY <= NW.second && maxY >= NW.second) NWC = true;
						if(minX == NE.first && minY <= NE.second && maxY >= NE.second) NEC = true;
						if(minX == SE.first && minY <= SE.second && maxY >= SE.second) SEC = true;
						if(minX == SW.first && minY <= SW.second && maxY >= SW.second) SWC = true;
						
						if(minX < NW.first && minY < NW.second && maxY > NW.second)
						{
							if(reds[k].second < reds[(k+1)%reds.size()].second) NWP += 2;
							else NWP -= 2;
						}
						else if(minX < NW.first && minY <= NW.second && maxY >= NW.second)
						{
							if(reds[k].second < reds[(k+1)%reds.size()].second) NWP += 1;
							else NWP -= 1;
						}
						if(minX < NE.first && minY < NE.second && maxY > NE.second)
						{
							if(reds[k].second < reds[(k+1)%reds.size()].second) NEP += 2;
							else NEP -= 2;
						}
						else if(minX < NE.first && minY <= NE.second && maxY >= NE.second)
						{
							if(reds[k].second < reds[(k+1)%reds.size()].second) NEP += 1;
							else NEP -= 1;
						}
						if(minX < SE.first && minY < SE.second && maxY > SE.second)
						{
							if(reds[k].second < reds[(k+1)%reds.size()].second) SEP += 2;
							else SEP -= 2;
						}
						else if(minX < SE.first && minY <= SE.second && maxY >= SE.second)
						{
							if(reds[k].second < reds[(k+1)%reds.size()].second) SEP += 1;
							else SEP -= 1;
						}
						if(minX < SW.first && minY < SW.second && maxY > SW.second)
						{
							if(reds[k].second < reds[(k+1)%reds.size()].second) SWP += 2;
							else SWP -= 2;
						}
						else if(minX < SW.first && minY <= SW.second && maxY >= SW.second)
						{
							if(reds[k].second < reds[(k+1)%reds.size()].second) SWP += 1;
							else SWP -= 1;
						}
					}
					else
					{
						if((minY > NW.second && minY < SW.second && minX < NW.first && maxX > NW.first) || (minY > NE.second && minY < SE.second && minX < NE.first && maxX > NE.first))
						{
							within = false;
							break;
						}
						if(minY == NW.second && minX <= NW.first && maxX >= NW.first) NWC = true;
						if(minY == NE.second && minX <= NE.first && maxX >= NE.first) NEC = true;
						if(minY == SE.second && minX <= SE.first && maxX >= SE.first) SEC = true;
						if(minY == SW.second && minX <= SW.first && maxX >= SW.first) SWC = true;
					}
				}
				if(NWP != 0) NWC = true;
				if(NEP != 0) NEC = true;
				if(SEP != 0) SEC = true;
				if(SWP != 0) SWC = true;
				if(within && NWC && NEC && SEC && SWC)
				{
					result = newArea;
				}
			}
		}	
	}
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::vector<std::pair<int64_t,int64_t>> reds;
	parseInput(reds);
	resultA = largestRectangle(reds);
	resultB = redGreenRectangle(reds);
	std::cout << "resultA: " << resultA << '\n' << "resultB: " << resultB << std::endl;

	return 0;
}

