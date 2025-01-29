#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

tFila *CriaFila(){
  tFila *aux = malloc(sizeof(tFila));
  aux->prim = NULL;
  aux->ult = NULL;
  return aux;
}

tNoFila *CriaNoFila(tNoFila nosolto){
  tNoFila *aux = malloc(sizeof(tNoFila));
  aux->turno = nosolto.turno;
  aux->prazo = nosolto.prazo;
  aux->paus = nosolto.paus;
  aux->espadas = nosolto.espadas;
  aux->ouros = nosolto.ouros;
  aux->copas = nosolto.copas;
  aux->premio = nosolto.premio;
  aux->prox = NULL;
  return aux;
}

int FilaVazia(tFila *fila){
  if (fila->prim == NULL && fila->ult == NULL){
    return 1;
  }else{
    return 0;
  }  
}

void LiberaFila(tFila *fila){
  tNoFila *aux;
  fila->ult = NULL;
  while (!FilaVazia(fila)) {
    aux = fila->prim->prox;
    free(fila->prim);
    fila->prim = aux;
  }
  free(fila);
}

void InsereNoFila(tFila *fila, tNoFila *novoNo){
  if(FilaVazia(fila)){
    fila->prim = novoNo;
    fila->ult = novoNo;
  }else{
    fila->ult->prox = novoNo;
    fila->ult = novoNo;
  }
}

tNoFila *RemoveNoFila(tFila *fila){
  tNoFila *aux;
  if (!FilaVazia(fila)){
    if(TamanhoFila(fila)==1){
      aux = fila->prim;
      fila->prim = fila->prim->prox;
      fila->ult = NULL;
      aux->prox = NULL;
      return aux;
    }else{
      aux = fila->prim;
      fila->prim = fila->prim->prox;
      aux->prox = NULL;
      return aux;
    }
  }
  return NULL;
}

tNoFila *AcessaPrimFila(tFila *fila){
  tNoFila *aux = fila->prim;
  return aux;
}

int TamanhoFila(tFila *fila){
  int i=1;
  tNoFila *auxprim = fila->prim, *auxult = fila->ult;
  if(FilaVazia(fila)){
    return 0;
  }else{
    if(fila->prim == fila->ult){
      return 1;
    }else{
      while(auxprim != auxult){
        auxprim = auxprim->prox;
        i++;
      }
      return i;
    }
  }
}

void ImprimePrimFila(tFila *fila){
  tNoFila *aux = fila->prim;
  if (!FilaVazia(fila)){
    printf("(P:%d, E:%d, O:%d, C:%d).",aux->paus,aux->espadas,aux->ouros,aux->copas);
  }
}

void ImprimePrimFilaP(tFila *fila){
  tNoFila *aux = fila->prim;
  if(aux != NULL){
    printf("(\u2663:%d, \u2660:%d, \033[1;31m\u2666\033[0m:%d, \033[1;31m\u2665\033[0m:%d). ",aux->paus,aux->espadas,aux->ouros,aux->copas);
  }
}