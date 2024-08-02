#include "../headers/player.hpp"

bool Player::isPlayingWhite(){
    return this->isWhite;
}

void Player::copia(int origem[8][4], int destino[8][4]){
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 4; j++){
            destino[i][j] = origem[i][j];
        }
    }
}

bool Player::ePermitido(int linha, int coluna, int mov){
    /*
        Movimentos permitidos
        - Mover para quadrados adjacentes frontais
        - Capturar uma peça adversária, tanto peças adversárias frontais e traseiras  

        DIREÇÕES
        1 - Esquerda Frontal
        2 - Direita Frontal
        3 - Esquerda Traseira
        4 - Direita Traseira
    */
    
    // Tratamento de movimentos sem captura 1 2 3 4
    if(mov==1 || mov==2 || mov==3 || mov==4){
        // esse bloco é pra evitar repitir linha+- nos blocos abaixo
        if((mov==1 || mov==2) && tabuleiro[linha][coluna]==1) // sobe na matriz
            linha--;
        else if((mov==3 || mov==4) && tabuleiro[linha][coluna]==-1) // desce na matriz
            linha++;

        // Verificando indice fora do campo [8][4]
        if(linha<0 || linha >7)
            return 0;

        // tratamento diferente para linhas impares e pares
        // lembrando que a linha ja foi alterada lá em cima, então precisa inverter neste bloco
        if(linha%2!=0){ // ÍMPAR
            if(mov == 2 || mov == 4)
                coluna++;
        }
        else{ // PAR
            if(mov == 1 || mov == 3)
                coluna--;
        }

        if(coluna<0 || coluna>3)
            return 0;
        
        return tabuleiro[linha][coluna] == 0;
    }

    // Tratamento de movimentos de CAPTURA 5 6 7 8

    else if(mov==5 || mov==6 || mov==7 || mov==8){
        int eLinha = linha, eColuna = coluna; // valores para peça adversária
        int equipe = tabuleiro[linha][coluna];

        // tratamento para peça que vai fazer a captura
        if(mov==5 || mov==6) // sobe na matriz
            linha-=2;
        else if(mov==7 || mov==8) // desce na matriz
            linha+=2;

        // Verificando indice fora do campo [8][4]
        if(linha<0 || linha >7)
            return 0;

        if(mov==5 || mov==7) // sobe na matriz
            coluna--;
        else if(mov==6 || mov==8) // desce na matriz
            coluna++;
        
        if(coluna<0 || coluna>3)
            return 0;

        // tratamento para peça inimiga
        if(mov==5 || mov==6) // sobe na matriz
            eLinha--;
        else if(mov==7 || mov==8) // desce na matriz
            eLinha++;

        // tratamento diferente para linhas impares e pares
        // lembrando que a linha ja foi alterada lá em cima, então precisa inverter neste bloco
        if(eLinha%2!=0){ // ÍMPAR
            if(mov == 6 || mov == 8)
                eColuna++;
        }
        else{ // PAR
            if(mov == 5 || mov == 7)
                eColuna--;
        }
        
        if(equipe==1)              // sem tratamento para King <<<<<<<<<<<
            return tabuleiro[linha][coluna] == 0 && tabuleiro[eLinha][eColuna]==-1; // (tabuleiro[eLinha][eColuna]==-10) king
        
        else
            return tabuleiro[linha][coluna] == 0 && tabuleiro[eLinha][eColuna]==1; // (tabuleiro[eLinha][eColuna]==10) king
    }

    return false;
}

void Player::movePeca(int i, int j, int mov, int tabuleiroFilho[8][4]){
    /*
        Legenda:
        i e j: posição da peça a ser movida
        mov  : movimento (andar peça ou capturar)
        tabuleiroFilho: para atualizar o tabuleiro
    */

    int pecaAtual = tabuleiroFilho[i][j]; // obtendo a peca
    tabuleiroFilho[i][j] = 0; // zera o quadrado origem
    
    // movimentos de peça    
    if(mov == 1){
        if(i%2==0) // linha PAR
            tabuleiroFilho[i-1][j] = pecaAtual;
        else // linha IMPAR
            tabuleiroFilho[i-1][j-1] = pecaAtual;
    }
    
    else if(mov == 2){
        if(i%2==0)
            tabuleiroFilho[i-1][j+1] = pecaAtual;
        else
            tabuleiroFilho[i-1][j] = pecaAtual;
    }
    
    else if(mov == 3){
        if(i%2==0)
            tabuleiroFilho[i+1][j] = pecaAtual;
        else
            tabuleiroFilho[i+1][j-1] = pecaAtual;
    }
    
    else if(mov == 4){
        if(i%2==0)
            tabuleiroFilho[i+1][j+1] = pecaAtual;
        else
            tabuleiroFilho[i+1][j] = pecaAtual;
    }

    // ataques (igual para os 2 jogadores)
    else if(mov == 5){
        tabuleiroFilho[i-2][j-1] = pecaAtual; // nao muda para PAR e IMPAR
        if(i%2==0) // linha PAR
            tabuleiroFilho[i-1][j] = 0;
        else // linha IMPAR
            tabuleiroFilho[i-1][j-1] = 0;
    }

    else if(mov == 6){
        tabuleiroFilho[i-2][j+1] = pecaAtual;
        if(i%2==0)
            tabuleiroFilho[i-1][j+1] = 0;
        else
            tabuleiroFilho[i-1][j] = 0;
    }

    else if(mov == 7){
        tabuleiroFilho[i+2][j-1] = pecaAtual;
        if(i%2==0)
            tabuleiroFilho[i+1][j] = 0;
        else
            tabuleiroFilho[i+1][j-1] = 0;
    }

    else if(mov == 8){
        tabuleiroFilho[i+2][j+1] = pecaAtual;
        if(i%2==0)
            tabuleiroFilho[i+1][j+1] = 0;
        else
            tabuleiroFilho[i+1][j] = 0;
    }
}