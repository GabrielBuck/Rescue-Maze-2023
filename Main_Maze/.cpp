#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <bitset>

using namespace std;

//Informacoes a serem recebidas
bool passagens[4];
char cor;
char mov;
int ori;

//Mapa
const int SIZE = 9;
unsigned char mapa[SIZE][SIZE] = {0};

//Cordenadas do robo
int x = 4, y = x;

void atualizacao(bool passagens[4], char cor);
void orientacao();
void move_cordenada();

int main(){


    //Loop de atualizacao do mapa
    while(true){

        //Movimenta as cordenadas
        cout << "Informe o movimento e orientacao: ";
        cin >> mov >> ori;
        orientacao();
        move_cordenada();
        //for(int i=0;i<4;i++){cout << passagens[i];}

        //Recebe as iformacoes sobre o quadrado atual
        cout << "Informe as passagens e cor: ";
        for(int i=0;i<4;i++){cin >> passagens[i];}
        cin >> cor;

        //Atualiza a matriz de acordo
        atualizacao(passagens,cor);

        // Exibe a matriz na tela
        for (int i = 0; i < SIZE; i++){
            for (int j = 0; j < SIZE; j++){
                 cout << "[" << setw(2) << hex << (int)mapa[i][j] << "] ";
                 //cout << bitset<8>(mapa[i][j]).to_string();
            }
            cout << endl;
        }
    }

}

//Funcao que atualiza a posicao atual do mapa
void atualizacao(bool passagens[4], char cor){

    bool bit_cor[2];

    //Adiciona os bits das passagens
    for(int i=0; i<4; i++) {
        mapa[x][y] |= (1 << passagens[i]);
        mapa[x][y] = mapa[x][y] << 1;
    }
    //Verifica e adiciona os bits da cor
    if(cor=='w'){bit_cor[0] = false; bit_cor[1] = true;}
    else if(cor=='b'){bit_cor[0] = true; bit_cor[1] = false;}
    else if(cor=='s'){bit_cor[0] = true; bit_cor[1] = true;}
    for(int i=0; i<2; i++) {
        mapa[x][y] |= (1 << bit_cor[i]);
        mapa[x][y] = mapa[x][y] << 1;
    }

}

//Funcao que ajusta as passagens, de acordo com a orientacao do robo
void orientacao(){
    bool aux;
    if(ori==0){}//Frente
    else if(ori==1){//Direita
        aux = passagens[0];
        passagens[0] = passagens[3];
        passagens[3] = passagens[2];
        passagens[2] = passagens[1];
        passagens[1] = aux;
    }else if(ori==2){//Tras
        aux = passagens[0];
        passagens[0] = passagens[2];
        passagens[2] = aux;

        aux = passagens[1];
        passagens[1] = passagens[3];
        passagens[3] = aux;
    }else if(ori==3){//Esquerda
        aux = passagens[0];
        passagens[0] = passagens[1];
        passagens[1] = passagens[2];
        passagens[2] = passagens[3];
        passagens[3] = aux;
    }
}

//
void move_cordenada(){
    if(ori==0 && mov == 'F'){x--;}
    else if(ori==1 && mov == 'F'){y++;}
    else if(ori==2 && mov == 'F'){x++;}
    else if(ori==3 && mov == 'F'){x--;}
}

