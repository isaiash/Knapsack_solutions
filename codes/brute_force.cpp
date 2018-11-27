#include<iostream>
#include<stdio.h>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<ctime>

/* Globale variable */
int id;
int size;
int capacity;
int *weight,*profit,*selected;

/*Funtion declaration*/
int readFile(char*);
int printResult(int);
int bruteforce();

int main(int argc, char ** argv){
	char *input;
	int i,totalProfit=0,totalWeight=0;
	if(argc!=2) {
		printf("\nError: Invalid number of arguments!\n\n");
		return 0;
	}
	input = argv[1];
	readFile(input);

	totalProfit = bruteforce();

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

int bruteforce() {
    if (size > 30){
        printf("%d -1 -1\n", id);
        return 0;
    }
	int i,j,isSelected=1,maxWt,maxProfit;
	unsigned long long int iter=pow(2, size);
	int result=0;
	int temp[size];
	
	std::clock_t start;
    double duration;
	start = std::clock();
	/*Initialize the temp array */
	for(i =0;i<size;i++)
		temp[i]=0;
	for(i=0;i<iter;i++) {
		isSelected=1;
		maxProfit=0;
		maxWt=0;
		
		for(j=0;j<size;j++){
			if(temp[j]==1){
				maxWt+=weight[j];
				maxProfit+= profit[j];
			}
		}
		if( maxWt <= capacity && maxProfit>result){
			/*Copies the selected item in array. */
			for(j=0;j<size;j++)
				selected[j]=temp[j];
			result=maxProfit;
			duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	        printf("%d %d %f\n", id, result, duration);
	        if(duration > 300) return 0;
		}
		
		/*Calculate the next subset */
		for(j=0;j<size;j++){
			temp[j]=temp[j]+isSelected;
			isSelected = temp[j]/2;
			temp[j]=temp[j]%2;	
		} 
	}
	return result;
}
