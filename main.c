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
  int codigo[]={ESQUERDA,DIREITA,MEIO};
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
  sala *salaEscolhida;
  int escolha;
  int ganhou = 1;

  sala_gerar(&raiz,0,0); // Gera a sala inicial
  ultimaSala = sala_gerar_certos(raiz,codigo,0); // Gera as salas certas a serem seguidas a partir do codigo

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
  SDL_Texture* tPorta =  SDL_CreateTextureFromSurface(renderer,surBranca);
  SDL_Texture* tPlayer = SDL_CreateTextureFromSurface(renderer,surBranca);
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
  rectPlayer.w = 50;
  rectPlayer.h = 50;
  // Coloca ele no meio da tela (temporariariamente, tbm precisa faze isso funcionar quando redimensionar)
  rectPlayer.x = (S_LARGURA - rectPlayer.w)/2;
  rectPlayer.y = (S_ALTURA -rectPlayer.h)/2;
  int mvCim = 0,mvDir = 0,mvEsq = 0,mvBaix = 0;  // Booleanas para mover
  float xPos = rectPlayer.x, yPos = rectPlayer.y, xVel, yVel;
  const int Velocidade = 5; // Velocidade (quantos pixels se move por frame)

  unsigned int printarTexto = 2; // Comeca em 2 so pra renderizar junto com o fullscreen
  int mudouDeSala = 1; // Positivo se tiver mudado de sala, incialmente fica em um pra gerar salas adjacentes
  enum modos{ inicial, trancada, normal };
  enum modos modoTexto = inicial; // O inicial eh usado somente uma vez, cada modo muda o texto exibido
  int sair = 0; // Booleana pra sair do loop do jogo

  while (!sair){

    /* -----Gera as salas adjacentes se nao existirem ---- */
    if (mudouDeSala){
      mudouDeSala--;
      if (atual->direita == 0) sala_gerar(&(atual)->direita,atual,1);
      if (atual->esquerda == 0) sala_gerar(&(atual)->esquerda,atual,1);
      if (atual->meio == 0) sala_gerar(&(atual)->meio,atual,1);
    }
    /* --------------------Printar texto------------------------------------ */
    if (printarTexto){
      printarTexto--;
      SDL_GetWindowSize(window,&S_LARGURA,&S_ALTURA);
      SDL_DestroyTexture(texto);
      rectTexto.y = S_ALTURA - 50; // Eh o mesmo pros tres modos
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
              printarTexto = 1;
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

      /* Testando SDL
         printf("\n\t Voce pode entrar pela porta da esquerda (1)");
         printf("\n\t Voce pode entrar pela porta do meio (2)");
         printf("\n\t Voce pode entrar pela porta da direita (3)");
         printf("\n\t Voce pode entrar pela porta sozinha a tras (4)");
         printf("\n\n\t Voce escolhe...  ");
         scanf(" %i",&escolha);
         if (escolha == ESQUERDA){
         salaEscolhida = atual->esquerda;
         }else if (escolha == MEIO){
         salaEscolhida = atual->meio;
         }else if (escolha == DIREITA){
         salaEscolhida = atual->direita;
         }else if ((escolha == 4)&&(atual->tras == 0)){
         printf("\n A porta esta trancada");  // Se ele estivevr na raiz/salaInicial
         continue;
         }else if (escolha == 4){
         salaEscolhida = atual->tras;
         }else{
         printf("\n\t Voce procura por algo que nao existe... \n\n");// Nao digitou uma escolha valida, reinicia a funcao
         continue;
         }


      /* ------------------------Mecanica de entrada e saida---------------------------------------------- */
      /* Testando SDL
         if (salaEscolhida == ultimaSala){
      // Ao chegar na ultima sala ele ganha
      ganhou = 0;
      break;
      }else if (atual->errada == 1){

      // Fazer o que acontece se a sala que a pessoa esta atualmente eh uma errada

      if(salaEscolhida == atual->tras){

      /// ele foi pra tras
      int deOndeSai;
      // Descobrir de qual porta ele saira
      deOndeSai = salas_percorrer_voltar(atual,salaEscolhida);

      if (deOndeSai == ESQUERDA){
      salas_percorrer_printar_esquerda();
      }else if(deOndeSai == DIREITA){
      salas_percorrer_printar_direita();
      }else{
      salas_percorrer_printar_meio();
      }
      }else{
      // se ele nao foi pra tras ele necessariamente vai entrar em outra sala pela porta de tras, aquela sozinha
      salas_percorrer_printar_tras();
      }
      }else{
      // EH UMA DAS SALAS CERTAS
      if (salaEscolhida->tras == atual){
      // se escolheu uma das que vao pra frente
      salas_percorrer_printar_tras();
      }else{
      // foi pra tras
      if (salaEscolhida->direita == atual){
      salas_percorrer_printar_direita();
      }
      if (salaEscolhida->esquerda == atual){
      salas_percorrer_printar_esquerda();
      }
      if (salaEscolhida->meio == atual){
      salas_percorrer_printar_meio();
      }
      }
      }
      atual = salaEscolhida;
      continue;
      Testando SDL*/
    } // Fin do handle event

    // determinar velocidade do player
    xVel = 0;
    yVel = 0;
    if (mvCim  && !mvBaix)yVel = -Velocidade;
    if (mvBaix && !mvCim) yVel =  Velocidade;
    if (mvDir  && !mvEsq) xVel =  Velocidade;
    if (mvEsq  && !mvDir) xVel = -Velocidade;
    // Atualizar posicoes
    xPos += xVel;
    yPos += yVel;
    // Detectar colisoes

    // Aplicar movimento
    rectPlayer.y = (int) yPos;
    rectPlayer.x = (int) xPos;

    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderCopy(renderer, texto, NULL, &rectTexto); 
    SDL_RenderCopy(renderer, tPlayer, NULL, &rectPlayer);
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
