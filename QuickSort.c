#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define MLD 1000000000.0 //10**9
#define THRESHOLD 10
int Partition (int A [], int p, int r)
{
	int x=A[r];
	int i=p-1;
	int j;
	int buff;
	for(j=p;j<=r;++j)
	{
		 if(A[j]<=x)
		 {
			 ++i;
			 //swap
			 buff=A[j];
			 A[j]=A[i];
			 A[i]=buff;
		 }
	}
	
	if(i<r)
	return i;
	else 
	return i-1;
}

void bubbleSort (int A[], int p, int r)
{
	int n=r;
	int i, buff;
	do
	{
		for(i=p;i<n-1;++i)
		{
			if(A[i]>A[i+1])
			{
				//swap
				buff=A[i+1];
				 A[i+1]=A[i];
				 A[i]=buff;
			}
		}
		n=n-1;
	}while(n>1);
}

void Quicksort (int A [], int p, int r)
{
	
	if(p<r)
	{
		int q=Partition(A,p,r);
		Quicksort(A,p,q);
		Quicksort(A,q+1,r);
	}
}

void QuicksortBubble (int A [], int p, int r)
{
	if((r-p )<THRESHOLD)
	bubbleSort(A,p,r);
	else if(p<r)
	{
		int q=Partition(A,p,r);
		Quicksort(A,p,q);
		Quicksort(A,q+1,r);
	}
}

void generateRandomTable (int A[],int size)
{
	int i, source;
	source=time(NULL);
	srand(source);
	for(i=0;i<size;++i)
	{
		A[i]=rand();
	}
}

void generateDescTable (int A[],int size)
{
	int i;
	
	for(i=0;i<size;++i)
	A[i]=size-i;
}

void showResults(double result1[], double result2[], int n)
{
	int i;
	for(i=0;i<n;++i)
	{
		//printf("n: %d ",i*50);
		printf("%3.10lf", result1[i]);
		printf("\t |");
		printf("%3.10lf", result2[i]);
		printf("\t |");
		printf("%3.5lf", ((i+1)*50*log((i+1)*50))/result1[i]);
		printf("\t |");
		printf("%3.5lf", ((i+1)*50*log((i+1)*50))/result2[i]);
		printf("\n");
	}
	
}

void showResultsSquare(double result1[], double result2[], int n)
{
	int i;
	for(i=0;i<n;++i)
	{
		//printf("n: %d ",i*50);
		printf("%3.10lf", result1[i]);
		printf("\t |");
		printf("%3.10lf", result2[i]);
		printf("\t |");
		printf("%3.5lf", ((i+1)*50*(i+1)*50)/result1[i]);
		printf("\t |");
		printf("%3.5lf", ((i+1)*50*(i+1)*50)/result2[i]);
		printf("\n");
	}
	
}

int main()
{
	struct timespec tp0, tp1;
	int i,j;
	double Tn,sum=0;
	int n=50;
	int A[5000];
	
	double result1[100];
	double result2[100];
	double result3[100];
	double result4[100];
	for(j=0;j<100;++j)
	{
		sum=0;
		for(i=0;i<10;++i)
		{
			generateRandomTable(A,n);
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&tp0);
			Quicksort(A,0,n);
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&tp1);
			Tn=(tp1.tv_sec+tp1.tv_nsec/MLD)-(tp0.tv_sec+tp0.tv_nsec/MLD);
			sum=sum+Tn;
		}
		
		
		sum=sum/n;
	  //printf("Wspolczynnik dla danych losowych(n*log(n)) :  %3.10lf , n: %d \n", (n*log(n))/sum, n);
	  result1[j]=sum;
	  n=n+50;
	}
	n=50;
	for(i=0;i<100;++i)
	{
		generateDescTable(A,n);
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&tp0);
		Quicksort(A,0,n);
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&tp1);
		Tn=(tp1.tv_sec+tp1.tv_nsec/MLD)-(tp0.tv_sec+tp0.tv_nsec/MLD);
		//printf("Wspolczynnik dla danych niekorzystnych(n*n) :   %3.10lf , n: %d \n", (n*n)/Tn, n);
		 result2[i]=Tn;
		 n=n+50;
	}
		  

	///======================
	printf("\n");
	 sum=0;
	n=50;
	for(j=0;j<100;++j)
	{
		sum=0;
		for(i=0;i<10;++i)
		{
			generateRandomTable(A,n);
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&tp0);
			QuicksortBubble(A,0,n);
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&tp1);
			Tn=(tp1.tv_sec+tp1.tv_nsec/MLD)-(tp0.tv_sec+tp0.tv_nsec/MLD);
			sum=sum+Tn;
		}
		
		
		sum=sum/n;
	  //printf("Wspolczynnik dla danych losowych(mod. Quicksort) :  %3.10lf , n: %d \n", n*log(n)/sum, n);
	  result3[j]=sum;
	  n=n+50;
	}
	
	n=50;
	for(i=0;i<100;++i)
	{
		generateDescTable(A,n);
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&tp0);
		QuicksortBubble(A,0,n);
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&tp1);
		Tn=(tp1.tv_sec+tp1.tv_nsec/MLD)-(tp0.tv_sec+tp0.tv_nsec/MLD);
		//printf("Wspolczynnik dla danych niekorzystnych(n*n) :   %3.10lf , n: %d \n", (n*n)/Tn, n);
		 result4[i]=Tn;
		 n=n+50;
	}
		 
	printf("\n Porownanie czasow dzialania \n");
	printf("mod. Quicksort=Quicksort z sortowaniem babelkowym od progu: %d \n", THRESHOLD);
	printf("Dane losowe \n Quicksort \t |mod. Quicksort |Qsort: n*log(n)/czas   |mQsort: n*log(n)/czas \n");
	showResults(result1,result3,100);
	
	printf("Dane niekorzystne \n Quicksort \t |mod. Quicksort |Qsort: n*n/czas \t |mQsort: n*n/czas \n");
	showResultsSquare(result2,result4,100);
	
	return 0;
}
