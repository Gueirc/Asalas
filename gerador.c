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
