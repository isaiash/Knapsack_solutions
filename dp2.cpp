//dp dual
#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <ctime>

#define MAXN 2000
#define SAMPLE 0.99

using namespace std;

int main(int argc, char *argv[]){
	int n, cap, id;
	vector<int> weights;
	vector<int> values;
	bitset<MAXN> solution;

	if(argc!=2){
		cerr<<"Usage: "<<argv[0]<<" 'instace file name'"<<endl;
	}

	ifstream infile(argv[1]);

	infile>>id;
	infile>>n;
	infile>>cap;

	int it_weight, it_value;

	int weight_ub=0;	
	for(int i=0; i<n; i++){
		infile>>it_weight;
		weight_ub+=it_weight;
		weights.push_back(it_weight);
	}

	int value_ub=0;
	for(int i=0; i<n; i++){
		infile>>it_value;
		value_ub+=it_value;
		values.push_back(it_value);
	}


	clock_t start, last, interm, finish;
	double duration;

	start=clock();
	last=clock();

	vector<vector<int >> dp_table(n+1,vector<int>(value_ub+1,weight_ub+1));

	//calcular solucion cada 1s y ver mejora cada 1 s? medio s?
	int l, r, m, max_val, rem_val, col;
	for(int i=1; i<=n; i++){
		for(int j=0; j<=value_ub; j++){
			if(values[i-1]>=j)
				dp_table[i][j]=min(dp_table[i-1][j],weights[i-1]);
			else if(values[i-1]<j)
				dp_table[i][j]=min(dp_table[i-1][j],dp_table[i-1][j-values[i-1]]+weights[i-1]);
			else
				dp_table[i][j]=dp_table[i-1][j];
		}
		interm=clock();
		duration = ( interm - last) / (double) CLOCKS_PER_SEC;
		if(duration>=SAMPLE){
			l=1;
			r=value_ub;
			while(l<r){
				m=l+(r-l+1)/2;
				if(dp_table[i][m]<=cap)
					l=m;
				else if(dp_table[i][m]>cap)
					r=m-1;
				else
					break;
			}

			max_val=r;
			rem_val=dp_table[i][r];
			col=r;

			for (int i = n; i > 0 && rem_val > 0; i--) {

				if (rem_val == dp_table[i - 1][col])
					continue;
				else {
					solution.set(i-1);
					rem_val = rem_val - weights[i - 1];
					col = col - values[i - 1]; 
				}
			}
			duration=(interm-start)/(double)CLOCKS_PER_SEC;
			cout<<id<<" "<<r<<" "<<duration<<endl;
			last=interm;
		}

	}

	max_val=r;
	rem_val=dp_table[n][r];
	col=r;

	for (int i = n; i > 0 && rem_val > 0; i--) {

		if (rem_val == dp_table[i - 1][col])
			continue;
		else {
			solution.set(i-1);
			rem_val = rem_val - weights[i - 1];
			col = col - values[i - 1]; 
		}
	}

	finish=clock();
	
	duration = ( finish - start ) / (double) CLOCKS_PER_SEC;

	cout<<id<<" "<<max_val<<" "<<duration<<endl;

	/*
	//print solution
	for(int i=0; i<n; i++){
		cout<<solution[i];
	}
	cout<<endl;
	*/
	
	/*
	//print table
	for(int i=0; i<=n; i++){
		for(int j=0; j<=value_ub; j++){
			cout<<dp_table[i][j]<<" ";
		}
		cout<<endl;
	}
	*/
	
}