#ifndef H_PLAYER_HPP
#define H_PLAYER_HPP

#include "player.hpp"
#include <vector>

class H_Player : public Player{
private:

public:
    H_Player(int tab[8][4], bool isWhite);
    void teste(int tab[8][4]);
};


#endif