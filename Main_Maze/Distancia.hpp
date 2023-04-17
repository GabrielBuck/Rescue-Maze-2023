#ifndef Distancia_hpp
#define Distancia_hpp

#include <NewPing.h> /*!< Inclusão da biblioteca do Ultrassonico */

#define ESPESSURA_PAREDE 10 /*!< Define a espessura da parede (em Mm) */
#define SONAR_NUM 6         // Numero de sensors.
#define MAX_DISTANCE 200    //Distancia Maxima (em cm).


class Distancia {

private:


public:

  //Armazena os valores das leituras
  int dist[6];
  
  // Funcao que percorre cada sensor e exibe os resultados.
  void leitura() {
    for (uint8_t i = 0; i < SONAR_NUM; i++) {  
      delay(50); // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
      dist[i]sonar[i].ping_cm();
    }
  }

};
#endif