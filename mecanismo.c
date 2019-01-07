#include<stdio.h>
#include<stdlib.h>
#include"assalas.h"


static void salas_percorrer_printar_tras(){
	printf("\n Voce entra numa sala identica a anterior, existem tres portas na sua frente, uma ao lado da outra. Alem da porta de onde voce veio, que esta atras de voce.");
}
static void salas_percorrer_printar_direita(){
	printf("\n Voce entra numa sala identica a anterior, existe uma porta na sua frente; a porta de onde voce entrou e duas do seu lado direito");
}
static void salas_percorrer_printar_esquerda(){
	printf("\n Voce entra numa sala identica a anterior, existe uma porta na sua frente; a porta de onde voce entrou e duas do seu lado esquerdo");
	print_portas(ESQUERDA);
}
static void salas_percorrer_printar_meio(){
	printf("\n Voce entra numa sala identica a anterior, existe uma porta na sua frente; a porta de onde voce entrou, uma do seu lado direito e outra do seu lado esquerdo"); 
}


static int salas_percorrer_voltar(sala *atual, sala *salaEscolhida){
// O usuario volta por qual porta? Isso acontece somente se ele estiver voltando de uma sala errada, sesim, ele tem que voltar da outra sala errada. Cada sala tem duas salas erradas e uma certa, entao quando ele voltar de uma errada, ele tem que sair da outra porta errada que ele nao tinha entrado.
	int escolhaCerta;
	int deOndeFoi;
	if (salaEscolhida->direita == salaEscolhida->salaCerta){
		escolhaCerta = DIREITA;
	}else if (salaEscolhida->esquerda == salaEscolhida->salaCerta){
		escolhaCerta = ESQUERDA;
	}else if (salaEscolhida->meio == salaEscolhida->salaCerta){
		escolhaCerta = MEIO;
	}
	if (salaEscolhida->direita == atual){
		deOndeFoi = DIREITA;
	}else if (salaEscolhida->esquerda == atual){
		deOndeFoi = ESQUERDA;
	}else if (salaEscolhida->meio == atual){
		deOndeFoi = MEIO;
	}

	if (DIREITA != escolhaCerta && DIREITA != deOndeFoi){
		return DIREITA;                
	}else if(ESQUERDA != escolhaCerta && ESQUERDA != deOndeFoi){
		return ESQUERDA;
	}else{
		return MEIO;
	}
}

int salas_percorrer(sala *atual, sala *anterior, sala *ultimaSala){
	sala **salaEscolhida;
// Gera as salas adjacentes se nao existirem
	if (atual->direita == 0) sala_gerar(&(atual)->direita,atual,1);
	if (atual->esquerda == 0) sala_gerar(&(atual)->esquerda,atual,1);
	if (atual->meio == 0) sala_gerar(&(atual)->meio,atual,1);
	/* --------------------Perguntar sala------------------------------------ */
	int escolha;
	printf("\n\t Voce pode entrar pela porta da esquerda (1)");
	printf("\n\t Voce pode entrar pela porta do meio (2)");
	printf("\n\t Voce pode entrar pela porta da direita (3)");
	printf("\n\t Voce pode entrar pela porta sozinha a tras (4)");
	printf("\n\n\t Voce escolhe...  ");
	scanf(" %i",&escolha);
	if (escolha == ESQUERDA){
		salaEscolhida = &(atual)->esquerda;
	}else if (escolha == MEIO){
		salaEscolhida = &(atual)->meio;
	}else if (escolha == DIREITA){
		salaEscolhida = &(atual)->direita;
	}else if ((escolha == 4)&&(atual->tras == 0)){
		printf("\n A porta esta trancada");  // Se ele estivevr na raiz/salaInicial
		salas_percorrer(atual, anterior, ultimaSala);
	}else if (escolha == 4){
		salaEscolhida = &(atual)->tras;
	}else{
		printf("\n\t Voce procura por algo que nao existe... \n\n");// Nao digitou uma escolha valida, reinicia a funcao
		salas_percorrer(atual, anterior, ultimaSala);
	}


	/* ---------------------------------------------------------------------- */

	if (*salaEscolhida == ultimaSala){
		// Se eke chegou na ultima sala ele ganhou entao retorna 0
		return 0; 
	}else if (atual->errada == 1){

		// Fazer o que acontece se a sala que a pessoa esta atualmente eh uma errada

		 if(salaEscolhida == &(atual)->tras){

			/// ele foi pra tras
			int deOndeSai;
			// Descobrir de qual porta ele saira
			deOndeSai = salas_percorrer_voltar(atual,*salaEscolhida);

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
		if ((*salaEscolhida)->tras == atual){
			// se escolheu uma das que vao pra frente
			salas_percorrer_printar_tras();
		}else{
			// foi pra tras
			if ((*salaEscolhida)->direita == atual){
				salas_percorrer_printar_direita();
			}
			if ((*salaEscolhida)->esquerda == atual){
				salas_percorrer_printar_esquerda();
			}
			if ((*salaEscolhida)->meio == atual){
				salas_percorrer_printar_meio();
			}
		}
	}
	return salas_percorrer(*salaEscolhida,atual,ultimaSala); // chama a funcao denovo com a nova sala, percebi que o parametro " sala* anterior" eh inutil e nem uso ele.
}
