#include <iostream>
#include <utility> // Biblioteca para usar o tipo pair

using namespace std;



//Posicao atual do robo
int x_atual = 0;
int y_atual = 0;
char orientacao;

char movimentacao = 'R';

//Cor
char cor;

//Quatro valores de distancia e um de cor
int quad[5];

//Caso da parede
int np;

//Tamanho de criacao do mapa
const int ROWS = 3;
const int COLS = 3;




int main() {

    pair<int, char> mapa[ROWS][COLS]; // Criação do mapa

    // Inicialização da matriz com valores aleatórios
    for(int i=0; i<ROWS; i++) {
        for(int j=0; j<COLS; j++) {
            mapa[i][j] = make_pair( -1,'U');
        }
    }

    //Ponto de inicio do mapa (MEIO)
    x_atual = 1;
    y_atual = 1;



    //Entramos no loop de atualizacao do mapa
    while(true){


        //Recebe os novos valores
        cout << "Quadrados e cor: ";
        for(int i=0;i<5;i++){
            cin >> quad[i];
        }

        //Aqui fazemos alteracao de acordo com a orientacao do robo
        int aux; // variável auxiliar para trocar valores
        switch(orientacao) {
            case 'D':// Orientado para DIREITA
                cout << "GIREI PRA DIREITA" << endl;
                aux = quad[0];
                quad[0] = quad[3];
                quad[3] = quad[2];
                quad[2] = quad[1];
                quad[1] = aux;
                break;
            case 'E':
                // Orientado para ESQUERDA
                cout << "GIREI PRA ESQUERDA" << endl;
                aux = quad[0];
                quad[0] = quad[1];
                quad[1] = quad[2];
                quad[2] = quad[3];
                quad[3] = aux;
                break;
            case 'T':
                // Orientado para TRAS
                cout << "GIREI PRA TRAS" << endl;
                swap(quad[0], quad[2]);
                swap(quad[1], quad[3]);
                break;
            default:
                // caso o valor de ori seja inválido, não faz nada
                break;
        }

        //Realiza a movimentacao e atuliza a posicao
        switch(movimentacao){
            case 'R':
                break;
            case 'F':
                if (orientacao == 'F'){y_atual--;}//Norte
                if (orientacao == 'E'){x_atual--;}//Oeste
                if (orientacao == 'T'){y_atual++;}//Sul
                if (orientacao == 'D'){x_atual++;}//Leste
                break;
        }

        //Fazemos a atualizacao do mapa de acordo com as novas informacoes
        if(quad[0] == 0 && quad[1] == 0 && quad[2] == 0){ np = 14;}
        //Paredes em U
        else if(quad[0] == 0 && quad[1] == 0 && quad[3] == 0){ np = 13;}
        else if(quad[0] == 0 && quad[2] == 0 && quad[3] == 0){ np = 12;}
        else if(quad[1] == 0 && quad[2] == 0 && quad[3] == 0){ np = 11;}
        //Paredes opostas
        else if(quad[1] == 0 && quad[3] == 0){ np = 10;}
        else if(quad[0] == 0 && quad[2] == 0){ np = 9;}
        //Paredes em L
        else if(quad[0] == 0 && quad[2] == 0){ np = 8;}
        else if(quad[2] == 0 && quad[3] == 0){ np = 7;}
        else if(quad[1] == 0 && quad[2] == 0){ np = 6;}
        else if(quad[1] == 0 && quad[1] == 0){ np = 5;}
        //Paredes individuais
        else if(quad[3] == 0){ np = 4;}
        else if(quad[2] == 0){ np = 3;}
        else if(quad[1] == 0){ np = 2;}
        else if(quad[0] == 0){ np = 1;}
        else{np = 0;}
        //Verificacao de cor
        if(quad[4] == 3){cor = 'B';}
        else if(quad[4] == 2){cor = 'S';}
        else{cor = 'W';}

        //Atualiza o mapa
        mapa[y_atual][x_atual] = make_pair( np,cor);

        // Impressão da matriz
        for(int i=0; i<ROWS; i++) {
            for(int j=0; j<COLS; j++) {
                cout << "[" << mapa[i][j].first << "," << mapa[i][j].second << "] ";
            }
            cout << endl;
        }

        //Decide a movimentacao e a orientacao
        cout << "Movimentacao e orientacao: ";
        cin >> movimentacao >> orientacao;

    }
    return 0;
}
