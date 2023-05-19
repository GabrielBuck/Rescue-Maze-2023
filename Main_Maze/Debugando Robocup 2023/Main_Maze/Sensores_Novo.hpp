#ifndef Sensores_hpp
#define Sensores_hpp

#include <Wire.h>
#include <Ultrasonic.h>        /*!< Inclusão da biblioteca do Ultrassonico link: "https://github.com/ErickSimoes/Ultrasonic/blob/master/"*/
#include <MPU6050.h>           /*!< Inclusão da biblioteca do MPU */
#include <Adafruit_MLX90614.h> /*!< Inclusão da biblioteca do MLX */


#define MAX_DISTANCE 200  //Distancia Maxima (em cm).
#define TIMEOUT 20000     //Tempo maximo de espera para o retorno do pulso do ultrassonico.
#define OFFSET 0          /*!< Define o valor de correcao para MPU */
#define DIMENSIONAL 0     /*!< Define uma constante de correcao para MPU*/

MPU6050 gyroscope;

/*Ordem dos Ultrassonicos em sentido Horário! */
Ultrasonic frente(12, 13, TIMEOUT);
Ultrasonic direita_f(12, 13, TIMEOUT);
Ultrasonic direita_t(12, 13, TIMEOUT);
Ultrasonic tras(12, 13, TIMEOUT);
Ultrasonic esquerda_t(12, 13, TIMEOUT);
Ultrasonic esquerda_f(12, 13, TIMEOUT);

class Sensores {

private:

  unsigned long ultima_passagem = 0;  //Usada na medicao do tempo
  float angulo_z = 0;                 // Angulo atual

  /*!<Retorna intervalo de tempo entre as chamadas*/
  unsigned long tempo() {
    unsigned long tempo_atual = millis();
    unsigned long tempo_decorrido = tempo_atual - ultima_passagem;
    ultima_passagem = tempo_atual;
    return tempo_decorrido;
  }

public:

  /******************** ULTRASSONICO ********************/
  int dist[6];  //Armazena os valores das leituras

  /*Percorre cada sensor e exibe os resultados. EM SENTIDO HORARIO!!!*/
  void ler_dist() {
    dist[0] = frente.read();
    dist[1] = direita_f.read();
    dist[2] = direita_t.read();
    dist[3] = tras.read();
    dist[4] = esquerda_t.read();
    dist[5] = esquerda_f.read();
  }
  /******************** MPU ***********************/

  /*!< Inicializa MPU */
  void begin_mpu() {
    gyroscope.begin();              //Iniciando giroscopio.
    gyroscope.config_filter(6);     //Setando a 5Hz filtro passa baixa.
    gyroscope.config_gyro(0);       //Setando completos 200°/s ecala.
    gyroscope.config_accel(3);      //Setando 16g completos de escala.
    gyroscope.convert_value(true);  //Ajuste na conversão de valor
  }

  /*!<Retorna o angulo atual da MPU*/
  float angulo_mpu() {
    angulo_z = angulo_z + ((gyroscope.z_gyro() - OFFSET) * DIMENSIONAL) * tempo();
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
    float offset;
    for (int i = 0; i < 10000; i++) {
      offset = offset + gyroscope.z_gyro();
    }
    offset = offset / 10000;
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