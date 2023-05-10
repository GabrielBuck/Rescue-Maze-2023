#ifndef Operacional_hpp
#define Operacional_hpp

/*A classe Operacional tem como funcao juntar e abstrair, as leituras dos sensores,
e o funcionamento dos Atuadores.
As funcoes dessa classe ainda nao sao as ideias para utilizicacao no codigo main*/

/*!< Incluindo classes */
#include "Motor.hpp"
#include "Sensores.hpp"

Motor motores;
Sensores sensores;

#define CIRCUNFERENCIA_RODA 20
#define COMPRIMENTO_ROBO 150
#define NUM_PASSOS 10

class Operacional {

private:

  int
    quadrado_ant[2],
    passos = 0;

  float
    a = 0;

  bool
    ultimo_passo,  //Encoder
    passo;

  unsigned long
    b = 0;

  /*!< Todas funcoes PRIVADAS */


public:

  /*!< Todas Funcoes PUBLICAS */

  /******************** DISTANCIAS **********************/
  int Ki = 0,
      Kp = 0,
      Kd = 0,
      setpoint = 80;

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

  /* PID calcula um valor de correcao para mante o robo alinhado com base nas distancias*/
  float pid_esquerdo(int entrada, bool zerar = false) {

    entrada = entrada % 300; 
    float erro = setpoint - entrada;  // Cálculo do erro
    static float ultimo_erro = 0;                             // Armazenamento do erro da última iteração
    static float integral = 0;                                // Armazenamento da soma dos erros anteriores

    float proporcional = Kp * erro;                           // Cálculo do termo proporcional
    integral += erro;                                         // Soma do erro acumulado
    float derivativo = Kd * (erro - ultimo_erro);             // Cálculo do termo derivativo
    ultimo_erro = erro;                                       // Armazenamento do erro atual para a próxima iteração
    float saida = proporcional + Ki * integral + derivativo;  // Cálculo do sinal de controle
    return saida;                                             // Retorna o sinal de controle
  }

  /*PID direito*/
  float pid_direito(int entrada, bool zerar = false) {

    entrada = entrada % 300;
    float erro = setpoint - entrada;
    static float ultimo_erro = 0;
    static float integral = 0;

    float proporcional = Kp * erro;
    integral += erro;
    float derivativo = Kd * (erro - ultimo_erro);
    ultimo_erro = erro;
    float saida = proporcional + Ki * integral + derivativo;
    return saida;
  }

  /******************* MOVIMENTACAO ********************/

  /*! Gira o Robo */
  void girar(char com) {
    int aux[] = { 500, 500, 500, 500 };  // Inicia com valores de 'E'
    sensores.zerar_mpu();

    if (com == 'E') {
      while (sensores.angulo_mpu() > -90) {
        motores.potencia(aux);
      }

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

  /*! Espera 5seg no azul*/
  void espera_azul() {
    parar();
    delay(5000);
  }

  /*! Movimenta o robo para frente*/
  void movimento(int velocidade = 500, int diferenca_lateral = 0, int quadrados = 1) {
    motores.mesma_potencia(velocidade, diferenca_lateral);
  }
};
#endif