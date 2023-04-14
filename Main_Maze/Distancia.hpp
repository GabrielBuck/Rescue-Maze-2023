#ifndef Distancia_hpp
#define Distancia_hpp

#include "Laser.hpp" /*!< Inclusão da classe de leitura do Laser */
#include <Ultrasonic.h> /*!< Inclusão da biblioteca do Ultrassonico */

#define NUMLASER 4  /*!< Define o número de sensores laser usados */
#define ESPESSURA_PAREDE 10 /*!< Define a espessura da parede (em Mm) */

#define PORTAS {} /*!< Definindo portas para XSHUT */




class Distancia{

  private:
  
    int *LaserShut; /*!< Defines para orientação dos sensores Laser */

    int distanciasLidas[NUMLASER]; /*!< Defines para orientação dos sensores Laser */

    Laser *Leitor; /*!< Ponteiro para o vetor de Lasers */



  public:

    void setup() {
        LaserShut = new int [NUMLASER]PORTAS;
        Leitor = new Laser[NUMLASER];

        for (int cont = 0 ; cont < NUMLASER ; ++cont) {
          Leitor[cont].setup(LaserShut[cont]);
          //        Serial.println(cont); // DEBUG
          //        delay(100);
        }
        //      Serial.println("Setou todos"); // DEBUG
      }

    /* ! A função registra os valores de distancia de todos os lados internamente, em variáveis do tipo int*/
    void registrar (void) {
      for (int cont = 0 ; cont < NUMLASER ; cont++) {
        distanciasLidas[cont] = Leitor[cont].read();
      }
    }

};
#endif