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

// g++ campocv.cpp -o campo -I/usr/include/opencv4 -lprotobuf -lpthread -lopencv_core -lopencv_imgproc -lopencv_highgui -L/usr/local/share/opencv4

#include "EstadoDamas.hpp"

#include <float.h> // Obter o valor de infinito (DBL_MAX)
#include <vector> // receber os estados filhos
#include <cmath>  // funções max e min.

// bibliotecas usadas para depuracao
#include <cstdlib>
#include <time.h> 

// bibliotecas usadas para o campo
#include <unistd.h> // usada para o sleep()
#include "opencv2/opencv.hpp"
using namespace cv;
#include <cmath>
#include <thread>

EstadoDamas * escolhaIA , * atual;
int tabuleiroGlut[8][4];
double maiorH = -DBL_MAX;
int maxProfundide = 7, turno = 0;

// variaveis usadas nas configurações do jogo
bool pecasIA=true, pecasJ=false, semMov=false;
short int dif = 7;
// 1- muito fácil / 3 - fácil / 5 - médio / 7 - difícil / 9 - muito difícil

void drawPiece(Mat& img, int x, int y, Scalar color, bool isKing) {
    circle(img, Point(x, y), 30, color, -1);

    if (isKing) {
        circle(img, Point(x, y), 15, Scalar(0, 180, 250), -1);
    }
}

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
            if(escolhaIA->geteMax())
                if(profundidade == maxProfundide){
                    if(h > maiorH){
                        maiorH = h;
                        escolhaIA = dynamic_cast<EstadoDamas *>(filhos[i]);
                    }
                }
            /*
            // SEGREDO!!!
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
            if(!escolhaIA->geteMax())
                if(profundidade == maxProfundide){
                    if(h > maiorH){
                        maiorH = h;
                        escolhaIA = dynamic_cast<EstadoDamas *>(filhos[i]);
                    }
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

void formataTabuleiro(int tabuleiro[8][4]){
    for(int j=0;j<4;j++)
        tabuleiro[0][j] = tabuleiro[0][j] == 1 ? tabuleiro[0][j]*3 : tabuleiro[0][j];

    for(int j=0;j<4;j++)
        tabuleiro[7][j] = tabuleiro[7][j] == -1 ? tabuleiro[7][j]*3 : tabuleiro[7][j];
}

void config(){
    std::cout << "\n\tPAINEL DE CONFIGURAÇÃO\n" << std::endl;
    std::cout << "Cor IA\n(0)Pretas ou (1)Brancas: " << std::endl;
    std::cin >> pecasIA;
    pecasJ = true;
    if(pecasIA)
        pecasJ = false;
    std::cout << "Dificuldade: " << std::endl;
    std::cout << " (1) - muito fácil \n (3) - fácil \n (5) - médio \n (7) - difícil \n (9) - muito difícil" << std::endl;
    std::cin >> dif;
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
    
    /*
    int tabuleiro[8][4] = {
        { 0, -1, 0, -1},
        { -1, -1, 0, 0},
        { 0, 1, 0, -1},
        { 0, 1, 0, 0},
        { 1, 1, 1, 0},
        { 0, 0, 0, 0},
        { 0, 0, 0, 0},
        { 1, 1, 1, 1}
    };
    */
    /* 
      int tabuleiro[8][4] = {
        { 0, -1, 0, -1},
        { -1, -1, 0, 0},
        { 0, 0, 0, -1},
        { 0, 1, 1, 0},
        { 1, 1, 1, 0},
        { 0, 0, 0, 0},
        { 0, 0, 0, 0},
        { 1, 1, 1, 1}
    };
    */
    /*
    int tabuleiro[8][4] = {
        {-1, -1, 0, -1},
        { -1, -1, 0, 0},
        { 0, 1, 0, -1},
        { 0, 1, 0, 0},
        { 1, 1, 1, 0},
        { 0, 0, 1, 0},
        { 0, 0,-1, 0},
        { 1, 1, 0, 0}
    };
    */
    /*
    int tabuleiro[8][4] = {
        { 0, 0, 0, 0},
        { 0, 0, 0, 0},
        { 0, 0, 1, 0},
        { 0,-1, 0, 0},
        { 0, 0, 1, 0},
        { 0,-1, -1, 0},
        { 0, 1, 0, 0},
        { 0, 0, 0, 0}
    };
    */
    /*
    int tabuleiro[8][4] = {
        { 0, 0, -1, 0},
        { 0, 0, -1, 0},
        { 0, 0, 0, 0},
        { 0, 0,-1, 0},
        { 0, 0, 0, 0},
        { 0,-1, -1, 0},
        { 0, 1, 0, 0},
        { 0, 0, 0, 0}
    };
    */
    /*
    int tabuleiro[8][4] = {
        { 3, 0, 0, 0},
        { -1, 0, 0, 0},
        { -1, 0, 0, 0},
        { 0, -1, 0, 0},
        { 0, -1, 0, 0},
        { 0, 0, -1, 0},
        { 0, 0, 0, 0},
        { 0, 0, 0, 0}
    };
    */
    /*int tabuleiro[8][4] = {
        { 0, 0, 0, 0},
        { 1, 1, 0, 0},
        { -3, 0, 1, 0},
        { 1, 1, 0, 0},
        { 0, 0, 0, 0},
        { 1, 0, 0, 0},
        { -1, 0, 1, 0},
        { 0, 0, 0, 0}
    };*/
    /*
    int tabuleiro[8][4] = {
        { 0, 0, 0, 0},
        { 1, 1, 1, 0},
        { -3, 0, 1, 0},
        { 1, 1, 0, 0},
        { 0, 0, 0, 0},
        { 1, 0, 0, 0},
        { -1, 0, 1, 0},
        { 0, 0, 0, 0}
    };
    

   int tabuleiro[8][4] = {
        { -1, 0, 0, 0},
        { 0, 1, 1, 0},
        { 0, 0, 0, 0},
        { 0, 0, 1, 0},
        { 0, 0, 0, 0},
        { 1, 0, 0, 0},
        { -1, 0, 0, 0},
        { 0, 0, 0, 0}
    };
    */

    formataTabuleiro(tabuleiro);

    Mat img(800, 800, CV_8UC3, Scalar(255, 255, 255));

    // Desenhando o tabuleiro completo
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Scalar color = (i + j) % 2 == 0 ? Scalar(200, 200, 200) : Scalar(0, 152, 0);
            rectangle(img, Rect(j * 100, i * 100, 100, 100), color, -1);
        }
    }

    int k=-1;
    // Desenhando as peças
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ((i + j) % 2 != 0) {
                k++;
                if (tabuleiro[i][k] == -1) {
                    Scalar color = Scalar(0, 0, 0);
                    drawPiece(img, j * 100 + 50, i * 100 + 50, color, false);
                }
                else if (tabuleiro[i][k] == -3){
                    Scalar color = Scalar(0, 0, 0);
                    drawPiece(img, j * 100 + 50, i * 100 + 50, color, true);
                }
                else if (tabuleiro[i][k] == 1) {
                    Scalar color = Scalar(250, 250, 250);
                    drawPiece(img, j * 100 + 50, i * 100 + 50, color, false);
                }
                else if (tabuleiro[i][k] == 3){
                    Scalar color = Scalar(250, 250, 250);
                    drawPiece(img, j * 100 + 50, i * 100 + 50, color, true);
                }
            }
        }
        k=-1;
    }

    imshow("Damas", img);
    waitKey(0);
    
    escolhaIA = new EstadoDamas(tabuleiro, false);
    EstadoDamas * atual = new EstadoDamas(tabuleiro, true);
    atual->imprime();
    std::cout<<std::endl<<"Heuristica Inicial: "<<atual->heuristica()<<std::endl;
    std::cout<<std::endl<<"Dificuldade: "<<maxProfundide<<std::endl;
    std::cout<<std::endl<<"----------------"<<std::endl<<std::endl;
    
    // PAINEL DE CONFIGURAÇÃO
    //config();
    
    // Enquanto o jogo não acabar ... 
    
    //while(!atual->eFolha()){
    for(;;){ // MVA METOD OF while(true)
        maxProfundide = dif;
        escolhaIA->seteMax(pecasIA);
        turno++;
        std::cout<<"Turno: "<<turno <<std::endl;
        maiorH = -DBL_MAX;
        double h = minimax(atual, escolhaIA->geteMax(), -DBL_MAX, DBL_MAX, maxProfundide);
        std::cout<<std::endl<<"-------IA-------"<<std::endl<<std::endl;
        srand(time(0)); 
        int randomNum = rand() % 8;
        sleep((int)(randomNum/2) + 1);
        try{
            escolhaIA->imprime();
        }  
        catch(...){
            std::cout<<"Sem Tabuleiro"<<std::endl;
        }

        
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 4; j++){
                tabuleiroGlut[i][j] = escolhaIA->tabuleiro[i][j];
            }
        }
        
        // Desenhando o tabuleiro completo
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Scalar color = (i + j) % 2 == 0 ? Scalar(200, 200, 200) : Scalar(0, 152, 0);
                rectangle(img, Rect(j * 100, i * 100, 100, 100), color, -1);
            }
        }

        int k=-1;
        // Desenhando as peças
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if ((i + j) % 2 != 0) {
                    k++;
                    if (tabuleiroGlut[i][k] == -1) {
                        Scalar color = Scalar(0, 0, 0);
                        drawPiece(img, j * 100 + 50, i * 100 + 50, color, false);
                    }
                    else if (tabuleiroGlut[i][k] == -3){
                        Scalar color = Scalar(0, 0, 0);
                        drawPiece(img, j * 100 + 50, i * 100 + 50, color, true);
                    }
                    else if (tabuleiroGlut[i][k] == 1) {
                        Scalar color = Scalar(250, 250, 250);
                        drawPiece(img, j * 100 + 50, i * 100 + 50, color, false);
                    }
                    else if (tabuleiroGlut[i][k] == 3){
                        Scalar color = Scalar(250, 250, 250);
                        drawPiece(img, j * 100 + 50, i * 100 + 50, color, true);
                    }
                }
            }
            k=-1;
        }

        imshow("Damas", img);
        waitKey(10);
        
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
            sleep(10);
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
            pecasIA = false;
            pecasJ = true;
            escolhaIA = new EstadoDamas(tabuleiro, pecasIA);
            atual = new EstadoDamas(tabuleiro, pecasJ);
            /*
            return 0;
            break;
            */
        }
        else{

        

        //if(maxProfundide>=9)
            limpaArvore(atual);
        
        std::cout<<std::endl<<"-----PLAYER-----"<<std::endl<<std::endl;
    
        std::vector <Estado *> escolhas = escolhaIA->expandir(pecasJ);
        /*
        std::cout << "Escolha sua Jogada\n" << std::endl;
        for(int i = 0; i < escolhas.size(); i++){
            atual = dynamic_cast<EstadoDamas *>(escolhas[i]);
            std::cout <<"     ("<< i <<")" << std::endl;
            atual->imprime();
            std::cout << "----------------\n" << std::endl;
        }
        */
        if(!escolhas.empty()){
            /*
            int escolha;
            std::cout<<"Numero da jogada: "; std::cin >> escolha;
            atual = dynamic_cast<EstadoDamas *>(escolhas[escolha]);
            
            */
            //JOGAR AUTOMATICO DE FORMA ALEATORIA
            srand(time(0)); 
            int randomNum = rand() % escolhas.size();
            sleep(((int)randomNum/2)+1);
            std::cout<<"Numero da jogada: " << randomNum << std::endl; 
            atual = dynamic_cast<EstadoDamas *>(escolhas[randomNum]);
            
            
        }else{
            std::cout << "Sem jogadas disponiveis" << std::endl;
            semMov=true;
            return 0;
        }
        

        //atual = escolhaIA->jogadaHumano();
        //std::cout << "atual: " << atual << std::endl;
        std::cout<<std::endl<<"Debug:"<<std::endl;
        std::cout<<"eFolha: "<<atual->eFolha()<<std::endl;
        std::cout<<"Heuristica: "<<atual->heuristica()<<std::endl;
        std::cout<<std::endl<<"----------------"<<std::endl<<std::endl;
        h = atual->heuristica();
        if(atual->eFolha() || semMov){
            if(semMov){
                std::cout << "Brancas venceram!!!" << std::endl;
            }
            if(h < 0){
                std::cout << "Pretas venceram!!!" << std::endl;
            }
            sleep(10);
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
            pecasIA = true;
            pecasJ = false;
            escolhaIA = new EstadoDamas(tabuleiro, pecasIA);
            atual = new EstadoDamas(tabuleiro, pecasJ);
            /*
            return 0;
            break;
            */
        }
        
    
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 4; j++){
                tabuleiroGlut[i][j] = atual->tabuleiro[i][j];
            }
        }

        // Desenhando o tabuleiro completo
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Scalar color = (i + j) % 2 == 0 ? Scalar(200, 200, 200) : Scalar(0, 152, 0);
                rectangle(img, Rect(j * 100, i * 100, 100, 100), color, -1);
            }
        }

        k=-1;
        // Desenhando as peças
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if ((i + j) % 2 != 0) {
                    k++;
                    if (tabuleiroGlut[i][k] == -1) {
                        Scalar color = Scalar(0, 0, 0);
                        drawPiece(img, j * 100 + 50, i * 100 + 50, color, false);
                    }
                    else if (tabuleiroGlut[i][k] == -3){
                        Scalar color = Scalar(0, 0, 0);
                        drawPiece(img, j * 100 + 50, i * 100 + 50, color, true);
                    }
                    else if (tabuleiroGlut[i][k] == 1) {
                        Scalar color = Scalar(250, 250, 250);
                        drawPiece(img, j * 100 + 50, i * 100 + 50, color, false);
                    }
                    else if (tabuleiroGlut[i][k] == 3){
                        Scalar color = Scalar(250, 250, 250);
                        drawPiece(img, j * 100 + 50, i * 100 + 50, color, true);
                    }
                }
            }
            k=-1;
        }

        imshow("Damas", img);
        waitKey(10);
        }
    }
    
    return 0;
}