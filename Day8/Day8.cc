#include <iostream>
#include <fstream>
#include <vector>
#include <set>

struct Node
{
	int64_t x = 0;
	int64_t y = 0;
	int64_t z = 0;
};

struct Dist
{
	uint64_t dist = 0;
	unsigned from = 0;
	unsigned to = 0;
};

struct Box
{
	int parent = -1;
	std::set<unsigned> children;
};
	
void parseInput(std::vector<Node> &nodes)
{
	std::string line;
	std::ifstream input("input.txt");

	if(input.is_open())
	{
		while(getline(input,line))
		{
			Node node;
			size_t pos = line.find_first_of(",");
			node.x = std::stoll(line.substr(0,pos));
			pos++;
			size_t pos2 = line.find_first_of(",",pos);
			node.y = std::stoll(line.substr(pos,pos2-pos));
			pos2++;
			node.z = std::stoll(line.substr(pos2));
			nodes.push_back(node);
		}		
	}
	input.close();
}

uint64_t connect(const std::vector<Node> &nodes)
{
	uint64_t result = 0;
	
	std::vector<Dist> distances;
	Dist dist;
	dist.dist = UINT64_MAX;
	distances.resize(1000,dist);
	
	for(unsigned i=0; i<nodes.size()-1; i++)
	{
		for(unsigned j=i+1; j<nodes.size(); j++)
		{
			dist.dist = ((nodes[i].x - nodes[j].x)*(nodes[i].x - nodes[j].x)) + ((nodes[i].y - nodes[j].y)*(nodes[i].y - nodes[j].y)) + ((nodes[i].z - nodes[j].z)*(nodes[i].z - nodes[j].z));
			if(dist.dist < distances.back().dist)
			{
				dist.from = i;
				dist.to = j;
				for(unsigned k=0; k<distances.size(); k++)
				{
					if(dist.dist < distances[k].dist)
					{
						distances.insert(distances.begin()+k, dist);
						distances.pop_back();
						break;
					}
				}
			}
		}
	}
	
	std::vector<Box> boxes;
	boxes.resize(nodes.size());
	
	for(unsigned i=0; i<distances.size(); i++)
	{
		int parent = distances[i].from;
		while(boxes[parent].parent != -1) parent = boxes[parent].parent;
		int otherParent = distances[i].to;
		while(boxes[otherParent].parent != -1) otherParent = boxes[otherParent].parent;
		if(parent != otherParent)
		{
			unsigned temp = std::max(parent,otherParent);
			parent = std::min(parent,otherParent);
			otherParent = temp;
			boxes[parent].children.insert(otherParent);
			for(auto oC:boxes[otherParent].children) boxes[parent].children.insert(oC);
			boxes[otherParent].children.clear();
			boxes[otherParent].parent = parent;
		}
	}

	uint64_t first = 0;
	uint64_t second = 0;
	uint64_t third = 0;
	
	for(unsigned i=0; i<boxes.size(); i++)
	{
		if(boxes[i].children.size()+1 > first)
		{
			third = second;
			second = first;
			first = boxes[i].children.size() + 1;
		}
		else if(boxes[i].children.size()+1 > second)
		{
			third = second;
			second = boxes[i].children.size() + 1;
		}
		else if(boxes[i].children.size()+1 > third)
		{
			third = boxes[i].children.size() + 1;
		}
	}
	
	result = first*second*third;
	
	return result;
}

uint64_t connectAll(const std::vector<Node> &nodes)
{
	uint64_t result = 0;
	
	std::vector<Dist> distances;
	Dist dist;
	dist.dist = UINT64_MAX;
	distances.resize(10000,dist);
	
	for(unsigned i=0; i<nodes.size()-1; i++)
	{
		for(unsigned j=i+1; j<nodes.size(); j++)
		{
			dist.dist = ((nodes[i].x - nodes[j].x)*(nodes[i].x - nodes[j].x)) + ((nodes[i].y - nodes[j].y)*(nodes[i].y - nodes[j].y)) + ((nodes[i].z - nodes[j].z)*(nodes[i].z - nodes[j].z));
			if(dist.dist < distances.back().dist)
			{
				dist.from = i;
				dist.to = j;
				for(unsigned k=0; k<distances.size(); k++)
				{
					if(dist.dist < distances[k].dist)
					{
						distances.insert(distances.begin()+k, dist);
						distances.pop_back();
						break;
					}
				}
			}
		}
	}
	
	std::vector<Box> boxes;
	boxes.resize(nodes.size());
	
	for(unsigned i=0; i<distances.size(); i++)
	{
		int parent = distances[i].from;
		while(boxes[parent].parent != -1) parent = boxes[parent].parent;
		int otherParent = distances[i].to;
		while(boxes[otherParent].parent != -1) otherParent = boxes[otherParent].parent;
		if(parent != otherParent)
		{
			unsigned temp = std::max(parent,otherParent);
			parent = std::min(parent,otherParent);
			otherParent = temp;
			boxes[parent].children.insert(otherParent);
			for(auto oC:boxes[otherParent].children) boxes[parent].children.insert(oC);
			boxes[otherParent].children.clear();
			boxes[otherParent].parent = parent;
		}
		if(boxes[parent].children.size() == nodes.size()-1)
		{
			result = nodes[distances[i].from].x*nodes[distances[i].to].x;
			break;
		}
	}


	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::vector<Node> nodes;
	parseInput(nodes);
	resultA = connect(nodes);
	resultB = connectAll(nodes);
	std::cout << "resultA: " << resultA << '\n' << "resultB: " << resultB << std::endl;

	return 0;
}

