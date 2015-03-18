///Wariant [U+OD]
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NAME_LENGTH 40
#define SOURCE_TXT "nazwiska.txt"
#define SOURCE2_TXT  "nazwiska2.txt"
struct NameStats {
	int number;
	char name [MAX_NAME_LENGTH];
};

struct NameStatsHash {
    enum fieldStatus{DEL, FREE, FULL};
    enum fieldStatus status;
    struct NameStats* pointer;
};
///===================
///Funkcja countElementsInSourceFile zwraca liczbe wierszy w pliku źródlowym
///
int CountElementsInSourceFile (FILE* source)
{
    int lineCounter=0;
    char buff;
    while(fscanf(source, "%c", &buff)!=EOF)
        if(buff=='\n')
        ++lineCounter;
        return lineCounter+1;
}
///===================
///Funkcja ReadSourceFiles wypelnia tablice Tab wartosciami podanymi w plikach zrodlowych
///
void ReadSourceFiles (int length,struct NameStats Tab[length],FILE* source1, FILE* source2)
{
    rewind(source1);
    int i;
    char buff [MAX_NAME_LENGTH];
    for(i=0;i<length;++i)
    {
        fscanf(source1, "%s", Tab[i].name);
    }
    for(i=0;i<length*2;++i)
    {
        fscanf(source2, "%s", buff);
        if(i%2==0)
        {
            sscanf(buff, "%d", &Tab[i/2].number);
        }

    }

}

int HashFunction(int key, int m){
	return key%m;
}

int SecondHashFunction(int key, int m){
    return 1+(key%(m-2));
}

int ConvertStringToInt(char* string)
{
	int length=strlen(string);
	int result;
	if(length==0)
		return 0;
	else if(length==1)
		return string[1];
	else
	{
		int i;
		result=256*string[0]+string[1];
		for(i=2;i<=length-2;i=i+2)
            result=result^(256*string[i]+string[i+1]);

		if(length%2!=0)
			result=result+string[length-1];
	}

    if(result>=0)
        return result;
  else
	{
		printf("Blad przy konwersji: %s \n", string);
		return 0;
	}
}

void Insert(struct NameStats* arg,struct NameStatsHash table[], int size)
{
    int i;
    int hash;
    int key=ConvertStringToInt(arg->name);
    for(i=0;i<size;++i)
    {
        hash=(HashFunction(key, size)+i*SecondHashFunction(key,size))%size;
        if(table[hash].status!=FULL)
        {
            table[hash].pointer=arg;
            table[hash].status=FULL;
            break;
        }

    }

}

void Delete(struct NameStats* arg,struct NameStatsHash table[], int size)
{
      int i;
    int hash;
    int key=ConvertStringToInt(arg->name);
    int found=0;
    for(i=0;i<size;++i)
    {
        hash=(HashFunction(key, size)+i*SecondHashFunction(key,size))%size;
        if(table[hash].status==FREE)
        {
            found=0;
            break;
        }
        if(table[hash].status==FULL&&table[hash].pointer->name==arg->name)
        {
            table[hash].status=DEL;
            found=1;
            break;
        }

    }
    if(found==0)
        printf("W tablicy nie znaleziono nazwiska: %s .\n", arg->name);
}

void ShowHashTable(struct NameStatsHash hashTable[], int size)
{
    int i;
    for(i=0;i<size;++i)
    {
        switch(hashTable[i].status)
        {
            case FULL:
                {
                    printf("i: %d, Nazwisko: %s, Liczba: %d. \n", i, hashTable[i].pointer->name, hashTable[i].pointer->number);
                    break;
                }

            case DEL:
                {
                    printf("i: %d, Pole ma znacznik DEL. \n", i);
                    break;
                }

            case FREE:
                {
                   printf("i: %d, Pole wolne. \n", i);
                   break;
                }

        }
    }
    printf("------------------------ \n");

}

int main(){
    ///Wczytanie plikow zrodlowych
	FILE* source;
    source=fopen(SOURCE_TXT, "r");
    if(source==NULL)
    {
        printf("Nie mozna wczytac pliku wejsciowego. \n");
        return 1;
    }
	int tableSize=CountElementsInSourceFile(source);
	FILE* source2;
    source2=fopen(SOURCE2_TXT, "r");
    if(source2==NULL)
    {
        printf("Nie mozna wczytac pliku wejsciowego. \n");
        return 1;
    }
	struct NameStats table[tableSize];
	ReadSourceFiles(tableSize,table,source,source2);
    fclose(source);
    fclose(source2);

	///Przygotowanie tablicy do testow operacji Wstaw i Usun
	int i;
	int testTableSize=50;
	struct NameStatsHash testTable [testTableSize];
    for(i=0;i<testTableSize;++i)
        testTable[i].status=FREE;

	///Wypelnienie tablicy testowej do 50%
	for(i=0;i<testTableSize/2;++i)
        Insert(&table[i],testTable,testTableSize);

    ///Wyswietlenie zawartosci tablicy testowej
    printf("Test operacji Wstaw. \n");
    ShowHashTable(testTable,testTableSize);

    ///Test operacji Usun
    for(i=0;i<testTableSize/4;++i)
        Delete(&table[i],testTable,testTableSize);


    printf("Test operacji Usun. \n");
    ///proba usuniecia elementu, ktory nie wystepuje w tablicy
    struct NameStats testName;
    strcpy(testName.name, "lubicz");
    testName.number=12345;
    Delete(&testName,testTable,testTableSize);
    ///Wyswietlenie zawartosci tablicy testowej
    ShowHashTable(testTable,testTableSize);


	///Inicjalizacja tablicy hashTable
	int hashTableSize=(tableSize*5)/4;
    struct NameStatsHash hashTable[hashTableSize];
    for(i=0;i<hashTableSize;++i)
        hashTable->status=FREE;
    int counter=0;
    printf("Test operacji Usun na tablicy z adresowaniem otwartym podwojnym: \n");
    ///Wypelnienie tablicy do 80%
    for(i=0;i<tableSize;++i){
        Insert(&table[i],hashTable, hashTableSize);
        ++counter;
    }
        printf("Wstawiono %d elementow do tablicy. \n", counter);
        counter=0;
        /*
    for(i=0;i<tableSize;++i)
    if(hashTable[i].status==FULL)
    printf("%s, %d \n", hashTable[i].pointer->nazwisko,hashTable[i].pointer->liczba);
    */
    ///Usuniecie polowy wstawionych elementow
    for(i=0;i<tableSize/2;++i){
        Delete(&table[i],hashTable,hashTableSize);
        ++counter;
    }
        printf("Usunieto %d elementow z tablicy. \n", counter);
        counter=0;
    ///Ponowne uzupelnienie do 80%
    for(i=tableSize/2;i<tableSize;++i)
    {
        Insert(&table[i],hashTable,hashTableSize);
        ++counter;
    }
        printf("Wstawiono %d elementow do tablicy. \n", counter);
        counter=0;
    ///Zliczenie pol ze znacznikiem DEL

    for(i=0;i<hashTableSize;++i)
        if(hashTable[i].status==DEL)
        ++counter;
    printf("Pola tablicy ze znacznikiem DEL: %d \n", counter);
	return 0;
}

