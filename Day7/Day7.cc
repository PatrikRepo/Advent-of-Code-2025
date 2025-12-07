#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <algorithm>

struct Node
{
	unsigned x = 0;
	unsigned y = 0;
	uint64_t paths = 1;
};
	
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

uint64_t split(const std::vector<std::string> &map, bool quantum)
{
	uint64_t result = 0;
	
	Node node;
	for(unsigned i=0; i<map[0].size(); i++)
	{
		if(map[0][i] == 'S')
		{
			node.x = i;
			break;
		}
	}
	
	while(node.y < map.size() && map[node.y][node.x] != '^') node.y++;
	if(node.y == map.size()) return 0;
		
	std::deque<Node> openList;
	openList.push_back(node);
	
	while(!openList.empty())
	{
		node = openList.front();
		openList.pop_front();
		if(!quantum) result++;
		
		Node lNode = node;
		lNode.x--;
		Node rNode = node;
		rNode.x++;
		
		while(lNode.y < map.size() && map[lNode.y][lNode.x] != '^') lNode.y++;
		if(lNode.y < map.size())
		{
			auto it = std::find_if(openList.begin(), openList.end(), [&lNode](Node &oNode){ return lNode.x == oNode.x && lNode.y == oNode.y;});
			if(it == std::end(openList)) openList.push_back(lNode);
			else it->paths += lNode.paths;
		}
		else if(quantum) result += lNode.paths;
		while(rNode.y < map.size() && map[rNode.y][rNode.x] != '^') rNode.y++;
		if(rNode.y < map.size())
		{
			auto it = std::find_if(openList.begin(), openList.end(), [&rNode](Node &oNode){ return rNode.x == oNode.x && rNode.y == oNode.y;});
			if(it == std::end(openList)) openList.push_back(rNode);
			else it->paths += rNode.paths;
		}
		else if(quantum) result += rNode.paths;
		
		std::sort(openList.begin(), openList.end(), [](Node &a, Node &b){return a.y < b.y;});
	}
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::vector<std::string> map;
	parseInput(map);
	resultA = split(map, false);
	resultB = split(map, true);
	std::cout << "resultA: " << resultA << '\n' << "resultB: " << resultB << std::endl;

	return 0;
}

