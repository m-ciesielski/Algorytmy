//                                          pmp@inf.univ.gda.pl  2014
// drukowanie drzew cz-cz z wartownikiem wspolnym
// dla wszystkich wezlow:
// drukowanie na ekran tekstowy i przez dot

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define RED 1        /* stala oznaczajaca kolor wezla */
#define BLACK 0      /* stala oznaczajaca kolor wezla */
#define SZER_EKR 80  /* szerokosc ekranu */
#define IL_POZ   5   /* ilosc poziomow drzewa, ktore beda wydrukowane   */
                     /* gwiazdka bedzie sygnalizowac istnienie nizszych */
                     /* poziomow                                        */

/* struktury danych do reprezentowania drzewa */
typedef struct wezel *Wskwezla; /* wskaznik na wezel drzewa  */
typedef struct wezel{
	int klucz;
	Wskwezla left, right, p;
	int kolor;
} Twezla ;  /* typ wezla */

/* drzewa z wartownikami: wezel wskazywany przez "nil" jest wartownikiem
   zastepujacym NULL; dla korzenia pole "p" ma wartosc "nil";
   pole nil->p musi byc ustawione odpowiednio w przy usuwaniu
*/
Wskwezla nil;

void nilInit(void){
/* funkcja inicjujaca nil; musi byc wywolana przed budowaniem drzewa */
  nil= (Wskwezla) malloc(sizeof(Twezla));
  nil->p = NULL;
  nil->kolor = BLACK;
  nil->left = nil->right = NULL;
}

void drukuj(Wskwezla w);
/* funkcja drukujaca drzewo binarne na ekranie (tutaj tylko deklaracja) */
/* funkcja drukuje drzewo o korzeniu "w"                                */

void drukujDot(Wskwezla r);
  // generuje w pliku drzewo1.gv lub drzewo0.gv (naprzemiennie)
  // opis drzewa o korzeniu r do wydrukowania przez program dot
  // zlecenie "dot -Tpdf -O drzewo1.gv" utworzy plik "drzewo1.gv.pdf"

/* ------------  implementacja ------------------------------------- */
char wydruk[IL_POZ+1][SZER_EKR];

 void drukujost(Wskwezla w, int l, int p,int poziom){
       int srodek = (l+p)/2;
       if (w==nil)   return;
       wydruk[poziom][srodek]='*';
}

void drukujwew(Wskwezla w, int l, int p,int poziom){
       int srodek = (l+p)/2;
       int i,dl;
       char s[19];
       if (w==nil)    return;
       if (w->kolor==BLACK)
         dl=sprintf(s,"%d",w->klucz);
       else
	 //	        dl=sprintf(s,"\e[31m%+d\e[0m",w->klucz);
       dl=sprintf(s,"%+d",w->klucz);
       for (i=0;i<dl;i++)
         wydruk[poziom][srodek-dl/2+i]=s[i];
       if (++poziom<IL_POZ){
         drukujwew(w->left,l,srodek,poziom) ;
         drukujwew(w->right,srodek+1,p,poziom) ;
       }
       else {
         drukujost(w->left,l,srodek,poziom) ;
         drukujost(w->right,srodek+1,p,poziom) ;
       }
}

void drukuj(Wskwezla w){
  int j,i;
  for (i=0;i<=IL_POZ;i++)
    for (j=0;j<SZER_EKR;j++)
      wydruk[i][j] = ' ';
  drukujwew(w,0,SZER_EKR,0);
  for (i=0;i<=IL_POZ;i++){
      for (j=0;j<SZER_EKR;j++)
        putchar(wydruk[i][j]);
      printf("\n");
  }
}

void drukujKrawedz(FILE *plikwy, Wskwezla r, int z, Wskwezla syn, int zs){
// wezel r o numerze z jest juz wydrukowany
// teraz drukujemy jego syna "syn" o numerze zs oraz krawedz miedzy nimi
  if (syn == nil){
    fprintf(plikwy,"%d [shape=circle, style=invisible, label=\"",zs);
    fprintf(plikwy,"%d ",0);
    fprintf(plikwy,"\"]\n");
    fprintf(plikwy,"%d -- %d [style=invis];\n ",z,zs);
  } else{
    if (syn->kolor == RED)
      fprintf(plikwy,"%d [shape=circle, color=red, label=\"",zs);
    else
      fprintf(plikwy,"%d [shape=circle, color=black, label=\"",zs);
    fprintf(plikwy,"%d ",syn->klucz);
    fprintf(plikwy,"\"]\n");
    fprintf(plikwy,"%d -- %d ;\n",z,zs);
  }
}

int rekDrukujDot(Wskwezla r, int z, FILE *plikwy){
  // drukuje drzewo o korzeniu r
  // z  - numer wezla r
  // zwraca najwiekszy numer wezla w poddrzewie, ktorego jest korzeniem
  // zakladamy, ze korzen r jest juz wydrukowany
  int nz,i;
  if (r == nil) return z;
  else{
    drukujKrawedz(plikwy,r,z,r->left,z+1);
    nz=rekDrukujDot(r->left,z+1,plikwy);
    drukujKrawedz(plikwy,r,z,r->right,nz+1);
    nz=rekDrukujDot(r->right,nz+1,plikwy);
    return nz;
  }
}

void drukujDot(Wskwezla r){
  // generuje w pliku drzewo1.gv lub drzewo0.gv (naprzemiennie)
  // opis drzewa o korzeniu r do wydrukowania przez program dot
  // zlecenie "dot -Tpdf -O drzewo1.gv" utworzy plik "drzewo1.gv.pdf"
  static int gdzie=0;
  FILE *plikwy;
  if (gdzie) {
    plikwy=fopen("drzewo1.gv","w");
    gdzie=0;
  }else{
    plikwy=fopen("drzewo0.gv","w");
    gdzie=1;
  }
  fprintf(plikwy, "graph drzewo{\n");
  fprintf(plikwy, "size = \"20,200\"");
  if (r!=nil){
    if (r->kolor == RED)
      fprintf(plikwy,"%d [shape=circle, color=red, label=\"",0);
    else
      fprintf(plikwy,"%d [shape=circle, color=black, label=\"",0);
    fprintf(plikwy,"%d ",r->klucz);
    fprintf(plikwy,"\"]\n");
    rekDrukujDot(r,0,plikwy);
  }
  fprintf(plikwy, "}\n");
  fclose(plikwy);
  printf("wydrukowane drzewo%d.gv\n",(gdzie+1)%2);
}

/* ------------- miejsce na inne funkcje -------------------- */
Wskwezla nowyWezel(int klucz, int kolor){
// tworzy nowy wezel. Przydatne do testowania wydruku
     Wskwezla w = (Wskwezla) malloc(sizeof(Twezla));
     w->p = nil;
     w->klucz=klucz;
     w->kolor = kolor;
     w->left = w->right = nil;
     return w;
}

typedef struct wezel2{
    int key;
    int id;
    //double data;
    int color; //0-czarne, 1-czerwone
    int parent; //id ojca
    int left; //id lewego dziecka
    int right; //id prawego dziecka
}wezel2;
/**
* Funkcja tworzy i drukuje drzewo z podanego w argumencie pliku tekstowego. Na podstawie danych zczytanych z pliku
* tworzona jest tablica(tree) wezlow posredniczacych(wezel2),
* zawierajaca strukture drzewa czerwono-czarnego. Wezel posredniczacy(wezel2)posiada
* wlasny identyfikator(id) i identyfikatory polaczonych z nim wezlow(ojca i dzieci), co umo¿liwia rekonstrukcje drzewa.
* Tablica wezlow posredniczacych  jest kopiowana do tablicy sortedTree w taki sposob, ze
* indeks elementu tablicy odpowiada polu id tego elementu.
* Z tablicy sortedTree odtwarzane jest drzewo, ktorego elementami sa wezly docelowe(wezel).
* Drzewo jest drukowane na ekran i konwertowane do pliku gv za pomoca funkcji drukuj i drukujDot.
*
* zrodlo - wskaznik do pliku
* nazwaPliku - nazwa pliku z zapisanym drzewem
**/
void stworzDrzewo(FILE *zrodlo, char *nazwaPliku)
{
    int tree_size;
    zrodlo=fopen(nazwaPliku, "r");
    fscanf(zrodlo, "%d", &tree_size);
    char string_buff [80];
    int value_buff;
    wezel2 tree [tree_size];
    int i=0;
    while(strcmp(string_buff, "END_OF_GRAPH")!=0)
    {
        fscanf(zrodlo, "%s", string_buff);
        fscanf(zrodlo, "%d", &value_buff);
            if(strcmp(string_buff, "ID")==0)
                tree[i].id=value_buff;
            else if(strcmp(string_buff, "KEY")==0)
                    tree[i].key=value_buff;
            else if(strcmp(string_buff, "COLOR")==0)
                    tree[i].color=value_buff;
            else if(strcmp(string_buff, "PARENT")==0)
                    tree[i].parent=value_buff;
            else if(strcmp(string_buff, "LEFT")==0)
                    tree[i].left=value_buff;
            else if(strcmp(string_buff, "RIGHT")==0)
                    tree[i].right=value_buff;
            else if(strcmp(string_buff, "END")==0)
                    ++i;
    }
    Wskwezla korzen;
    printf("i: %d \n ",i);
    Wskwezla treeKonw [tree_size];
    //posortowanie tablicy posredniczacych wezlow wg ID
    wezel2 sortedTree [tree_size];
    for(i=0;i<tree_size;++i)
    {
        sortedTree[tree[i].id]=tree[i];
    }
    //utworzenie docelowych wezlow
    for(i=0;i<tree_size;++i)
    {
        treeKonw[i]=nowyWezel(sortedTree[i].key, sortedTree[i].color);
       // printf("converted: i: %d kolor: %d klucz: %d \n ",i, treeKonw[i]->kolor, treeKonw[i]->klucz);
    }


    for(i=0;i<tree_size;++i)
    {
        if(sortedTree[i].left!=-1)
            treeKonw[i]->left=treeKonw[sortedTree[i].left];
        else
            treeKonw[i]->left=nil;
        if(sortedTree[i].right!=-1)
            treeKonw[i]->right=treeKonw[sortedTree[i].right];
        else
            treeKonw[i]->right=nil;
        if(sortedTree[i].parent!=-1)
            treeKonw[i]->p=treeKonw[sortedTree[i].parent];
        else
        {
            treeKonw[i]->p=nil;
             korzen=treeKonw[i];
        }
    }
    drukuj(korzen);
     drukujDot(korzen);
    fclose(zrodlo);
}


/* ----------------- program testujacy -----------------------*/

///*
int main(){
     nilInit();
    FILE *plik;
    stworzDrzewo(plik, "RBTOutput.txt");
    return 0;
}
//*/





