typedef struct _nofila{
  int turno;
  int prazo;
  int paus;
  int espadas;
  int ouros;
  int copas;
  int premio;
  void *prox;
}tNoFila;

typedef struct _fila{
  tNoFila *prim;
  tNoFila *ult;
}tFila;

tFila *CriaFila();
tNoFila *CriaNoFila(tNoFila nosolto);
int FilaVazia(tFila *fila);
void LiberaFila(tFila *fila);
void InsereNoFila(tFila *fila, tNoFila *novoNo);
tNoFila *RemoveNoFila(tFila *fila);
tNoFila *AcessaPrimFila(tFila *fila);
int TamanhoFila(tFila *fila);
void ImprimePrimFila(tFila *fila);
void ImprimePrimFilaP(tFila *fila);