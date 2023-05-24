
//   _____       __                _
//  |  __ \     / _|              | |
//  | |  | | ___| |_ ___ _ __   __| | ___ _ __ ___
//  | |  | |/ _ \  _/ _ \ '_ \ / _` |/ _ \ '__/ __|
//  | |__| |  __/ ||  __/ | | | (_| |  __/ |  \__ \
//  |_____/ \___|_| \___|_| |_|\__,_|\___|_|  |___/
//

/*!< Incluindo classes  */
//#include "Motor.hpp"
#include "Sensores_Novo.hpp"
#include "Operacional.hpp"
#include "Estrategia.hpp"

Estrategia estra;
Sensores sen;
Operacional op1;

/*!<********** Declaração de todas variaveis ***********/


/*!< ************** Setup do Código *******************/
void setup() {

  Serial.begin(9600);
  //Serial.println("Comecei");
  /*!< Inicializacoes nescessarias >!*/
  sen.begin_mpu();
}



/****************** Inicio do Loop *************************/
void loop() {

  sen.angulo_mpu();
  //sen.calibrar_offset();
  //delay(500);
}
