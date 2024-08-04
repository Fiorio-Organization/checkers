#ifndef ESTADO_HPP
#define ESTADO_HPP

#include <vector>
#include <iostream>

class Estado{
public:
    // Retorna TRUE se o jogo terminou
    virtual bool eFolha() = 0;
    // Retorna o valor que será propagado para o pai
    virtual double heuristica() = 0;
    // Gera os estados filhos
    virtual std::vector<Estado *> expandir(bool eMax) = 0; 
};


#endif
