#ifndef Motor_hpp
#define Motor_hpp

#include "Comunicacao.hpp"

Comunicacao com;

class Motor {

private:


public:

  /*! Funcao que coloca todos motores na mesma velocidade, e realiza seus espelhamentos*/
  void mesma_potencia(int vel, int dif_lado = 0) {
    //Caso normal da movimentacao
    int aux[4] = { -(vel - dif_lado), -(vel - dif_lado), (vel + dif_lado), (vel + dif_lado) };
    potencia(aux);  //E enviado para os motores os valores de correcao
  }

  /*! Funcao que inicializa todas variaveis e itens nescesarrios para o funcionamento dos motores */
  void begin() {
  }

  /*! Funcao que faz o envio das velocidade individualmente para cada motor*/
  void potencia(int vel[4]) {
    //Filtro para valores erroneos
    for (int i = 0; i < 4; i++) {
      if (vel[i] > 1000) {
        vel[i] = 1000;
      } else if (vel[i] < -1000) {
        vel[i] = -1000;
      }
    }

    //Serial.println(vel[0]);
    //Serial.println(vel[1]);
    //Serial.println(vel[2]);
    //Serial.println(vel[3]);
    com.envio_motores(vel);

  }
};
#endif