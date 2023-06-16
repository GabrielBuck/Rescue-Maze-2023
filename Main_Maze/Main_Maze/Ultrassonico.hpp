#ifndef Ultrassonico_hpp
#define Ultrassonico_hpp


#define MAX_DISTANCE 200 /*!<Distancia Maxima (em cm).*/
#define TIMEOUT 50000    /*!<Tempo maximo de espera para o retorno do pulso do ultrassonico.*/



class Ultrassonico {

private:

  byte _trigger, _echo;

  /*!<Retorna intervalo de tempo entre as chamadas*/
  

public:

  Utrassonico(byte trigger, byte echo){
    _trigger = trigger;
    _echo = echo;
    pinMode(_trigger, OUTPUT);
    digitalWrite(_trigger, LOW);
    pinMode(_echo, INPUT);
  }

  float read(){
    unsigned long tempo;

    digitalWrite(_trigger, HIGH);
    delayMicrosseconds(10);
    digitalWite(_trigger, LOW);

    tempo = micros();
    while(digitalRead(_echo) == 0 && (micros() - tempo) < 10000);
    if((micros() - tempo) > 10000) return -1.0;

    tempo = micros();
    while(digitalRead(_echo) == true && (micros() - tempo) < 10000);
    

  }


  
};
#endif