#ifndef Motor_hpp
#define Motor_hpp

#include <Dynamixel2Arduino.h> /*!< Inclusão da biblioteca dos Motores "https://github.com/ROBOTIS-GIT/Dynamixel2Arduino"*/


/*!< Esssa classe faz o controle de 4 motores Dynamixel.
    Ela fornece métodos para configurar os motores e definir suas e velocidades.
    A classe utiliza a biblioteca "Dynamixel 2 Arduino" para se comunicar com os motores Dynamixel. */


#define DXL_SERIAL Serial3
#define DEBUG_SERIAL SerialUSB

const uint8_t ID_EF = 1,
              ID_ET = 6,
              ID_DF = 11,
              ID_DT = 5;

const float DXL_PROTOCOL_VERSION = 2.0;

Dynamixel2Arduino motord1(DXL_SERIAL);
Dynamixel2Arduino motord2(DXL_SERIAL);
Dynamixel2Arduino motore1(DXL_SERIAL);
Dynamixel2Arduino motore2(DXL_SERIAL);

//This namespace is required to use Control table item names
using namespace ControlTableItem;


class Motor_Novo {

private:


public:

  /*! Inicializa todos itens nescesarrios para o funcionamento dos motores */
  void begin() {

    motord1.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
    motord2.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
    motore1.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
    motore2.setPortProtocolVersion(DXL_PROTOCOL_VERSION);

    // Set Port baudrate to 57600bps. This has to match with DYNAMIXEL baudrate.
    motord1.begin(100000);
    motord2.begin(100000);
    motore1.begin(100000);
    motore2.begin(100000);

    //Modo velocidade
    motord1.setOperatingMode(ID_DF, OP_VELOCITY);
    motord2.setOperatingMode(ID_DT, OP_VELOCITY);
    motore1.setOperatingMode(ID_EF, OP_VELOCITY);
    motore2.setOperatingMode(ID_ET, OP_VELOCITY);

    //Torque
    motord1.torqueOn(ID_DF);
    motord2.torqueOn(ID_DT);
    motore1.torqueOn(ID_EF);
    motore2.torqueOn(ID_ET);
  }

  /*! Faz o envio das velocidade individualmente para cada motor*/
  // E-Manual(http://emanual.robotis.com) for available range of value.
  void potencia(int vel[4]) {

    //Filtro para valores erroneos
    for (int i = 0; i < 4; i++) {
      if (vel[i] > 1000) {
        vel[i] = 1000;
      } else if (vel[i] < -1000) {
        vel[i] = -1000;
      }
    }

    //Cada motor recebe sua velocidade
    motord1.setGoalVelocity(ID_DF, vel[0]);
    motord2.setGoalVelocity(ID_DT, vel[1]);
    motore1.setGoalVelocity(ID_EF, vel[2]);
    motore2.setGoalVelocity(ID_ET, vel[3]);
  }

  /*! Coloca todos motores na mesma velocidade, e realiza seus espelhamentos*/
  void mesma_potencia(int vel, int dif_lado = 0) {
    //Espelha os valores
    int aux[4] = { -(vel - dif_lado), -(vel - dif_lado), (vel + dif_lado), (vel + dif_lado) };
    potencia(aux);  //E enviado para os motores os valores de correcao
  }

};
#endif