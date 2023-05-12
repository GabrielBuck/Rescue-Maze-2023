#ifndef Operacional_hpp
#define Operacional_hpp

/*A classe Operacional tem como funcao juntar e abstrair, as leituras dos sensores,
e o funcionamento dos Atuadores.
As funcoes dessa classe ainda nao sao as ideias para utilizicacao no codigo main*/

/*!< Incluindo classes */
#include "Motor.hpp"
#include "Sensores.hpp"
#include "PID.hpp"

Motor motores;
Sensores sensores;

#define CIRCUNFERENCIA_RODA 20
#define COMPRIMENTO_ROBO 150
#define NUM_PASSOS 10


/*! Constroi os PIDs*/
//KP, KI, KD, Setpoint
PID pidE(0.5, 0.2, 0.1, 100);  //Esquerdo
PID pidD(0.5, 0.2, 0.1, 100);  //Direito
PID pidG(0.5, 0.2, 0.1, 100);  //Giroscopio
PID pidF(0.5, 0.2, 0.1, 100);  //Frontal


class Operacional {

private:

  int
    quadrado_ant[2];

  bool
    ultimo_passo,  //Encoder
    passo;

public:

  /*! Inicializacao dos Motores e Sensores*/
  void begin() {
    sensores.begin_enconder();
    sensores.begin_mpu();
    motores.begin();
  }
  /**************** PIDS ******************/

  /*! PID para manter o robo no proprio eixo*/
  int PID_lateral() {
    int aux = ((pidE.calcular(dist[5]) + pidD.calcular(dist[1])) / 2 + pidG.calcular(sensores.angulo_mpu())) / 2;
    return aux;
  }


  /******************** DISTANCIAS **********************/
  int dist[6];

  /*! Lê as distancias dos 6 ultrassonicos*/
  void ler_distancias() {
    sensores.ler_dist();
    for (int i = 0; i < 6; i++) {
      dist[i] = sensores.dist[i];
    }
  }

  /*! Estima o angulo atual com base em dois valores de distancia*/
  float angulo(int ef, int et, int df, int dt) {
    int angulo;
    int cat_op;
    int frente;
    int tras;

    //Decidimos qual lado do robo e o mais proximo para medir o angulo
    if (ef + et <= df + dt) {
      frente = ef;
      tras = et;
    } else {
      frente = df;
      tras = dt;
    }

    if (frente >= tras) {
      cat_op = frente - tras;
    } else {
      cat_op = frente - tras;
    }

    angulo = atan(cat_op / COMPRIMENTO_ROBO) * 180 / M_PI;

    //Correcao dependendo da angulacao
    if (ef >= df) {
      angulo = angulo * -1;
    }

    return angulo;
  }

  /******************* MOVIMENTACAO ********************/
  /*! Girar o Robo */
  void girar(char com) {
    int aux[] = { 500, 500, 500, 500 };  // Inicia com valores de 'E'
    sensores.zerar_mpu();

    //Esquerda
    if (com == 'E') {
      while (sensores.angulo_mpu() > -90) {
        motores.potencia(aux);
      }
      //Direita
    } else if (com == 'D') {
      aux[0] = -500;
      aux[1] = -500;
      aux[2] = -500;
      aux[3] = -500;
      while (sensores.angulo_mpu() < 90) {
        motores.potencia(aux);
      }
    }
    sensores.zerar_mpu();
  }

  /*! Para todos motores*/
  void parar() {
    motores.mesma_potencia(0);
  }

  /*! Volta de re quando entramos em um quadrado preto*/
  void sair_preto() {
    motores.mesma_potencia(-500);
  }

  /*! Espera 5 seg no azul*/
  void espera_azul() {
    parar();
    delay(5000);
  }

  /*! Movimenta o robo para frente*/
  void movimento(int velocidade = 500, int diferenca_lateral = 0, int quadrados = 1) {
    motores.mesma_potencia(velocidade, diferenca_lateral);
  }

  /*! São definidos os parametros de distancia e do Encoder, para a troca*/
  void setar_quadrado(int frente, int tras) {
    quadrado_ant[0] = frente;
    quadrado_ant[1] = tras;
    sensores.zerar_encoder();
  }

  /*Um dos parametros da troca*/
  bool troca_encoder() {
    if (sensores.ler_encoder() >= NUM_PASSOS) {  //Checa se foram passos suficientes
      sensores.zerar_encoder();
      return true;
    } else {
      return false;
    }
  }

  /* Verifica a troca de quadrado */
  bool troca_quadrado(int f_atual, int t_atual) {
    if (f_atual <= quadrado_ant[0] - 300 || t_atual >= quadrado_ant[1] + 300) {  // Troca pelas distancias
      return true;
    } else if (troca_encoder() == true) {  // Troca pelo Encoder
      return true;
    } else {  // Nao houve troca
      return false;
    }
  }
};
#endif