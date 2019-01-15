#include<stdio.h>
#include<stdlib.h> 
#include"asalas.h"
#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_image.h>


int main(){
  // -> escolher entre os dois tipos de codigo

  /* ------Codigo definido --------------*/
  //	 Exemplo:
  int codigo[]={DIREITA,ESQUERDA,DIREITA};
  /* ----------------------------------- */


  /* ------Codigo aleatorio------------- */
  //	int *codigo;
  //	int maxCodigo;
  //	int direcao;
  //	maxCodigo = (rand() % 3)+3;  /// Tamanho maximo eh 5 e minimo eh 3
  //	codigo = malloc(sizeof(int) * maxCodigo);
  //	for (int i = 0; i < maxCodigo; i++){
  //		direcao = (rand() % 3) +1; /// 1 a 3 (esquerda,meio,direita)
  //		codigo[i] = direcao;
  //	}
  /* ----------------------------------- */

  // -> Inicializacoes do jogo
  sala *raiz;  // primeira sala 
  sala *ultimaSala; // Ultima sala do codigo, 
  sala *salaEscolhida = 0;
  int escolha;

  sala_gerar(&raiz,0,0); // Gera a sala inicial
  int pos = 0;
  while(1){
    // Gera as salas certas de acordo com o codigo 
    if (codigo[pos] == ESQUERDA){
      sala_gerar(&(raiz->esquerda),raiz,0);
      raiz->salaCerta = raiz->esquerda;
      raiz = raiz->esquerda;
      pos++;
      continue;
    }else if(codigo[pos] == DIREITA){
      sala_gerar((&raiz->direita),raiz,0);
      raiz->salaCerta = raiz->direita;
      raiz = raiz->direita;
      pos++;
      continue;
    }else if(codigo[pos] == MEIO){
      sala_gerar(&(raiz->meio),raiz,0);
      raiz->salaCerta = raiz->meio;
      raiz = raiz->meio;
      pos++;
      continue;
    }else{
      break;
    }
  }
  ultimaSala = raiz;
  while(raiz->tras != 0){
    raiz = raiz->tras;
  }

  sala *atual = raiz;

  /* ----------------- Inicializar SDL  e SDLttf------------------------ */
  if (SDL_Init(SDL_INIT_VIDEO) != 0){
    fprintf(stderr, "Impossivel inicializar sdl2: %s \n", SDL_GetError());
    return EXIT_FAILURE;
  }
  int S_LARGURA =  640;
  int S_ALTURA =  480;
  SDL_Window *window = SDL_CreateWindow("Asalas", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, S_LARGURA, S_ALTURA, SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
  if (window == NULL){
    fprintf(stderr, "SDL_CreateWindow Erro: %s \n",SDL_GetError());
    return EXIT_FAILURE;
  }
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL){
    fprintf(stderr, "SDL_CreateRederer Erro: %s \n",SDL_GetError());
    return EXIT_FAILURE;
  }
  if (TTF_Init() != 0){
    fprintf(stderr, "Impossivel inicializar SDL_TTF (Fonte e texto): %s \n",TTF_GetError());
    return EXIT_FAILURE;
  }
  TTF_Font* fonte = TTF_OpenFont("res/OpenSans-Regular.ttf",24); 
  if (fonte == NULL){
    fprintf(stderr, "Impossivel abrir OpenSans-Regular.ttf  : %s \n",TTF_GetError());
    return EXIT_FAILURE;
  }
  SDL_Color branco = {255,255,255};
  SDL_Event evento;
  SDL_Surface* surTexto = NULL;
  SDL_Texture* texto = NULL;
  SDL_Rect rectTexto; 
  rectTexto.x = 0;             // Esses dois sao constantes em qualquer texto
  rectTexto.h = 50; 

  // -> Sprites
  SDL_Surface* surBranca = SDL_CreateRGBSurface(0,100,100,8,0,0,0,0);
  if (!surBranca){
    fprintf(stderr, "Erro ao criar surface: %s \n",SDL_GetError());
    return EXIT_FAILURE;
  }
  SDL_Texture* tPorta    =  SDL_CreateTextureFromSurface(renderer,surBranca);
  SDL_Texture* tPlayer   =  SDL_CreateTextureFromSurface(renderer,surBranca);
  SDL_FreeSurface(surBranca);
  if (!tPorta){
    fprintf(stderr, "Erro ao criar textura: %s \n",SDL_GetError());
    return EXIT_FAILURE;
  }
  if (!tPlayer){
    fprintf(stderr,"Erro ao criar textura: %s \n",SDL_GetError());
    return EXIT_FAILURE;
  }
  // Retangulo do jogador
  SDL_Rect rectPlayer;
  rectPlayer.w = 25;
  rectPlayer.h = 25;
  // Coloca ele no meio da tela (temporariariamente, tbm precisa faze isso funcionar quando redimensionar)
  rectPlayer.x = (S_LARGURA - rectPlayer.w)/2;
  rectPlayer.y = (S_ALTURA -rectPlayer.h)/2;
  int mvCim = 0,mvDir = 0,mvEsq = 0,mvBaix = 0;  // Booleanas para mover
  float xPos = rectPlayer.x, yPos = rectPlayer.y, xVel, yVel;
  const int Velocidade = 10; // Velocidade (quantos pixels se move por frame)
  // Retangulo das portas
  SDL_Rect rectPesq, rectPdir, rectPmei, rectPtras;
  rectPmei.w  = 50;
  rectPmei.h  = 50;
  rectPesq.w  = 50;
  rectPesq.h  = 50;
  rectPdir.w  = 50;
  rectPdir.h  = 50;
  rectPtras.w = 50;
  rectPtras.h = 50;

  enum modos{ inicial, trancada, normal };
  enum modos modoTexto = inicial; // O inicial eh usado somente uma vez, cada modo muda o texto exibido
  int sair = 0; // Booleana pra sair do loop do jogo
  int redimensionar = 2; // Booleana para redimensionar o jogo
  unsigned int printarTexto = 2; // Comeca em 2 so pra renderizar junto com o fullscreen
  int mudouDeSala = 1; // Positivo se tiver mudado de sala, incialmente fica em um pra gerar salas adjacentes
  int SDLdeOndeAparece = 4; // 4 = tras

  int nvLarg;
  int nvAltu;

  while (!sair){
    /* --------------------Printar texto e Redmensionar objetos------------------------------------ */
    if (printarTexto || redimensionar){
      SDL_GetWindowSize(window,&nvLarg,&nvAltu);
      int altDiff = S_ALTURA- nvAltu;
      int larDiff = S_LARGURA - nvLarg;
      S_ALTURA = nvAltu;
      S_LARGURA = nvLarg;
      SDL_DestroyTexture(texto);
      rectTexto.y = S_ALTURA - 50; // Eh o mesmo pros tres modos
      if (printarTexto) printarTexto--;
      if (redimensionar){
        redimensionar--;   /// A porta de cima eh a da direita
        rectPmei.y  = (S_ALTURA - rectPmei.h - rectTexto.h)/2;
        rectPesq.y  = (S_ALTURA - rectPesq.h - rectTexto.h);
        rectPdir.y  = 0;
        rectPdir.x  = 0;
        rectPmei.x  = 0; 
        rectPesq.x  = 0;
        rectPtras.y = (S_ALTURA - rectPtras.h - rectTexto.h)/2;
        rectPtras.x = S_LARGURA - rectPtras.w;
        if (altDiff > 0) rectPlayer.y = rectPlayer.y + altDiff; // Não sei se funciona ou nao mas era suposto manter a posição do player quando redimensiona
        else rectPlayer.y = rectPlayer.y - altDiff;
        if (larDiff > 0) rectPlayer.x = rectPlayer.x + larDiff;
        else rectPlayer.x = rectPlayer.x - larDiff;

      }

      if (mudouDeSala){
        mudouDeSala--;
        /* -----Gera as salas adjacentes se nao existirem ---- */
        if (atual->direita == 0) sala_gerar(&(atual)->direita,atual,1);
        if (atual->esquerda == 0) sala_gerar(&(atual)->esquerda,atual,1);
        if (atual->meio == 0) sala_gerar(&(atual)->meio,atual,1);
        /*----- Posiciona o player na sala certa --- */
        switch(SDLdeOndeAparece){
          case DIREITA:
            yPos = (rectPdir.y + rectPdir.h - rectPlayer.h);
            xPos = (rectPdir.x + rectPlayer.w + 40);
            break;
          case ESQUERDA:
            yPos = (rectPesq.y + rectPesq.h - rectPlayer.h);
            xPos = (rectPesq.x + rectPlayer.w + 40);
            break;
          case MEIO:
            yPos = (rectPmei.y + rectPmei.h - rectPlayer.h);
            xPos = (rectPmei.x + rectPlayer.w + 40);
            break;
          case 4:
            yPos = (rectPtras.y + rectPtras.h - rectPlayer.h);
            xPos = (rectPtras.x - 40);
            break;
          default:
            fprintf(stderr,"Erro ao posicionar player\n");
            return EXIT_FAILURE;
        }

      }
      switch(modoTexto){
        case inicial:
          surTexto = TTF_RenderText_Blended(fonte,"A sala fede velhice, a porta nas suas costas se encontra trancada",branco);
          texto = SDL_CreateTextureFromSurface(renderer, surTexto);
          SDL_FreeSurface(surTexto);
          rectTexto.w = S_LARGURA;
          break;
        case trancada:
          surTexto = TTF_RenderText_Blended(fonte,"A porta se encontra trancada",branco);
          texto = SDL_CreateTextureFromSurface(renderer, surTexto);
          SDL_FreeSurface(surTexto);
          rectTexto.w = S_LARGURA/2;
          break;
        case normal:
          surTexto = TTF_RenderText_Blended(fonte,"Entraste em uma sala aparentemente igual a anterior",branco);
          texto = SDL_CreateTextureFromSurface(renderer, surTexto);
          SDL_FreeSurface(surTexto);
          rectTexto.w = S_LARGURA/2;
          break; 
        default:
          fprintf(stderr, "Erro logico ao printar texto \n");
          return EXIT_FAILURE;
      }
    }
    /* -------------------------------------------------------------------- */

    while (SDL_PollEvent(&evento)){
      switch(evento.type){
        case SDL_QUIT:
          sair = 1;
          break;
        case SDL_WINDOWEVENT:
          switch(evento.window.event){
            case SDL_WINDOWEVENT_RESIZED:
            case SDL_WINDOWEVENT_SIZE_CHANGED:
              redimensionar = 1;
              break;
          }
        case SDL_KEYDOWN:
          switch(evento.key.keysym.scancode){
            case SDL_SCANCODE_D:
            case SDL_SCANCODE_RIGHT:
              mvDir = 1;
              break;
            case SDL_SCANCODE_UP:
            case SDL_SCANCODE_W:
              mvCim = 1;
              break;
            case SDL_SCANCODE_DOWN:
            case SDL_SCANCODE_S:
              mvBaix = 1;
              break;
            case SDL_SCANCODE_LEFT:
            case SDL_SCANCODE_A:
              mvEsq = 1;
              break;
          }
          break;
        case SDL_KEYUP:
          switch(evento.key.keysym.scancode){
            case SDL_SCANCODE_D:
            case SDL_SCANCODE_RIGHT:
              mvDir = 0;
              break;
            case SDL_SCANCODE_UP:
            case SDL_SCANCODE_W:
              mvCim = 0;
              break;
            case SDL_SCANCODE_DOWN:
            case SDL_SCANCODE_S:
              mvBaix = 0;
              break;
            case SDL_SCANCODE_LEFT:
            case SDL_SCANCODE_A:
              mvEsq = 0;
              break;

          }
          break;
      }
    }

    // determinar velocidade do player
    xVel = 0;
    yVel = 0;
    if (mvCim  && !mvBaix)yVel = -Velocidade;
    if (mvBaix && !mvCim) yVel =  Velocidade;
    if (mvDir  && !mvEsq) xVel =  Velocidade;
    if (mvEsq  && !mvDir) xVel = -Velocidade;
    // Atualizar posicoes
    xPos += xVel  ;
    yPos += yVel  ;
    // Detectar colisoes com a tela
    if (xPos >= S_LARGURA - rectPlayer.w) xPos = S_LARGURA - rectPlayer.w;
    if (xPos <= 0) xPos = 0;
    if (yPos >= S_ALTURA - rectPlayer.h - rectTexto.h) yPos = S_ALTURA- rectPlayer.h - rectTexto.h;
    if (yPos <= 0) yPos = 0;
    // Aplicar movimento
    rectPlayer.y = (int) yPos;
    rectPlayer.x = (int) xPos;
    // Colisão com a porta
    if (((yPos >= rectPmei.y || yPos + rectPlayer.h >= rectPmei.y) && yPos <= rectPmei.y + rectPmei.h) && xPos <= rectPmei.w){ 
      escolha = MEIO;
      mudouDeSala = 1;
    }else if (((yPos >= rectPdir.y || yPos + rectPlayer.h >= rectPdir.y) && yPos <= rectPdir.y + rectPdir.h) && xPos <= rectPdir.w){
      escolha = DIREITA;
      mudouDeSala = 1;
    }else if (((yPos >= rectPesq.y || yPos + rectPlayer.h >= rectPesq.y) && yPos <= rectPesq.y + rectPesq.h) && xPos <= rectPesq.w){
      escolha = ESQUERDA;
      mudouDeSala = 1;
    }else if (((yPos >= rectPtras.y ||yPos + rectPlayer.h >= rectPtras.y)&& yPos <= rectPtras.y + rectPtras.h) && xPos + rectPlayer.w >= rectPtras.x){  // 4 = tras
      escolha = 4;
      mudouDeSala = 1;
    }
    /* ------> LOGICA DO PROGRAMA */
    if (mudouDeSala == 1){ 

      if (escolha == ESQUERDA){
        salaEscolhida = atual->esquerda;
        modoTexto = normal;
      }else if (escolha == MEIO){
        salaEscolhida = atual->meio;
        modoTexto = normal;
      }else if (escolha == DIREITA){
        salaEscolhida = atual->direita;
      }else if ((escolha == 4)&&(atual->tras == 0)){
        modoTexto = trancada;  // Se ele estivevr na raiz/salaInicial
        salaEscolhida = 0;
      }else if (escolha == 4){
        salaEscolhida = atual->tras;
        modoTexto = normal;
      }else{
        fprintf(stderr,"Erro logico na logica do programa, linha %i \n",__LINE__);
        return EXIT_FAILURE;
      }


      /* ------------------------Mecanica de entrada e saida---------------------------------------------- */
      if (salaEscolhida == 0){
        SDLdeOndeAparece = 4;
      }else if (salaEscolhida == ultimaSala){
        // Ao chegar na ultima sala ele ganha
        sair = 1; // Momentaneamente
      }else if (atual->errada == 1){

        // Fazer o que acontece se a sala que a pessoa esta atualmente eh uma errada

        if(salaEscolhida == atual->tras){

          /// ele foi pra tras
          int deOndeSai;
          // Descobrir de qual porta ele saira
          deOndeSai = salas_percorrer_voltar(atual,salaEscolhida);

          if (deOndeSai == ESQUERDA){
            SDLdeOndeAparece = ESQUERDA;
          }else if(deOndeSai == DIREITA){
            SDLdeOndeAparece = DIREITA;
          }else{
            SDLdeOndeAparece = MEIO;
          }
        }else{
          // se ele nao foi pra tras ele necessariamente vai entrar em outra sala pela porta de tras, aquela sozinha
          SDLdeOndeAparece = 4;
        }
      }else{
        // EH UMA DAS SALAS CERTAS
        if (salaEscolhida->tras == atual){
          // se escolheu uma das que vao pra frente
          SDLdeOndeAparece = 4;
        }else{
          // foi pra tras
          if (salaEscolhida->direita == atual){
            SDLdeOndeAparece = DIREITA;
          }
          if (salaEscolhida->esquerda == atual){
            SDLdeOndeAparece = ESQUERDA;
          }
          if (salaEscolhida->meio == atual){
            SDLdeOndeAparece = MEIO;
          }
        }
      }
      if (salaEscolhida != 0)
        atual = salaEscolhida;
      printarTexto = 1;


    }

    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderCopy(renderer, texto,   NULL, &rectTexto); 
    SDL_RenderCopy(renderer, tPlayer, NULL, &rectPlayer);
    SDL_RenderCopy(renderer, tPorta,  NULL, &rectPesq);
    SDL_RenderCopy(renderer, tPorta,  NULL, &rectPtras);
    SDL_RenderCopy(renderer, tPorta,  NULL, &rectPmei);
    SDL_RenderCopy(renderer, tPorta,  NULL, &rectPdir);
    SDL_RenderPresent(renderer);
  }
  TTF_CloseFont(fonte);
  SDL_DestroyTexture(texto);
  SDL_DestroyTexture(tPorta);
  SDL_DestroyTexture(tPlayer);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;

}
