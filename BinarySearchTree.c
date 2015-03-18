#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
typedef struct Key{
    int day;
    int month;
}Key;

typedef struct Node{
    Key key;
    double data; //Kurs
    struct Node *left;
    struct Node *right;
}Node;

///Funkcja CompareKeys porownuje wartosci kluczy,
/// zwraca 0 gdy argumenty sa rowne, 1 gdy wiekszy jest pierwszy argument
/// , 2 gdy wiekszy jest drugi argument
int CompareKeys(Key arg1, Key arg2){
    if(arg1.month==arg2.month)
    {
        if(arg1.day==arg2.day)
            return 0;
        else if(arg1.day>arg2.day)
            return 1;
        else
            return 2;
    }
    else
    {
        if(arg1.month>arg2.month)
            return 1;
        else return 2;
    }
}

///Funkcja zwraca rozmiar drzewa
int TreeSize(Node *root){
    if(root==NULL)
    return 0;
return(TreeSize(root->left)+1+TreeSize(root->right));
}


int DLListSize(Node *root){
	if(root==NULL)
    return 0;
return(1+TreeSize(root->right));
}
Node* FindMin(Node *node)
{
    //W drzewie nie ma wezla podanego w argumencie
        if(node==NULL)
            return NULL;
        //Przejscie do lewego poddrzewa
        if(node->left)
                return FindMin(node->left);
         //zwrocenie wezla podanego w argumencie, gdy lewy syn nie istnieje
        else
                return node;
}
Node* FindMax(Node *node)
{
        //W drzewie nie ma wezla podanego w argumencie
        if(node==NULL)
                return NULL;
        //Przejscie do prawego poddrzewa
        if(node->right)
                return FindMax(node->right);
        //zwrocenie wezla podanego w argumencie, gdy prawy syn nie istnieje
        else
                return node;
}

Node * Insert(Node *node, Key key,double data)
{
        if(node==NULL)
        {
                Node *temp;
                temp = (Node *)malloc(sizeof(Node));
                temp -> data = data;
                temp->key=key;
                temp -> left = temp -> right = NULL;
                return temp;
        }

        if(CompareKeys(key, node->key)==1)
        {
                node->right = Insert(node->right,key,data);
        }
        else if(CompareKeys(key, node->key)==2)
        {
                node->left = Insert(node->left,key,data);
        }
       //Gdy dane znajduja sie juz w drzewie zwroc wezel
        return node;

}


Node * Delete(Node *node, Key key)
{
        Node *temp;
        if(node==NULL)
        {
                printf("Nie znaleziono elementu o kluczu: \n Dzien: %d, Miesiac: %d", node->key.day, node->key.month);
        }
        else if(CompareKeys(key, node->key)==2)
        {
                node->left = Delete(node->left, key);
        }
        else if(CompareKeys(key, node->key)==1)
        {
                node->right = Delete(node->right, key);
        }
        else
        {
                   //Usuwanie wezla i przejscie na jego miejsce najmniejszego elementu z
                   // prawego poddrzewa, usuniecie prawego syna
                if(node->right && node->left)
                {
                        temp = FindMin(node->right);
                        node -> data = temp->data;
                        node -> right = Delete(node->right,temp->key);
                }
                else
                {
                        //Przypadek, gdy jest tylko jeden syn, lub nie ma zadnego,

                        temp = node;
                        if(node->left == NULL)
                                node = node->right;
                        else if(node->right == NULL)
                                node = node->left;
                        free(temp);
                }
        }
        return node;

}

Node * Find(Node *node, Key key)
{

        if(node==NULL)
        return NULL;
         // Szukanie w prawym poddrzewie
        if(CompareKeys(key, node->key)==1)
                return Find(node->right,key);

        // Szukanie w lewym poddrzewie
        else if(CompareKeys(key, node->key)==2)
                return Find(node->left,key);
        //koniec szukania
        else
            return node;
}


void PrintInorder(Node *node)
{
        if(node==NULL)
        {
                return;
        }
        PrintInorder(node->left);
        printf("Miesiac: %d Dzien: %d Kurs: %lf \n",node->key.month,node->key.day,node->data);
       // if(node->left)
        //printf("Lisc lewy:(m:%d, d:%d) \n", node->left->key.month,node->left->key.day);
        //if(node->right)
        //    printf("Lisc prawy:(m:%d, d:%d) \n", node->right->key.month,node->right->key.day);
        PrintInorder(node->right);
}

///Generowanie losowego klucza
Key CreateRandomKey(){
    Key key;
    key.day=(rand()%31+1);
    key.month=(rand()%12+1);
    return key;
}

double CreateRandomData(int seed){
    int sign=rand()%2;
    if(sign==1)
        return (double)(seed-seed*rand()%11);
    else
    return (double)(seed+seed*rand()%11);
}

Node* ConvertBinaryTreeToDLListUtil(Node* root)
{
    if (root == NULL)
        return root;
    // Konwersja lewego poddrzewa jesli istnieje
    if (root->left != NULL)
    {
        Node* left = ConvertBinaryTreeToDLListUtil(root->left);
        while(left->right!=NULL)
            left=left->right;

        left->right = root;

        root->left = left;
    }

    // Konwersja prawego poddrzewa
    if (root->right!=NULL)
    {
        Node* right = ConvertBinaryTreeToDLListUtil(root->right);

        while(right->left!=NULL)
            right=right->left;

        right->left = root;

        root->right = right;
    }

    return root;
}
///Funkcja przeksztalca drzewo binarne w dwukierunkowe listy dowiazaniowe
Node* ConvertBinaryTreeToDLList(Node *root)
{
    // Base case
    if (root == NULL)
        return root;
   // PrintInorder(root);


    root = ConvertBinaryTreeToDLListUtil(root);
    //Przesuniecie korzenia na skrajnie lewa pozycje w liscie
    while (root->left != NULL)
        root = root->left;

    return (root);
}
///Funkcja wypisuje zawartosc dwukierunkowej listy dowiazaniowej
void PrintDLList(Node *arg)
{
    Node *pointer=arg;
    while(pointer!=NULL)
    {
     printf("Miesiac: %d Dzien: %d Kurs: %lf \n",pointer->key.month,pointer->key.day,pointer->data);
     pointer=pointer->right;
    }
}
/*
Node* ConvertDLListToBinaryTreeUtil(Node *root, Node *arg)
{
	if(arg==NULL)
	return NULL;
	else
	{
		Insert(root, arg->key,arg->data);
		ConvertDLListToBinaryTreeUtil(root,arg->left);
		ConvertDLListToBinaryTreeUtil(root,arg->right);
	}
	return root;
}
*/
Node* MoveLeft(Node *arg, int value)
{
    int i;
    for(i=0;i<value;++i)
    {
        arg=arg->left;
        if(!arg)
            break;
    }
    return arg;
}

Node* MoveRight(Node *arg, int value)
{
    int i;
    for(i=0;i<value;++i)
    {
        arg=arg->right;
        if(!arg)
            break;
    }
    return arg;
}

void ConvertDLListToBinaryTreeUtil(Node *arg, int shift)
{

    printf("arg: %d.%d shift: %d \n", arg->key.day,arg->key.month, shift);
    if(shift==0)
    {
        return;
    }
    Node *leftPointer;
    Node *rightPointer;
    leftPointer=MoveLeft(arg, shift);
    rightPointer=MoveRight(arg, shift);
    arg->left=leftPointer;
    arg->right=rightPointer;
    ConvertDLListToBinaryTreeUtil(leftPointer, shift/2);
    ConvertDLListToBinaryTreeUtil(rightPointer, shift/2);
}

Node* ConvertDLListToBinaryTree(Node *arg)
{

    Node *root=NULL;
    Node *pointer=arg;

    /*
    while(pointer!=NULL)
    {
     root=Insert(root, pointer->key, pointer->data);
     pointer=pointer->right;
    }
    return root;
    */

    int mid=DLListSize(arg)/2;
    int i;
    pointer=MoveRight(pointer, mid);
    root=Insert(root, pointer->key, pointer->data);
    ConvertDLListToBinaryTreeUtil(root, (mid/2)+1);
	return root;

}
///Funkcja scala dwie dwukierunkowe listy dowiazaniowe
Node* mergeDLLists(Node *a, Node *b)
{
  Node* result = NULL;


  if (a == NULL)
     return(b);
  else if (b==NULL)
     return(a);

int comp=CompareKeys(a->key, b->key);
  if (comp==2||comp==0)
  {
     result = a;
     result->right = mergeDLLists(a->right, b);
  }
  else
  {
     result = b;
     result->right = mergeDLLists(a, b->right);
  }
  return(result);
}



///Funkcja ValidateSearchKey sprawdza, czy klucz jest poprawny(1<=key.day<=31 i 1<=key.month<=12)
///jesli nie, odpowiednio zmienia wartosci klucza

void ValidateSearchKey(Key *key){
    if(key->day==32)
    {
        if(key->month==12)
        {
            key->day=1;
            key->month=1;
        }
        else
        {
        key->day=1;
        ++key->month;
        }

    }
    else if(key->day==0)
    {
        if(key->month==1)
        {
            key->day=31;
            key->month=12;
        }
        else
        {
            key->day=31;
            --key->month;
        }

    }
}
///Funkcja zwraca dane liscia o podanym kluczu lub ich przyblizenie
//jesli jest to mozliwe
double GetDataUtil(Node *node, Key key, int range){
    Node *foundNode=Find(node, key);
    if(foundNode)
        return foundNode->data;
    //Wyznaczenie przyblizenia
    else
    {
        int i;
        Key searchKey;
        searchKey.day=key.day;
        searchKey.month=key.month;

        Node *next;
        Node *prev;
        int neighbourFound=0;

        //szukanie w przod
        for(i=0;i<range;++i)
        {
            ++searchKey.day;
            ValidateSearchKey(&searchKey);
            next=Find(node, searchKey);
            if(next)
            {
                neighbourFound=1;
                break;
            }
        }
        //jesli znaleziono wezel w granicach szukania, szukanie w druga strone
        if(neighbourFound==1)
        {
            for(i=0;i<range;++i)
            {
                --searchKey.day;
                ValidateSearchKey(&searchKey);
                prev=Find(node, searchKey);
                if(prev)
                {
                    neighbourFound=1;
                    break;
                }
            }
            if(neighbourFound==1)
                return (prev->data+next->data)/2;
            else
                return -1;
        }
        else
            return -1;
    }
}

void GetDataOrApprox(Node *node, Key key, int range)
{

    double result=GetDataUtil(node,key, range);
    if(result==-1)
        printf("Nie mozna uzyskac zadanej wartosci, ani jej przyblizenia. \n Zbyt malo elementow w drzewie lub zbyt maly zasieg szukania sasiednich wezlow. \n");
    else
        printf("Znaleziona wartosc (lub przyblizenie): %lf (Dla miesiac: %d, dzien: %d) \n", result, key.month, key.day);
}
///Wyswietlenie informacji o kursie akcji w danym przedziale czasowym
void GetDataInTimeSpan (Node *node, Key startKey, Key endKey)
{
    if(CompareKeys(startKey,endKey)==1)
    {
        printf("Podana data pocz¹tkowa jest pó¿niejsza od koñcowej");
        return;
    }
    if(CompareKeys(startKey,endKey)==0)
    {
		GetDataOrApprox(node, startKey, 50);
		return;
	}

      Key searchKey;
    searchKey.day=startKey.day;
    searchKey.month=startKey.month;
    Node *foundNode;
    printf("Wartosci akcji w okresie od %d.%d do %d.%d \n", startKey.day, startKey.month, endKey.day, endKey.month);
    while(CompareKeys(searchKey, endKey)!=0)
    {
        foundNode=Find(node, searchKey);
        if(foundNode)
            printf("Miesiac : %d Dzien: %d Kurs: %lf \n", searchKey.month, searchKey.day, foundNode->data);

        ++searchKey.day;
        ValidateSearchKey(&searchKey);
    }
}


void TestTree(Node *root)
{
     //PrintInorder(root);
        printf("\n");
        Key searchKey;
        searchKey.day=10;
        searchKey.month=6;
        GetDataOrApprox(root, searchKey, 50);
        printf("\n");
        Key startKey;
        startKey.day=10;
        startKey.month=2;
        Key endKey;
        endKey.day=12;
        endKey.month=6;
        GetDataInTimeSpan(root, startKey, endKey);
        printf("\n");

}
int main(){
    Node *root = NULL;
    srand(time(NULL));
    int i;
    int seed=rand()%200+1;
    for(i=0;i<20;++i)
        root=Insert(root, CreateRandomKey(), CreateRandomData(seed));
   // TestTree(root);
    seed=rand()%200+1;
    Node *root2=NULL;
    for(i=0;i<25;++i)
        root2=Insert(root2, CreateRandomKey(), CreateRandomData(seed));

        TestTree(root);
    printf("Pierwsze drzewo: \n");
    PrintInorder(root);
    Node *list1=ConvertBinaryTreeToDLList(root);
        ////PrintDLList(list1);
        printf("Drugie drzewo: \n");
        PrintInorder(root2);
    Node *list2=ConvertBinaryTreeToDLList(root2);
    //PrintDLList(list2);
    printf("\n");
    Node *list3=mergeDLLists(list1,list2);
    printf(" Scalona lista: \n");
    PrintDLList(list3);
    Node *mergedTree=ConvertDLListToBinaryTree(list3);
    printf(" Scalone drzewo: \n");
    PrintInorder(mergedTree);

return 0;
}
