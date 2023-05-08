#ifndef Distancia_hpp
#define Distancia_hpp

#include <NewPing.h> /*!< Inclusão da biblioteca do Ultrassonico */

#define ESPESSURA_PAREDE 10 /*!< Define a espessura da parede (em Mm) */
#define COMPRIMENTO_ROBO 150 /*!< Define a espessura da parede (em Mm) */
#define SONAR_NUM 6         // Numero de sensors.
#define MAX_DISTANCE 200    //Distancia Maxima (em cm).


class Distancia {

private:
  float setpoint = 0; // Objetivo que o PID tenta atingir
  float Kp = 1.0; // Ganho proporcional
  float Ki = 0.2; // Ganho integral
  float Kd = 0.1; // Ganho derivativo



public:

  /*!< Todas Funcoes PUBLICAS */
  void leitura();
  float angulo(int ef, int et, int df, int dt);
  float pid();
  float pid_esquerdo(float entrada);
  float pid_direito(float entrada);

  int dist[6]; //Armazena os valores das leituras
  float angulo = 0; 

  // Funcao que percorre cada sensor e exibe os resultados.
  /*!< 
  1:Frente
  2:Direita F 
  3:Direita T 
  4:Tras
  5:Esquerda F
  6:Esquerda T
   */
  void leitura() {
    for (int i = 0; i < SONAR_NUM; i++) {  
      delay(50); // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
      dist[i] = sonar[i].ping_cm();
    }
  }

  //Funcao que estima o angulo atual com base em dois valores de distancia 
  float angulo(int ef, int et, int df, int dt) {

    //Decidimos qual lado do robo e o mais proximo para medir o angulo
    if(ef + et <= df + dt){
      frente = ef;
      tras = et;
    }else {
      frente = df;
      tras = dt;
    }

    if (frente >= tras) { int cat_op = frente - tras }
    else{ int cat_op = frente - tras }

    angulo = atan(cat_op / COMPRIMENTO_ROBO) * 180 / M_PI;

    //Correcao dependendo da angulacao
    if(ef >= df){
      angulo = angulo*-1
    }

    return angulo;
  }

  //Funcao calcula media dos dois PIDs de distancia
  float pid(){
    float saida = (pid_esquerdo() + pid_direito()) / 2
    return saida
  }

  // A Funcao PID calcula um valor de correcao para mante o robo alinhado com base em tres 
  // A funcao recebe 1 valor de distancia da esquerda
  float pid_esquerdo(float entrada) {

    entrada = entrada % 300
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

  // A funcao recebe 1 valor de distancia da direita
  float pid_direito(float entrada) {
    
    entrada = entrada % 300
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