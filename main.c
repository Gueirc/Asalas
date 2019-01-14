#include<stdio.h>
#include<stdlib.h> 
#include"assalas.h"
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


	sala *raiz;  // primeira sala 
	sala *ultimaSala; // Ultima sala do codigo, 
	sala *salaEscolhida;
	int escolha;
	sala_gerar(&raiz,0,0); // Gera a sala inicial
	ultimaSala = sala_gerar_certos(raiz,codigo,0); // Gera as salas certas a serem seguidas a partir do codigo
  sala *atual = raiz;
	printf("\n Voce se ve diante de tres portas, uma do lado da outra, estao um pouco longe, logo atras de voce tem uma porta trancada");
  while (1){
// Gera as salas adjacentes se nao existirem
	if (atual->direita == 0) sala_gerar(&(atual)->direita,atual,1);
	if (atual->esquerda == 0) sala_gerar(&(atual)->esquerda,atual,1);
	if (atual->meio == 0) sala_gerar(&(atual)->meio,atual,1);
	/* --------------------Perguntar sala------------------------------------ */
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


	/* ---------------------------------------------------------------------- */

	if (salaEscolhida == ultimaSala){
		// Se eke chegou na ultima sala ele ganhou entao retorna 0
    printf("Voce ganhou");
		return 0; 
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
}

}
