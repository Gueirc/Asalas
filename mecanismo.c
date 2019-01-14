#include<stdio.h>
#include<stdlib.h>
#include"asalas.h"


void salas_percorrer_printar_tras(){
	printf("\n Voce entra numa sala identica a anterior, existem tres portas na sua frente, uma ao lado da outra. Alem da porta de onde voce veio, que esta atras de voce.");
}
void salas_percorrer_printar_direita(){
	printf("\n Voce entra numa sala identica a anterior, existe uma porta na sua frente; a porta de onde voce entrou e duas do seu lado direito");
}
void salas_percorrer_printar_esquerda(){
	printf("\n Voce entra numa sala identica a anterior, existe uma porta na sua frente; a porta de onde voce entrou e duas do seu lado esquerdo");
}
void salas_percorrer_printar_meio(){
	printf("\n Voce entra numa sala identica a anterior, existe uma porta na sua frente; a porta de onde voce entrou, uma do seu lado direito e outra do seu lado esquerdo"); 
}


int salas_percorrer_voltar(sala *atual, sala *salaEscolhida){
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

