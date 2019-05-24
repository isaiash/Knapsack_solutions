//Ahnaf Raihan
//BestFirstSearch - main.cpp

/* main.cpp contains methods: void knapsack3, float bound, bool comparePW, and int main. main.cpp serves to demonstrate the use of the Best-First Search algorithm in order to solve the 0-1 knapsack problem. */

#include <stdio.h>
#include <iostream>
#include <queue>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include<ctime>

#define SAMPLE 0.0001

using namespace std;

vector <int> optimalSolution; // initialize a global vector used to keep track of optimal solutions


/* Globale variable */
int id;
int n;
int W;
int *w,*p,*selected;

/* create a node structure that will be used to create our tree. Each node holds an integer to designate its current level, current profit, current weight, a float to identify its upper bound, and a vector k to keep track of its individual optimal solution state. We also implement an operator overload in order to compare nodes, specifically used by the priority queue. This operator compares nodes based on their bounds. */

struct node {
    int level;
    int profit;
    int weight;
    float bound;
    vector <int> k;
    
    bool operator<(const node &rhs) const{
        return (bound < rhs.bound);
    }
};

/*create a structure sorts used when sorting items in the knapsack by their profit-to-weight ratios. Sorts has a float for its price-to-weight ratio, and an index to designate its original location before sorting. */

struct sorts {
    float pw;
    int index;
};

//function comparePW takes two sorts and returns the sorts with greater pw
bool comparePW (sorts one, sorts two) {
    return one.pw > two.pw;
}

//initialize functions
void knapsack3(int n, int p[], int w[], int W, int maxprofit, std::clock_t start);
float bound(int n, int p[], int w[], int W, node u);
int readFile(char*);

/* int main serves as the driver for BestFirstSearch and takes in 2 files from the terminal: <input.txt> <output.txt>. It then identifies the number of items n and the max weight W from file input. It sets maxprofit to 0 by default and then creates and fills arrays p and w with the profits and weights of each item from file input. It then creates an array s of sorts, and initializes the price/weight and index of each item. After initializing the array, it sorts the sorts by pw using the sort function along with our comparePW function. It then utilizes the sorted sorts array to identify the correct decresing order of p/w of our items, and creates new arrays pSorted and wSorted to hold the new, ordered item weights and profits. Main then calls the knapsack3 to find the optimal solution to our problem with the given input*/

int main(int argc, char ** argv) {
    //int n = 0; //initialize number of items n
    //int W = 0; //initialize max weight W
    int maxprofit = 0; //initialize maximum profit to 0
    ifstream infile (argv[1]); //create an ifstream to parse our input file
    string line; //string to hold each line
    string buf; //string to hold each number
    
    char *input;
	//if(argc!=2) {
	//	printf("\nError: Invalid number of arguments!\n\n");
	//	return 0;
	//}
	input = argv[1];
	readFile(input);
    
    std::clock_t start = clock();
	
    sorts s[n]; //create array of sorts of size n items
    
    for (int i = 0; i < n; i++) { //populate array of sorts with respective price/weight's and index's
        s[i].pw = (float)p[i]/(float)w[i];
        s[i].index = i;
    }
    
    sort (s, s + n, comparePW); //sort array of sorts by pw
    
    int pSorted[n]; //create array to hold sorted profits
    int wSorted[n]; //create array to hold sorted weights
    
    for (int i = 0; i < n; i++) { //populate sorted profit and weight arrays with knowedge of p/w order from sorts
        pSorted[i] = p[s[i].index];
        wSorted[i] = w[s[i].index];
    }
    
    //call our knapsack 3 function
    knapsack3(n, pSorted, wSorted, W, maxprofit, (clock() - start ) / (double) CLOCKS_PER_SEC);
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
		n = atoi(num);
		//Read capacity
		ch=fgetc(fp);
		j=0;
		while(ch!=' '){
			num[j++]=ch;
			ch=fgetc(fp);
		}
		num[j]='\n';
		W = atoi(num);
		// create weight and profit array
		w = (int*) malloc(sizeof(int)*n);
		p = (int*) malloc(sizeof(int)*n);
		selected = (int*) malloc(sizeof(int)*n);
		ch=fgetc(fp);
		//Read the weights.
		for(i=0;i<n;i++){
			j=0;
			while(ch!=' '){
				num[j++]=ch;
				ch=fgetc(fp);
			}
			num[j]='\n';
			w[i]=atoi(num);
			ch=fgetc(fp);
		}
		//Read the profit.
		for(i=0;i<n;i++){
			j=0;
			while(ch!=' ' && ch!=EOF){
				num[j++]=ch;
				ch=fgetc(fp);
			}
			num[j]='\n';
			p[i]=atoi(num);
			ch=fgetc(fp);
		}	
	}	
	fclose(fp);
	return 0;
}


/* knapsack3 serves as a function to solve our knapsack problem in which we are given a set of items, each with a weight and a profit, and we must determine the number of each item to include in a collection so that the total weight is less than or equal to a given maximum weight W and the total value is as large as possible. Knapsack3 specifically uses the Branch & Bound Best-First search method to solve this problem. Knapsack3 uses a priority queue to organize nodes by their upper bounds, and calls for the help of our bound function to do so. knapsack3 uses an algorithm similar to Breadth-First Search, in which we have a root node v, and continue to explore neighboring nodes first, before continuing to further levels. Knapsack3 optimizes this method by identifying promising, unexplored nodes and simply expanding the one with the most promising/best bound. knapsack3 takes an integer for number of items n,  profit and weight arrays - assumed sorted, a maximum weight W greater than or equal to 0, and a maximum profit maxprofit. */

void knapsack3(int n, int p[], int w[], int W, int maxprofit, clock_t init_start) {
    int optSize = 0; //initialize current optimal size 0
    int numNodes = 1; //initalize current number of nodes visited to 1, to account for root node
    int numLeaves = 0; //initalize current number of leaf nodes visited to 0

    clock_t last, interm;
    double duration;

    last=clock();
    
    //initialize priority queue of nodes Q
    priority_queue<node>Q;
    
    //initialize nodes u and v, with v as root
    node u, v;
    
    //initialize Q to be empty
    v.level = 0; v.profit = 0; v.weight = 0; v.k.resize(n+1, 0); u.k.resize(n+1,0);
    maxprofit = 0; //set current maximum profit to 0
    
    v.bound = bound(n, p, w, W, v);
    Q.push(v); //enqueue v
    //remove node with best bound

    while(!Q.empty()){
        
        v= Q.top();
        Q.pop();
        //expand v to see if it is promising
        if(v.bound > maxprofit) {
            numNodes += 2; //increment number of total nodes visited
            u.level = v.level + 1; // child of v
            //"yes" child
            
            u.weight = v.weight + w[u.level-1];
            u.profit = v.profit + p[u.level-1];
            u.k = v.k; //pass down optimization history to child
            u.k.at(u.level-1) = 1; //designate part of personal solution
            if(u.weight <= W && u.profit > maxprofit) {
                maxprofit = u.profit;
                interm=clock();
                duration = ( interm - last) / (double) CLOCKS_PER_SEC;
                if(duration>=SAMPLE){
                    cout << id << " " << maxprofit << " " << ( interm - init_start ) / (double) CLOCKS_PER_SEC << endl;
                    last = interm;
                }
                optimalSolution = u.k; //add to global optimal solution
            }
            
            //find the upper bound of our child u
            u.bound = bound(n, p, w, W, u);
            if(u.bound > maxprofit  && u.weight < W) {
                Q.push(u);
            }
            else
                numLeaves++;
            //"no" child
            
            u.weight = v.weight;
            u.profit = v.profit;
            u.k.at(u.level - 1) = 0;
            u.bound = bound(n, p, w, W, u);
            //node chosen to be expanded
            if(u.bound > maxprofit  && u.weight < W) {
                Q.push(u);
            }
            else
                numLeaves++;
            //node not expanded
        }//queue is empty
        else
            numLeaves++; //increment number of leaf nodes visited
    }
    
    //calculate size of our optimal solution from optimalSolution global variable
    //for (int i = 0; i < optimalSolution.size(); i++) {
    //    if (optimalSolution.at(i) == 1)
    //        optSize++;
    //}
    cout << id << " " << maxprofit << " " << ( clock() - init_start ) / (double) CLOCKS_PER_SEC << endl;
}


/* Our bound function is used to calculate the upper bound of a node given a set of items with weights w and profits p. The upper bound gives us a heuristic to judge nodes upon, in which we designate the most promising node by the node with the best upper bound in the current circumstances. bound takes an integer n for total number of items, integer arrays p and w for profits and weights - assumed sorted, an integer W for max weight, and a node u designating our current node being examined. bound returns a float value for the upper bound of u */

float bound(int n, int p[], int w[], int W, node u) {
    int j, k;
    int tw; //integer to hold total weight
    float bound; // upper bound return value
    
    if(u.weight >= W) //over maximum weight
        return 0;
    else {
        bound = (float)u.profit;
        j = u.level;
        tw = u.weight;
        while(j <= n && tw + w[j] <= W) { //add items
            tw = tw + w[j];
            bound = bound + p[j];
            ++j;
        }
        k = j;
        if(k < n) // take fraction of item
            bound = bound + (W - tw) * p[k]/w[k];
        return bound;
    }
}
