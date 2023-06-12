#ifndef Operacional_hpp
#define Operacional_hpp

/*A classe Operacional tem como funcao juntar e abstrair, as leituras dos sensores,
e o funcionamento dos Atuadores.
As funcoes dessa classe ainda nao sao as ideias para utilizicacao no codigo main*/

/*!< Incluindo classes */
//#include "Motor_Novo.hpp"
#include "Motor.hpp"
#include "Sensores_Novo.hpp"
#include "PID.hpp"

Motor motores;
Sensores sensores;

#define CIRCUNFERENCIA_RODA 20
#define COMPRIMENTO_ROBO 12
#define NUM_PASSOS 10


/*! Constroi os PIDs*/
//KP, KI, KD, Setpoint
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
    //sensores.begin_enconder();
    sensores.begin_mpu();
  }
  /**************** CORES *****************/
  char cor() {
    //Le o sensor
    char cor = 'w';
    return cor;
  }

  /**************** PIDS ******************/

  /*! PID para diagonal */
  int PID_diagonal() {
    int aux = pidG.calcular(sensores.angulo_mpu());
    return aux;
  }

  /*! Setar PID para andar em diagonal*/
  void setar_PID_diagonal(int setpoint = 0) {
    pidG.setSetpoint(setpoint);
  }

  /******************** DISTANCIAS **********************/
  int dist[6];
  bool passagens[4];


  /*! Lê as distancias dos 6 ultrassonicos*/
  void ler_distancias() {
    sensores.ler_dist();
    for (int i = 0; i < 4; i++) {
      dist[i] = sensores.dist[i];
      Serial.print("Dist ");
      Serial.print(i);
      Serial.print(":");
      Serial.println(dist[i]);
    }
    Serial.println("--------------");
  }

  /*! Verifica a existencia de passagens nas quatro direcoes*/
  void medir_passagens() {
    if (dist[0] >= 25) { passagens[0] = true; }
    if (dist[1] + dist[2] / 2 >= 25) { passagens[1] = true; }
    if (dist[3] >= 25) { passagens[2] = true; }
    if (dist[4] + dist[5] / 2 >= 25) { passagens[3] = true; }
  }

  /*! Estima o angulo atual com base em dois valores de distancia*/
  float angulo() {

    ler_distancias();
    int df = dist[1],
        dt = dist[2],
        et = dist[4],
        ef = dist[5];
    float angulo,
      cat_op,
      frente,
      tras;

    //Decidimos qual lado do robo e o mais proximo para medir o angulo
    if (ef + et <= df + dt) {
      frente = ef;
      tras = et;
    } else {
      frente = df;
      tras = dt;
    }

    if (frente > tras) {
      cat_op = frente - tras;
    } else {
      cat_op = tras - frente;
    }

    angulo = asin(cat_op / COMPRIMENTO_ROBO) * 180 / M_PI;

    //Correcao dependendo da angulacao
    if (df >= ef) {
      angulo = angulo * -1;
    }

    Serial.print("Angulo: ");
    Serial.println(angulo);
    Serial.print("Cateto Oposto: ");
    Serial.println(cat_op);
    return angulo;
  }



  /********************************************************************* MOVIMENTACAO ********************************************************************/

  /**************************************** BÁSICAS ****************************************/

  /*! Girar o Robo */
  void girar(char com) {
    int aux[] = { -500, -500, -500, -500 };  // Inicia com valores de 'E'
    sensores.zerar_mpu();

    //Esquerda
    if (com == 'E') {
      while (sensores.angulo_mpu() <= 720) {
        motores.potencia(aux);
      }
      //Direita
    } else if (com == 'D') {
      aux[0] = 500;
      aux[1] = 500;
      aux[2] = 500;
      aux[3] = 500;
      while (sensores.angulo_mpu() >= -720) {
        motores.potencia(aux);
      }
    }
    parar();
    sensores.zerar_mpu();
  }

  /*! Para todos motores*/
  void parar() {
    motores.mesma_potencia(0);
  }

  /*! Volta de re quando entramos em um quadrado preto*/
  void sair_preto() {
    motores.mesma_potencia(-300);
  }

  /*! Espera 5 seg no azul*/
  void espera_azul() {
    parar();
    delay(5000);
  }

  /*! Movimenta o robo para frente*/
  void movimento(int velocidade = 500, int diferenca_lateral = 0) {
    motores.mesma_potencia(velocidade, diferenca_lateral);
  }

  /****************************************** TROCA DE QUADRADO *********************************************/

  /*! São definidos os parametros de distancia e do Encoder, para a troca*/
  void setar_quadrado(int frente, int tras) {
    quadrado_ant[0] = frente;
    quadrado_ant[1] = tras;
    sensores.zerar_encoder();
  }

  /*Um dos parametros da troca*/
  bool troca_encoder() {
    if (sensores.ler_encoder() >= trajetoria) {  //Checa se foram passos suficientes
      sensores.zerar_encoder();
      return true;
    } else {
      return false;
    }
  }

  /* Verifica a troca de quadrado */
  bool troca_quadrado(int f_atual, int t_atual) {
    if (f_atual <= quadrado_ant[0] - 35 || t_atual >= quadrado_ant[1] + 35) {  // Troca pelas distancias
      return true;
      //} else if (troca_encoder() == true) {  // Troca pelo Encoder
      //return true;
    } else {  // Nao houve troca
      return false;
    }
  }

  /************************************ CORREÇÃO *********************************************/

  /*Correcao do angulo do robo a cada parada*/
  void correcao() {
    float ang = angulo();
    //Parte 1 alinha o robo com a parede
    int aux[] = { 300, 300, 300, 300 };  // Inicia com valores para esquerda
    sensores.zerar_mpu();

    //Ajuste para esquerda
    if (ang <= -10.0) {
      while (sensores.angulo_mpu() < 0) {
        motores.potencia(aux);
      }
    }  //Ajuste para direita
    else if (ang >= 10.0) {
      aux[0] = -300;
      aux[1] = -300;
      aux[2] = -300;
      aux[3] = -300;
      while (sensores.angulo_mpu() < 0) {
        motores.potencia(aux);
      }
    }
    sensores.zerar_mpu();
  }

  /*AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA */
  void correcao_trajetoria() {
    int aux[] = { 300, 300, 300, 300 };  // Inicia com valores para esquerda

    //Escolhemos o lado mais proximo e verificamos se ele é coerente
    if (dist[1] < dist[5] && dist[5] != -1) {  //Próximo da esquerda
      aux[0] = -300;
      aux[1] = -300;
      aux[2] = -300;
      aux[3] = -300;
    }

    if (dist[5] < dist[1] && dist[1] != -1) {  //Próximo da direita
    }
  }

  int trajetoria = 30.0;

  /*Cálculo do comprimento da nova trajetória */
  void calcular_trajetoria() {

    int cateto;

    //Escolhemos o lado mais proximo e verificamos se ele é coerente
    if (dist[1] - dist[5] <= -5 && dist[5] != -1) {  //Próximo da esquerda
      cateto = (15 - (dist[1] % 30));
    }

    else if (dist[5] < dist[1] <= -5 && dist[1] != -1) {  //Próximo da direita
      cateto = (15 - (dist[5] % 30));
    }
    //Caso nao nescessite de correcao
    else {
    }

    //Nova distancia a ser percorrida (Deve ser passada para a troca)
    trajetoria = sqrt(pow(cateto, 2) + pow(30, 2));  // Calculate the hypotenuse
  }
};
#endif