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

#include "EstadoDamas.hpp"

#include <float.h> // Obter o valor de infinito (DBL_MAX)
#include <vector> // receber os estados filhos
#include <cmath>  // funções max e min.

EstadoDamas * escolhaIA;
double maiorH = -DBL_MAX;
int maxProfundide = 13;


double minimax(Estado * atual, bool eMax, double alfa, double beta, int profundidade){
    // Se o estado for folha, OU atingiu profundidade méxima
    if(atual->eFolha() || profundidade == 0){
        return atual->heuristica();
    }
    double h;
    // Verificar se o nó é de máximo ou mínimo.
    if(eMax){
        // Vez do MAX
        h = -DBL_MAX; // menos infinito.
        // Gerar os filhos de MAX
        std::vector <Estado *> filhos = atual->expandir(true);
        for(int i = 0; i < filhos.size(); i++){
            double hFilho = minimax(filhos[i], false, alfa, beta, profundidade - 1);
            h = std::max(h, hFilho);
            alfa = std::max(alfa, hFilho);
            // Tô na raiz?
            if(profundidade == maxProfundide){
                if(h > maiorH){
                    maiorH = h;
                    escolhaIA = dynamic_cast<EstadoDamas *>(filhos[i]);
                }
            }
            // SEGREDO!!!
            if(alfa >= beta){
                return h;
            }
        }
    }else{
        // Vez do MIN
        h = DBL_MAX; // mais infinito.
        // Gerar os filhos de MIN
        std::vector <Estado *> filhos = atual->expandir(false);
        for(int i = 0; i < filhos.size(); i++){
            double hFilho = minimax(filhos[i], true, alfa, beta, profundidade - 1);
            h = std::min(h, hFilho);
            beta = std::min(beta, hFilho);
            if(alfa >= beta){
                return h;
            }
        }
    }
    // Se nenhuma poda ocorreu, propaga para o pai o h calculado!
    return h;
}

void limpaArvore(EstadoDamas * estado){
    if(!estado->eFolha()){
        for(int i=0; i<estado->filhos.size();i++)
            limpaArvore(estado->filhos[i]);
    }
    if(estado!=escolhaIA){
        delete estado;
    }
}

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
    /*
    int tabuleiro[8][4] = {
        { 0, 0, 0, 0},
        { 0, 0, 0, 0},
        { 1, 0,-1,-1},
        { 0, 0, 0, 0},
        {-1, 0, 0, 0},
        { 1, 0,-1, 0},
        { 0, 0, 0, 0},
        { 0, 0, 0, 0}
    };
    */
    EstadoDamas * atual = new EstadoDamas(tabuleiro, true);
    atual->imprime();
    std::cout<<std::endl<<"Heuristica Inicial: "<<atual->heuristica()<<std::endl;
    
    std::cout<<std::endl<<"----------------"<<std::endl<<std::endl;
    
    // Enquanto o jogo não acabar ... 
    while(!atual->eFolha()){
        maiorH = -DBL_MAX;
        double h = minimax(atual, true, -DBL_MAX, DBL_MAX, maxProfundide);
        std::cout<<std::endl<<"-------IA-------"<<std::endl<<std::endl;
        escolhaIA->imprime();
        
        
        std::cout<<std::endl<<"Debug:"<<std::endl;
        std::cout<<"eFolha: "<<escolhaIA->eFolha()<<std::endl;
        std::cout<<"Heuristica: "<<escolhaIA->heuristica()<<std::endl;
        std::cout<<std::endl<<"----------------"<<std::endl<<std::endl;
        
        /*for(int i=0;i < escolhaIA->filhos.size();i++)
            std::cout << "escolhaIA: " <<escolhaIA->filhos[i] << std::endl;
        */
        if(escolhaIA->eFolha()){
            if(h > 0){
                std::cout << "Brancas venceram!!!" << std::endl;
            }
            break;
        }

        if(maxProfundide>=9)
            limpaArvore(atual);
        
        std::cout<<std::endl<<"-----PLAYER-----"<<std::endl<<std::endl;
        atual = escolhaIA->jogadaHumano();
        //std::cout << "atual: " << atual << std::endl;
        std::cout<<std::endl<<"Debug:"<<std::endl;
        std::cout<<"eFolha: "<<atual->eFolha()<<std::endl;
        std::cout<<"Heuristica: "<<atual->heuristica()<<std::endl;
        std::cout<<std::endl<<"----------------"<<std::endl<<std::endl;
        h = atual->heuristica();
        if(atual->eFolha()){
            if(h < 0){
                std::cout << "Pretas venceram!!!" << std::endl;
            }
            break;
        }
    }
    return 0;
}