//n=length(A)
//k liczba mozliwych wartosci w A//dlugosc c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define SOURCE_TXT "nazwiska.txt"
#define OUTPUT_TXT "nazwiskaOut.txt"
#define OUTPUT2_TXT "nazwiskaOutQS.txt"
#define MAX_NAME_LENGTH 40
#define MLD 1000000000.0 //10**9

///===================
///Funkcja countElements zwraca liczbe wierszy w pliku źródlowym
///
int countElements (FILE* source)
{
    int lineCounter=0;
    char buff;
    while(fscanf(source, "%c", &buff)!=EOF)
        if(buff=='\n')
        ++lineCounter;
        return lineCounter+1;
}
///===================
///Funkcja insertElements wypelnia tablice A wartosciami podanymi w pliku zrodlowym
///
void insertElements (int length,char Tab[length][MAX_NAME_LENGTH],char* A [length], FILE* source)
{
    rewind(source);
    int i;
    for(i=0;i<length;++i)
    {
        fscanf(source, "%s", Tab[i]);
       A[i]=Tab[i];
    }

}
///Funkcja podzialu QSorta
int Partition (char* A[], int p, int r)
{
	char* x=A[r];
	int i=p-1;
	int j;
	char* buff;
	for(j=p;j<=r;++j)
	{
		 if(strcmp(A[j],x)<=0)
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


void Quicksort (char* A[], int p, int r)
{

	if(p<r)
	{
		int q=Partition(A,p,r);
		Quicksort(A,p,q);
		Quicksort(A,q+1,r);
	}
}



///Tworzenie pliku wyjsciowego
void makeOutputFile (int length, char* A[length], char* FILENAME)
{
    FILE* output;
    output=fopen(FILENAME, "w");
    if(output==NULL)
    {
        printf("Nie moĹĽna utworzyc pliku wyjsciowego. \n");
        return;
    }
    int i;

    for(i=0;i<length;++i)
        fprintf(output,"%s \n",A[i]);


    fclose(output);
}
///Sortowanie przez zliczanie dla lancuchow znakow
void CountingSortString(char* A[], char* B[], int C[],int L[], int k, int n, int index)
{
	int AInt [n];
	int i;

	for(i=0;i<n;++i)
	{

		AInt[i]=(int)A[i][index];
		if(AInt[i]>127||AInt[i]<0)
            AInt[i]=0;
	}


	for (i=0;i<k;++i)
		C[i]=0;

	for(i=0;i<n;++i)
		C[AInt[i]]+=1;

	for(i=1;i<k;++i)
		C[i]+=C[i-1];

	for(i=n-1;i>=0;--i)
	{
		--C[AInt[i]];
		B[C[AInt[i]]]=A[i];
	}

}
///Wyliczanie dlugosci lancuchow znakow w tablicy
void CalcutateStringsLength(char* A[], int B[], int stringsCount)
{
	int i,j;
	for(i=0;i<stringsCount;++i)
	{
			j=0;
			B[i]=0;
		while(A[i][j]!=0)
		{
		++B[i];
		++j;
		}
	}

}

void radixSortString (char* A[], char* B[], int C[], int k, int n, int L[])
{
	int i,j;
	int maxL=L[0];

	for(i=0;i<n;++i)
	if(L[i]>maxL)maxL=L[i];

	for(i=maxL-1;i>=0;--i)
    {
        CountingSortString(A,B,C,L, k,n,i);
        for(j=0;j<n;++j)
        {
            A[j]=B[j];
        }
    }
}

void QSortSrings (char* A[],int n)
{
        Quicksort(A,0,n-1);
}


int main()
{
    struct timespec tp0, tp1;
    double Tn;
    //Wczytanie pliku ĹĽrĂłdlowego
    FILE* source;
    source=fopen(SOURCE_TXT, "r");
    if(source==NULL)
    {
        printf("Nie mozna wczytac pliku wejsciowego. \n");
        return 1;
    }

    //wyznaczenie dlugosci tablicy
    int length=countElements(source);

    char Tab[length][MAX_NAME_LENGTH];

    char* A [length];
    char* B [length];
    int C [128];
    insertElements(length,Tab,A,source);
    fclose(source);


int L[length];
CalcutateStringsLength(A,L,length);

//sortowanie pozycyjne
clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&tp0);
radixSortString(A,B,C,128,length,L);
clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&tp1);

Tn=(tp1.tv_sec+tp1.tv_nsec/MLD)-(tp0.tv_sec+tp0.tv_nsec/MLD);
  printf("czas RadixSort: %3.10lf\n",Tn);

makeOutputFile(length,B, OUTPUT_TXT);
//odtwarzanie wyjsciowej tablicy
int i;
for(i=0;i<length;++i)
    A[i]=Tab[i];

//sortowanie szybkie
clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&tp0);
QSortSrings(A,length);
clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&tp1);

Tn=(tp1.tv_sec+tp1.tv_nsec/MLD)-(tp0.tv_sec+tp0.tv_nsec/MLD);
  printf("czas QSort: %3.10lf\n",Tn);
makeOutputFile(length,A, OUTPUT2_TXT);


	return 0;
}
