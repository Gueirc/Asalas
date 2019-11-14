#ifndef ASSALAS_H
#define ASSALAS_H

#define ESQUERDA 1
#define MEIO 2
#define DIREITA 3


typedef struct sala sala;
struct sala{
    sala *esquerda;
    sala *direita;
    sala *meio;
    sala *tras;
    sala *salaCerta; // qual das escolhas, meio,direita ou esquerda, é uma 
    //sala especial se a sala atual for errada, a salaCerta faz voltar pro 
    //início, se for certa, funciona normalmente
    int errada; // booleana se a essa sala está ou não no caminho certo
};
			/* Duas maneiras diferentes de fazer isso,
			 * com a primeira voce pode so deixar o 
			 * typedef no header e "declarar"
			 * (struct sala{...}) em outro arquivo.
			 * O que acontece eh que as sala dentro da
			 * struct sala na foram "typedefadas" ainda
			 * entao precisa fazer um dos dois. */

//typedef struct sala{
//    struct sala *esquerda;
//    struct sala *direita;
//    struct sala *meio;
//    struct sala *tras;
//    struct sala *salaCerta;
//    int errada;
//}sala;


// Gera uma sala 
void sala_gerar(sala **inicial, sala *anterior, int ehErrada);

//Descontinuado
int salas_percorrer(sala *atual, sala *anterior, sala *ultimaSala);

// Retorna portal pela qual jogador volta ao voltar pela porta de tras 
// de uma sala
int salas_percorrer_voltar(sala *atual, sala *salaEscolhida); 
#endif
