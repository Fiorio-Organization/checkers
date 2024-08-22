#include "EstadoDamas.hpp"

EstadoDamas::EstadoDamas(int tabuleiro[8][4], bool eMax){
    // Copia o tabuleiro vindo do pai para o filho.
    this->copia(tabuleiro, this->tabuleiro);
    this->eMax = eMax;
}


bool EstadoDamas::geteMax(){
    return this->eMax;
}


void EstadoDamas::copia(int origem[8][4], int destino[8][4]){
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 4; j++){
            destino[i][j] = origem[i][j];
        }
    }
}

bool EstadoDamas::eFolha(){
    /*
        CONDICÕES DE FIM DE JOGO
        1 - Um dos jogadores perder todas as peças (Pretas = 0 ou Brancas = 0)
        2 - Um dos jogadores não consegue fazer um movimento válido
        3 - Nenhum dos jogadores tem condição possível de vitória
        4 - É repetido 3 movimentos idênticos consecutivos por ambos os jogadores (adaptada, não necessária)
    */

    // REGRA 1
    bool pecasMIN = false;
    bool pecasMAX = false;

    // REGRA 2
    bool podeMoverMIN = false;
    bool podeMoverMAX = false;

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 4; j++){
            if(tabuleiro[i][j] == 1){
                pecasMAX = true; // MAX tem pecas (Regra 1)

                if(!podeMoverMAX){
                    for(int k=1; k<=8; k++){
                        podeMoverMAX = ePermitido(tabuleiro,i,j,k);
                        if(podeMoverMAX) // MAX pode mover (Regra 2)
                            break;
                    }
                }
            }

                         
            else if(tabuleiro[i][j] == -1){
                pecasMIN = true; // MIN tem pecas (Regra 1)

                if(!podeMoverMIN){
                    for(int k=1; k<=8; k++){
                        podeMoverMIN = ePermitido(tabuleiro,i,j,k);
                        if(podeMoverMIN) // MIN pode mover (Regra 2)
                            break;
                    }
                }  
            }
        }
    }
    return (!pecasMAX && !pecasMIN) || (!podeMoverMAX || !podeMoverMIN); // é folha
}

double EstadoDamas::heuristica(){
    /*
        Base: Quanto mais peças tenho, melhor é minha situação de jogo
        Melhor para brancas quando retorna positivo
        Melhor para pretas quando retorna negativo
    */
   int pecasMIN = 0;
   int pecasMAX = 0;

   for(int i = 0; i < 8; i++){
        for(int j = 0; j < 4; j++){
            if(tabuleiro[i][j] == 1 || tabuleiro[i][j] == 5) //branca
                pecasMAX+= tabuleiro[i][j];
            else if(tabuleiro[i][j] == -1 || tabuleiro[i][j] == -5) //preta
                pecasMIN+= tabuleiro[i][j];
        }
    }

    return pecasMAX+pecasMIN;
}

bool EstadoDamas::ePermitido(int tabuleiro[8][4], int linha, int coluna, int mov){
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

void EstadoDamas::movePeca(int i, int j, int mov, int tabuleiroFilho[8][4]){
    /*
        Legenda:
        i e j são a posição da peça a ser movida
        mov é o movimento (andar peça ou capturar)
        vector filhos                                        <<<< completar
    */

    int pecaAtual = tabuleiroFilho[i][j]; // obtendo a peca
    tabuleiroFilho[i][j] = 0; // zera o quadrado da peça selecionada, pois ela vai se mover
    
    // movimentos de peça    
    if(mov == 1){
        if(i%2==0) // linha PAR
            tabuleiroFilho[i-1][j] = i-1 == 0 ? pecaAtual*5 : pecaAtual;
        else // linha IMPAR
            tabuleiroFilho[i-1][j-1] = i-1 == 0 ? pecaAtual*5 : pecaAtual;
    }
    
    else if(mov == 2){
        if(i%2==0)
            tabuleiroFilho[i-1][j+1] = i-1 == 0 ? pecaAtual*5 : pecaAtual;
        else
            tabuleiroFilho[i-1][j] = i-1 == 0 ? pecaAtual*5 : pecaAtual;
    }
    
    else if(mov == 3){
        if(i%2==0)
            tabuleiroFilho[i+1][j] = i+1 == 7 ? pecaAtual*5 : pecaAtual;
        else
            tabuleiroFilho[i+1][j-1] = i+1 == 7 ? pecaAtual*5 : pecaAtual;
    }
    
    else if(mov == 4){
        if(i%2==0)
            tabuleiroFilho[i+1][j+1] = i+1 == 7 ? pecaAtual*5 : pecaAtual;
        else
            tabuleiroFilho[i+1][j] = i+1 == 7 ? pecaAtual*5 : pecaAtual;
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

std::vector<Estado *> EstadoDamas::seqCaptura(int tabuleiro[8][4], int i, int j, int mov, short int profundidade){
    std::vector <Estado *> filhos;
    bool teveJogada = false;
    short int prof_local=profundidade - 1, prof_global = this->profJogada;

    //if(prof_local > this->profJogada)
        //this->profJogada = prof_local;

    // mudar valores de i e j para apontar a peca certa na matriz
    
    //movePeca(i,j,mov,tabuleiro);

    if(mov == 5){ 
        i-=2;
        j--;
    }
    else if(mov == 6){
        i-=2;
        j++;
    }

    else if(mov == 7){
        i+=2;
        j--;
    }
        
    else if(mov == 8){
        i+=2;
        j++;
    }
    int i_local = i, j_local = j;
    int tabuleiroFilho[8][4]; // criar filho temporario igual o pai
    this->copia(tabuleiro, tabuleiroFilho);

    for(int mov=5;mov<=8;mov++){
        if(ePermitido(tabuleiro,i,j,mov)){
            teveJogada = true;
            prof_local = profundidade;
            this->copia(tabuleiro, tabuleiroFilho);
            movePeca(i,j,mov,tabuleiroFilho);
            
            if(this->profJogada < profundidade){
                this->profJogada = profundidade;
                if(!filhos.empty())
                    filhos.erase(filhos.begin(),filhos.begin()+filhos.size());
            }
            //prof_local = ;
            seqCaptura(tabuleiroFilho, i, j, mov, profundidade + 1);
            
        }
    }

    if(this->profJogada == prof_local && teveJogada==true || this->profJogada==1){
        bool existe=false;
        /*
        for(int i=0; i< this->filhos.size();i++)
            if(this->filhos[i]->seExisteEstado(tabuleiroFilho)){
                filhos.push_back(this->filhos[i]);
                existe = true;
            }
        */
        if(!existe){
            EstadoDamas * filho = new EstadoDamas(tabuleiroFilho, this->eMax);
        
            filhos.push_back(filho);
            this->filhos.push_back(filho);
            std::vector<EstadoDamas *> teste;
            for(int l=0;l<this->filhos.size();l++){
                teste.push_back(this->filhos[l]);
            }
        }
    } 
    return filhos;
}

std::vector<Estado *> EstadoDamas::expandir(bool eMax){
    std::vector <Estado *> filhos,newFilhos;
    int c=0,x=-1;
    short int prof_local=0;

    if(eMax == 1)
        x = 1;

    this->profJogada = 0;
    
    for(int i=0; i<8; i++)
        for(int j=0; j<4; j++)
            if(tabuleiro[i][j] == x){
                for(int mov=5;mov<=8;mov++){
                    if(ePermitido(tabuleiro,i,j,mov)){
                        // funcaoCaptura();
                        int tabuleiroFilho[8][4]; // criar filho temporario igual o pai
                        this->copia(this->tabuleiro, tabuleiroFilho);
                        movePeca(i,j,mov,tabuleiroFilho);
                        this->profJogada = this->profJogada<1 ? 1 : this->profJogada;
                        
                        prof_local = this->profJogada;
                        newFilhos = seqCaptura(tabuleiroFilho, i, j, mov, 2);
                        if(prof_local!=this->profJogada){ // this->profJogada foi alterado?
                            // foi alterado, então deletar todo conteudo do vetor filhos
                            if(!filhos.empty())
                                filhos.erase(filhos.begin(),filhos.begin()+filhos.size());
                        }

                        if(!newFilhos.empty())
                            for(int l=0;l<newFilhos.size();l++)
                                filhos.push_back(newFilhos[l]);
                    }
                }
                
            }
        
    //if(filhos.empty()){
    if(this->profJogada==0){
        for(int i=0; i<8; i++)
            for(int j=0; j<4; j++)
                if(tabuleiro[i][j] == x){
                    // funcaoMovimento();
                    if(x==1){
                        for(int mov=1; mov<=2; mov++) // 1 = supEsq | 2 = supDir
                            if(ePermitido(tabuleiro,i,j,mov)){
                                int tabuleiroFilho[8][4]; // criar filho temporario igual o pai
                                this->copia(this->tabuleiro, tabuleiroFilho);
                                movePeca(i,j,mov,tabuleiroFilho);

                                bool existe=false;
                                /*
                                for(int i=0; i< this->filhos.size();i++)
                                    if(this->filhos[i]->seExisteEstado(tabuleiroFilho)){
                                        filhos.push_back(this->filhos[i]);
                                        existe = true;
                                    }
                                */
                                if(!existe){
                                    EstadoDamas * filho = new EstadoDamas(tabuleiroFilho, this->eMax);
                                
                                    filhos.push_back(filho);
                                    this->filhos.push_back(filho);
                                }
                            }
                    }else{
                        for(int mov=3; mov<=4; mov++) //  3 = infEsq | 4 = infDir
                            if(ePermitido(tabuleiro,i,j,mov)){
                                int tabuleiroFilho[8][4]; // criar filho temporario igual o pai
                                this->copia(this->tabuleiro, tabuleiroFilho);
                                movePeca(i,j,mov,tabuleiroFilho);
                                
                                bool existe=false;
                                /*
                                for(int i=0; i< this->filhos.size();i++)
                                    if(this->filhos[i]->seExisteEstado(tabuleiroFilho)){
                                        filhos.push_back(this->filhos[i]);
                                        existe = true;
                                    }
                                */
                                if(!existe){
                                    EstadoDamas * filho = new EstadoDamas(tabuleiroFilho, this->eMax);
                                
                                    filhos.push_back(filho);
                                    this->filhos.push_back(filho);
                                }
                            }
                    }
 
                }
    }
    
    return filhos;
    /*
    if(eMax){ // pecas brancas
        for(int i=0; i<8; i++)
            for(int j=0; j<4; j++)
                if(tabuleiro[i][j] == 1){
                    // verifica se eh possivel capturar
                    seqCaptura(tabuleiroFilho, i, j, profundidade++, x);
                    seqCaptura(tabuleirom,i,j,profundidade, filhos, x);
                
                    for(int mov=1; mov<=8; mov++) //1 = supEsq | 2 = supDir | 3 = Esq | 4 = Dir | 5 = infEsq | 6 = infDir / dir é pra onde ta olhando

                        //trata 1 peca e a move para a direcao dir somente
                        if((mov<=2 || mov>=5) && ePermitido(i,j,mov)){ // verifica se o espaco na direcao dir é vazio para fazer o movimento nessa direcao mesmo
                            int tabuleiroFilho[8][4]; // criar filho temporario igual o pai
                            this->copia(this->tabuleiro, tabuleiroFilho);
                            movePeca(i,j,mov,tabuleiroFilho);
                            seqCaptura(tabuleiroFilho, i, j, mov, prof_local, x);
                            
    */
                            //DEBUG
                            /*
                            c++;
                            std::cout<<"CONTADOR: "<<c<<std::endl;
                            std::cout<<"i: "<<i<<std::endl;
                            std::cout<<"j: "<<j<<std::endl;
                            std::cout<<"mov: "<<mov<<std::endl;
                            
                            std::cout<<"Tabuleiro PAI"<<std::endl;
                            imprime(this->tabuleiro);
                            std::cout<<"Tabuleiro FILHO"<<std::endl;
                            imprime(tabuleiroFilho);
                            */
                            /*
                            bool existe=false;
                            for(int i=0; i< this->filhos.size();i++)
                                if(this->filhos[i]->seExisteEstado(tabuleiroFilho)){
                                    filhos.push_back(this->filhos[i]);
                                    existe = true;
                                }
                            if(!existe){
                                EstadoDamas * filho = new EstadoDamas(tabuleiroFilho, this->eMax);
                            
                                filhos.push_back(filho);
                                this->filhos.push_back(filho);
                            }
                            
                        }   
                }
    }
    else // eMax == false
        for(int i=0; i<8; i++)
            for(int j=0; j<4; j++)
                if(tabuleiro[i][j] == -1)
                    // verifica se tem 1 peca aliada nessa direcao dir e se é possivel fazer o movimento para tal direcao
                    for(int mov=3; mov<=8; mov++) //1 = supEsq | 2 = supDir | 3 = Esq | 4 = Dir | 5 = infEsq | 6 = infDir / dir é pra onde ta olhando
                        //trata 1 peca e a move para a direcao dir somente
                        if(ePermitido(i,j,mov)){ // verifica se o espaco na direcao dir é vazio para fazer o movimento nessa direcao mesmo
                            int tabuleiroFilho[8][4]; // criar filho temporario igual o pai
                            this->copia(this->tabuleiro, tabuleiroFilho);

                            movePeca(i,j,mov,tabuleiroFilho);
                            */
                            //DEBUG
                            /*
                            c++;
                            std::cout<<"CONTADOR: "<<c<<std::endl;
                            std::cout<<"i: "<<i<<std::endl;
                            std::cout<<"j: "<<j<<std::endl;
                            std::cout<<"mov: "<<mov<<std::endl;
                            
                            std::cout<<"Tabuleiro PAI"<<std::endl;
                            imprime(this->tabuleiro);
                            std::cout<<"Tabuleiro FILHO"<<std::endl;
                            imprime(tabuleiroFilho);
                            */
                            /*
                            bool existe=false;
                            for(int i=0; i< this->filhos.size();i++)
                                if(this->filhos[i]->seExisteEstado(tabuleiroFilho)){
                                    filhos.push_back(this->filhos[i]);
                                    existe = true;
                                }
                            if(!existe){
                                EstadoDamas * filho = new EstadoDamas(tabuleiroFilho, this->eMax);
                            
                                filhos.push_back(filho);
                                this->filhos.push_back(filho);
                            }
                        }   
    
    return filhos;
    */
}

EstadoDamas * EstadoDamas::jogadaHumano(){
    int l,c,mov;
    std::cout<<"Jogada Humana"<<std::endl;
    
    while(true){
        std::cout<<"Escolha a peca [linha coluna]: ";
        std::cin>>l>>c;
        if(this->tabuleiro[l][c]== -1){

            std::cout<<"Movimento (1): Sup esq   |   Ataque (5): Sup esq"<<std::endl;
            std::cout<<"Movimento (2): Sup dir   |   Ataque (6): Sup dir"<<std::endl;
            std::cout<<"Movimento (3): Inf esq   |   Ataque (7): Inf esq"<<std::endl;
            std::cout<<"Movimento (4): Inf dir   |   Ataque (8): Inf dir"<<std::endl;
            std::cout<<"Escolha: ";
            std::cin>>mov;

            if(ePermitido(this->tabuleiro,l,c,mov)){
                int tabuleiroHumano[8][4];
                this->copia(this->tabuleiro, tabuleiroHumano);

                this->movePeca(l,c,mov,tabuleiroHumano);
                
                /*
                for(int i=0; i<this->filhos.size();i++)
                    if(this->filhos[i]->seExisteEstado(tabuleiroHumano))
                        return this->filhos[i];
                */
                return new EstadoDamas(tabuleiroHumano, this->eMax);
            }
            else
                std::cout<<"[ERRO]Movimento/Ataque irregular"<<std::endl;

        }else{
            std::cout<<"[ERRO]Peca adversaria"<<std::endl;
        }
    }
}

void EstadoDamas::imprime(){
    for(int i=0;i<8;i++){
        for(int j=0;j<4;j++){
            if(i%2==0) //linha é par, então a ordem de impressão dos quadrados é "branco/preto" 
              std::cout << " ";  
            
            if(this->tabuleiro[i][j]==1)
                std::cout << "b" << " ";
            else if(this->tabuleiro[i][j]==5)
                std::cout << "B" << " ";
            else if(this->tabuleiro[i][j]==-1)
                std::cout << "p" << " ";
            else if(this->tabuleiro[i][j]==-5)
                std::cout << "P" << " ";
            else
                std::cout << "O" << " ";

            if(i%2!=0) //linha é ímpar, então a ordem de impressão dos quadrados é "preto/branco" 
              std::cout << " ";  
            
        }
        std::cout << std::endl;
    }
}

void EstadoDamas::imprime(int tabuleiro[8][4]){
    for(int i=0;i<8;i++){
        for(int j=0;j<4;j++){
            if(i%2==0) //linha é par, então a ordem de impressão dos quadrados é "branco/preto" 
              std::cout << " ";  
            
            if(tabuleiro[i][j]==1)
                std::cout << "b" << " ";
            else if(tabuleiro[i][j]==5)
                std::cout << "B" << " ";
            else if(tabuleiro[i][j]==-1)
                std::cout << "p" << " ";
            else if(tabuleiro[i][j]==-5)
                std::cout << "P" << " ";
            else
                std::cout << "O" << " ";

            if(i%2!=0) //linha é ímpar, então a ordem de impressão dos quadrados é "preto/branco" 
              std::cout << " ";  
            
        }
        std::cout << std::endl;
    }
}

bool EstadoDamas::seExisteEstado(int tabuleiro[8][4]){
    for(int i=0;i<7;i++)
        for(int j=0;j<4;j++){
            if(tabuleiro[i][j] != this->tabuleiro[i][j])
                return false;
        }
    return true;
}