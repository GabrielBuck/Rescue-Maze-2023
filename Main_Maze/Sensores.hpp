#ifndef Sensores_hpp
#define Sensores_hpp

#include <Ultrasonic.h> /*!< Inclusão da biblioteca do Ultrassonico */
#include <MPU6050.h>/*!< Inclusão da biblioteca do MPU */

#define ESPESSURA_PAREDE 10 /*!< Define a espessura da parede (em Mm) */


class Sensores{

  private:

    /*!< Funcao que inicializa a MPU */
    void begin_mpu(){

      MPU6050 gyroscope;
      gyroscope.begin();           //Iniciando giroscopio. 
      gyroscope.config_filter(6);  //Setando a 5Hz filtro passa baixa.
      gyroscope.config_gyro(3);    //Setando completos 2000°/s ecala.
      gyroscope.config_accel(3);   //Setando 16g completos de escala.
      gyroscope.convert_value(true); //Ajuste na conversão de valor
    }

    /*!< Funcao que inicializa os sensores Ultrassonicos */
    void begin_ultrassonico(){
    }


  public:
    
    /*!< Funcao que inicializa todos Sensores */
    void begin_sensores(){
      begin_mpu();
      begin_ultrassonico();
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
      
};
#endif