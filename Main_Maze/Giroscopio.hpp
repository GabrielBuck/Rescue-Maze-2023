#ifndef Giroscopio_hpp
#define Giroscopio_hpp

#include <MPU6050.h>/*!< Inclusão da biblioteca do MPU */
#include "Sensores.hpp"

Sensores sensores;

/*!< .*/

class Giroscopio {

  private:
    float angulo_z = 0;    
    float setpoint = 0; // Objetivo que o PID tenta atingir
    float Kp = 1.0; // Ganho proporcional
    float Ki = 0.2; // Ganho integral
    float Kd = 0.1; // Ganho derivativo
    
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
    // A Funcao PID calcula um valor de correcao para mante o robo alinhado com base em tres 
    float pid_angulo(float entrada) {
      
      float erro = setpoint - entrada; // Cálculo do erro
      static float ultimo_erro = 0; // Armazenamento do erro da última iteração
      static float integral = 0; // Armazenamento da soma dos erros anteriores

      float proporcional = Kp * erro; // Cálculo do termo proporcional
      integral += erro; // Soma do erro acumulado
      float derivativo = Kd * (erro - ultimo_erro); // Cálculo do termo derivativo
      ultimo_erro = erro; // Armazenamento do erro atual para a próxima iteração
      float saida = proporcional + Ki * integral + derivativo; // Cálculo do sinal de controle
      return saida; // Retorna o sinal de controle
    }

};
#endif