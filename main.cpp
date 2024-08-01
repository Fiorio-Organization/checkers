/*
       MAPEAMENTO DO TABULEIRO
        0 - livre
        1 - brancas
        2 - pretas

        0    2   2   2   2 
        1  2   2   2   2
        2    2   2   2   2
        3  0   0   0   0
        4    0   0   0   0
        5  1   1   1   1
        6    1   1   1   1
        7  1   1   1   1
           0 1 2 3 4 5 6 7

*/

#include <float.h> // Obter o valor de infinito (DBL_MAX)
#include <vector> // receber os estados filhos
#include <cmath>  // funções max e min.

int main(){

    // tabuleiro inicial
    
    
    int tabuleiro[8][4] = {
        {-1,-1,-1,-1},
        {-1,-1,-1,-1},
        {-1,-1,-1,-1},
        { 0, 0, 0, 0},
        { 0, 0, 0, 0},
        { 1 ,1 ,1 ,1},
        { 1 ,1 ,1 ,1},
        { 1 ,1 ,1 ,1}
    };

    return 0;
}