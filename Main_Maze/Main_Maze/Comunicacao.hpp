#ifndef Comunicacao_hpp
#define Comunicacao_hpp


/*!<*/

class Comunicacao {

private:


public:

  void envio_motores(int pot_int[4]) {

    //Convertemos as potencias de Inteiros para Strings Para realizar o envio via Serial
    //Potencias em string
    String pot_df = "",
           pot_dt = "",
           pot_ef = "",
           pot_et = "";

    pot_df = pot_int[0];
    pot_dt = pot_int[1];
    pot_ef = pot_int[2];
    pot_et = pot_int[3];

    //Enviamos as quatro potencias em forma de string para o Mega
    //Cada caracter de cada potência é inviado individualmente

    byte i = 0;

    //Direito Frontal
    while (pot_df[i] != '\0') {
      Serial3.write(pot_df[i]);
      //Serial.println(pot_df[i]);
      i++;
    }
    Serial.println(pot_df);
    delay(500);
    i = 0;


    //Direito Traseiro
    while (pot_dt[i] != '\0') {
      Serial3.write(pot_dt[i]);
      //Serial.println(pot_dt[i]);
      i++;
    }
    i = 0;
    Serial.println(pot_dt);
    delay(500);


    //Esquerdo Frontal
    while (pot_ef[i] != '\0') {
      Serial3.write(pot_ef[i]);
      //Serial.println(pot_ef[i]);
      i++;
    }
    i = 0;
    Serial.println(pot_ef);
    delay(500);

    //Esquerdo Traseiro
    while (pot_et[i] != '\0') {
      Serial3.write(pot_et[i]);
      //Serial.println(pot_et[i]);
      i++;
    }
    Serial.println(pot_et);

    Serial.println("--------------");
    delay(500);
  }
};
#endif