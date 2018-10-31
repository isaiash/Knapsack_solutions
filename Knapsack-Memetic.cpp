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
#include <random>

#define population_size 20
#define initial_population_size 20
#define max_num_iter 1000
#define prob_mutate 0.01
#define num_max_selection 5
#define prob_uniform_CrossOver 0.5
#define item_en_mochila 500 ///<------cambiar por cada tipo de instancias
#define num_de_corridas 10

typedef std::vector<std::pair<int, int> > ItemVector;

template<typename T>
static std::string toBinaryString(const T& x){
    std::stringstream ss;
    ss << std::bitset<sizeof(T) * 64>(x);
    return ss.str();
}

struct inst{
    ItemVector items; //peso, beneficio
    int max_weight;
    int num_items;
};
struct soln{
    std::bitset<item_en_mochila> config;
    int num_items;
    int fitness;
    double solv_time;
};
struct indiv{
    std::bitset<item_en_mochila> config;
    int fitness;
    int cost;
};

void printInst(inst &instance){
    printf("Peso maximo: %d, Numero de items: %d\n", instance.max_weight, instance.num_items);
    for (int i = 0; i < instance.num_items; i++){
        printf("Item %d: Peso=%d, Beneficio=%d\n", i, instance.items[i].first, instance.items[i].second);
    }
    printf("\n");
}

void printSoln(soln solution){
    printf("Funcion fitness: %d, tiempo: %f\n", solution.fitness, solution.solv_time);
    std::cout<< solution.config << std::endl;
}

int GetInstances(std::vector<inst> &instances, const char* filename, bool verbose){
    std::string line;
    std::ifstream infile(filename);
    //int basura;
    if (infile){
        while (std::getline(infile, line)){
            //std::cout<<line<<std::endl;
            inst newInst;
            std::istringstream iss(line);
            //iss >> basura;
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
            if (verbose){  // <--- cambiar luego
                printf("Pushed New Instance:\n");
                printInst(newInst);
            }
        }
    }
    
    return 0;
}

bool sortFunc(const std::pair<int,float>& p1, const std::pair<int,float>& p2) {
    return p1.second > p2.second;
}

int Greedy(inst &instance, indiv &individuo, bool verbose){
    //process
    int solnWeight = individuo.cost;
    int solnFittness = individuo.fitness;
    std::bitset<item_en_mochila> solnConfig = individuo.config;
    //if (verbose) std::cout<< "solnConfig antes de cambio: " << solnConfig << std::endl;
    std::pair<int, float> datos;
    std::vector<std::pair<int, float>> densidad_orden;
    for (int i = 0; i < instance.num_items; i++){//find highest CWR
        //std::cout << instance.items[i].second <<"/" << instance.items[i].first << std::endl;
        //printf("item %i beneficio %d y peso %d \n", i, instance.items[i].second, instance.items[i].first);
        datos = std::make_pair(i, (float) (instance.items[i].second / (float) instance.items[i].first));
        densidad_orden.push_back(datos);
    }
    std::sort(densidad_orden.begin(), densidad_orden.end(), sortFunc);
    for (int i = 0; i < densidad_orden.size(); i++){
        if (solnWeight + instance.items[densidad_orden[i].first].first <= instance.max_weight && !solnConfig.test(i)){
            solnWeight += instance.items[densidad_orden[i].first].first;
            solnFittness += instance.items[densidad_orden[i].first].second;
            solnConfig.set(densidad_orden[i].first);
        }
        else if(solnWeight + instance.items[densidad_orden[i].first].first > instance.max_weight || solnConfig.test(i)){
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

bool Factible(std::vector<indiv> &individuo, std::bitset<item_en_mochila> &NewConfig, inst &instancia, bool verbose){
    int Costo_Acum=0;
    int Ganancia=0;
    for (int i=0; i<instancia.num_items; i++) {
        Costo_Acum += NewConfig[i]*instancia.items[i].first;
        Ganancia += NewConfig[i]*instancia.items[i].second;
    }
    if (Costo_Acum <= instancia.max_weight) {
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

void generar_individuo_inicial(inst &instancia,std::vector<indiv> &individuo, bool verbose){
    float r = 0.3; //parametro p en distribucion bernoulli
    std::bitset<item_en_mochila> NewConfig;
    for (int i=0; i<initial_population_size; i++) {
        do{
            //r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); //r aleatorio
            std::random_device rd;
            std::mt19937 gen( rd());
            std::bernoulli_distribution d(r); //lo genera de acuerdo a una distribución bernoulli
            for( int n = 0; n <instancia.num_items; ++n) {
                NewConfig[n] = d(gen);
            }
        }while (!Factible(individuo,NewConfig,instancia,verbose));
    }
}

int Mutation(inst &instances, indiv &Offspring, bool verbose){
    if (verbose) printf("Aplicando Mutacion...\n");
    int pos =rand()%instances.num_items;
    if (Offspring.config.test(pos)) {
        Offspring.config.flip(pos);
        Offspring.fitness -= instances.items[pos].second;
        Offspring.cost -= instances.items[pos].first;
    }
    else{
        Offspring.config.flip(pos);
        Offspring.fitness += instances.items[pos].second;
        Offspring.cost += instances.items[pos].first;
        //si ahora estamos sobre el peso maximo
        if(Offspring.cost > instances.max_weight){
            while (Offspring.cost > instances.max_weight) {
                pos = rand()%instances.num_items;
                if(Offspring.config.test(pos)){
                    Offspring.cost -= instances.items[pos].first;
                    Offspring.fitness -= instances.items[pos].second;
                    Offspring.config.flip(pos);
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
        indiv NewIndiv{
            .config =Offpring1.config,
            .fitness=Offpring1.fitness,
            .cost   =Offpring1.cost
        };
        poblacion.push_back(NewIndiv);
    }
    else{
        indiv NewIndiv{
            .config =Offpring2.config,
            .fitness=Offpring2.fitness,
            .cost   =Offpring2.cost
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
int Convertir_Factible_CrossOver(indiv &Offspring, std::bitset<item_en_mochila> &NewConfig,inst &instancia){
    //Verifica Factibilidad, si no es factible, lo reconstruye eliminando objetos hasta que se hace factible
    int Costo_Acum=0;
    int Ganancia=0;
    for (int i=0; i<instancia.num_items; i++) {
        Costo_Acum += NewConfig[i]*instancia.items[i].first;
        Ganancia += NewConfig[i]*instancia.items[i].second;
    }
    if (Costo_Acum <= instancia.max_weight) {
        Offspring.config = NewConfig;
        Offspring.fitness = Ganancia;
        Offspring.cost = Costo_Acum;
    }
    else{
        int pos;
        while (Costo_Acum > instancia.max_weight) {
            pos = rand()%instancia.num_items;
            if(NewConfig.test(pos)){
                Costo_Acum -= instancia.items[pos].first;
                Ganancia -= instancia.items[pos].second;
                NewConfig.flip(pos);
            };
        }
        Offspring.config = NewConfig;
        Offspring.fitness = Ganancia;
        Offspring.cost = Costo_Acum;
    }
    return 0;
}
int CrossOver(indiv &Offspring1,indiv &Offspring2, inst &instances,std::vector<indiv> &poblacion, bool verbose){
    if (verbose) printf("Realizando CrossOver...\n");
    std::bitset<item_en_mochila> NewOffpring1, NewOffpring2;
    int Parent1,Parent2;
    Parent1 = ParentSelection(poblacion, verbose);
    Parent2 = ParentSelection(poblacion, verbose);
    while (Parent2 == Parent1) {
        Parent2 = ParentSelection(poblacion, verbose);
    }
    for (int i=0; i<instances.num_items; i++) {
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
    Convertir_Factible_CrossOver(Offspring1, NewOffpring1, instances);
    Convertir_Factible_CrossOver(Offspring2, NewOffpring2, instances);
    return 0;
}
int Memetic(std::vector<inst> &instances, std::vector<soln> &solutions, bool verbose, int max_time, bool resul_check){
    //start timer
    std::clock_t start;
    double duration;
    for (int instancia = 0 ; instancia<instances.size() ; instancia++) {
        std::cout<<"instancia: "<<instancia+1 <<" corriendo... "<<std::endl;
        start = std::clock();
        std::vector<indiv> poblacion;
        indiv Offspring1,Offspring2;
        //generar sol inicial
        generar_individuo_inicial(instances[instancia],poblacion,verbose);
        //Aplicar LS ->FDD: Greedy de densidad
        for (int i=0; i < population_size; i++) {
            Greedy(instances[instancia],poblacion[i],verbose);
        }
        int num_iter=0;
        while (num_iter < max_num_iter && (std::clock() - start) / (double) CLOCKS_PER_SEC < max_time) { //termina por iteraciones y por tiempo
            CrossOver(Offspring1,Offspring2,instances[instancia], poblacion, verbose);
            //vemos si se realiza una mutacion a los hijos obtenidos anteriormente
            if(rand()%instances[instancia].num_items < prob_mutate){
                Mutation(instances[instancia],Offspring1,verbose);
            }
            if(rand()%instances[instancia].num_items < prob_mutate){
                Mutation(instances[instancia],Offspring2,verbose);
            }
            //Aplicar LS ->FDD: Greedy de densidad para mejorar la cosa
            Greedy(instances[instancia], Offspring1, verbose);
            Greedy(instances[instancia], Offspring2, verbose);
            Selection_Offspring(poblacion,Offspring1,Offspring2,verbose);
            //se encontro mejor sol y se guarda
            //stop timer -> save time
            if (num_iter%20 == 0 || num_iter != 0) { //cada 20 iter eliminar exceso de poblacion
                Reduccion_de_poblacion(poblacion, verbose, resul_check);
            }
            //std::cout << " La mejor solucion hasta ahora: " << poblacion[0].config << std::endl;
            num_iter++;
        }
        Reduccion_de_poblacion(poblacion, verbose, resul_check);
        //termina una instancias -> se calcula tiempo y se guarda todo
        duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;
        if (verbose) printf("duración de %f segundos \n", duration);
        if (resul_check) std::cout << " La mejor solucion es " << poblacion[0].fitness << std::endl;
        if (verbose) std::cout << poblacion[0].config << std::endl;
        soln newSoln = {
            .config     = poblacion[0].config,
            .num_items     = instances[instancia].num_items,
            .fitness         = poblacion[0].fitness,
            .solv_time      = duration
        };
        solutions.push_back(newSoln);
        poblacion.clear();
        if (verbose) printSoln(newSoln);
    }
    return 0;
}
int main(int argc, const char** argv){
    std::srand(time(0));
    std::vector<int> Mejores_soluciones;
    std::vector<inst> instances;
    std::vector<soln> solutions;
    bool verbose,results_check;
    verbose = false;
    results_check = true;
    int max_time = 5; //segundos
    GetInstances(instances, argv[1], verbose);
    int num_ejecuciones=0;
    while (num_ejecuciones<num_de_corridas) {
        Memetic(instances, solutions, verbose, max_time, results_check);
        Mejores_soluciones.push_back(solutions[num_ejecuciones].fitness);
        num_ejecuciones++;
    }
    if (results_check) {
        printf("Soluciones encontradas: ");
        for (int i = 0; i<Mejores_soluciones.size(); i++) std::cout<< Mejores_soluciones[i] << " ";
    }
    std::cout<<"\n La mejor solucion de las "<< num_de_corridas << " corridas es " << *std::max_element(Mejores_soluciones.begin(), Mejores_soluciones.end()) <<std::endl;
    return 0;
}
