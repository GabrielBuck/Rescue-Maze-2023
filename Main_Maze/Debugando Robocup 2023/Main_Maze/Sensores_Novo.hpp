#ifndef Sensores_hpp
#define Sensores_hpp

#include <Ultrasonic.h>        /*!< Inclusão da biblioteca do Ultrassonico link: "https://github.com/ErickSimoes/Ultrasonic/blob/master/"*/
#include <MPU6050.h>           /*!< Inclusão da biblioteca do MPU */
#include <Adafruit_MLX90614.h> /*!< Inclusão da biblioteca do MLX */


#define MAX_DISTANCE 200  //Distancia Maxima (em cm).
#define TIMEOUT 50000     //Tempo maximo de espera para o retorno do pulso do ultrassonico.
#define OFFSET 0.85//0.88       /*!< Define o valor de correcao para MPU */
#define DIMENSIONAL 6.8     /*!< Define uma constante de correcao para MPU*/

MPU6050 gyroscope;

/*Ordem dos Ultrassonicos em sentido Horário! */
//Ultrasonic frente(12, 13, TIMEOUT);
Ultrasonic direita_f(48, 50, TIMEOUT);
Ultrasonic direita_t(38, 40, TIMEOUT);
//Ultrasonic tras(12, 13, TIMEOUT);
Ultrasonic esquerda_t(26, 32, TIMEOUT);
Ultrasonic esquerda_f(28, 30, TIMEOUT);

class Sensores {

private:

  float ultima_passagem = 0;  //Usada na medicao do tempo
  float angulo_z = 0;         // Angulo atual

  /*!<Retorna intervalo de tempo entre as chamadas*/
  float tempo() {
    float tempo_atual = (float)micros() / 1000000.0;
    float tempo_decorrido = tempo_atual - ultima_passagem;
    ultima_passagem = tempo_atual;
    return tempo_decorrido;
  }

public:

  /******************** ULTRASSONICO ********************/
  int dist[4];  //Armazena os valores das leituras

  /*Percorre cada sensor e exibe os resultados. EM SENTIDO HORARIO!!!*/
  void ler_dist() {
    //dist[0] = frente.read();
    dist[0] = direita_f.read();
    dist[1] = direita_t.read();
    //dist[3] = tras.read();
    dist[2] = esquerda_t.read();
    dist[3] = esquerda_f.read();
  }
  /******************** MPU ***********************/

  /*!< Inicializa MPU */
  void begin_mpu() {
    gyroscope.begin();              //Iniciando giroscopio.
    gyroscope.config_filter(6);     //Setando a 5Hz filtro passa baixa.
    gyroscope.config_gyro(1);       //Setando completos 200°/s ecala.
    gyroscope.config_accel(3);      //Setando 16g completos de escala.
    gyroscope.convert_value(true);  //Ajuste na conversão de valor
  }

  /*!<Retorna o angulo atual da MPU*/
  float angulo_mpu() {
    angulo_z += ((gyroscope.z_gyro() - OFFSET) * DIMENSIONAL) * tempo();
    Serial.print("Z ang: ");
    Serial.print(gyroscope.z_gyro());
    /*Serial.print(" X ang: ");
    Serial.print(gyroscope.x_gyro());
    Serial.print(" Y ang: ");
    Serial.print(gyroscope.y_gyro());
    Serial.print(" X Acel: ");
    Serial.print(gyroscope.x_accel());
    Serial.print(" Y Acel: ");
    Serial.print(gyroscope.y_accel());
    Serial.print(" Z Acel: ");
    Serial.print(gyroscope.z_accel());*/
    Serial.print(" Angulo: ");
    Serial.print(angulo_z);
    Serial.print(" Tempo: ");
    Serial.println(tempo());
    return angulo_z;
  }

  /*!< Funcao que zera as referencia da MPU*/
  void zerar_mpu() {
    ultima_passagem = millis();
    angulo_z = 0;
  }


  /*!< Caso tenha troca da MPU e ESSENCIAL recalibrar.
      Guia para a calibracao: 
      Manter o robo em uma mesa ESTAVEL,
      sem NENHUM tipo de interferencia ate o fim da calibracao.
      Ao fim da medicao subistituir a constante OFFSET pelo novo valor*/
  void calibrar_offset() {
    double offset;
    for (int i = 0; i < 10000; i++) {
      offset = offset + gyroscope.z_gyro();
      offset = offset / 2;
      Serial.println(i);
    }
    Serial.print("Offset estimado: ");
    Serial.println(offset);
  }

  /*!< Funcao que le a inclincao em relacao ao solo com MPU*/
  void inclinacao_mpu() {
    static int count = 0;

    float x_value = gyroscope.x_accel();
    float z_value = gyroscope.z_accel();

    float teta = atan(x_value / z_value) * 180 / PI;  //Formula para o angulo de inclinacao

    if (count >= 100) {
      count = 0;
    } else if (teta <= 0) count++;
    else count = 0;
  }

  /****************** ENCODER *******************/

  bool passo;
  int passos;
  bool ultimo_passo;

  /*!< Inicializa o encoder >*/
  void begin_enconder() {
    pinMode(14, INPUT);
  }

  /*!< Faz a leitura e retorna os passos >*/
  int ler_encoder() {
    passo = digitalRead(14);
    if (ultimo_passo != passo) {
      passos++;
      passo = ultimo_passo;
    }
    return passos;
  }

  /*!< Zera todos parametros para o Encoder >*/
  bool zerar_encoder() {
    ultimo_passo = passo;
    passos = 0;
  }
  /***************** MLX TEMPERATURA *****************/
};
#endif