//dp dual
#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <ctime>

#define MAXN 1000

using namespace std;

int main(int argc, char *argv[]){
	int n, cap;
	vector<int> weights;
	vector<int> values;
	bitset<MAXN> solution;

	if(argc!=2){
		cerr<<"Usage: "<<argv[0]<<" 'instace file name'"<<endl;
	}

	ifstream infile(argv[1]);

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


	clock_t start, finish;
	double duration;

	start=clock();

	vector<vector<int >> dp_table(n+1,vector<int>(value_ub+1,weight_ub+1));

	for(int i=1; i<=n; i++){
		for(int j=1; j<=value_ub; j++){
			if(values[i-1]>=j)
				dp_table[i][j]=min(dp_table[i-1][j],weights[i-1]);
			else if(values[i-1]<j)
				dp_table[i][j]=min(dp_table[i-1][j],dp_table[i-1][j-values[i-1]]+weights[i-1]);
			else
				dp_table[i][j]=dp_table[i-1][j];
		}
	}

	//binary search last row for cap
	int l=1, r=value_ub, m;
	while(l<=r){
		m=(l+r)/2;
		if(m<cap)
			l=m;
		else if(m>cap)
			r=m+1;
		else
			break;
	}

	int max_val=dp_table[n][m];

	int rem_val=max_val, col=m;

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
	
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

	cout<<m<<" "<<duration<<endl;

	/*
	for(int i=0; i<n; i++){
		cout<<solution[i];
	}
	cout<<endl;
	*/

	/*
	for(int i=0; i<=n; i++){
		for(int j=0; j<=value_ub; j++){
			cout<<dp_table[i][j]<<" ";
		}
		cout<<endl;
	}
	*/
}