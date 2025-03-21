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

// bibliotecas usadas para depuracao
#include <cstdlib>
#include <time.h> 

EstadoDamas * escolhaIA;
double maiorH = -DBL_MAX;
int maxProfundide = 7, turno = 0;
// 1- muito fácil / 3 - fácil / 5 - médio / 7 - difícil / 9 - muito difícil

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
            //alfa = std::max(alfa, hFilho);
            // Tô na raiz?
            if(profundidade == maxProfundide){
                if(h > maiorH){
                    maiorH = h;
                    escolhaIA = dynamic_cast<EstadoDamas *>(filhos[i]);
                }
            }
            /* 
            // Poda
            if(alfa >= beta){
                return h;
            }
            */
        }
    }else{
        // Vez do MIN
        h = DBL_MAX; // mais infinito.
        // Gerar os filhos de MIN
        std::vector <Estado *> filhos = atual->expandir(false);
        for(int i = 0; i < filhos.size(); i++){
            double hFilho = minimax(filhos[i], true, alfa, beta, profundidade - 1);
            h = std::min(h, hFilho);
            /*
            beta = std::min(beta, hFilho);
            if(alfa >= beta){
                return h;
            }
            */
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

void formataTabuleiro(int tabuleiro[8][4]){
    for(int j=0;j<4;j++)
        tabuleiro[0][j] = tabuleiro[0][j] == 1 ? tabuleiro[0][j]*3 : tabuleiro[0][j];

    for(int j=0;j<4;j++)
        tabuleiro[7][j] = tabuleiro[7][j] == -1 ? tabuleiro[7][j]*3 : tabuleiro[7][j];
}

int main(){

    // tabuleiro inicial
    /**/
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

    formataTabuleiro(tabuleiro);
    
    escolhaIA = new EstadoDamas(tabuleiro, true);
    EstadoDamas * atual = new EstadoDamas(tabuleiro, true);
    atual->imprime();
    std::cout<<std::endl<<"Heuristica Inicial: "<<atual->heuristica()<<std::endl;
    std::cout<<std::endl<<"Dificuldade: "<<maxProfundide<<std::endl;
    std::cout<<std::endl<<"----------------"<<std::endl<<std::endl;
    
    while(!atual->eFolha()){
        turno++;
        std::cout<<"Turno: "<<turno <<std::endl;
        maiorH = -DBL_MAX;
        double h = minimax(atual, escolhaIA->geteMax(), -DBL_MAX, DBL_MAX, maxProfundide);
        std::cout<<std::endl<<"-------IA-------"<<std::endl<<std::endl;
        try{
            escolhaIA->imprime();
        }  
        catch(...){
            std::cout<<"Sem Tabuleiro"<<std::endl;
        }

        
        
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

        //if(maxProfundide>=9)
            limpaArvore(atual);
        
        std::cout<<std::endl<<"-----PLAYER-----"<<std::endl<<std::endl;
    
        std::vector <Estado *> escolhas = escolhaIA->expandir(false);
        std::cout << "Escolha sua Jogada\n" << std::endl;
        for(int i = 0; i < escolhas.size(); i++){
            atual = dynamic_cast<EstadoDamas *>(escolhas[i]);
            std::cout <<"     ("<< i <<")" << std::endl;
            atual->imprime();
            std::cout << "----------------\n" << std::endl;
        }
        if(!escolhas.empty()){
            // Escolher entre jogar manualmente ou automatico
            /*
            int escolha;
            std::cout<<"Numero da jogada: "; std::cin >> escolha;
            atual = dynamic_cast<EstadoDamas *>(escolhas[escolha]);
            
            */
            //JOGAR AUTOMATICO DE FORMA ALEATORIA
            srand(time(0)); 
            int randomNum = rand() % escolhas.size();
            std::cout<<"Numero da jogada: " << randomNum << std::endl; 
            atual = dynamic_cast<EstadoDamas *>(escolhas[randomNum]);
            
        }else{
            std::cout << "Sem jogadas disponiveis" << std::endl;
            return 0;
        }
        
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