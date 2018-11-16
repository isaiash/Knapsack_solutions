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

#define item_en_mochila 2000

typedef std::vector<std::pair<int, int> > ItemVector;

struct inst{
    ItemVector items; //weight, cost
    int id;
    int max_weight;
    int num_items;
};
struct soln{
    std::bitset<item_en_mochila> config;
    int id;
    int num_items;
    int cost;
    double time;
};

//helpers
template<typename T>
static std::string toBinaryString(const T& x){
    std::stringstream ss;
    ss << std::bitset<sizeof(T) * 64>(x);
    return ss.str();
}
void printInst(inst instance){
    printf("Id: %d, Max Weight: %d, Num Items: %d\n", instance.id, instance.max_weight, instance.num_items);
    for (int i = 0; i < instance.num_items; i++)
    {
        printf("Item %d: Weight=%d, Cost=%d\n", i, instance.items[i].first, instance.items[i].second);
    }
    printf("\n");
}

void printSoln(soln solution){
    printf("Id: %d, n: %d, cost: %d\n", solution.id, solution.num_items, solution.cost);
    std::cout << "sol: " << solution.config << std::endl;
}

int GetInstances(std::vector<inst> &instances, const char* filename, bool verbose){
    std::string line;
    std::ifstream infile(filename);
    if (infile){
        while (std::getline(infile, line)){
            inst newInst;
            std::istringstream iss(line);
            iss >> newInst.id;
            iss >> newInst.num_items;
            iss >> newInst.max_weight;
            newInst.items.resize(newInst.num_items);
            for (int i = 0; i <= newInst.num_items; i++){
                iss >> newInst.items[i].first;
            }
            for (int i = 0; i <= newInst.num_items; i++){
                iss >> newInst.items[i].second;
            }
            instances.push_back(newInst);
            if (verbose){
                printf("Pushed New Instance:\n");
                printInst(newInst);
            }
        }
    }
    return 0;
}

int DropCostBits(std::vector<inst> &instances, int numBits, bool verbose){
    if (numBits == 0) return 0;
    for (int i = 0; i < instances.size(); i++){
        if (verbose) printf("Assigning instance %d items to bins...\n", i);
        for (int j = 0; j < instances[i].num_items; j++){
            instances[i].items[j].second = instances[i].items[j].second & ~((int)std::pow(2, numBits) - 1);
        }
    }
    return 0;
}

int RoundCosts(std::vector<inst> &instances, int binSize, bool verbose){
    if (binSize == 1) return 0;
    for (int i = 0; i < instances.size(); i++){
        if (verbose) printf("Assigning instance %d items to bins...\n", i);
        for (int j = 0; j < instances[i].num_items; j++){
            int margin = binSize - (instances[i].items[j].second % binSize);
            instances[i].items[j].second += margin;
            if (verbose) printf("Rounding item %d cost up by %d\n", j, margin);
        }
    }
    return 0;
}

//algos
int Greeeeeedy(std::vector<inst> instances, std::vector<soln> &solutions, bool verbose){
    //start timer
    std::clock_t start;
    double duration;
    start = std::clock();
    //process
    while (!instances.empty()){
        inst curInst = instances.back();
        instances.pop_back();
        int solnWeight = 0;
        int solnCost = 0;
        std::bitset<item_en_mochila> solnConfig;
        for (int j = 0; j < curInst.num_items; j++){
            int topCWR = 0;
            int topCWRindex = -1;
            for (int i = 0; i < curInst.num_items; i++){//find highest CWR
                int curCWR = curInst.items[i].second / curInst.items[i].first;
                if (curCWR > topCWR && solnWeight + curInst.items[i].first <= curInst.max_weight && !solnConfig.test(i)){
                    topCWR = curCWR;
                    topCWRindex = i;
                }
            }
            if (topCWRindex != -1){//add it to the solution
                solnWeight += curInst.items[topCWRindex].first;
                solnCost += curInst.items[topCWRindex].second;
                solnConfig.set(topCWRindex);
            }
        }
        //stop timer
        duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
        if (verbose) printf("Tiempo de resolución instancia %d: %f\n", curInst.id ,duration);
        soln newSoln = {
            .config     = solnConfig,
            .id         = curInst.id,
            .num_items     = curInst.num_items,
            .cost         = solnCost,
            .time         = duration
        };
        solutions.push_back(newSoln);
        if (verbose) printSoln(newSoln);
    }
    return 0;
}

int WriteSolution(std::vector<soln> &solutions, bool verbose){
    if (verbose) printf("Escribiendo instancias..\n");
    std::ofstream myfile;
    std::string name;
    for(unsigned int i = 0; i < solutions.size(); i++){
        name = std::to_string(solutions[i].id);
        name += "_greedy.txt";
        myfile.open(name, std::fstream::in | std::fstream::out | std::fstream::app);
        if (!myfile.is_open()) {
            if (verbose) printf("Archivo no fue encontrado o no existe, creando archivo con tal nombre...\n");
            myfile.open(name,  std::fstream::in | std::fstream::out | std::fstream::trunc);
            myfile << solutions[i].id <<"\t"<<solutions[i].cost<<"\t"<<solutions[i].time<<"\t"<<std::endl;
            myfile.close();
        }
        else{
            if (verbose) printf("Archivo encontrado, comenzando grabado.\n");
            myfile << solutions[i].id <<"\t"<<solutions[i].cost<<"\t"<<solutions[i].time<<"\t"<<std::endl;
            myfile.close();
        }
    }
    return 0;
}

int WriteSolutiontoConsole(std::vector<soln> &solutions, bool verbose){
    if (verbose) printf("Mostrando instancias..\n");
    for(unsigned int i = 0; i < solutions.size(); i++){
        std::cout << solutions[i].id <<"\t"<<solutions[i].cost<<"\t"<<solutions[i].time<<"\t"<<std::endl;
    }
    return 0;
}

int main(int argc, const char** argv){
    
    std::vector<inst> instances;
    std::vector<soln> solutions;
    
    bool verbose = false;

    GetInstances(instances, argv[1], verbose);
    Greeeeeedy(instances, solutions, verbose);
    //WriteSolution(solutions,verbose);
    WriteSolutiontoConsole(solutions,verbose);
    instances.clear();
    solutions.clear();
    
    return 0;
}