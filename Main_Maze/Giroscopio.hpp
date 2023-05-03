#ifndef Giroscopio_hpp
#define Giroscopio_hpp

#include <MPU6050.h>/*!< Inclusão da biblioteca do MPU */
#include "Sensores.hpp"

#define OFFSET 0 /*!< Define o valor de correcao para MPU */
#define DIMENSIONAL 0 /*!< Define uma constante de correcao para MPU*/

Sensores sensores;

/*!< .*/

class Giroscopio {

  private:

    /*!< Todas Funcoes PRIVADAS */
    unsigned long tempo();

    float angulo_z = 0; // Angulo atual
    float setpoint = 0; // Objetivo que o PID tenta atingir
    float Kp = 1.0; // Ganho proporcional
    float Ki = 0.2; // Ganho integral
    float Kd = 0.1; // Ganho derivativo
    static unsigned long ultima_passagem = 0; //Usada na medicao do tempo

    /*!< Funcao que mede intervalo de tempo entre as chamadas*/
    unsigned long tempo() {
      unsigned long tempo_atual = millis();
      unsigned long tempo_decorrido = tempo_atual - ultima_passagem;
      ultima_passagem = tempo_atual;
    return tempo_decorrido;
    }
    
  public:

    /*!< Todas Funcoes PUBLICAS */
    void zerar_mpu();
    void calibrar_offset();
    float angulo_mpu();
    void inclinacao_mpu();
    float pid_angulo(float entrada);

   /*!< Funcao que zera as referencia da MPU*/
    void zerar_mpu(){
      ultima_passagem = millis();
      angulo_z = 0;
    }

    /*!< Caso tenha troca da MPU e ESSENCIAL recalibrar.
      Guia para a calibracao: 
      Manter o robo em uma mesa ESTAVEL,
      sem NENHUM tipo de interferencia ate o fim da calibracao.
      Ao fim da medicao subistituir a constante OFFSET pelo novo valor*/
    void calibrar_offset(){
      float offset;
      for(int i=0; i<10000; i++){
        offset = offset + gyroscope.z_gyro()
      }
      offset = offset / 10000
      Serial.print("Offset estimado: ");
      Serial.println(offset);
    }
    
    
    /*!< Funcao que le a velocidade angular Z da MPU.
      A MPU nos retorna a velocidade angular no eixo chamado.
      portanto para realizar a medicao do angulo multiplicamos pelo intervalo de tempo decorrido
      entre as leituras.
      Tambem sao corrigidos erros de OFFSET e DIMENSIONAL, inerentes da MPU*/
    float angulo_mpu(){
      angulo_z = angulo_z + ((gyroscope.z_gyro() - OFFSET) * DIMENSIONAL) * tempo()
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

    /*A Funcao PID calcula um valor de correcao para mante o robo alinhado
     com base em tres partes */
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