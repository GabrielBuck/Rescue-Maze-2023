#ifndef Mapa_hpp
#define Mapa_hpp

/*! */

#include "Mapa.hpp"

//Informacoes a serem recebidas
bool passagens[4];
char cor;
char mov;
int ori;

//Mapa
const int SIZE = 3;


//Cordenadas do robo
int x = 1,
    y = x,
    last_x,
    last_y,
    checkpoint_x = x,
    checkpoint_y = y;

class Mapa {

private:

  unsigned char mapa[SIZE][SIZE] = { { 0 } };

public:

  /*! Salva as cordenadas atuais !*/
  void save_cord() {
    last_x = x;
    last_y = y;
  }
  /* Recebe a cor e as passagens do quadrado novo */
  void recebe_passagens_cor(aux[4], char color) {
    for (int i = 0; i < 4; i++) {
      passagens[i] = aux[i];
    }
  }

  /* Atualiza os bits da posicao atual do mapa */
  void atualizacao(bool passagens[4], char cor) {

    bool bit_cor[3];

    //Adiciona os bits das passagens
    for (int i = 0; i < 4; i++) {
      mapa[x][y] |= (passagens[i] << (7 - i));
    }
    //Verifica e adiciona os bits da cor
    if (cor == 'w') {
      bit_cor[0] = false;
      bit_cor[1] = false;
      bit_cor[2] = true;
    } else if (cor == 'b') {
      bit_cor[0] = false;
      bit_cor[1] = true;
      bit_cor[2] = false;
    } else if (cor == 's') {
      bit_cor[0] = false;
      bit_cor[1] = true;
      bit_cor[2] = false;
    } else if (cor == 'p') {
      bit_cor[0] = true;
      bit_cor[1] = false;
      bit_cor[2] = false;
    }

    for (int i = 0; i < 3; i++) {
      mapa[x][y] |= (bit_cor[i] << (2 - i));
    }
  }

  /* Ajusta os valores recebidos das passagens, de acordo com a orientacao do robo */
  void orientacao(int ori) {
    bool aux;
    if (ori == 0) {
    }                     //Frente
    else if (ori == 1) {  //Direita
      aux = passagens[0];
      passagens[0] = passagens[3];
      passagens[3] = passagens[2];
      passagens[2] = passagens[1];
      passagens[1] = aux;
    } else if (ori == 2) {  //Tras
      aux = passagens[0];
      passagens[0] = passagens[2];
      passagens[2] = aux;

      aux = passagens[1];
      passagens[1] = passagens[3];
      passagens[3] = aux;
    } else if (ori == 3) {  //Esquerda
      aux = passagens[0];
      passagens[0] = passagens[1];
      passagens[1] = passagens[2];
      passagens[2] = passagens[3];
      passagens[3] = aux;
    }
  }

  /*Movimentas as cordenadas atuais do robo
- 'F' move em 1 de seguindo a orientacao
- buraco move para o ultimo quadrado visitado
- sequestro move para o ultimo checkpoint visitado*/
  void move_cordenada(bool buraco, bool sequestro) {
    if (buraco == true) {
      x = last_x;
      y = last_y;
    } else if (sequestro == true) {
      x = last_x;
      y = last_y;
    } else if (ori == 0 && mov == 'F') {
      x--;
    } else if (ori == 1 && mov == 'F') {
      y++;
    } else if (ori == 2 && mov == 'F') {
      x++;
    } else if (ori == 3 && mov == 'F') {
      x--;
    }
  }
};
#endif