#include "listacarta.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int aleatorio(int n) {
  srand(time(NULL));
  if(n==1) return 1;
  return n = rand() % n;
};

Tno *CriaLista() {
  Tno *lista = NULL;
  return lista;
}

int ListaVazia(Tno *lista) {
  if (lista == NULL)
    return 1;
  else
    return 0;
}

void LiberaLista(Tno **lista) {
  Tno *aux;
  while (!ListaVazia(*lista)) {
    aux = (*lista)->prox;
    free(*lista);
    (*lista) = aux;
  }
}

Tno *CriaNo(Tcarta carta){
  Tno *novo = malloc(sizeof(Tno)); 
  strcpy(novo->carta.face, carta.face);
  novo->carta.naipe = carta.naipe;
  novo->carta.valor = carta.valor;
  strcpy(novo->carta.nome, carta.nome);
  strcpy(novo->carta.naipeP, carta.naipeP);
  novo->prox = NULL;
  return novo;
}

void InsereNo(Tno **lista, Tno *novoNo, int pos){
  int i=1;
  Tno *aux, **aux2;
  
  aux2 = lista;
  while(i < pos && (*aux2) != NULL){
    aux2 = (Tno**) &((*aux2)->prox);
    i++;
  }
  aux = novoNo;
  aux->prox = (*aux2);
  (*aux2) = aux;
}

Tno *RemoveNo(Tno **lista, int pos){
  int i;
  Tno *aux, *aux2;
  aux = (*lista);
  if(pos == 1){
    (*lista) = aux->prox;
    aux->prox = NULL;
    return aux;
  } else{
    for(i=1; i<pos-1;i++) aux = aux->prox;
    aux2 = aux->prox;
    aux->prox = aux2->prox;
    aux2->prox = NULL;
    return aux2;
  }
}

Tno *AcessaNo(Tno *lista, int pos){
  int i;
  Tno *aux = lista;
  for(i=0; i<pos-1;i++) aux = aux->prox;
  return aux;
}

void MudaPos(Tno **lista, int posOrig, int posDest){
  Tno *aux = RemoveNo(lista, posOrig);
  InsereNo(lista, aux, posDest);
}

void MudaLista(Tno **listaOrig, Tno **listaDest, int tamanho){
  Tno *aux = RemoveNo(listaOrig, 1);
  InsereNo(listaDest,aux,tamanho);
}

int TamanhoLista(Tno *lista){
  int i=1;
  Tno *aux = lista;
  if(ListaVazia(lista)){
    return 0;
  }else{
    if(aux->prox == NULL){
      return 1;
    }else{
      while(aux->prox != NULL){
        aux = aux->prox;
        i++;
      }
      return i;
    }
  }
}

void SorteiaCarta(Tno **listaOrig, Tno **listaDest, int tamanho){
  Tno *aux;
  if(tamanho>0) InsereNo(listaDest,RemoveNo(listaOrig, aleatorio(tamanho)),5);
}

int ContaBonus(Tno *lista, int tamanho){
  int i,pt=0,sequencia=0,naipe=0,np=0,ne=0,no=0,nc=0;
  Tcarta aux[5];
  
  //bota na variavel auxiliar pra facilitar a visualização da conta
  for(i=0;i<tamanho;i++) aux[i] = AcessaNo(lista,i+1)->carta;

  for(i=0;i<tamanho;i++){

    //testando se é par
    if(aux[i].valor == aux[i+1].valor && i+1<tamanho){
      pt+=2;
      
      //se for um par, entao testa se é uma trinca
      if(aux[i].valor == aux[i+2].valor && i+2<tamanho){
        pt+=3;
          
        //se for uma trinca, testa se é uma quadra
        if (aux[i].valor == aux[i+3].valor && i+3<tamanho){
          pt+=15;
          i++;
        }
        i++;
      }
    }
  }

  //testando sequencia a partir da primeira carta
  for(i=0;i<tamanho;i++) {
    if(aux[i].valor == aux[i+1].valor-1 && i+1<tamanho){
      if(!sequencia || aux[i].valor != aux[i-1].valor+1) sequencia++;
      sequencia++;
    }
  }

  //testando naipe
  for(i=0;i<tamanho;i++){
    switch(aux[i].naipe){
      case 'P':
        np++;
        break;
      case 'E':
        ne++;
        break;
      case 'O':
        no++;
        break;
      case 'C':
        nc++;
        break;
      default:
        break;
    }
  }
  if(np>=2) naipe+=np;
  if(ne>=2) naipe+=ne;
  if(no>=2) naipe+=no;
  if(nc>=2) naipe+=nc;
  
  //converte para pontos
  if(sequencia>=2) pt+=(2*sequencia);
  pt+=(3*naipe); 
  
  if (pt==0){
    //só aparecer no modo simples
    printf("Nenhum bônus alcançado.\n");
  }else{
    printf("Bônus alcançado: %d\n",pt);
  }
  return pt;
}

void ImprimeLista(Tno *lista) {
  Tno *aux = lista;
  int i=0;
  if(!ListaVazia(aux)){
    while (!ListaVazia(aux)) {
      printf("│%2s-%c ", aux->carta.face, aux->carta.naipe);
      aux = aux->prox;
    }
    printf("│");
  }
}

void ImprimeListaP(Tno *lista){
  Tno *aux = lista;
  Tcarta carta[5];
  int i=0,j,tamanho=0;
  if(!ListaVazia(aux)){
    while (!ListaVazia(aux)){
      carta[tamanho] = aux->carta;
      aux = aux->prox;
      tamanho++;
    }
  }
  printf("\t\t");
  for(i=0;i<tamanho;i++) printf("┌─────┐ ");
  printf("\n\t\t");
  for(i=0;i<tamanho;i++){
    if(carta[i].valor == 10){
      if(carta[i].naipe == 'C' || carta[i].naipe == 'O'){
        printf("│\033[1;31m%s\033[0m   │ ",carta[i].face);
      }else{
        printf("│%s   │ ",carta[i].face);
      }
      
    }else{
      if(carta[i].naipe == 'C' || carta[i].naipe == 'O'){
        printf("│\033[1;31m%s\033[0m    │ ",carta[i].face);
      }else{
        printf("│%s    │ ",carta[i].face);
      }
    }
  }
  printf("\n\t\t");
  for(i=0;i<tamanho;i++){
    if(carta[i].naipe == 'C' || carta[i].naipe == 'O'){
      printf("│  \033[1;31m%s\033[0m  │ ",carta[i].naipeP);
    }else{
      printf("│  %s  │ ",carta[i].naipeP);
    }
  }
  printf("\n\t\t");
  for(i=0;i<tamanho;i++){
    if(carta[i].valor == 10){
      if(carta[i].naipe == 'C' || carta[i].naipe == 'O'){
        printf("│   \033[1;31m%s\033[0m│ ",carta[i].face);
      }else{
        printf("│   %s│ ",carta[i].face);
      }
    }else{
      if(carta[i].naipe == 'C' || carta[i].naipe == 'O'){
        printf("│    \033[1;31m%s\033[0m│ ",carta[i].face);
      }else{
        printf("│    %s│ ",carta[i].face);
      }
    }
  }
  printf("\n\t\t");
  for(i=0;i<tamanho;i++) printf("└─────┘ ");
}