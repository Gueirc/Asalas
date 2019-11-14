#include<stdio.h>
#include<stdlib.h>
#include"asalas.h"

void sala_gerar(sala **atual, sala *anterior, int boolerrada){
// Gera a sala atual, coloca tras apontando pra anterior, e coloca as outras em 0. A boolerrada indica se a sala eh uma das certas(0), ou uma das erradas(1). Se for uma errada gera qual dos caminhos vai ser a sala certa aleatoriamente, a sala certa e esse caminho (que apontam pra mesma sala) apontam pra salainicial/raiz.
	*atual =  malloc(sizeof(sala));
	(*atual)->direita = 0;
	(*atual)->esquerda = 0;
	(*atual)->meio = 0;
	(*atual)->tras = anterior;
	(*atual)->errada = boolerrada;
	sala *paraRaiz = *atual;

	if (boolerrada == 1){
		int salaAleatoria = (rand() % 3) +1;
		while (paraRaiz->tras != 0){
			paraRaiz = paraRaiz->tras;
		}
		(*atual)->salaCerta = paraRaiz;
		if (salaAleatoria == ESQUERDA){
			(*atual) ->esquerda = paraRaiz;
		}
		if (salaAleatoria == DIREITA){
			(*atual)->direita = paraRaiz;
		}
		if (salaAleatoria == MEIO){
			(*atual)->meio = paraRaiz;
		}
	}
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

