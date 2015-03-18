#include <stdio.h>
//Domyslne nazwy plikow wejsciowych i wyjsciowych
#define SOURCE_TXT "heap.txt"
#define OUTPUT_TXT "outHeap.txt"

///===================
///Funkcja countElements zwraca liczbe wierszy w pliku Ÿródlowym (liczbe elementow
///kopca)
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
void insertElements (int length,int A [length], FILE* source)
{
    rewind(source);

    int i;

    for(i=0;i<length;++i)
    {
        fscanf(source, "%d", &A[i]);
    }
}

///===================
///Funkcja Heapify nadaje poddrzewu o korzeniu i porzadek kopcowy
///
void Heapify (int A[],int heapSize, int i)
{
    //lewy syn
    int l=2*i+1;
    //prawy syn
    int r=l+1;

    int largest;
    int buff;

    if(l<heapSize && A[l]>A[i])
        largest=l;
    else
        largest=i;

    if(r<heapSize && A[r]>A[largest])
        largest=r;

    if(largest!=i)
    {
                //swap
            buff=A[largest];
            A[largest]=A[i];
            A[i]=buff;

            Heapify(A,heapSize,largest);
    }

}
///===================
///Funkcja ItHeapify nadaje poddrzewu o korzeniu i porzadek kopcowy bez uzycia
///rekursji
///
void ItHeapify (int A[],int heapSize, int i)
{
    int l,r, largest, buff;
    while(i<heapSize)
    {
            //lewy syn
            l=2*i+1;
            //prawy syn
            r=l+1;

            if(l<heapSize && A[l]>A[i])
                largest=l;
            else
                largest=i;

            if(r<heapSize && A[r]>A[largest])
                largest=r;

            if(largest!=i)
            {
                        //swap
                    buff=A[largest];
                    A[largest]=A[i];
                    A[i]=buff;

                    i=largest;
            }
            else
                break;
    }
}

///===================
///Funkcja BuildHeap nadaje tablicy A porzadek kopcowy
///
void BuildHeap (int length, int A[length])
{
    int i;
    int heapSize=length;
    for(i=length/2;i>=0;--i)
        Heapify(A, heapSize,i);
        //ItHeapify(A, heapSize,i);
}

///===================
///Funkcja HeapSort sortuje kopcowo tablice A, elementy o najwiekszej wartosci po
///posortowaniu
///znajduja sie na koncu tablicy
///
void HeapSort (int length, int A[length])
{
    BuildHeap(length, A);
    int i;
    int buff;
    int heapSize=length;
    for(i=length-1;i>0;--i)
    {
        //swap
            buff=A[i];
            A[i]=A[0];
            A[0]=buff;

            --heapSize;
            Heapify(A,heapSize,0);
           //ItHeapify(A,heapSize,0);

    }
}

///===================
///Funkcja makeOutputFile tworzy plik wyjsciowy zawierajacy elementy tablicy A po
///sortowaniu kopcowym
/// w kolejnosci od najmniejszego do najwiekszego (od elementu o indeksie 0 do
///elementu o indeksie length-1)
///
void makeOutputFile (int length, int A[length])
{
    FILE* output;
    output=fopen(OUTPUT_TXT, "w");
    if(output==NULL)
    {
        printf("Nie mo¿na utworzyc pliku wyjsciowego. \n");
        return;
    }
    int i;

    for(i=0;i<length;++i)
        fprintf(output,"%d \n",A[i]);


    fclose(output);
}


int main ()
{
    //Wczytanie pliku ¿ródlowego
    FILE* source;
    source=fopen(SOURCE_TXT, "r");
    if(source==NULL)
    {
        printf("Nie mo¿na wczytac pliku wejsciowego. \n");
        return 1;
    }

    //wyznaczenie dlugosci tablicy
    int length=countElements(source);

    //deklaracja tablicy A
    int A[length];

    //wypelnienie tablicy A
    insertElements(length, A, source);

    //zamkniecie pliku zrodlowego
    fclose(source);

    //sortowanie kopcowe tablicy A
    HeapSort(length,A);

    //utworzenie pliku wyjsciowego
    makeOutputFile(length,A);

    return 0;
}

