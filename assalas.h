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
    sala *salaCerta;
    int errada;
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


void sala_gerar(sala **inicial, sala *anterior, int ehErrada);

sala* sala_gerar_certos(sala*,const int*,int);

int salas_percorrer(sala *atual, sala *anterior, sala *ultimaSala);

void print_portas(int)
#endif
