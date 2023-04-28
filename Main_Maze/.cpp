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
int last_x,
    last_y,
    checkpoint_x = x,
    checkpoint_y = y;



void atualizacao(bool passagens[4], char cor);
void orientacao();
void move_cordenada(bool buraco = false, bool sequestro = false);

int main(){


    /*Loop de atualizacao do mapa*/
    while(true){

        /*Salvamos as ultimas cordenadas em caso de buraco*/
        last_x = x;
        last_y = y;

        /*Movimenta as cordenadas atuais do mapa*/
        cout << "Informe o movimento e orientacao: ";
        cin >> mov >> ori;
        orientacao();
        move_cordenada();
        //for(int i=0;i<4;i++){cout << passagens[i];}

        /*Recebe as iformacoes sobre o quadrado atual*/
        cout << "Informe as passagens e cor: ";
        for(int i=0;i<4;i++){cin >> passagens[i];}
        cin >> cor;

        /*Atualiza os bits da atual posicao do mapa*/
        atualizacao(passagens,cor);

        /*Agimos de acordo com a cor encontrada
        -Preto/buraco, as cordenadas voltam para o ultimo quadrado
        -Prata/checkpoint, salvamos as cordenadas para uma eventual falha de progresso.*/
         if(cor == 'b'){
            move_cordenada(true);
         }
         else if(cor == 's'){
            checkpoint_x = x;
            checkpoint_y = y;
         }

        //Exibe a matriz na tela
        for (int i = 0; i < SIZE; i++){
            for (int j = 0; j < SIZE; j++){
                 cout << "[" << setw(2) << hex << (int)mapa[i][j] << "] ";
                 cout << bitset<8>(mapa[i][j]).to_string();
            }
            cout << endl;
        }
    }

}

/*Funcao que atualiza os bits da posicao atual do mapa*/
void atualizacao(bool passagens[4], char cor){

    bool bit_cor[2];

    //Adiciona os bits das passagens
    for(int i=0; i<4; i++) {
        mapa[x][y] |= (passagens[i] << (7-i));
    }
    //Verifica e adiciona os bits da cor
    if(cor=='w'){bit_cor[0] = false; bit_cor[1] = true;}
    else if(cor=='b'){bit_cor[0] = true; bit_cor[1] = false;}
    else if(cor=='s'){bit_cor[0] = true;bit_cor[1] = true;}

    for(int i=0; i<2; i++) {
        mapa[x][y] |= (bit_cor[i] << (3-i));
    }

}

/*Funcao que ajusta os valores recebidos
das passagens, de acordo com a orientacao do robo*/
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

/*Movimentas as cordenadas atuais do robo
- 'F' move em 1 de acordo com a orientacao
- buraco move para o ultimo quadrado visitado
- sequestro move para o ultimo checkpoint visitado*/
void move_cordenada(bool buraco, bool sequestro){
    if(buraco == true){
        x = last_x;
        y = last_y;
        }
    else if(sequestro == true){
        x = last_x;
        y = last_y;
        }
    else if(ori==0 && mov == 'F'){x--;}
    else if(ori==1 && mov == 'F'){y++;}
    else if(ori==2 && mov == 'F'){x++;}
    else if(ori==3 && mov == 'F'){x--;}
}

