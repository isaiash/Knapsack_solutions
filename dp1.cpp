//dp con tabla de capacidad
#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <algorithm>
#include <ctime>

#define MAXN 1000
#define SAMPLE 0.9

using namespace std;

int main(int argc, char *argv[]){
	int n, cap, id;
	vector<int> weights;
	vector<int> values;
	bitset<MAXN> solution;

	if(argc!=2){
		cerr<<"Usage: "<<argv[0]<<" 'instacefilename'"<<endl;
	}

	ifstream infile(argv[1]);

	infile>>id;
	infile>>n;//number of items
	infile>>cap;//knapsack capacity

	int it_weight, it_value;
	
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
			//reconstruct solution
			max_val=dp_table[i][cap];
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
			}

			//display solution and time
			duration=(interm-start)/(double) CLOCKS_PER_SEC;
			cout<<id<<" "<<dp_table[i][cap]<<" "<<duration<<endl;
			last=interm;
		}
	}

	max_val=dp_table[n][cap];
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
	}

	finish=clock();

	duration = ( finish - start ) / (double) CLOCKS_PER_SEC;

	cout<<id<<" "<<max_val<<" "<<duration<<endl;
}