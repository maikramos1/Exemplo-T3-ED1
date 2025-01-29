#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "listacarta.h"
#include "fila.h"

int main(void) {
  int i,menu1,menu2,menu3,posorigem,posdestino,bonus;
  int pt=0,monte=0,desc=0,qdescarte=0,qdescartetemp=0;
  int rp=0,re=0,ro=0,rc=0,remb=0,trn=1,trntarefa=0;
  int ptfinal=0,rect=0,recn=0,recr=0,nt=0;
  Tcarta carta;
  tNoFila nosolto;
  Tno *lista = CriaLista(), *mao = CriaLista();
  Tno *descarte = CriaLista(), *descartetemp = CriaLista();
  tFila *filatarefas = CriaFila();
  FILE *baralho = fopen("baralho.dat", "r");
  FILE *tarefas = fopen("tarefas.dat","r");
  char *rgb[] = {"\033[1;31m","\033[1;32m","\033[1;34m"};
  
  //ler arquivo e armazenar na lista
  for (i=1;i<53;i++){
    fscanf(baralho,"%2s %c %d %[^\n]s", carta.face, &carta.naipe, &carta.valor, carta.nome);
    
    //perfumaria naipe unicode
    switch(carta.naipe){
      case 'P':
        strcpy(carta.naipeP, "\u2663");
        break;
      case 'C':
        strcpy(carta.naipeP, "\u2665");
        break;
      case 'E':
        strcpy(carta.naipeP, "\u2660");
        break;
      case 'O':
        strcpy(carta.naipeP, "\u2666");
        break;
      default:
        break;
    }
    InsereNo(&lista,CriaNo(carta),i);
    monte++;
  }

  //ler arquivo e armazenar na fila
  for(i=0;i<10;i++){
    fscanf(tarefas,"%d %d %d %d %d %d %d", &nosolto.turno, &nosolto.prazo, &nosolto.paus, &nosolto.espadas, &nosolto.ouros, &nosolto.copas, &nosolto.premio);
    InsereNoFila(filatarefas,CriaNoFila(nosolto));
  }
  
  printf("Bem-vindo(a)!\n");
  do{
    printf("\nSelecione seu modo de jogo\n");
    printf("1 - Simples\n");
    printf("2 - COM PERFUMARIAS\n");
    printf("3 - Sair\n");
    printf("Digite a opção desejada: ");
    scanf("%d", &menu1);
    
    switch(menu1){
      case 1:
        do{
          printf("\nMenu Modo Simples:\n");
          printf("1 - Iniciar\n");
          printf("2 - Voltar ao menu anterior\n");
          printf("Digite a opção desejada: ");
          scanf("%d", &menu2);

          switch(menu2){
            case 1:
              
              //completa a mão com 5 cartas
              for (i=0;i<5;i++) SorteiaCarta(&lista, &mao, monte--);
              
              //começa o jogo
              do{
                printf("\nP:%2d  -  E:%2d  -  O:%2d  -  C:%2d  -  *:%2d\t\t     (%d) \n\n",rp,re,ro,rc,remb,trn);
                printf("Tarefas: ");
                if(!FilaVazia(filatarefas)){
                  if(filatarefas->prim->turno <= trn){
                    trntarefa = filatarefas->prim->turno + filatarefas->prim->prazo - trn;
                    printf("%d ",trntarefa);
                    ImprimePrimFila(filatarefas);
                  }else{
                    printf("Nenhuma tarefa no momento!");
                  };
                }else{
                  printf("Fim das tarefas!\n");
                };
                printf("\n\n\nMonte (%d)\t\t\t\t\t\t\t\t\tDescarte (%d)",monte,desc);
                printf("\n\n\t");
                ImprimeLista(mao);
                printf("\nMão :  ");
                for (i=0;i<TamanhoLista(mao);i++) printf("%d     ",i+1);
                printf("\n\nMenu:\n");
                printf("1 − Reposicionar carta\n");
                printf("2 - Descartar cartas\n");
                printf("3 - Cumprir tarefa\n");
                //printf("4 - Reembaralhar tudo (*:%d)\n",remb);
                printf("5 - Finalizar Turno\n");
                printf("\n\nDigite a opção desejada: ");
                scanf("%d", &menu3);

                switch(menu3){
                  case 1:
                    printf("Posição de Origem: ");
                    scanf("%d",&posorigem);
                    printf("Posição de Destino: ");
                    scanf("%d",&posdestino);
                    if(posorigem>0 && posorigem<6 && posdestino>0 && posdestino<6 && (!ListaVazia(mao))) MudaPos(&mao, posorigem, posdestino);
                    break;
              
                  case 2:
                    printf("Descartar quantas cartas? ");
                    scanf("%d",&qdescartetemp);
                    if(qdescartetemp>0 && qdescartetemp<(6-qdescarte) && (qdescartetemp+desc)<53){
                      for(i=1;i<=qdescartetemp;i++){
                        MudaLista(&mao, &descartetemp, qdescartetemp);
                        carta=AcessaNo(descartetemp,i)->carta;
                        switch(carta.naipe){
                          case 'P':
                            rp+=carta.valor;
                            break;
                          case 'E':
                            re+=carta.valor;
                            break;
                          case 'O':
                            ro+=carta.valor;
                            break;
                          case 'C':
                            rc+=carta.valor;
                            break;
                        }
                        desc++;
                        if(monte<0) monte=0;
                      }
                      qdescarte+=qdescartetemp;
                      pt+=ContaBonus(descartetemp,qdescartetemp);
                      if(pt){
                        printf("Deseja receber o bônus em qual naipe?\n");
                        printf("1 - Paus\n");
                        printf("2 - Espadas\n");
                        printf("3 - Ouros\n");
                        printf("4 - Copas\n");
                        printf("Digite a opção desejada: ");
                        while(pt){
                          scanf("%d",&bonus);
                          switch(bonus){
                            case 1:
                              rp+=pt;
                              pt=0;
                              break;
                            case 2:
                              re+=pt;
                              pt=0;
                              break;
                            case 3:
                              ro+=pt;
                              pt=0;
                            case 4:
                              rc+=pt;
                              pt=0;
                              break;
                            default:
                              printf("Digite uma opção válida: ");
                              break;
                          }
                        }
                      }

                      for(i=1;i<=qdescartetemp;i++) MudaLista(&descartetemp, &descarte, qdescartetemp);
                      if(monte<0) monte=0;
                      //LiberaLista(&descartetemp);
                    }
                    
                    break;
                  
                  case 3:
                    //Cumprir Tarefa
                    nosolto = *AcessaPrimFila(filatarefas);
                    if(nosolto.turno <= trn && rp >= nosolto.paus && re >= nosolto.espadas && ro >= nosolto.ouros && rc >= nosolto.copas){
                      rp -= nosolto.paus;
                      re -= nosolto.espadas;
                      ro -= nosolto.ouros;
                      rc -= nosolto.copas;
                      remb += nosolto.premio;
                      rect += nosolto.paus * trntarefa;
                      rect += nosolto.espadas * trntarefa;
                      rect += nosolto.ouros * trntarefa;
                      rect += nosolto.copas * trntarefa;
                      printf("Tarefa cumprida com sucesso!\n");
                      RemoveNoFila(filatarefas);
                    }else{
                      if(nosolto.turno > trn){
                        printf("Nenhuma tarefa ativa no momento!");
                      }else{
                        printf("Recursos insuficientes!");
                      }
                    }
                    break;
                  /*
                  case 4:
                    //Reembaralhar tudo
                    break;
                  */
                  case 5:
                    //Encerrar turno
                    for(i=1;i<=qdescarte;i++){
                      SorteiaCarta(&lista, &mao, monte--);
                    }
                    if(monte<0) monte=0;
                    qdescarte = 0;
                    qdescartetemp = 0;
                    trn++;
                    if(!FilaVazia(filatarefas)){
                      if(TamanhoFila(filatarefas)==1 && (filatarefas->prim->prazo + filatarefas->prim->turno) <= trn ) {
                        nosolto = *RemoveNoFila(filatarefas);
                        recn += (nosolto.paus+nosolto.espadas+nosolto.ouros+nosolto.copas);
                      }
                      while(TamanhoFila(filatarefas)>1){
                        if((filatarefas->prim->prazo + filatarefas->prim->turno) <= trn){
                          nosolto = *RemoveNoFila(filatarefas);
                          recn += (nosolto.paus+nosolto.espadas+nosolto.ouros+nosolto.copas);
                        }else{
                          break;
                        }
                      }
                      
                    }

                    if(ListaVazia(mao) && ListaVazia(lista)){
                      if(!FilaVazia(filatarefas)){
                        while(TamanhoFila(filatarefas)>=1){
                          nosolto = *RemoveNoFila(filatarefas);
                          recn += (nosolto.paus+nosolto.espadas+nosolto.ouros+nosolto.copas);
                        }
                      }
                      recr=(rp+re+ro+rc);
                      ptfinal = (rect+(recr/2))-(recn);
                      printf("\nFim de Jogo!\n");
                      printf("Pontuação final: %d\n", ptfinal);
                      LiberaLista(&lista);
                      LiberaLista(&mao);
                      LiberaLista(&descartetemp);
                      LiberaLista(&descarte);
                      LiberaFila(filatarefas);
                      menu3=6;
                      menu2=2;
                      menu1=3;
                    }
                    break;
                  
                  default:
                    printf("Opção inválida!\n");
                    menu3=7;
                    break;
                }
            }while(menu3!=6);
              break;

            case 2:
              printf("\nVoltando ao menu anterior.");
              break;

            default:
              printf("Opção inválida!\n");
              break;
          }
        }while(menu2!=2);
        break;
      
      case 2:
        do{
          system("clear");
          printf("♣ ♠ ♦ ♥ ♣ ♠ ♦ ♥ ♣ ♠ ♣ ♥ ♦ ♠ ♣ ♥ ♦ ♠ ♣\n");
          printf("♠ ♦ ♥ ♣  CHOOSE YOUR DESTINY  ♣ ♥ ♦ ♠\n");
          printf("♦ ♥ ♣ ♠ ♦ ♥ ♣ ♠ ♦ ♥ ♦ ♠ ♣ ♥ ♦ ♠ ♣ ♥ ♦\n");
          printf("1 - Iniciar\n");
          printf("2 - Voltar ao menu anterior\n");
          printf("Digite a opção desejada: ");
          scanf("%d", &menu2);

          switch(menu2){
            case 1:

              //completa a mão com 5 cartas
              for (i=0;i<5;i++) SorteiaCarta(&lista, &mao, monte--);

              //começa o jogo
              do{
                system("clear");
                printf("\u2663: %d  ━━  \u2660: %d  ━━  \033[1;31m\u2666\033[0m: %d  ━━  \033[1;31m\u2665\033[0m: %d  ━━  *: %d\t\t (%d)\n\n",rp,re,ro,rc,remb,trn);
                printf("Tarefas: ");
                if(!FilaVazia(filatarefas)){
                  if(filatarefas->prim->turno <= trn){
                    trntarefa = filatarefas->prim->turno + filatarefas->prim->prazo - trn;
                    printf("%d ",trntarefa);
                    ImprimePrimFilaP(filatarefas);
                  }else{
                    printf("Nenhuma tarefa no momento!");
                  };
                }else{
                  printf("Fim das tarefas!\n");
                };
                printf("\n\n\nMonte (%d)\t\t\t\t\t\t\t\t\tDescarte (%d)",monte,desc);
                printf("\n\n");
                ImprimeListaP(mao);
                printf("\n\tMão :  ");
                for (i=0;i<TamanhoLista(mao);i++) printf("%d\t   ",i+1);
                printf("\n\n\nMenu:\n\n");
                printf("1 − Reposicionar carta\n");
                printf("2 - Descartar cartas\n");
                printf("3 - Cumprir tarefa\n");
                printf("\x1B[9m4 - Reembaralhar tudo (*:%d)\x1B[0m\n",remb);
                printf("5 - Finalizar turno\n\n");
                if(nt==0) printf("\n");
                if(nt==1){
                  printf("Tarefa cumprida com sucesso!\n");
                  nt=0;
                }
                if(nt==2){
                  printf("Nenhuma tarefa ativa no momento!\n");
                  nt=0;
                }
                if(nt==3){
                  printf("Recursos insuficientes!\n");
                  nt=0;
                }
                printf("Digite a opção desejada: ");
                scanf("%d", &menu3);

                switch(menu3){
                  case 1:
                    printf("Posição de Origem: ");
                    scanf("%d",&posorigem);
                    printf("Posição de Destino: ");
                    scanf("%d",&posdestino);
                    if(posorigem>0 && posorigem<6 && posdestino>0 && posdestino<6 && (!ListaVazia(mao))) MudaPos(&mao, posorigem, posdestino);
                    break;

                  case 2:
                    printf("Descartar quantas cartas? ");
                    scanf("%d",&qdescartetemp);
                    if(qdescartetemp>0 && qdescartetemp<(6-qdescarte) && (qdescartetemp+desc)<53){
                      for(i=1;i<=qdescartetemp;i++){
                        MudaLista(&mao, &descartetemp, qdescartetemp);
                        carta=AcessaNo(descartetemp,i)->carta;
                        switch(carta.naipe){
                          case 'P':
                            rp+=carta.valor;
                            break;
                          case 'E':
                            re+=carta.valor;
                            break;
                          case 'O':
                            ro+=carta.valor;
                            break;
                          case 'C':
                            rc+=carta.valor;
                            break;
                        }
                        desc++;
                        if(monte<0) monte=0;
                      }
                      qdescarte+=qdescartetemp;
                      pt+=ContaBonus(descartetemp,qdescartetemp);
                      if(pt){
                        printf("Deseja receber o bônus em qual naipe?\n");
                        printf("1 - \u2663\n");
                        printf("2 - \u2660\n");
                        printf("3 - \033[1;31m\u2666\033[0m\n");
                        printf("4 - \033[1;31m\u2665\033[0m\n");
                        printf("Digite a opção desejada: ");
                        while(pt){
                          scanf("%d",&bonus);
                          switch(bonus){
                            case 1:
                              rp+=pt;
                              pt=0;
                              break;
                            case 2:
                              re+=pt;
                              pt=0;
                              break;
                            case 3:
                              ro+=pt;
                              pt=0;
                            case 4:
                              rc+=pt;
                              pt=0;
                              break;
                            default:
                              printf("Digite uma opção válida: ");
                              break;
                          }
                        }
                      }
                      for(i=1;i<=qdescartetemp;i++) MudaLista(&descartetemp, &descarte, qdescartetemp);
                      if(monte<0) monte=0;
                    }
                    break;

                  case 3:
                  //Cumprir Tarefa
                  nosolto = *AcessaPrimFila(filatarefas);
                  if(nosolto.turno <= trn && rp >= nosolto.paus && re >= nosolto.espadas && ro >= nosolto.ouros && rc >= nosolto.copas){
                    rp -= nosolto.paus;
                    re -= nosolto.espadas;
                    ro -= nosolto.ouros;
                    rc -= nosolto.copas;
                    remb += nosolto.premio;
                    rect += nosolto.paus * trntarefa;
                    rect += nosolto.espadas * trntarefa;
                    rect += nosolto.ouros * trntarefa;
                    rect += nosolto.copas * trntarefa;
                    nt=1;
                    RemoveNoFila(filatarefas);
                  }else{
                    if(nosolto.turno > trn){
                      nt=2;
                    }else{
                      nt=3;
                    }
                  }
                  break;
                  /*
                  case 4:
                    //Reembaralhar tudo
                    break;
                  */
                  case 5:
                    //Encerrar turno
                    for(i=1;i<=qdescarte;i++){
                      SorteiaCarta(&lista, &mao, monte--);
                    }
                    if(monte<0) monte=0;
                    qdescarte = 0;
                    qdescartetemp = 0;
                    trn++;
                    if(!FilaVazia(filatarefas)){
                      if(TamanhoFila(filatarefas)==1 && (filatarefas->prim->prazo + filatarefas->prim->turno) <= trn ) {
                        nosolto = *RemoveNoFila(filatarefas);
                        recn += (nosolto.paus+nosolto.espadas+nosolto.ouros+nosolto.copas);
                      }
                      while(TamanhoFila(filatarefas)>1){
                        if((filatarefas->prim->prazo + filatarefas->prim->turno) <= trn){
                          nosolto = *RemoveNoFila(filatarefas);
                          recn += (nosolto.paus+nosolto.espadas+nosolto.ouros+nosolto.copas);
                        }else{
                          break;
                        }
                      }
                    }
                    if(ListaVazia(mao) && ListaVazia(lista)){
                      if(!FilaVazia(filatarefas)){
                        while(TamanhoFila(filatarefas)>=1){
                          nosolto = *RemoveNoFila(filatarefas);
                          recn += (nosolto.paus+nosolto.espadas+nosolto.ouros+nosolto.copas);
                        }
                      }
                      recr=(rp+re+ro+rc);
                      ptfinal = (rect+(recr/2))-(recn);
                      LiberaLista(&lista);
                      LiberaLista(&mao);
                      LiberaLista(&descartetemp);
                      LiberaLista(&descarte);
                      LiberaFila(filatarefas);
                      for(i=0;;i++){
                        system("clear");
                        printf("%s♣ ♠ ♦ ♥ ♣ ♠ ♦ ♥ ♣ ♠ ♣ ♥ ♦ ♠ ♣ ♥ ♦ ♠ ♣\033[0m\n",rgb[i%3]);
                        printf("%s♠           Fim de Jogo !           ♠\033[0m\n",rgb[(i+1)%3]);
                        printf("%s♦ ♥ ♣ ♠ ♦ ♥ ♣ ♠ ♣ ♥ ♦ ♠ ♣ ♥ ♦ ♠ ♣ ♥ ♦\033[0m\n",rgb[(i+2)%3]);
                        printf("%s♣    Pontuação Final    : %4d      ♣\033[0m\n",rgb[i%3],ptfinal);
                        printf("%s♠   Encerrado no Turno  :   %2d      ♠\033[0m\n",rgb[(i+1)%3],trn-1);
                        printf("%s♦ ♥ ♣ ♠ ♦ ♥ ♣ ♠ ♣ ♥ ♦ ♠ ♣ ♥ ♦ ♠ ♣ ♥ ♦\033[0m\n",rgb[(i+2)%3]);
                        sleep(1);
                      }
                    }
                    break;

                  default:
                    printf("Opção inválida!\n");
                    menu3=7;
                    break;
                }
            }while(menu3!=6);
              break;

            default:
              break;
          }
        }while(menu2!=2);
        break;
      
      case 3:
        printf("Obrigado por jogar!");
        LiberaLista(&lista);
        LiberaLista(&mao);
        LiberaLista(&descarte);
        LiberaLista(&descartetemp);
        LiberaFila(filatarefas);
        break;

      default:
        printf("Opção inválida!");
        break;
      
    }
    
  }while(menu1!=3);

  fclose(baralho);
  fclose(tarefas);
  
  return 0;
}
