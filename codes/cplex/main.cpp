//
//  main.cpp
//  cplex
//
//  Created by Daniel on 02.11.17.
//  Copyright © 2017 Daniel Neira. All rights reserved.
//
#include <ilcplex/ilocplex.h>
#include<stdio.h>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include <ctime>

/* Globale variable */
int id;
int size;
int capacity;
int *weight,*profit;

/*Global Variables - own Cplex */
std::clock_t start;
int id_global;
int max_global;

/*Funtion declaration*/
int readFile(char*);
int printResult(int);
int CPLEX_SOLVER();

/*Para mostrar sols intermedias*/
ILOSTLBEGIN
ILOSIMPLEXCALLBACK0(MyCallback) {
    if ( isFeasible() ) {
        if (getObjValue() - floor(getObjValue()) <= 0 && getObjValue() > max_global){
            cout <<id << "\t"<< getObjValue() <<"\t" << (std::clock() - start )/(double) CLOCKS_PER_SEC << endl;
            max_global = getObjValue();
        }
    }
};

int main(int argc, char** argv){
    char *input;
    if(argc!=2) {
        printf("\nError: Invalid number of arguments!\n\n");
        return 0;
    }
    input = argv[1];
    readFile(input);
    CPLEX_SOLVER();
    
    /*Free the allocated memory*/
    free(weight);
    free(profit);
    return 0;
}

int readFile(char * filename) {
    FILE *fp;
    char num[255];
    int i=0,j=0;
    
    fp = fopen(filename,"r");
    if(fp == NULL) {
        printf("\nERROR in opening the file!\n\n");
        return 0;
    }
    char ch;
    ch=fgetc(fp);
    while(ch!='\n' && ch!=EOF) {
        //Read id;
        while(ch!=' '){
            num[j++]=ch;
            ch=fgetc(fp);
        }
        num[j]='\n';
        id = atoi(num);
        //Read size;
        ch=fgetc(fp);
        j=0;
        while(ch!=' '){
            num[j++]=ch;
            ch=fgetc(fp);
        }
        num[j]='\n';
        size = atoi(num);
        //Read capacity
        ch=fgetc(fp);
        j=0;
        while(ch!=' '){
            num[j++]=ch;
            ch=fgetc(fp);
        }
        num[j]='\n';
        capacity = atoi(num);
        // create weight and profit array
        weight = (int*) malloc(sizeof(int)*size);
        profit = (int*) malloc(sizeof(int)*size);
        ch=fgetc(fp);
        //Read the weights.
        for(i=0;i<size;i++){
            j=0;
            while(ch!=' '){
                num[j++]=ch;
                ch=fgetc(fp);
            }
            num[j]='\n';
            weight[i]=atoi(num);
            ch=fgetc(fp);
        }
        //Read the profit.
        for(i=0;i<size;i++){
            j=0;
            while(ch!=' ' && ch!=EOF){
                num[j++]=ch;
                ch=fgetc(fp);
            }
            num[j]='\n';
            profit[i]=atoi(num);
            ch=fgetc(fp);
        }
    }
    fclose(fp);
    return 0;
}

int CPLEX_SOLVER(){
    double duration;
    max_global = 0;
    //Aca empieza a cargar los datos en CPLEX
    IloEnv env;//creando ambiente
    try {
        //Iniciar cplex
        IloModel model(env);//declaracion modelo
        IloNumVarArray item(env,size,0,1,ILOBOOL);// declaracion de variables como boolean
        IloExpr expr(env);//expresion para restriccion
        //std::cout<<" id: "<<curInst.id<<std::endl;
        for (int i = 0; i<size; i++) {
            expr += weight[i]*item[i];//llenado de los pesos
            //std::cout<< curInst.items[i].first<<"* item["<<i<<"]"<<" + ";
        }
        model.add(expr <= capacity);
        expr.end();
        /*std::cout<<" <= "<<capacity<<std::endl;
         for (int i = 0; i<size; i++) {
         std::cout<< curInst.items[i].second<<"* item["<<i<<"]"<<" + ";
         }
         std::cout<<std::endl;*/
        //Ingreso a Cplex
        IloObjective ReduceCost = IloAdd(model, IloMaximize(env));
        for (int i = 0; i<size; i++){ //llenando con los costos/beneficios
            ReduceCost.setLinearCoef(item[i], profit[i]);
        }
        IloCplex Solver(model);
        Solver.setOut(env.getNullStream()); //para no mostrar el motor de Cplex
        Solver.setWarning(env.getNullStream());
        Solver.use(MyCallback(env));
        //Solver.setParam(IloCplex::TiLim, 1);// Para darle un limite de tiempo
        start = std::clock(); //start time
        if (!Solver.solve()){
            env.error() << "Failed to optimize IP." << endl;
            throw(-1);
        }
        duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
        IloNumArray vals(env);
        //env.out() << "Solution status = " << Solver.getStatus() << endl;
        //env.out() << "Solution status = " << Solver.getStatus() << endl;
        //env.out() << "Solution value = " << Solver.getObjValue() << endl;
        env.out() << id << "\t"<< Solver.getObjValue() << "\t" << duration<<endl;
        //Solver.getValues(vals, item);
        //env.out() << "Values = " << vals<< endl;
    }
    catch (IloException& e) {
        cerr << "Concert exception caught: " << e << endl;
    }
    catch (...) {
        cerr << "Unknown exception caught" << endl;
    }
    env.end();
    //fuera de cplex
    return 0;
}
