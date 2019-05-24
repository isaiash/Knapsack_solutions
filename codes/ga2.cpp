#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<iostream>
#include <fstream>
#include <vector>

#define maxit 30
unsigned int b,i,j,k,it=1,e1,e2,E,maxb,maxO=0;
double O;

unsigned int pivotlistf(unsigned int first,unsigned int last, std::vector<double> &r, std::vector<int> &ri){

  double pivotvalue=0.0,tempr=0.0;
  unsigned int pivotpoint=0,index=0,tempw=0;

  pivotvalue=r[first];
  pivotpoint=first;
  for (index=first+1;index<=last;index++)
   if (r[index]>pivotvalue){
    pivotpoint=pivotpoint+1;

    tempr=r[pivotpoint];
    r[pivotpoint]=r[index];
    r[index]=tempr;

    tempw=ri[pivotpoint];
    ri[pivotpoint]=ri[index];
    ri[index]=tempw;

   }
   tempr=r[first];
   r[first]=r[pivotpoint];
   r[pivotpoint]=tempr;

   tempw=ri[first];
   ri[first]=ri[pivotpoint];
   ri[pivotpoint]=tempw;

   return pivotpoint;
 }

void quicksortf(unsigned int first,unsigned int last, std::vector<double> &r, std::vector<int> &ri){
 unsigned int pivot=0;
 if (first<last){
  pivot=pivotlistf(first,last,r,ri);
  quicksortf(first,pivot-1,r,ri);
  quicksortf(pivot+1,last,r,ri);
 }
}

unsigned int pivotlisti(unsigned int first,unsigned int last, std::vector<int> &popf,std::vector<int> &popfi, std::vector<double> &r){
  unsigned int pivotvalue=0,tempi=0;
  unsigned int pivotpoint=0,index=0,tempw=0;
  pivotvalue=popf[first];
  pivotpoint=first;
  for (index=first+1;index<=last;index++)
   if (r[index]>pivotvalue){
    pivotpoint=pivotpoint+1;

    tempi=popf[pivotpoint];
    popf[pivotpoint]=popf[index];
    popf[index]=tempi;

    tempw=popfi[pivotpoint];
    popfi[pivotpoint]=popfi[index];
    popfi[index]=tempw;

   }
   tempi=popf[first];
   popf[first]=popf[pivotpoint];
   popf[pivotpoint]=tempi;

   tempw=popfi[first];
   popfi[first]=popfi[pivotpoint];
   popfi[pivotpoint]=tempw;

   return pivotpoint;
 }

void quicksorti(unsigned int first,unsigned int last, std::vector<int> &popf,std::vector<int> &popfi, std::vector<double> &r){
 unsigned int pivot=0;
    if (first<last){
        pivot=pivotlisti(first,last,popf,popfi,r);
        quicksorti(first,pivot-1,popf,popfi,r);
        quicksorti(pivot+1,last,popf,popfi,r);
    }
}

int main(int argc, char *argv[]){
	int n, cap, id;
	if(argc != 2){
        std::cerr<<"Usage: "<<argv[0]<<" 'instace file name'"<<std::endl;
        return 0;
    }
    std::ifstream infile(argv[1]);

    infile>>id;
    infile>>n;
    infile>>cap;

    std::cout << id << " "<< n <<" "<< cap << std::endl;
    std::vector<int> c(n+1,0); //value
    std::vector<int> a(n+1,0); //weight
    std::vector<double> r(n+1,0.0);//density = value/weight =c/a
    std::vector<int> ri(n+1,0); //density order
    int it_weight, it_value;
    double it_density;
    for(int i=1; i<=n; i++){
            infile>>it_weight;
            a[i] = it_weight;
    }
    unsigned int m = n;
    for(int i=1; i<=n; i++){
        infile >> it_value;
        it_density=it_value/(double) a[i];
        c[i] = it_value;
        r[i] = it_density;
        ri[i] = i;
    }

    for(int i=0;i<a.size();i++){
        std::cout << a[i] << " "; 
    } std::cout << std::endl;

    for(int i=0;i<c.size();i++){
        std::cout << c[i] << " "; 
    } std::cout << std::endl;

    for(int i=0;i<ri.size();i++){
        std::cout << ri[i] << " "; 
    } std::cout << std::endl;

    for(int i=0;i<r.size();i++){
        std::cout << r[i] << " "; 
    } std::cout << std::endl;

    std::vector< std::vector<int> > pop(m+1, std::vector<int> (n+1,0));
    std::vector< std::vector<int> > pop2(m+1, std::vector<int> (n+1,0));
    std::vector<int> popf(m+1,0);
    std::vector<int> popfi(m+1,0);
    std::vector<int> pop2f(m+1,0);
    std::vector<double> rf(n+1,0);

    clock_t start = clock();
    std::cout << " paso 1 " << std::endl; 
	quicksortf(1,n,r,ri);
    
	srand( (unsigned)time( NULL ) );
    std::cout << " paso 2 " << std::endl;
	for(k=1;k<=m;k++){
		for(i=k;i<=n;i++)
			if (pop[k][0] + a[ri[i]] <= b){
				pop[k][ri[i]]=1;
				pop[k][0]+=a[ri[i]];
				popf[k]+=c[ri[i]];
			}

		while (pop[k][0] <= b){
			i=(rand() % n)+1;
			if (pop[k][i]==0)
			{
				pop[k][i]=1;
				pop[k][0]+=a[i];
				popf[k]+=c[i];
			}
		}
		for(i=n; i>=1 && pop[k][0]>b; i--)
			if (pop[k][ri[i]]==1){
				pop[k][ri[i]]=0;
				pop[k][0]-=a[ri[i]];
				popf[k]-=c[ri[i]];
			}
	}

iterasyon:
	for(i=1;i<=m;i++)
		popfi[i]=i;
    for(int pec= 0; pec < popfi.size();pec++)
        std::cout << popfi[pec] << " ";

	quicksorti(1,m, popf, popfi,r);
    std::cout << " paso 4 " << std::endl;
    for(int pec= 0; pec < popf.size();pec++)
        std::cout << popf[pec] << " ";
    std::cout<< std::endl;
    for(int pec= 0; pec < popfi.size();pec++)
        std::cout << popfi[pec] << " ";
    std::cout<< std::endl;
	for(k=1;k<=m/2;k++){
		for(j=0;j<=n;j++)
			pop2[k][j] = pop[popfi[k]][j];
		pop2f[k] = popf[k];
	}

	popf[0]=0;
	for(i=1;i<=m;i++)
		popf[0]+=popf[i];
	rf[0]=0;
	for(i=1;i<=m;i++)
		rf[i] = rf[i-1] + (100.0*popf[i]/popf[0]);
	
	for(k=m/2+1; k<=m; k++){
		i=(rand() % 100)+1;
		e1=0;
		while (i>rf[e1] && e1<m)
			e1++;
yenie2:
		i=(rand() % 100) + 1;
		e2=0;
		while (i>rf[e2] && e2<m)
			e2++;
		if (e1==e2)
			goto yenie2;
        std::cout << " paso 5 " << std::endl; 
		E = popf[e1]+popf[e2];
		O = 100*popf[e1]/E;
		pop2[k][0]=0;
		pop2f[k]=0;
		for(j=1;j<=n;j++){
			i=(rand() % 100)+1;
			if (i<=O)
				pop2[k][j]=pop[popfi[e1]][j];
			else
				pop2[k][j]=pop[popfi[e2]][j];
			if (pop2[k][j]==1){
				pop2[k][0]+=a[j];
				pop2f[k]+=c[j];
			}
		}
		for(i=n; i>=1 && pop2[k][0]>b; i--)
			if (pop2[k][ri[i]]==1){
				pop2[k][ri[i]]=0;
				pop2[k][0]-=a[ri[i]];
				pop2f[k]-=c[ri[i]];
			}
	}

	for(k=1; k<=m; k++){
		popf[k]=pop2f[k];
		for(j=0; j<=n; j++)
			pop[k][j]=pop2[k][j];

		if (popf[k]>maxO){
			maxO=popf[k];
			maxb=pop[popfi[k]][0];
			printf("it : %d  b : %d  maxO : %d\n",it,maxb,maxO);
		}
	}

//mutasyon
 	for(k=1; k<=m; k++){
		j=1;
		while (j<=n/4){
			i=(rand() % n)+1;
			if (pop[k][i]==1){
				pop[k][i]=0;
				pop[k][0]-=a[i];
				popf[k]-=c[i];
			}
			j++;
		}
        std::cout << " paso 7 " << std::endl; 
		for(i=1; i<=n && pop[k][0]<=b; i++)
			if (pop[k][ri[i]]==0){
				pop[k][ri[i]]=1;
				pop[k][0]+=a[ri[i]];
				popf[k]+=c[ri[i]];
			}

		for(i=n; i>=1 && pop[k][0]>b; i--)
			if (pop[k][ri[i]]==1){
				pop[k][ri[i]]=0;
				pop[k][0]-=a[ri[i]];
				popf[k]-=c[ri[i]];
			}	
	}
	it++;
	if (it < maxit) goto iterasyon;
	printf("\nTime : %f\n\n", (float) (clock() - start) / CLOCKS_PER_SEC);
}