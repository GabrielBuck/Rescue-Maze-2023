#ifndef Laser_hpp
#define Laser_hpp

#include "Adafruit_VL53L0X.h" /*!< Inclusão da biblioteca do sensor de distância à Laser VL53L0X */

class Laser {

  private:

    Adafruit_VL53L0X *lox; /*!< Ponteiro para a criação do objeto do Laser */

  public:

    Laser() {

    }

    /*! Função que realiza a leitura de distâncias dos sensores Laser
      \return measure.RangeMilliMeter É a distância lida pelo sensor Laser
    */
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

    void setup(int shutLaser) {
      lox = new Adafruit_VL53L0X(); /*!< Criação de objeto para a leitura de distância do Laser */

      pinMode(shutLaser, OUTPUT); /*!< Define a porta de reinicialização do Laser como OUTPUT */

      /*!< Reinicia o Laser por meio da porta XSHUT */
      digitalWrite(shutLaser, LOW);
      delay(10);
      digitalWrite(shutLaser, HIGH);
      delay(10);

      lox->begin(0x00 + shutLaser); /*!< Inicializa o laser no id pré definido*/
      lox->startRangeContinuous(0x00 + shutLaser);
    }

};
#endif
