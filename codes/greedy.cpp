#include<stdio.h>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include <ctime>

/* Globale variable */
int id;
int size;
int capacity;
int *weight,*profit,*selected;

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
	free(selected);
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
		selected = (int*) malloc(sizeof(int)*size);
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
int greedy() {
	int i,j,temp;
	int result=0,greedyCap=capacity;
	int tempArr[size],tempWt[size],tempProfit[size];
	float val1,val2;
	
	std::clock_t start;
    double duration;
	start = std::clock();
	
	for(i=0;i<size;i++) {
		tempArr[i]=i;
		tempWt[i]=weight[i];
		tempProfit[i]=profit[i];
		selected[i]=0;
	}

	for(i=0;i<size;i++) {
		for(j=0;j<size - i-1;j++) {
			val1 = ((float)tempProfit[j]/ (float)tempWt[j]);
			val2 = ((float)tempProfit[j+1]/(float) tempWt[j+1]);
			if( val1 < val2 ){
				//swap profit
				temp=tempProfit[j];
				tempProfit[j]=tempProfit[j+1];
				tempProfit[j+1]=temp;
				//swap weights
				temp=tempWt[j];
				tempWt[j]=tempWt[j+1];
				tempWt[j+1]=temp;
				//swap item number as well
				temp=tempArr[j];
				tempArr[j]=tempArr[j+1];
				tempArr[j+1]=temp;
			}
		}
	}
	
	for(i=0;i<size;i++) {
		if(tempWt[i] <= greedyCap && greedyCap > 0) {
			result+=tempProfit[i];
			greedyCap -=tempWt[i];
			selected[tempArr[i]]=1;
		}	
	}
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	printf("%d %d %f\n", id, result, duration);
	return result;
}
