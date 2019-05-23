#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <ctime>
#include <algorithm>

#define MAX_TABLE_SIZE 8000000000
#define MAX 2000
#define SAMPLE 0.1

using namespace std;

struct comp{
	const vector<double> &value_vector;

	comp(const vector<double> &val_vec):
		value_vector(val_vec) {}

	bool operator()(int i1, int i2){
		return value_vector[i1]>value_vector[i2];
	}
};

int main(int argc, char *argv[]){
	int n, cap, id;
	vector<int> weights;
	vector<int> values;
	vector<double> densities;
	//bitset<MAXN> solution;

	if(argc!=2){
		cerr<<"Usage: "<<argv[0]<<" 'instace file name'"<<endl;
		return 0;
	}

	ifstream infile(argv[1]);

	infile>>id;
	infile>>n;
	infile>>cap;

	int it_weight, it_value;
	double it_density;

	int weight_ub=0;	
	for(int i=0; i<n; i++){
		infile>>it_weight;
		weight_ub+=it_weight;
		weights.push_back(it_weight);
	}

	vector<int> d_order;
	for(int i=0; i<n; i++){
		infile>>it_value;
		it_density=it_value/(double)weights[i];
		values.push_back(it_value);
		densities.push_back(it_density);
		d_order.push_back(i);
	}

	clock_t start, last, interm, finish;
	double duration;

	start=clock();
	last=clock();
	
	sort(d_order.begin(),d_order.end(),comp(densities));

	int cur_w=weights[d_order[0]], value_ub=0, ind=0;

	//get value upper bound;
	while(cur_w<cap && ind<n-1){
		value_ub+=values[d_order[ind]];
		ind++;
		if(ind>n)
			break;
		cur_w+=weights[d_order[ind]];
	}

	if(ind<n)
		value_ub+=values[d_order[ind]];

	unsigned long long table_size=2*(value_ub+1)*sizeof(int);
	if(table_size>MAX_TABLE_SIZE){
		cout<<id<<" "<<-1<<" "<<-1<<endl;
		return 1;
	}

	vector<vector<int >> dp_table(2,vector<int>(value_ub+1,weight_ub+1));

	//calcular solucion cada 1s y ver mejora cada 1 s? medio s?
	int l, r, m, max_val, rem_val, col, index, index_b;
	for(int i=1; i<=n; i++){
		index = i%2;
		if (index)
			index_b=0;
		else
			index_b=1;
		for(int j=0; j<=value_ub; j++){
			if(values[i-1]>=j)
				dp_table[index][j]=min(dp_table[index_b][j],weights[i-1]);
			else if(values[i-1]<j)
				dp_table[index][j]=min(dp_table[index_b][j],dp_table[index_b][j-values[i-1]]+weights[i-1]);
			else
				dp_table[index][j]=dp_table[index_b][j];
		}
		interm=clock();
		duration = ( interm - last) / (double) CLOCKS_PER_SEC;
		if(duration>=SAMPLE){
			l=0;
			r=value_ub;
			while(l<r){
				m=(l+r+1)/2;
				if(dp_table[index][m]<=cap)
					l=m;
				else if(dp_table[index][m]>cap)
					r=m-1;
				else
					break;
			}

			max_val=r;

			/*
			rem_val=dp_table[i][r];
			col=r;
			for (int j = n; j > 0 && rem_val > 0; j--) {

				if (rem_val == dp_table[j - 1][col])
					continue;
				else {
					solution.set(j-1);
					rem_val = rem_val - weights[j - 1];
					col = col - values[j - 1]; 
				}
			}
			*/
			duration=(interm-start)/(double)CLOCKS_PER_SEC;
			cout<<id<<" "<<max_val<<" "<<duration<<endl;
			last=interm;
		}
	}

	int final_index = n%2;

	l=0;
	r=value_ub;
	while(l<r){
		m=(l+r+1)/2;
		if(dp_table[final_index][m]<=cap)
			l=m;
		else if(dp_table[final_index][m]>cap)
			r=m-1;
		else
			break;
	}

	max_val=r;

	/*
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
	}*/

	finish=clock();
	
	duration = ( finish - start ) / (double) CLOCKS_PER_SEC;

	cout<<id<<" "<<max_val<<" "<<duration<<endl;
	
	
	//print table
	/*for(int i=0; i<2; i++){
		for(int j=0; j<=value_ub; j++){
			cout<<dp_table[i][j]<<" ";
		}
		cout<<endl;
	}
	*/
}

