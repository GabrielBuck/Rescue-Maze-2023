#ifndef Giroscopio_hpp
#define Giroscopio_hpp

#include <MPU6050.h>/*!< Inclusão da biblioteca do MPU */
#include "Sensores.hpp"

Sensores sensores;

/*!< .*/

class Giroscopio {

  private:
    float angulo_z = 0;    
    
  public:

   /*!< Funcao que zera a referencia da MPU*/
    void zerar_mpu(){
      angulo_z = 0;
      gyroscope.begin();
    }
    
    /*!< Funcao que le o angulo da com MPU*/
    float angulo_mpu(){
      angulo_z = gyroscope.z_gyro();
      return angulo_z;
    }
    
    /*!< Funcao que le a inclincao em relacao ao solo com MPU*/
    void inclinacao_mpu(){
      static int count = 0;

      float x_value = gyroscope.x_accel();
      float z_value = gyroscope.z_accel();
      
      float teta = atan(x_value/z_value)*180/PI;//Formula para o angulo de inclinacao
      
      if(count >= 100){
        count = 0;
        }
      else if(teta <= 0) count++;
      else count = 0;
    }

};
#endif