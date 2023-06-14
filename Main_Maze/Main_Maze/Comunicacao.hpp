#ifndef Comunicacao_hpp
#define Comunicacao_hpp


/*!<*/

class Comunicacao {

private:


public:

  void envio_motores(int pot_int[4]) {

    Serial3.print('i');

    for (byte i = 0; i < 4; i++) {
      Serial3.println(pot_int[i]);
      Serial.println(pot_int[i]);
    }

    Serial3.print('p');
    Serial.println("--------------");
    //delay(500);
  }
};
#endif