
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// U OD wariant
#define MAX_WORD_LENGTH 40
#define SOURCE_TXT "3700.txt"

struct Element {
	int liczba;
	char* nazwisko;
};

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
void insertElements (int length,char Tab[length][MAX_WORD_LENGTH],char* A [length], FILE* source)
{
    rewind(source);
    int i;
    for(i=0;i<length;++i)
    {
        fscanf(source, "%s", Tab[i]);
        A[i]=(char*) malloc(sizeof(char)*strlen(Tab[i])+1);
       strcpy(A[i],Tab[i]);
    }

}

int HashFunction(int key, int m){
	return key%m;
}

int XorConversion(char a, char b, int result)
{
	return result^(256*a+b);
}

int MaxColission(int size, int T[size])
{
	int max=T[0];
	int i;
	for(i=1;i<size;++i)
	if(T[i]>max)max=T[i];
	return max;
}

int ZeroPositions(int size, int T[size])
{
	int counter=0;
	int i;
	for(i=0;i<size;++i)
	if(T[i]==0)++counter;
	return counter;
	
}

int AverageColissions(int size, int T[size])
{
	int sum=0;
	int amount=0;
	int i;
	for(i=0;i<size;++i)
	if(T[i]!=0)
	{
		++amount;
		sum=sum+T[i];
	}
	return sum/amount;
}

int ConvertString(char* string)
{
	int i;
	int result=0;
	int length=strlen(string);
		result=256*string[0]+string[1];
		
		for(i=2;i<=length-2;i=i+2)
		{
			result=XorConversion(string[i],string[i+1],result);
		}
		
		if(length%2!=0)
		{
			//printf("%c, \n", string[length-1]);
			result=result^(256*string[length-1]);
		}
		
	if(result>=0)
		return result;
	else
	{
		printf("Blad przy konwersji: %s \n", string);
		return 0;
	}
		
	
}


int main(){
	int m=8677;
	int i;
	int T[m];
	for(i=0;i<m;++i)
	T[i]=0;
	FILE* source;
    source=fopen(SOURCE_TXT, "r");
    if(source==NULL)
    {
        printf("Nie mozna wczytac pliku wejsciowego. \n");
        return 1;
    }
	int tableSize=countElements(source);
	
	char stringTable [tableSize][MAX_WORD_LENGTH];
	char* stringPointerTable [tableSize];
	insertElements(tableSize,stringTable,stringPointerTable,source);
    fclose(source);
	/*
	char* dd="abced";
	char* gg="abcex";
	
	printf("%d \n",ConvertString(dd) );
	printf("%d \n",ConvertString(gg) );
	*/
	int keyTable[tableSize];
	for(i=0;i<tableSize;++i)
	{
		keyTable[i]=ConvertString(stringPointerTable[i]);
	}
	
	for(i=0;i<2*m;++i)
	{
		++T[HashFunction(keyTable[i], tableSize)];
	}
	/*
	for(i=0;i<tableSize;++i)
	{
		printf("Klucz: %d Kolizje: %d \n",i, T[i]);
	}
	*/
	printf("m: %d \n", m);
	printf("Max: %d \n", MaxColission(tableSize, T));
	printf("Liczba pozycji zerowych: %d \n", ZeroPositions(tableSize, T));
	printf("Srednia: %d \n", AverageColissions(tableSize, T));
	
	return 0;
}
