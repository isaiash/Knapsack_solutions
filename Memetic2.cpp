#include <stdio.h>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <utility>
#include <vector>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>

#define population_size 20
#define initial_population_size 20
#define max_num_iter 1000
#define prob_mutate 0.01
#define num_max_selection 5 //Se elige los num_max_selection individuos para escoger siguiente padre
#define prob_uniform_CrossOver 0.5
#define num_de_corridas 10
#define prob_bernoulli 0.3 //parametro p en distribucion bernoulli para crear individuos iniciales

/* Globale variable */

int id=0;
int size=0;
int capacity=0;
int *weight,*profit;
int best_fitness = -1; //para ver si va cambiando el mejor fitness e ir grabandolo

typedef std::vector< std::pair<int, int> > ItemVector;
struct soln{
    std::vector<bool> config;
    int id;
    int num_items;
    int fitness;
    double solv_time;
};
struct indiv{
    std::vector<bool> config;
    int fitness;
    int cost;
};

bool sortFunc(const std::pair<int,float>& p1, const std::pair<int,float>& p2) {
    return p1.second > p2.second;
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

int Greedy(indiv &individuo, bool verbose){
    //process
    int solnWeight = individuo.cost;
    int solnFittness = individuo.fitness;
    std::vector<bool> solnConfig = individuo.config;
    //if (verbose) std::cout<< "solnConfig antes de cambio: " << solnConfig << std::endl;
    std::pair<int, float> datos;
    std::vector< std::pair<int, float> > densidad_orden;
    for (int i = 0; i < size; i++){//find highest CWR
        //std::cout << instance.items[i].second <<"/" << instance.items[i].first << std::endl;
        //printf("item %i beneficio %d y peso %d \n", i, instance.items[i].second, instance.items[i].first);
        datos = std::make_pair(i, (float) (profit[i]/ (float) weight[i]));
        densidad_orden.push_back(datos);
    }
    std::sort(densidad_orden.begin(), densidad_orden.end(), sortFunc);
    for (int i = 0; i < densidad_orden.size(); i++){
        if (solnWeight + weight[densidad_orden[i].first]<= capacity && !solnConfig[i]){
            solnWeight += weight[densidad_orden[i].first];
            solnFittness += profit[densidad_orden[i].first];
            solnConfig[densidad_orden[i].first] = true;
        }
        else if(solnWeight + weight[densidad_orden[i].first] > capacity || solnConfig[i]){
            break;
        }
    }
    //if (verbose) std::cout<< "solnConfig despues de cambio: " <<solnConfig << std::endl;
    if (solnConfig != individuo.config) {
        individuo.config = solnConfig;
        individuo.cost = solnWeight;
        individuo.fitness = solnFittness;
    }
    densidad_orden.clear();
    return 0;
}

bool Factible(std::vector<indiv> &individuo, std::vector<bool> &NewConfig, bool verbose){
    int Costo_Acum=0;
    int Ganancia=0;
    for (int i=0; i<size; i++) {
        Costo_Acum += NewConfig[i]*weight[i];
        Ganancia += NewConfig[i]*profit[i];
    }
    if (Costo_Acum <= capacity) {
        indiv NewIndivido = {
            .config = NewConfig,
            .fitness = Ganancia
        };
        //if (verbose) std::cout<< NewConfig <<std::endl;
        individuo.push_back(NewIndivido);
        return true;
    }
    else{
        return false;
    }
}

void generar_individuo_inicial(std::vector<indiv> &individuo, bool verbose){
    float r = prob_bernoulli;
    std::vector<bool> NewConfig;
    for (int i=0; i<initial_population_size; i++) {
        do{
            //r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); //r aleatorio
            std::random_device rd;
            std::mt19937 gen( rd());
            std::bernoulli_distribution d(r); //lo genera de acuerdo a una distribución bernoulli
            for( int n = 0; n <size; ++n) {
                NewConfig.push_back(d(gen));
            }
        }while (!Factible(individuo,NewConfig,verbose));
    }
}

int Mutation(indiv &Offspring, bool verbose){
    if (verbose) printf("Aplicando Mutacion...\n");
    int pos =rand()%size;
    if (Offspring.config[pos]) {
        Offspring.config[pos] = false;
        Offspring.fitness -= profit[pos];
        Offspring.cost -= weight[pos];
    }
    else{
        Offspring.config[pos] = true;
        Offspring.fitness += profit[pos];
        Offspring.cost += weight[pos];
        //si ahora estamos sobre el peso maximo
        if(Offspring.cost > capacity){
            while (Offspring.cost > capacity) {
                pos = rand()%size;
                if(Offspring.config[pos]){
                    Offspring.cost -= weight[pos];
                    Offspring.fitness -= profit[pos];
                    Offspring.config[pos] = false;
                };
            }
        }
    }
    return 0;
}

bool sort_Poblacion(const indiv& p1, const indiv& p2) {
    return p1.fitness > p2.fitness;
}

int Selection_Offspring(std::vector<indiv> &poblacion, indiv &Offpring1, indiv &Offpring2, bool verbose){
    if (verbose) printf("Eligiendo individuo desde hijos...\n");
    if (Offpring1.fitness >= Offpring2.fitness) {
        indiv NewIndiv = {
            .config     =   Offpring1.config,
            .fitness    =   Offpring1.fitness,
            .cost       =   Offpring1.cost
        };
        poblacion.push_back(NewIndiv);
    }
    else{
        indiv NewIndiv = {
            .config     =   Offpring2.config,
            .fitness    =   Offpring2.fitness,
            .cost       =   Offpring2.cost
        };
        poblacion.push_back(NewIndiv);
    }
    return 0;
}

int Reduccion_de_poblacion(std::vector<indiv> &poblacion, bool verbose, bool result_check){
    if (verbose) printf("Eligiendo mejores individuos...\n");
    std::sort(poblacion.begin(), poblacion.end(), sort_Poblacion);
    //eliminar los que que viene despues del este.
    poblacion.erase(poblacion.begin()+20,poblacion.end());
    if (result_check) printf("Solucion actual : %d \n", poblacion[0].fitness);
    return 0;
}

int ParentSelection(std::vector<indiv> &poblacion, bool verbose){
    //Tournament
    if (verbose) printf("Eligiendo padres para CrossOver...\n");
    std::vector<int> selection;
    for(int i = 0 ;i<poblacion.size();i++) selection.push_back(i);
    std::random_shuffle(selection.begin(), selection.end());
    int max_fittness = -1;
    int argumin = -1;
    for (int i=0; i<num_max_selection; i++) {
        if (max_fittness < poblacion[selection[i]].fitness) {
            max_fittness = poblacion[selection[i]].fitness;
            argumin = selection[i];
        }
    }
    selection.clear();
    return argumin;
}
int Convertir_Factible_CrossOver(indiv &Offspring, std::vector<bool> &NewConfig){
    //Verifica Factibilidad, si no es factible, lo reconstruye eliminando objetos hasta que se hace factible
    int Costo_Acum=0;
    int Ganancia=0;
    for (int i=0; i<size; i++) {
        Costo_Acum += NewConfig[i]*weight[i];
        Ganancia += NewConfig[i]*profit[i];
    }
    if (Costo_Acum <= capacity) {
        Offspring.config = NewConfig;
        Offspring.fitness = Ganancia;
        Offspring.cost = Costo_Acum;
    }
    else{
        int pos;
        while (Costo_Acum > capacity) {
            pos = rand()%size;
            if(NewConfig[pos]){
                Costo_Acum -= weight[pos];
                Ganancia -= profit[pos];
                NewConfig[pos] = false;
            };
        }
        Offspring.config = NewConfig;
        Offspring.fitness = Ganancia;
        Offspring.cost = Costo_Acum;
    }
    return 0;
}
int CrossOver(indiv &Offspring1,indiv &Offspring2,std::vector<indiv> &poblacion, bool verbose){
    if (verbose) printf("Realizando CrossOver...\n");
    std::vector<bool> NewOffpring1, NewOffpring2;
    int Parent1,Parent2;
    Parent1 = ParentSelection(poblacion, verbose);
    Parent2 = ParentSelection(poblacion, verbose);
    while (Parent2 == Parent1) {
        Parent2 = ParentSelection(poblacion, verbose);
    }
    for (int i=0; i<size; i++) {
        if( static_cast <float> (rand()) / static_cast <float> (RAND_MAX) <= prob_uniform_CrossOver){
            NewOffpring1[i] = poblacion[Parent1].config[i];
            NewOffpring2[i] = poblacion[Parent2].config[i];
        }
        else{
            NewOffpring2[i] = poblacion[Parent1].config[i];
            NewOffpring1[i] = poblacion[Parent2].config[i];
        }
    }
    //lo paso para ver si es factible, si no, de forma aleatoria se sacan objetos hasta que se convierte en factible y se acepta
    Convertir_Factible_CrossOver(Offspring1, NewOffpring1);
    Convertir_Factible_CrossOver(Offspring2, NewOffpring2);
    return 0;
}


int Memetic(std::vector<soln> &solutions, bool verbose, int max_time, bool resul_check){
    int result; 
    std::clock_t start; //start timer
    double duration;
    std::vector<indiv> poblacion;
    indiv Offspring1,Offspring2;
    start = std::clock();
    //generar sol inicial
    generar_individuo_inicial(poblacion,verbose);
    //Aplicar LS ->FDD: Greedy de densidad
    for (int i=0; i < population_size; i++) {
        Greedy(poblacion[i],verbose);
    }
    int num_iter=0;
    while (num_iter < max_num_iter && (std::clock() - start) / (double) CLOCKS_PER_SEC < max_time) { //termina por iteraciones y por tiempo
        CrossOver(Offspring1,Offspring2, poblacion, verbose);
        //vemos si se realiza una mutacion a los hijos obtenidos anteriormente
        if(rand()%size < prob_mutate){
            Mutation(Offspring1,verbose);
        }
        if(rand()%size < prob_mutate){
            Mutation(Offspring2,verbose);
        }
        //Aplicar LS ->FDD: Greedy de densidad para mejorar la cosa
        Greedy(Offspring1, verbose);
        Greedy(Offspring2, verbose);
        Selection_Offspring(poblacion,Offspring1,Offspring2,verbose);
        //se encontro mejor sol y se guarda
        //stop timer -> save time
        if (num_iter%20 == 0 || num_iter != 0) { //cada 20 iter eliminar exceso de poblacion y calcular mejor
            Reduccion_de_poblacion(poblacion, verbose, resul_check);
            if  (poblacion[0].fitness > best_fitness){
                result = poblacion[0].fitness;
                printf("%d %d %f\n", id, result, duration);
            }
        }
        num_iter++;
    }
    Reduccion_de_poblacion(poblacion, verbose, resul_check); //termina una instancias -> se calcula tiempo y se guarda todo si es mejor
    duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    if (verbose) printf("duración de %f segundos \n", duration);
    if (poblacion[0].fitness > best_fitness){
        result = poblacion[0].fitness;
        printf("%d %d %f\n", id, result, duration);
    }
    poblacion.clear();
    return 0;
}

int main(int argc, char** argv){
    char *input;
    if(argc!=2) {
        printf("\nError: Invalid number of arguments!\n\n");
        return 0;
    }
    input = argv[1];
    readFile(input);
    
    std::srand(time(0));
    std::vector<soln> solutions;
    std::clock_t  global_time = std::clock();
    bool verbose,results_check;
    verbose = false;
    results_check = true;
    int max_time = 25; //segundos maximos por cada corrida
    int num_ejecuciones = 0;
    int max_fitness = -1; //
    int cont_fitness_rep = 0; //veces que se repite el maximo fitness*/
    while (((std::clock() - global_time) / (double) CLOCKS_PER_SEC < 1) 
            && cont_fitness_rep<=50) { //Se ejecuta por una hora y mientras el max fitness no se repita mas de 30 veces, 
        //agregar (num_ejecuciones<num_de_corridas) para que tenga un numero de corridas definido
        Memetic(solutions, verbose, max_time, results_check);
        num_ejecuciones++;
        if (max_fitness == solutions[-1].fitness){
            cont_fitness_rep++;
        }
        else{
            max_fitness = solutions[-1].fitness;
        }
    }
    return 0;
}