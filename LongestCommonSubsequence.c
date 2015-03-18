#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STRING_SIZE 100
#define MAX_RESULTS 100

void PrintAllLCS(int i, int j,char* a,int lengthB, int auxTab[][lengthB], int mainTab[][lengthB], int LCSLength)
{
   static char prevResult[MAX_STRING_SIZE];
   static char temp[MAX_STRING_SIZE];
    if(i==0||j==0)
    {
        return;
    }


    if(auxTab[i][j]==0)
    {
        //printf("%c", a[i]);
        PrintAllLCS(i-1,j-1,a,lengthB,auxTab,mainTab,LCSLength);
        strncat(temp, a+(i-1), 1);
       // printf("temp: %s \n", temp);
        if(strlen(temp)==LCSLength)
        {
            if(strcmp(temp,prevResult)!=0)
            {
                printf("%s \n", temp);
                strcpy(prevResult,temp);
            }

            strcpy(temp, "\0");
        }

       // printf("0 i:%d j:%d \n",i,j);
    }
    if(auxTab[i][j]==1)
    {
        if(mainTab[i-1][j]==mainTab[i][j-1])
            auxTab[i][j]=3;
        PrintAllLCS(i-1,j,a,lengthB,auxTab,mainTab,LCSLength);
    }

    if(auxTab[i][j]==3)
    {
        PrintAllLCS(i,j-1,a,lengthB,auxTab,mainTab,LCSLength);
       // PrintAllLCS(i-1,j,a,lengthB,auxTab,mainTab);
    }

    if(auxTab[i][j]==2)
       PrintAllLCS(i,j-1,a,lengthB,auxTab,mainTab,LCSLength);
}


void LCS(char * a, char* b)
{
	int lengthA=strlen(a)+1;
	int lengthB=strlen(b)+1;
	//printf("%d \n", lengthA);
	//printf("%d \n", lengthB);
	int mainTab[lengthA][lengthB];
	int auxTab[lengthA][lengthB];
	//0-skos
	//1-gora
	//2-lewo
	//3-gora i lewo
	int i,j;

	for(i=0;i<lengthA;++i)
		mainTab[i][0]=0;

	for(i=0;i<lengthB;++i)
		mainTab[0][i]=0;

	for(i=0;i<lengthA;++i)
	for(j=0;j<lengthB;++j)
		auxTab[i][j]=-1;

	for(i=1;i<lengthA;++i)
	{
		for(j=1;j<lengthB;++j)
		{
			if(a[i-1]==b[j-1])
			{
				mainTab[i][j]=mainTab[i-1][j-1]+1;
				auxTab[i][j]=0;
			}
			else if(mainTab[i][j-1]>mainTab[i-1][j])
			{
				mainTab[i][j]=mainTab[i][j-1];
				auxTab[i][j]=2;
			}
			else
			{
				mainTab[i][j]=mainTab[i-1][j];
				auxTab[i][j]=1;
			}
		}
	}
    printf("    ");
    for(i=0;i<lengthB;++i)
	{

			printf("%c ",b[i]);
	}
	printf("\n");
	for(i=0;i<lengthA;++i)
	{
        if(i>0)
            printf("%c ", a[i-1]);
        else
            printf("  ");
		for(j=0;j<lengthB;++j)
		{

			printf("%d ",mainTab[i][j]);
		}
		printf("\n");
	}

	for(i=0;i<lengthA;++i)
	{
		for(j=0;j<lengthB;++j)
		{
			printf("%d ",auxTab[i][j]);
		}
		printf("\n");
	}

	printf("LCS length: %d \n", mainTab[lengthA-1][lengthB-1]);
    PrintAllLCS(lengthA-1, lengthB-1, a, lengthB,auxTab, mainTab,mainTab[lengthA-1][lengthB-1]);

    /*
	for(i=0;i<lengthA;++i)
	{
		for(j=0;j<lengthB;++j)
		{
			printf("%d ",auxTab[i][j]);
		}
		printf("\n");
	}
    */
}





int main () {
	char first[MAX_STRING_SIZE];
	char second[MAX_STRING_SIZE];
	//printf("Podaj pierwszy ciąg: \n");
	//scanf("%s", first);
	//printf("Podaj drugi ciąg: \n");
	//scanf("%s", second);
	strcpy(first, "123456789abc");
	strcpy(second, "abc789456123");
	LCS (first,second);

	return 0;
}
