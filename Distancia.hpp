#ifndef Distancia_hpp
#define Distancia_hpp

#include "Laser.hpp" /*!< Inclusão da classe de leitura do Laser */
#include <Ultrasonic.h>

#define NUMLASER 8 /*!< Define o número de sensores laser usados */

#define ESPESSURA_PAREDE 10 /*!< Define a espessura da parede (em CMs) */

#define PORTAS {2, 3, 4, 5, 6, 7, 8, 9} /*!< Definindo portas para XSHUT */

Ultrasonic ult(38, 36);

class Distancia {

  private:

    int *LaserShut; /*!< Defines para orientação dos sensores Laser */

    int distanciasLidas[NUMLASER]; /*!< Defines para orientação dos sensores Laser */

    const int trigger = 38,
              echo = 36;

    Laser *Leitor; /*!< Ponteiro para o vetor de Lasers */

  public:

    Distancia() {
    }

    void setup() {
      LaserShut = new int [NUMLASER]PORTAS;
      Leitor = new Laser[NUMLASER];
      //      distanciasLidas = new int [NUMLASER];

      for (int cont = 0 ; cont < NUMLASER ; ++cont) {
        Leitor[cont].setup(LaserShut[cont]);
        //        Serial.println(cont); // DEBUG
        //        delay(100);
      }
      //      Serial.println("Setou todos"); // DEBUG
    }

    /* ! A função registra os valores de distancia de todos os lados internamente, em variáveis do tipo int
      /sa Lasers
    */
    void registrar (void) {
      //      for (int cont = 0 ; cont < NUMLASER ; cont++) {
      //        distanciasLidas[cont] = Leitor[cont].read();
      //      }
    }
    void ler_sensor_laser (void) {
      for (int cont = 0 ; cont < NUMLASER ; cont++) {
        distanciasLidas[cont] = Leitor[cont].read();
      }
    }

    /*! A função retorna a distância em milímetros de um certo sensor, que é pedido como parâmetro
      \param lado Define qual sensor será checado
      \return result Um inteiro que representa a distância do robô até o lado pedido, em milimetros
      \sa Lasers
    */
    int dist(int num) {
      /*int i=0;
        if ((num >= 0 and num < 3) or num == 6){
        i = num + 1;
        //        Serial.print(distanciasLidas[num]);
        //        Serial.print(" ");
        distanciasLidas[num] = distanciasLidas[i];
        //        Serial.println(distanciasLidas[num]);
        }*/
      return distanciasLidas[num];
    }

    /*! A função retorna a distância em quadrados de um certo sensor, que é pedido como parâmetro
      \param lado Define qual senso será checado
      \return result Um inteiro que representa a distância do robô até o lado pedido, em quadrados
      \ o -1 representa que a distância lida está maior do que o range do sensor, portanto há pelo menos 3 quadrados
      \sa Lasers
    */
    int distQuad(int num) {
      int distsQuad;

      // 3+ Quadrados
      if (distanciasLidas[num] == -1 or (distanciasLidas[num] + 60) > 850) {
        distsQuad = 3;
      }

      // 0 Quadrados
      else if ((distanciasLidas[num] + 60) < 300) {
        distsQuad = 0;

        // 1 Quadrados
      } else if ((distanciasLidas[num] + 60) < 600) {
        distsQuad = 1;

        // 2 Quadrados
      } else if ((distanciasLidas[num] + 60) < 850) {
        distsQuad = 2;
      }

      return distsQuad;
    }

    /*! A função envia as distâncias em quadrados por meio da Comunicação serial para o Rasp
      \sa Comunicação Serial, Lasers
    */
    void rasp(bool rampa, bool descida = false) {
      Serial.print("#S");
      if (rampa == false and descida == false) {
        for (int i = 0; i < 7; i++) {
          Serial.print(distQuad(i));
          Serial.print(" ");
        }
        Serial.println(distQuad(7));
      }
      else if (rampa == true and descida == false) {
        Serial.print("-1 -1 ");
        Serial.print(distQuad(2));
        Serial.print(" ");
        Serial.print(distQuad(3));
        Serial.print(" ");
        Serial.print("-1 -1 ");
        Serial.print(distQuad(6));
        Serial.print(" ");
        Serial.println(distQuad(7));
      } else {
        //        Serial.println("DISTANCIAS DESCIDAS");
        Serial.print("-1 -1 ");
        Serial.print(distQuad(6));
        Serial.print(" ");
        Serial.print(distQuad(7));
        Serial.print(" ");
        Serial.print("-1 -1 ");
        Serial.print(distQuad(2));
        Serial.print(" ");
        Serial.println(distQuad(3));
      }
      //     for(int i=0;i<7;i++){
      //        Serial.print(distanciasLidas[i]);
      //        Serial.print(" ");
      //     }
      //     Serial.println(distanciasLidas[7]);
    }

    unsigned int distUlt(void) {
      unsigned int t, d;
      digitalWrite(trigger, 1);
      delayMicroseconds(10);
      digitalWrite(trigger, 0);
      t = pulseIn(echo, 1, 20000);
      d = 0.01715 * t * 10;
      return d;
    }

    int obstaculo(void) {
      int obs = 1;
      unsigned int dist = distUlt();

      if (dist <= 150 and distanciasLidas[0] <= 150 and distanciasLidas[0] != -1 and distanciasLidas[1] <= 150 and distanciasLidas[1] != -1) {
        obs = 0; // 0 - parede
      } else if (dist < 410 and (dist - distanciasLidas[0] >= 50 or dist - distanciasLidas[1] >= 50)) {
        obs = 2; // 2 - obstaculo
      }

      return obs;
    }
};
#endif
