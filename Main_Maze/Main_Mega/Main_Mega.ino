
//   _____       __                _
//  |  __ \     / _|              | |
//  | |  | | ___| |_ ___ _ __   __| | ___ _ __ ___
//  | |  | |/ _ \  _/ _ \ '_ \ / _` |/ _ \ '__/ __|
//  | |__| |  __/ ||  __/ | | | (_| |  __/ |  \__ \
//  |_____/ \___|_| \___|_| |_|\__,_|\___|_|  |___/
//

/*!< Incluindo classes  */
#include "Motor.hpp"

Motor motores;

int potencias[4];  // Array to store the received numbers


void setup() {

  Serial.begin(9600);
  Serial2.begin(9600);
  Serial.println("Iniciei MEGA");
  motores.begin();
}


void loop() {

  recebe_serial();
  motores.potencia(potencias);
}


/*!Recebe valores dos motores atraves da porta Serial*/
void recebe_serial() {

  for (int i = 0; i < 4; i++) {

    String recebe = "";

    //Le cada caracter rcebido e concatena eles em uma String
    while (Serial2.available() > 0) {
      char a = Serial2.read();
      //Serial.println(a);
      //Serial.println("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
      if (a != '\0') recebe.concat(a);
    }
    potencias[i] = recebe.toInt();
    Serial.println(potencias[i]);
    delay(500);
  }
  Serial.println("-----------------");
}
