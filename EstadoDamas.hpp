#ifndef ESTADO_DAMAS_HPP
#define ESTADO_DAMAS_HPP

#include "Estado.hpp"
#include <cstdlib>

class EstadoDamas : public Estado{
    private:
        int tabuleiro[8][4]; // 0 = vazio | 1 = MAX | -1 = MIN | 3 = fora
        bool eMax;
        void copia(int origem[8][4], int destino[8][4]);
        bool ePermitido(int tabuleiro[8][4], int linha, int coluna, int mov);
        bool ePermitidoRei(int tabuleiro[8][4], int linha, int coluna, int mov, int p);
        short int profRei;
        
    public:
        EstadoDamas(int tabuleiro[8][4], bool eMax);

        std::vector<Estado *> seqCaptura(int tabuleiro[8][4], int i, int j, int mov, short int profundidade);
        std::vector<Estado *> seqCapturaRei(int tabuleiro[8][4], int i, int j, int mov, int p, short int profundidade);
        short int profJogada; // profundidade da jogada, representa o num de movimentos de captura que a peca consegue fazer no maximo na jogada
        
        bool eFolha();
        double heuristica();

        //bool eAliado(int i,int j,int dir,int profundidade);
        
        void movePeca(int i, int j, int dir, int tabuleiroFilho[8][4]); //int iOrig, int jOrig,
        void moveRei(int i, int j, int dir, int tabuleiroFilho[8][4], int p);
        std::vector<Estado *> expandir(bool eMax);

        void temCoroacao();
        void temCoroacao(int tabuleiro[8][4]);

        EstadoDamas * jogadaHumano();

        void imprime();
        void imprime(int tabuleiro[8][4]);

        bool seExisteEstado(int tabuleiro[8][4]);
        std::vector<EstadoDamas *> filhos;

        void setProfRei(short int profRei);

        short int getProfRei();
        bool geteMax();

        /*[Linha][Coluna]
        (1)Move Superior esquerdo
            Par  : [-1][ 0]
            Impar: [-1][-1]
        (2)Move Superior direito
            Par  : [-1][+1]
            Impar: [-1][ 0]
        (3)Move Inferior esquerdo
            Par  : [+1][ 0]
            Impar: [+1][-1]
        (4)Move Inferior direito
            Par  : [+1][+1]
            Impar: [+1][ 0]

        (5)Come Superior Esquerdo
            Par  : [-2][-1]
            Impar: [-2][-1]
        (6)Come Superior Direito
            Par  : [-2][+1]
            Impar: [-2][+1]
        (7)Come Inferior Esquerdo
            Par  : [+2][-1]
            Impar: [+2][-1]
        (8)Come Inferior Direito
            Par  : [+2][+1]
            Impar: [+2][+1]
        */
};

#endif