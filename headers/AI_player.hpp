#ifndef AI_PLAYER_HPP
#define AI_PLAYER_HPP

#include "player.hpp"

class AI_Player : public Player{
private:
    
public:
    AI_Player(int tabuleiro[8][4], bool isWhite);

    bool eFolha(); // Retorna TRUE se o jogo terminou
    double heuristica(); // Retorna o valor que será propagado para o pai
    std::vector<AI_Player *> expandir(bool eMax); // Gera os estados filhos
};


#endif