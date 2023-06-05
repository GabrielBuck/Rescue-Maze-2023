
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

  if (Serial2.available()) {

    Serial.println("AAAAAAAAAAA");

    for (int i = 0; i < 4; i++) {
      potencias[i] = Serial2.parseInt();  // Read the next integer from serial
      Serial2.read();                     // Consume the space delimiter
    }


    //Print the received numbers to the Serial Monitor
    for (int i = 0; i < 4; i++) {
      Serial.print(potencias[i]);
      Serial.print(" ");
    }
    Serial.println();
  }
}
