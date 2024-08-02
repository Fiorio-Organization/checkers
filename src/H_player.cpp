#include "../headers/H_player.hpp"

H_Player::H_Player(int tab[8][4], bool isWhite){
    this->copia(tab, this->tabuleiro);
    this->isWhite = isWhite;

    // aponta suas pecas iniciais
    if(!isWhite){
        for(int i=0; i<3; i++)
            for(int j=0; j<4; j++)
                this->myPieces.push_back(&tabuleiro[i][j]);
    }
    else{
        for(int i=5; i<8; i++)
            for(int j=0; j<4; j++)
                this->myPieces.push_back(&tabuleiro[i][j]);
    }
}

void H_Player::teste(int tab[8][4]){
    copia(tab, this->tabuleiro);
    *myPieces[0] = 2;

    copia(this->tabuleiro, tab);
}