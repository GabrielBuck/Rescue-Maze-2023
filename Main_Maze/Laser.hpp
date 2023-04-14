#ifndef Laser_hpp
#define Laser_hpp

#include "Adafruit_VL53L0X.h" /*!< Inclusão da biblioteca do sensor de distância à Laser VL53L0X */

/*!<Essa classe "Laser" é responsável por controlar a leitura de um sensor de distância à laser (VL53L0X).
  Ela possui funções para configurar e inicializar o sensor, além de fornecer uma função para realizar a leitura da distância atual.
  Com isso, é possível obter informações precisas sobre a distância entre o sensor e um objeto em seu alcance*/


class Laser {
  private:

    Adafruit_VL53L0X *lox; /*!< Ponteiro para a criação do objeto do Laser */

  public:

    /*! Função que realiza a leitura de distâncias dos sensores Laser
      return measure.RangeMilliMeter É a distância lida pelo sensor Laser*/
    int read(void) {
      VL53L0X_RangingMeasurementData_t measure; /*!< Variavel interna da classe "Adafruit_VL53L0X" */

      lox->rangingTest(&measure, false); /*!< Lê a distância dos sensores */

      /*!< Checa se a leitura foi errônea, se for certa a função retorna a distância, e se não for ela retorna o inteiro 0 */
      if (measure.RangeStatus != 4 and measure.RangeMilliMeter < 1250) {
        return measure.RangeMilliMeter;
      } else {
        return -1;
      }
    }

    /*! Função que realiza a Inicializacao dos sensores Laser
      begin() Inicializa, verifica se o VL6180X foi encontrado e redefine o chip.*/
    void setup(int shutLaser) {
      lox = new Adafruit_VL53L0X(); /*!< Criação de objeto para a leitura de distância do Laser */

      pinMode(shutLaser, OUTPUT); /*!< Define a porta de reinicialização do Laser como OUTPUT */

      /*!< Reinicia o Laser por meio da porta XSHUT */
      digitalWrite(shutLaser, LOW);
      delay(10);
      digitalWrite(shutLaser, HIGH);
      delay(10);

      lox->begin(0x00 + shutLaser); /*!< Inicializa o laser no id pré definido*/
      lox->startRangeContinuous(0x00 + shutLaser); //
    }

};
#endif
