#include <ilcplex/ilocplex.h>
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

ILOSTLBEGIN
typedef std::vector<std::pair<int, int> > ItemVector;

struct inst{
    ItemVector items; //weight, cost
    int id;
    int max_weight;
    int num_items;
};
struct soln{
    uint64_t config;
    int id;
    int num_items;
    int cost;
};

void printInst(inst instance){
    printf("Id: %d, Max Weight: %d, Num Items: %d\n", instance.id, instance.max_weight, instance.num_items);
    for (int i = 0; i < instance.num_items; i++)
    {
        printf("Item %d: Weight=%d, Cost=%d\n", i, instance.items[i].first, instance.items[i].second);
    }
    printf("\n");
}

void printSoln(soln solution){
    printf("Id: %d, n: %d, cost: %d, config: %lld\n", solution.id, solution.num_items, solution.cost, solution.config);
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

int CPLEX_SOLVER(std::vector<inst> instances, std::vector<soln> &solutions, bool verbose){
    
    //process
    while (!instances.empty()){
        inst curInst = instances.back();
        instances.pop_back();
        std::bitset<40> solnConfig;
        //start timer
        std::clock_t start;
        double duration;
        int solnCost=-1;
        //Aca empieza a cargar los datos en CPLEX
        IloEnv env;//creando ambiente
        try {
            //Iniciar cplex
            IloModel model(env);//declaracion modelo
            IloNumVarArray item(env,curInst.num_items,0,1,ILOBOOL);// declaracion de variables como boolean
            IloExpr expr(env);//expresion para restriccion
            //std::cout<<" id: "<<curInst.id<<std::endl;
            for (int i = 0; i<curInst.num_items; i++) {
                expr += curInst.items[i].first*item[i];//llenado de los pesos
                //std::cout<< curInst.items[i].first<<"* item["<<i<<"]"<<" + ";
            }
            model.add(expr<=curInst.max_weight);
            expr.end();
            /*std::cout<<" <= "<<curInst.max_weight<<std::endl;
            for (int i = 0; i<curInst.num_items; i++) {
                std::cout<< curInst.items[i].second<<"* item["<<i<<"]"<<" + ";
            }
            std::cout<<std::endl;*/
            
            //Ingreso a Cplex
            IloObjective ReduceCost = IloAdd(model, IloMaximize(env));
            for (int i = 0; i<curInst.num_items; i++){ //llenando con los costos/beneficios
                ReduceCost.setLinearCoef(item[i], curInst.items[i].second);
            }
            IloCplex Solver(model);
            //Solver.setParam(IloCplex::TiLim, 1);// Para darle un limite de tiempo
            start = std::clock();
            if (!Solver.solve()) {
                env.error() << "Failed to optimize LP." << endl;
                throw(-1);
            }
            duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
            printf("%f\n", duration);
            IloNumArray vals(env);
            env.out() << "Solution status = " << Solver.getStatus() << endl;
            solnCost = (int) Solver.getStatus();
            env.out() << "Solution value = " << Solver.getObjValue() << endl;
            Solver.getValues(vals, item);
            env.out() << "Values = " << vals<< endl;
            for (int i = 0; i<curInst.num_items; i++){ //llenando con los costos/beneficios
                solnConfig[i] = Solver.getValue(item[i]);
            }
        }
        catch (IloException& e) {
            cerr << "Concert exception caught: " << e << endl;
        }
        catch (...) {
            cerr << "Unknown exception caught" << endl;
        }
        env.end();
        //fuera de cplex
        
        soln newSoln = {
            .config     = solnConfig.to_ullong(),
            .id         = curInst.id,
            .num_items    = curInst.num_items,
            .cost         = solnCost
        };
        solutions.push_back(newSoln);
        if (verbose) printSoln(newSoln);
    }
    return 0;
}

int main(int argc, const char** argv){
    
    std::vector<inst> instances;
    std::vector<soln> solutions;
    bool verbose = false;
    printf("Comienza resolución con Cplex")
    GetInstances(instances, argv[1], verbose);
    CPLEX_SOLVER(instances, solutions, verbose);
    
    instances.clear();
    solutions.clear();
    return 0;
}
