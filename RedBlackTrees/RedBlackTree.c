#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
typedef struct Node{
    int key;
    int id;
    double data;
    enum{BLACK,RED} color; //0-czarne, 1-czerwone
    struct Node *parent;
    struct Node *left; //lewe dziecko
    struct Node *right; //prawe dziecko
}Node;

//Zmienna zastepujaca NULL
Node* NIL;

void NILInit(){
    NIL= (Node*) malloc(sizeof(Node));
  NIL->parent = NULL;
  NIL->color = BLACK;
  NIL->left = NIL->right = NULL;
}

void InsertRBTree(Node **root, Node *node,Node *parent, int key,double data);
void RotateLeft(Node **root, Node *arg);
void RotateRight(Node **root, Node *arg);
int CompareKeys(int arg1, int arg2);
void PrintInorder(Node *node);
void WriteRBTFileUtil(FILE *file, Node *node);
void WriteRBTFile(FILE *file, Node *node, char *fileName);

int min(int a, int b)
{
    if(b<a)
        return b;
    else
        return a;
}

int max(int a, int b)
{
    if(b>a)
        return b;
    else
        return a;
}

///Funkcja zwraca minimalna glebokosc drzewa
int MinTreeDepth(Node *root)
{
    if(root==NIL)
        return 0;
    else
        return min(MinTreeDepth(root->left), MinTreeDepth(root->right))+1;
}
///Funkcja zwraca maksymalna glebokosc drzewa
int MaxTreeDepth(Node *root)
{
    if(root==NIL)
        return 0;
    else
        return max(MaxTreeDepth(root->left), MaxTreeDepth(root->right))+1;
}
///Funkcja zwraca liczbe czerwonych wezlow w drzewie o korzeniu arg
int RedNodesCount(Node *arg)
{
    if(arg==NIL)
        return 0;
    if(arg->color==RED)
        return(RedNodesCount(arg->left)+1+RedNodesCount(arg->right));
    else
        return(RedNodesCount(arg->left)+RedNodesCount(arg->right));
}


void getPathsLength(Node *root, int paths [])
{
    static int i=0;

    if(root->left!=NIL)
    {
        ++paths[i];
        getPathsLength(root->left, paths);
    }


}



///Funkcja zwraca rozmiar drzewa o korzeniu arg
int TreeSize(Node *arg){
    if(arg==NIL)
    return 0;
return(TreeSize(arg->left)+1+TreeSize(arg->right));
}

/**
* Funkcja CompareKeys porownuje wartosci kluczy,
* zwraca 0 gdy argumenty sa rowne, 1 gdy wiekszy jest pierwszy argument
* , 2 gdy wiekszy jest drugi argument
**/
int CompareKeys(int arg1, int arg2){
	if(arg1==arg2)
		return 0;
    if(arg1>arg2)
		return 1;
	else
		return 2;
}

/**
* Funkcja naprawiajaca drzewo, jesli naruszona zostanie jedna z regul drzewa czerwono-czarnego.
* root - wskaznik na wezel drzewa
* arg - wezel w ktorym nalezy zbadac poprawnosc drzewa
*
**/
void RepairBRTree(Node **root, Node *arg)
{
    while(arg->parent->color==RED)
	{
	    Node *u;//wuj
	    /**
	    *Przypadki, gdy ojciec jest lewym dzieckiem.
	    **/
	    if (arg->parent == arg->parent->parent->left)
	    {
	      u=arg->parent->parent->right; //wyznaczenie wujka
	      /**
	      * Przypadek 1 - wujek jest czerwony:
	      * Pokolorowanie ojca i wujka na czarno, dziadka na czerwono,
	      * ustawienie arg jako swojego dziadka (gdzie drzewo moglo zostac zepsute)
	      **/
	      if (u->color==RED)
	      {
				arg->parent->color=BLACK;
				u->color=BLACK;
				arg->parent->parent->color=RED;
				arg=arg->parent->parent;
			      }
			else
			{
                /**
                * Przypadek 2 - wujek jest czarny, badany wezel jest prawym dzieckiem:
                * arg staje sie swoim ojcem, dokonywana jest rotacja w lewo,
                * ojciec kolorowany jest na czarno, dziadek na czerwono,
                * dokonywana jest rotacja w prawo wzgledem dziadka arg
                **/
				if (arg == arg->parent->right)
				{
				  arg=arg->parent;
				  RotateLeft(root,arg);
				}
				/**
                * Przypadek 3 - wujek jest czarny, badany wezel jest lewym dzieckiem:
                * ojciec kolorowany jest na czarno, dziadek na czerwono,
                * dokonywana jest rotacja w prawo wzgledem dziadka arg
                **/
				arg->parent->color=BLACK;
				arg->parent->parent->color=RED;
				RotateRight(root,arg->parent->parent);
	      }
	    }
	    /**
	    *Przypadki, gdy ojciec jest prawym dzieckiem.
	    **/
	    else
	    {
		      u=arg->parent->parent->left; //wyznaczenie wuja
		      /**
              * Przypadek 1 - wujek jest czerwony:
              * Pokolorowanie ojca i wujka na czarno, dziadka na czerwono,
              * ustawienie arg jako swojego dziadka (gdzie drzewo moglo zostac zepsute)
              **/
		      if (u->color==RED)
		    {
			arg->parent->color=BLACK;
			u->color=BLACK;
			arg->parent->parent->color=RED;
			arg=arg->parent->parent;
		      }
		    else
		    {
		        /**
                * Przypadek 2 - wujek jest czarny, badany wezel jest lewym dzieckiem:
                * arg staje sie swoim ojcem, dokonywana jest rotacja w prawo,
                * ojciec kolorowany jest na czarno, dziadek na czerwono,
                * dokonywana jest rotacja w lewo wzgledem dziadka arg
                **/
				if (arg == arg->parent->left)
				{
				  arg=arg->parent;
				  RotateRight(root,arg);
				}
				/**
                * Przypadek 3 - wujek jest czarny, badany wezel jest prawym dzieckiem:
                * ojciec kolorowany jest na czarno, dziadek na czerwono,
                * dokonywana jest rotacja w lewo wzgledem dziadka arg
                **/
				arg->parent->color=BLACK;
				arg->parent->parent->color=RED;
				RotateLeft(root,arg->parent->parent);
	      }
	    }
    }
    //kolorowanie korzenia na czarno po zakonczeniu naprawy drzewa
    (*root)->color=BLACK;

}

/**
*Funkcja wstawiajaca nowy wezel do drzewa czerwono-czarnego
* root - korzen drzewa
* node - obecnie badany wezel (wstawienie nastepuje gdy node ma wartosc NIL)
* parent - ojciec node
* key - klucz nowego wezla
* data - dane nowego wezla
**/


void InsertRBTree(Node **root, Node *node,Node *parent, int key,double data)
{
            if(*root==NULL) //ustawienie wezla jako korzen, gdy korzen ma wartosc NULL
            {
                Node *temp;
                temp = (Node *)malloc(sizeof(Node));
                temp -> data = data;
                temp->key=key;
                temp -> left = temp -> right = NIL;
                temp -> parent = parent;
                temp->id=0;
                (*root)=temp;
                temp->parent=NIL;
                temp->color=BLACK;
            }
        else if(node==NIL) //wstawienie nowego wezla do drzewa, jesli obecnie przegladany wezel jest pusty
        {

            static int counter=1;
            printf("Wstawione wezly: %d \n", counter);
                Node *temp;
                temp = (Node *)malloc(sizeof(Node));
                temp -> data = data;
                temp->key=key;
                temp -> left = temp -> right = NIL;
                temp -> parent = parent;
                temp->id=counter;
                if(CompareKeys(key, parent->key)==1)
                    parent->right=temp;
                else
                    parent->left=temp;
                temp -> color = RED; //kolorwanie nowego wezla na czerwono

                    RepairBRTree(root, temp); //sprawdzenie poprawnosci drzewa po dodaniu nowego wezla
                ++counter; //zwiekszenie licznika wstawionych wezlow

        }

        else if(CompareKeys(key, node->key)==1) //przejscie w prawo
        {
                InsertRBTree(root, node->right, node, key, data);
        }
        else if(CompareKeys(key, node->key)==2) //przejscie w lewo
        {
                InsertRBTree(root, node->left, node, key, data);
        }
        else //pominiecie wstawienia, jesli w drzewie znajduje sie wezel o tym samym kluczu
        {
            printf("Pominiety klucz: %d \n", key);
            return;
        }


}
// Rotacja w lewo
void RotateLeft(Node **root, Node *x)
{
    if(x->right!=NIL)
    {
        Node *y=x->right; //y wskazuje na prawego syna x
        x->right=y->left; //prawy syn x zostaje zamieniony przez lewego syna y
        if(y->left!=NIL) //jesli lewy syn y nie byl pusty, ojcem lewego syna staje sie x
            y->left->parent=x;
        y->parent=x->parent; //ojcem y staje sie ojciec x
        if(x->parent==NIL) //jesli x byl korzeniem, y staje sie nowym korzeniem
        {
            (*root)=y;
        }
        else if(x==x->parent->left) //jesli x byl lewym synem, y staje sie lewym synem ojca x
                x->parent->left=y;
        else x->parent->right=y; //jesli x byl prawym synem, y staje sie lewym synem ojca x
        y->left=x; //podpiecie x jako lewego syna x
        x->parent=y; //y staje sie ojcem x
    }
}

// Rotacja w prawo
void RotateRight(Node **root,Node *y)
{
    if(y->left!=NIL)
    {
        Node *x = y->left;
        y->left = x->right;
        if (x->right != NIL)
            x->right->parent = y;
        x->parent =y->parent;
        if (x->parent == NIL)
        {
            (*root) = x;
        }

        else if (y == y->parent->left)
            y->parent->left = x;
        else y->parent->right = x;
        x->right = y;
        y->parent = x;
    }
}

/**
*Zapisanie struktury drzewa czerwono-czarnego w postaci pliku tekstowego. Delimiter koñca pliku to
*"END_OF_GRAPH", delimiter koñca opisu wêzla to "END".
* file - uchwyt do pliku
* node - korzen
* fileName - nazwa pliku
**/
void WriteRBTFile(FILE *file, Node *node, char *fileName)
{
    file=fopen(fileName, "w");
    char buff [10];
    sprintf(buff, "%d", TreeSize(node));
    printf("Rozmiar drzewa: %d \n",TreeSize(node));
    fprintf(file, buff);
    fprintf(file, "\n");
    WriteRBTFileUtil(file, node);
    fprintf(file, "END_OF_GRAPH");
    fclose(file);
}
/**
*Zapisanie pojedynczego wezla
*
**/
void WriteRBTFileUtil(FILE *file, Node *node)
{
            if(node==NIL)
        {
                return;
        }
        char buff[20];
        int i;
        for(i=0;i<20;++i)
            buff[i]='0';
        WriteRBTFileUtil(file, node->left);

        fprintf(file, "ID ");
        sprintf(buff, "%d", node->id);
        fprintf(file, buff);
        fprintf(file, "\n");

        fprintf(file, "KEY ");
        sprintf(buff, "%d", node->key);
        fprintf(file, buff);
        fprintf(file, "\n");

        fprintf(file, "COLOR ");
        sprintf(buff, "%d", node->color);
        fprintf(file, buff);
        fprintf(file, "\n");

        fprintf(file, "PARENT ");
        if(node->parent!=NIL)
            sprintf(buff, "%d", node->parent->id);
        else
           strcpy(buff, "-1");
        fprintf(file, buff);
        fprintf(file, "\n");

        fprintf(file, "LEFT ");
        if(node->left!=NIL)
            sprintf(buff, "%d", node->left->id);
        else
            strcpy(buff, "-1");
        fprintf(file, buff);
        fprintf(file, "\n");

        fprintf(file, "RIGHT ");
        if(node->right!=NIL)
            sprintf(buff, "%d", node->right->id);
        else
            strcpy(buff, "-1");
        fprintf(file, buff);
        fprintf(file, "\n");

        fprintf(file, "END");
        fprintf(file, "\n");
        WriteRBTFileUtil(file, node->right);
}

void PrintInorder(Node *node)
{
        if(node==NIL)
        {
                return;
        }
        PrintInorder(node->left);
        printf("ID: %d Klucz: %d  Wartosc: %lf \n",node->id, node->key,node->data);

        printf("\n");
        PrintInorder(node->right);
}

int CreateRandomKey(){return rand()%100;}
double CreateRandomData(){return rand()%200;}
///*
int main(){
    NILInit();
    Node *root = NULL;
    srand(time(NULL));
    int i;
    for(i=0;i<40;++i)
        InsertRBTree(&root,root, root, CreateRandomKey(), CreateRandomData());
    PrintInorder(root);
    printf("Liczba czerwonych wezlow: %d \n", RedNodesCount(root));
    printf("Maksymalna glebokosc drzewa: %d \n", MaxTreeDepth(root));
    printf("Minimalna glebokosc drzewa: %d \n", MinTreeDepth(root));
    FILE *file;
    WriteRBTFile(file, root, "RBTOutput.txt");
	return 0;
}
//*/

