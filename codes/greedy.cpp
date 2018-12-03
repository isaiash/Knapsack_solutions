#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <ctime>
#include <algorithm>

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

        for(int i=0; i<n; i++){
                infile>>it_weight;
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

	clock_t start, finish;
        double duration;

	start=clock();

        sort(d_order.begin(),d_order.end(),comp(densities));

        int cur_w=0, max_val=0;
	
	for(int i=0; i<n; i++){
		cur_w+=weights[d_order[i]];
		if(cur_w>cap)
			cur_w-=weights[d_order[i]];
		else
			max_val+=values[d_order[i]];
	}

	finish=clock();

	duration=(finish - start)/(double)CLOCKS_PER_SEC;

	cout<<id<<" "<<max_val<<" "<<duration<<endl;
}
