#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <vector>
#include <iostream>

class Player{
public:
    std::vector <int *> myPieces;
    bool isWhite;
    int tabuleiro[8][4];
    
    bool isPlayingWhite();
    void copia(int origem[8][4], int destino[8][4]);

    bool ePermitido(int linha, int coluna, int mov);
    void movePeca(int i, int j, int dir, int tabuleiroFilho[8][4]); //int iOrig, int jOrig,
};


#endif
