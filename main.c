#include<stdio.h>
#include<stdlib.h> 
#include"assalas.h"
int main(){
	// -> escolher entre os dois tipos de codigo
	
	/* ------Codigo definido --------------*/
//	 Exemplo:
//	int codigo[]={ESQUERDA,DIREITA,MEIO}
	/* ----------------------------------- */


	/* ------Codigo aleatorio------------- */
	int *codigo;
	int maxCodigo;
	int direcao;
	maxCodigo = (rand() % 3)+3;  /// Tamanho maximo eh 5 e minimo eh 3
	codigo = malloc(sizeof(int) * maxCodigo);
	for (int i = 0; i < maxCodigo; i++){
		direcao = (rand() % 3) +1; /// 1 a 3 (esquerda,meio,direita)
		codigo[i] = direcao;
	}
	/* ----------------------------------- */


	sala *raiz;   // primeira sala, tras dela apontara pra ela mesmo quando o programa chegar no (salas_percorrer) 
	sala *ultimaSala; // Ultima sala do codigo, 

	sala_gerar(&raiz,0,0); // Gera a sala inicial
	ultimaSala = sala_gerar_certos(raiz,codigo,0); // Gera as salas certas a serem seguidas a partir do codigo
	int ganhar;
	printf("\n Voce se ve diante de tres portas, uma do lado da outra, estao um pouco longe, logo atras de voce tem uma porta trancada");
	ganhar = salas_percorrer(raiz,0,ultimaSala); // Faz todo o percurso ate que usuario chegue na sala final/ultimaSala
	if (ganhar == 0){
		printf("\n\n\t Voce entra no seu quarto e vai dormir.\n");
	}else{
		printf("\n\n\n Voce acorda.\n");
	}

	exit(0);
}
