#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<iostream>

#define n 10000		// number of items
#define p 25		// density of knapsack problem
#define m n
#define maxit 30

FILE *f;
unsigned int b,i,j,k,it=1,e1,e2,E,maxb,maxO=0;
unsigned int a[n+1]={0},c[n+1]={0},ri[n+1]={0},pop[m+1][n+1]={0},popf[m+1]={0},popfi[m+1]={0},pop2[m+1][n+1]={0},pop2f[m+1]={0};
double r[n+1]={0},rf[m+1]={0},O;
char fins[12],ns[8],ps[8];

unsigned int pivotlistf(unsigned int first,unsigned int last){

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

void quicksortf(unsigned int first,unsigned int last){
 unsigned int pivot=0;
 if (first<last){
  pivot=pivotlistf(first,last);
  quicksortf(first,pivot-1);
  quicksortf(pivot+1,last);
 }
}

unsigned int pivotlisti(unsigned int first,unsigned int last){
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

void quicksorti(unsigned int first,unsigned int last){
 unsigned int pivot=0;

 if (first<last){
  pivot=pivotlisti(first,last);
  quicksorti(first,pivot-1);
  quicksorti(pivot+1,last);
 }
}

int main(){
	//itoa(n,ns,10);
	//itoa(p,ps,10);
	std::string sns = std::to_string(n);
	strcpy(ns, sns.c_str());
	std::string sps = std::to_string(p);
	strcpy(ps, sps.c_str());
	strcpy(fins,"");
	strncat(fins,ns,strlen(ns)+1);
	strncat(fins,"_",strlen(ns)+2);	
	strncat(fins,ps,strlen(ps)+1);
	strncat(fins,".txt",strlen(ns)+1+strlen(ps)+1);
	f=fopen(fins,"r");
	fscanf(f,"%d",&i);
    if (i!=n){
		printf("¡El tamaño es incompatible!");
		return 0;
	}
	for(i=1; i<=n; i++)
		fscanf(f,"%d %d",&a[i],&c[i]);
	fscanf(f,"%d",&b);
	fclose(f);
	clock_t start, end;
	start = clock();

	for(i=1;i<=n;i++){
		r[i]=1.0*c[i]/a[i];
		ri[i]=i;
	}
	quicksortf(1,n);

	srand( (unsigned)time( NULL ) );

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

	quicksorti(1,m);

	for(k=1;k<=m/2;k++){
		for(j=0;j<=n;j++)
			pop2[k][j]=pop[popfi[k]][j];
		pop2f[k]=popf[k];
	}

	popf[0]=0;
	for(i=1;i<=m;i++)
		popf[0]+=popf[i];
	rf[0]=0;
	for(i=1;i<=m;i++)
		rf[i]=rf[i-1] + (100.0*popf[i]/popf[0]);
	
	for(k=m/2+1; k<=m; k++){
		i=(rand() % 100)+1;
		e1=0;
		while (i>rf[e1] && e1<m)
			e1++;
yenie2:
		i=(rand() % 100)+1;
		e2=0;
		while (i>rf[e2] && e2<m)
			e2++;
		if (e1==e2)
			goto yenie2;

		E=popf[e1]+popf[e2];
		O=100*popf[e1]/E;
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
			if (pop[k][i]==1)
			{
				pop[k][i]=0;
				pop[k][0]-=a[i];
				popf[k]-=c[i];
			}
			j++;
		}
	
		for(i=1; i<=n && pop[k][0]<=b; i++)
			if (pop[k][ri[i]]==0)
			{
				pop[k][ri[i]]=1;
				pop[k][0]+=a[ri[i]];
				popf[k]+=c[ri[i]];
			}

		for(i=n; i>=1 && pop[k][0]>b; i--)
			if (pop[k][ri[i]]==1)
			{
				pop[k][ri[i]]=0;
				pop[k][0]-=a[ri[i]];
				popf[k]-=c[ri[i]];
			}	
	}
	it++;
	if (it<maxit) goto iterasyon;
	printf("\nTime : %f\n\n", (float) (clock() - start) / CLOCKS_PER_SEC);
}