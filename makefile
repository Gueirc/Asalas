asalas: main.c gerador.c mecanismo.c; gcc -o asalas main.c gerador.c mecanismo.c `sdl2-config --cflags --libs` -lSDL2_ttf -lSDL2_image -g
