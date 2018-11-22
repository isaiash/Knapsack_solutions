//dp con tabla de capacidad
#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <algorithm>
#include <ctime>

#define MAX_TABLE_SIZE 8000000000
#define MAXN 2000
#define SAMPLE 0.99

using namespace std;

int main(int argc, char *argv[]){
	int n, cap, id;
	vector<int> weights;
	vector<int> values;
	//bitset<MAXN> solution;

	if(argc!=2){
		cerr<<"Usage: "<<argv[0]<<" 'instacefilename'"<<endl;
	}

	ifstream infile(argv[1]);

	infile>>id;
	infile>>n;//number of items
	infile>>cap;//knapsack capacity

	int it_weight, it_value;

	unsigned long long table_size=(n+1)*(cap+1)*sizeof(int);
	
	//table too big for memory
	if(table_size>MAX_TABLE_SIZE){
		cout<<id<<" "<<-1<<" "<<-1<<endl;
		return 1;
	}
	
	for(int i=0; i<n; i++){
		infile>>it_weight;
		weights.push_back(it_weight);
	}

	for(int i=0; i<n; i++){
		infile>>it_value;
		values.push_back(it_value);
	}


	vector<vector<int >> dp_table(n+1,vector<int>(cap+1,0));

	clock_t start, last, interm, finish;
	double duration;

	start=clock();
	last=clock();

	int max_val, rem_val, col;
	for(int i=1; i<=n; i++){
		for(int j=0; j<=cap; j++){
			if(j>=weights[i-1]){
				dp_table[i][j]=max(dp_table[i-1][j],dp_table[i-1][j-weights[i-1]]+values[i-1]);
			}
			else
				dp_table[i][j]=dp_table[i-1][j];
		}
		interm=clock();
		duration=(interm-last)/(double) CLOCKS_PER_SEC;

		//display sol improvement every "SAMPLE" time
		if(duration>=SAMPLE){
			max_val=dp_table[i][cap];

			/*
			//solution config
			rem_val=max_val;
			col=cap;
			for (int j = n; j > 0 && rem_val > 0; j--) {

				if (rem_val == dp_table[j - 1][col])
					continue;
				else {
					solution.set(j-1);
					rem_val = rem_val - values[j - 1];
					col = col - weights[j - 1]; 
				}
			}*/

			//display solution and time
			duration=(interm-start)/(double) CLOCKS_PER_SEC;
			cout<<id<<" "<<max_val<<" "<<duration<<endl;
			last=interm;
		}
	}

	max_val=dp_table[n][cap];

	/*
	//solution config
	rem_val=max_val;
	col=cap;
	for (int i = n; i > 0 && rem_val > 0; i--) {

		if (rem_val == dp_table[i - 1][col])
			continue;
		else {
			solution.set(i-1);
			rem_val = rem_val - values[i - 1];
			col = col - weights[i - 1]; 
		}
	}*/

	finish=clock();

	duration = ( finish - start ) / (double) CLOCKS_PER_SEC;

	cout<<id<<" "<<max_val<<" "<<duration<<endl;
}