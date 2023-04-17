#ifndef Sensores_hpp
#define Sensores_hpp

#include <NewPing.h> /*!< Inclusão da biblioteca do Ultrassonico link: "https://bitbucket.org/teckel12/arduino-new-ping/wiki/Home"*/
#include <MPU6050.h>/*!< Inclusão da biblioteca do MPU */

#define ESPESSURA_PAREDE 10 /*!< Define a espessura da parede (em Mm) */
#define SONAR_NUM 6      // Numero de sensors.
#define MAX_DISTANCE 200 //Distancia Maxima (em cm).



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
      
      //Ordem dos sensores em sentido Horário !!!
      NewPing sonar[SONAR_NUM] = { // Array de objetos do sensor.
        NewPing(, , MAX_DISTANCE), // Pino de disparo de cada sensor, pino de eco e distância máxima para ping.
        NewPing(, , MAX_DISTANCE),
        NewPing(, , MAX_DISTANCE),
        NewPing(, , MAX_DISTANCE),
        NewPing(, , MAX_DISTANCE), 
        NewPing(, , MAX_DISTANCE)
      };
    }

  public:
    
    /*!< Funcao que inicializa todos Sensores */
    void begin_todos(){
      begin_mpu();
      begin_ultrassonico();
    }

};
#endif