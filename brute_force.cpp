#include <stdio.h>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <utility>
#include <vector>
#include <bitset>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cassert>

typedef std::vector<std::pair<int, int> > ItemVector;

struct inst{
	ItemVector items; //weight, cost
	int max_weight;
	int num_items;
};
struct soln{
	uint64_t config;
	int num_items;
	int cost;
};

int bbTopCost;
std::vector<int> bbTopConfig;

//helpers
template<typename T>
static std::string toBinaryString(const T& x)
{
    std::stringstream ss;
    ss << std::bitset<sizeof(T) * 64>(x);
    return ss.str();
}
template<typename T>
void pop_front(std::vector<T>& vec)
{
    assert(!vec.empty());
    vec.front() = std::move(vec.back());
    vec.pop_back();
}
void printInst(inst instance)
{
	printf("Max Weight: %d, Num Items: %d\n", instance.max_weight, instance.num_items);
	for (int i = 0; i < instance.num_items; i++){
		printf("Item %d: Weight=%d, Cost=%d\n", i, instance.items[i].first, instance.items[i].second);
	}
	printf("\n");
}

void printSoln(soln solution){
	//printf("n: %d, cost: %d, config: %lld\n", solution.num_items, solution.cost, solution.config);
	printf("%lld ", solution.config);
	//std::cout << "sol: " << toBinaryString(solution.config) << std::endl;
}

int GetInstances(std::vector<inst> &instances, const char* filename) {
	std::string line;
  	std::ifstream infile(filename);

  	if (infile){
	  	while (std::getline(infile, line)){
  			inst newInst;
	  		std::istringstream iss(line);

	  		iss >> newInst.num_items;
	  		iss >> newInst.max_weight;
	  		newInst.items.resize(newInst.num_items);

	  		for (int i = 0; i <= newInst.num_items-1; i++){
	  			iss >> newInst.items[i].first;
	  		}
	  		for (int i = 0; i <= newInst.num_items-1; i++){
	  		    iss >> newInst.items[i].second;
	  		}
	  		instances.push_back(newInst);
	  		//printInst(newInst);
	  	}
  	}

  	return 0;
}

int DropCostBits(std::vector<inst> &instances, int numBits){
	if (numBits == 0) return 0;
	for (int i = 0; i < instances.size(); i++){
		for (int j = 0; j < instances[i].num_items; j++){
			instances[i].items[j].second = instances[i].items[j].second & ~((int)std::pow(2, numBits) - 1);
		}
	}
	return 0;
}

int RoundCosts(std::vector<inst> &instances, int binSize){
	if (binSize == 1) return 0;
	for (int i = 0; i < instances.size(); i++){
		for (int j = 0; j < instances[i].num_items; j++){
			int margin = binSize - (instances[i].items[j].second % binSize);
			instances[i].items[j].second += margin;
		}
	}
	return 0;
}

int Bruteforce(std::vector<inst> instances, std::vector<soln> &solutions){
	std::clock_t start;
    double duration;

	while (!instances.empty()){
	    start = std::clock();
		inst curInst = instances.front();
		pop_front(instances);

		int topCost = 0;
		uint64_t topConfig = 0;

		for (uint64_t i = 0; i < std::pow(2, curInst.num_items); i++){
			int curCost = 0;
			int curWeight = 0;
			std::bitset<64> curConfig(i);

			for (int j = 0; j <= curInst.num_items; j++)
			{
				if (curConfig[j])
				{
					curCost += curInst.items[j].second;
					curWeight += curInst.items[j].first;
					
				}
			}
			if (curCost > topCost && curWeight <= curInst.max_weight)
			{
				//record potential solution
				topCost = curCost;
				topConfig = i;
			}
		}

		soln newSoln = {
			.config 	= topConfig,
			.num_items 	= curInst.num_items,
			.cost 		= topCost
		};
	    
		solutions.push_back(newSoln);
		printSoln(newSoln);
		
		//stop timer
	    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	    printf("%f\n", duration);
	}

	return 0;
}

int main(int argc, const char** argv) {

	std::vector<inst> instances;
	std::vector<soln> solutions;

	GetInstances(instances, argv[1]);

	printf("Running bruteforce algorithm...\n");
	Bruteforce(instances, solutions);

	instances.clear();
	solutions.clear();
	bbTopConfig.clear();

	return 0;
}
