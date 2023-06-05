#ifndef Comunicacao_hpp
#define Comunicacao_hpp


/*!<*/

class Comunicacao {

private:


public:

  void envio_motores(int num[4]) {

    for (int i = 0; i < 4; i++) {
      Serial3.print(num[i]);  // Send the number via serial
      Serial.print(num[i]);  // Send the number via serial
      Serial.print(" ");     // Add a space as a delimiter between numbers
      Serial3.print(" ");     // Add a space as a delimiter between numbers
      
    }

    Serial3.println();  // Print a new line to indicate the end of data
    Serial.println();  // Print a new line to indicate the end of data
    delay(500);
  }
};
#endif