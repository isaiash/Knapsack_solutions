//sacado de https://github.com/XapaJIaMnu/Knapsack/blob/master/solver.cpp
//sin ratio
#include <fstream>
#include <iostream>
#include <algorithm>
#include <boost/dynamic_bitset.hpp>
using namespace std;

//Container for item
class Item {
    int weight;
    int value;
    public:
        void setup (int, int);
        int get_value() const;
        int get_weight() const;
        float get_ratio() const;
};

void copytable(int** table, int y);

void printable(int** table, int x, int y);

void Item::setup (int v, int w){
    weight = w;
    value = v;
}

int Item::get_value () const{
    return value;
}

int Item::get_weight () const{
    return weight;
}

float Item::get_ratio () const{
    return ratio;
}

//Comparison for item class

bool operator<(const Item& x, const Item& y) {
    return x.get_weight() < y.get_weight();
}

int main (int argc, char* argv[]){

    std::ifstream infile(argv[1]); //Get the file
    int num_items, capacity; // Get the items and the capacity
    infile >> num_items >> capacity;

	std::cout << "Input file says the knapsack can hold a maximum of " << capacity << " units of weight and our set of data is " << num_items << " elements." << std::endl;

    Item* items = new Item[num_items + 1];

    int value,weight;
    for (int i = 0; i < num_items; i ++)
    {
        infile >> value >> weight;
        items[i+1].setup(value, weight);

	if (i == 0)
		std::cout << "The first element of our data has a value of: " << value << " and weighs: " << weight << " units." << std::endl;
    }

    //std::sort(items + 0, items + num_items);

    int** table = new int*[2];
    for (int i = 0; i< 2; i++){
        table[i] = new int[capacity+1];
    }


    std::vector<boost::dynamic_bitset<> > items_used(capacity+1, boost::dynamic_bitset<>(num_items+1)); //Bit array
    std::vector<boost::dynamic_bitset<> > items_used2(capacity+1, boost::dynamic_bitset<>(num_items+1)); //Bit array


    int a,b; //temps

    for (int i = 1; i<num_items+1; i++){
        for (int j = 1; j<capacity+1; j++){
            if (items[i].get_weight() <= j){
                a = table[0][j];
                b = table[0][j-items[i].get_weight()] + items[i].get_value();
                if (b>a){
                    //cout << "A " << a << "B " << b << endl;
                    //cout << "Before " << items_used[j] << endl;
                    items_used[j] = items_used2[j-items[i].get_weight()];
                    //cout << "After " << items_used[j] << endl;
                    items_used[j][i] = 1;
                    //cout << "Set j " << j << " i " << i << ' ' << items_used[j] << endl;
                    table[1][j] =b;
                } else{
                    table[1][j] =a;
                    items_used[j] = items_used2[j-1];
                }
                //table[1][j] = max(table[0][j],
                //    table[0][j-items[i].get_weight()] + items[i].get_value());
            }else{
                table[1][j] = table[0][j];
                items_used[j] = items_used2[j-1];
               /* for (int n = 1; n<i;n++){
                        items_used[j][n] = items_used[j-1][n-1];
                }*/
                //items_used[j] = items_used[j-1];
                //items_used[i-1][j-1]
            }
        }
        items_used2 = items_used;
        copytable(table, capacity+1);
    }
    //Print answer
    cout << "Maximum value that the knapsack can hold: " << table[1][capacity]<< " " << endl;
    cout << "The knapsack should carry: ";

    for (int i = 1; i<num_items; i++)
    {
        cout << items_used[capacity][i] << ' ';
    }

    cout << items_used[capacity][num_items] << endl;

    std::size_t tot_weight = 0;

    cout << "That means: ";
    for (int i = 1; i<num_items; i++)
    {
        if (items_used[capacity][i] == 1)
	{
		std::cout << "( " << items[i].get_value() << " " << items[i].get_weight() << " )";
		tot_weight += items[i].get_weight();
	}
    }
    if (items_used[capacity][num_items] == 1)
    {
	std::cout << "( " << items[num_items].get_value() << " " << items[num_items].get_weight() << " )" << std::endl;
	tot_weight += items[num_items].get_weight();
    }
    else
	std::cout << std::endl;

    cout << "Total knapsack weight is: " << tot_weight << std::endl;

    return 0;
}

void copytable(int** table, int y){
    for (int i = 0; i< y; i++){
        table[0][i] = table[1][i];
    }
}

void printable(int** table, int x, int y){
    for (int i = 0; i< x; i++){
        for (int j =0; j<y; j++){
            cout << table[x][y] << " ";
        }
        cout << endl;
    }
}