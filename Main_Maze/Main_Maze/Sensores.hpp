#include <MPU6050.h>

#ifndef Sensores_hpp
#define Sensores_hpp

#include <NewPing.h> /*!< Inclusão da biblioteca do Ultrassonico link: "https://bitbucket.org/teckel12/arduino-new-ping/wiki/Home"*/
#include <MPU6050.h> /*!< Inclusão da biblioteca do MPU */

#define SONAR_NUM 6       // Numero de sensors.
#define MAX_DISTANCE 200  //Distancia Maxima (em cm).
#define OFFSET 0          /*!< Define o valor de correcao para MPU */
#define DIMENSIONAL 0     /*!< Define uma constante de correcao para MPU*/

MPU6050 gyroscope;

/*Ordem dos Ultrassonicos em sentido Horário !!! */
NewPing sonar[SONAR_NUM] = {
  NewPing(1, 2, MAX_DISTANCE),  // Pino de disparo de cada sensor, pino de eco e distância máxima para ping.
  NewPing(3, 4, MAX_DISTANCE),
  NewPing(5, 6, MAX_DISTANCE),
  NewPing(7, 8, MAX_DISTANCE),
  NewPing(9, 10, MAX_DISTANCE),
  NewPing(11, 12, MAX_DISTANCE)
};

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

  /*Percorre cada sensor e exibe os resultados.
  1:Frente
  2:Direita F 
  3:Direita T 
  4:Tras
  5:Esquerda F
  6:Esquerda */
  void ler_dist() {
    for (int i = 0; i < SONAR_NUM; i++) {
      delay(50);  // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
      dist[i] = sonar[i].ping_cm();
    }
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