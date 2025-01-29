typedef struct carta{
  char face[3];
  char naipe;
  int valor;
  char nome[18];
  char naipeP[4];
}Tcarta;

typedef struct _no{
  Tcarta carta;
  void *prox;
}Tno;

Tno *CriaLista();
int ListaVazia(Tno *lista);
void LiberaLista(Tno **lista);
Tno *CriaNo(Tcarta carta);
void InsereNo(Tno **lista, Tno *novoNo, int pos);
Tno *RemoveNo(Tno **lista, int pos);
Tno *AcessaNo(Tno *lista, int pos);
void MudaPos(Tno **lista, int posOrig, int posDest);
void MudaLista(Tno **listaOrig, Tno **listaDest, int tamanho);
int TamanhoLista(Tno *lista);
void SorteiaCarta(Tno **listaOrig, Tno **listaDest, int tamanho);
int ContaBonus(Tno *lista, int tamanho);
void ImprimeLista(Tno *lista);
void ImprimeListaP(Tno *lista);