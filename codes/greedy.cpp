#include<iostream>
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
double *r;

/*Funtion declaration*/
int readFile(char*);
int printResult(int);
int greedy();

int main(int argc, char ** argv){
	char *input;
	int i,totalProfit=0,totalWeight=0;
	if(argc!=2) {
		printf("\nError: Invalid number of arguments!\n\n");
		return 0;
	}
	input = argv[1];
	readFile(input);

	totalProfit = greedy();

	/*Free the allocated memory*/
	free(r);
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
		r = (double*) malloc(sizeof(double)*size);
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
			r[i] = profit[i] / weight[i];
			ch=fgetc(fp);
		}	
	}	
	fclose(fp);
	return 0;
}
int greedy() {
    int knapsack = 0;
	int price = 0;
	std::clock_t start;
    double duration;
	start = std::clock();
	
	for(int j = 0; j < size; j++){

		double max = 0;
		int index = -1;

		for (int i = 0; i < size; i++) {
			if (r[i] >= max) {
				max = r[i];
				index = i;
			}
		}
		knapsack += weight[index];
		if (capacity < knapsack) {
			knapsack -= weight[index];
		}
		else {
			price += profit[index];
		}
		r[index] = -1;
	}
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	std::cout << id <<" "<<price<<" "<< duration << std::endl;
	return price;
}
