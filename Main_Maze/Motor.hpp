#ifndef Motor_hpp
#define Motor_hpp

#include "DynamixelMotor.h"    /*!< Inclusão da biblioteca dos Motores // link: "https://github.com/descampsa/ardyno/tree/master/examples"*/

Giroscopio giro;

/*!< Esssa classe faz o controle de 4 motores Dynamixel.
    Ela fornece métodos para configurar os motores e a MPU, definir suas e velocidades.
    A classe utiliza a biblioteca "ardyno" para se comunicar com os motores Dynamixel.*/

class Motor {

private:

  /*!< Todas Funcoes PRIVADAS */
  void potencia(int vel[4], int diferenca_lateral = 0);
  void mesma_potencia(int vel, int dif_lado = 0, bool giro = false);

  /*! Funcao que faz o envio das velocidade individualmente para cada motor*/
  void potencia(int vel[4], int diferenca_lateral) {
    //Filtro para valores erroneos
    for (int i = 0; i < 4; i++) {
      if (vel[i] > 1000) {
        vel[i] = 1000;
      } else if (vel[i] < -1000) {
        vel[i] = -1000;
      }
    }
    //Cada valor recebe sua respectiva velocidade
    motore1.speed(vel[0]);
    motore2.speed(vel[1]);
    motord1.speed(vel[2]);
    motord2.speed(vel[3]);
  }

  /*! Funcao que coloca todos motores na mesma velocidade, e realiza seus espelhamentos*/
  void mesma_potencia(int vel, int dif_lado = 0, bool giro = false) {
    //Caso normal da movimentacao
    if (giro == false) {
        potencia( -(vel - dif_lado), -(vel - dif_lado), (vel + dif_lado), (vel + dif_lado) ) //E enviado para os motores os valores de correcao
        //Caso de giro
    } else if (giro == true) {
      potencia(vel, vel, vel, vel)
    }

  public:

    /*!< Todas Funcoes PUBLICAS */
    void begin();
    void parar();
    void girar(char comando);
    void correcao();
    void sair_preto();
    void movimento(int velocidade = 500, int diferenca_lateral = 0, int quadrados = 1);

    /*! Funcao que inicializa todas variaveis e itens nescesarrios para o funcionamento dos motores */
    void begin() {

      // Velocidade da comunicacao com os motores
      const long unsigned int baudrate = 1000000;

      HardwareDynamixelInterface interface(Serial);
      DynamixelMotor motore1(interface, 11);  //define a porta do motor esquerdo 1
      DynamixelMotor motore2(interface, 5);   //define a porta do motor esquerdo 2
      DynamixelMotor motord1(interface, 1);   //define a porta do motor direito 1
      DynamixelMotor motord2(interface, 6);   //define a porta do motor direito 2
      interface.begin(baudrate);

      // Verifica se foi estabelecida a conexao
      uint8_t status[4] = { motore1.init(),
                            motore2.init(),
                            motord1.init(),
                            motord2.init() }

      for (int i = 0; i < 4; i++) {
        if (status[i] != DYN_STATUS_OK)
          Serial.print("Falha na conexao do motor: ");
        Serial.println(status[i]);
      }

      // Poe o Dynamixel no modo roda, e liga o torque
      motore1.enableTorque();
      motore2.enableTorque();
      motord1.enableTorque();
      motord2.enableTorque();
      motore1.wheelMode();
      motore2.wheelMode();
      motord1.wheelMode();
      motord2.wheelMode();
    }

    /*! Funcao que para todos motores*/
    void parar() {
      mesma_potencia(0);
    }


    /*! Funcao que gira o robo de acordo com o comando recebido*/
    void girar(char comando) {

      switch (comando) {
        //Robo gira ate atingir o angulo desejado
        giro.zerar_mpu();  //Zeramos nosso angulo para iniciar o giro, e tambem ao finalizar

        case 'D':
          while (giro.angulo_mpu() < 90) {
            mesma_potencia(200, true);
          }
          giro.zerar_mpu();
          break;

        case 'E':
          while (giro.angulo_mpu() > -90) {
            mesma_potencia(-200, true);
            giro.zerar_mpu();
          }
          break;
      }
    }

    /*! Funcao que "acerta" a posicao do robo*/
    void correcao() {
      while (giro.angulo_mpu() < dist.angulo) {
        mesma_potencia(200, true);
      }
    }

    /*! Funcao que volta de re quando entramos em um quadrado preto*/
    void sair_preto() {
      mesma_potencia(-500);
    }


    /*! Funcao que movinte o robo para frente*/
    void movimento(int velocidade = 500, int diferenca_lateral = 0, int quadrados = 1) {
      mesma_potencia(velocidade, diferenca_lateral);
    }
  };
#endif