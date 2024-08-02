#include "headers/player.hpp"
#include "headers/H_player.hpp"
//#include "headers/AI_player.hpp"
#include <iostream>

using namespace std;

void imprime(int tabuleiro[8][4]){
    for(int i=0;i<8;i++){
        for(int j=0;j<4;j++){
            if(i%2==0) //linha é par, então a ordem de impressão dos quadrados é "branco/preto" 
              std::cout << " ";  
            
            if(tabuleiro[i][j]==1)
                std::cout << "B" << " ";
            else if(tabuleiro[i][j]==-1)
                std::cout << "P" << " ";
            else
                std::cout << "O" << " ";

            if(i%2!=0) //linha é ímpar, então a ordem de impressão dos quadrados é "preto/branco" 
              std::cout << " ";  
            
        }
        std::cout << std::endl;
    }
}

int main(){
    // -1: Black | 1: White
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

    bool team;
    cout<<"White(1)|Black(0)"<<endl<<"Jogador: ";
    cin>>team;
    
    H_Player * player = new H_Player(tabuleiro, team);
    //Player * ia = new IA_player(tabuleiro, team);

    cout<<endl;
    imprime(tabuleiro);
    player->teste(tabuleiro);
    cout<<endl;
    imprime(tabuleiro);
    cout<<endl;
    player->movePeca(5,2,2,tabuleiro);
    imprime(tabuleiro);
}