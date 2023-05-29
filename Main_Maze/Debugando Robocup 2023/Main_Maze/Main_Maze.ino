
//   _____       __                _
//  |  __ \     / _|              | |
//  | |  | | ___| |_ ___ _ __   __| | ___ _ __ ___
//  | |  | |/ _ \  _/ _ \ '_ \ / _` |/ _ \ '__/ __|
//  | |__| |  __/ ||  __/ | | | (_| |  __/ |  \__ \
//  |_____/ \___|_| \___|_| |_|\__,_|\___|_|  |___/
//

/*!< Incluindo classes  */
#include "Operacional.hpp"
#include "Estrategia.hpp"
#include "Mapa.hpp"

Mapa mapa_;
Estrategia estra;

/*!<********** Declaração de todas variaveis ***********/

/*!<********** Protótipo das Funções ***********/
char comando_manual();

/*!< ************** Setup do Código *******************/
void setup() {

  Serial.begin(9600);
  Serial.println("Iniciei");
  /*!< Inicializacoes nescessarias >!*/
}



/****************** Inicio do Loop *************************/
void loop() {

  //Frente
  if (comando_manual() == 'F') {

    
    Serial.println("Frente!!");
    //Cordenadas de inicio
    mapa_.save_cord();
    mapa_.recebe_passagens_cor(op.passagens, op.cor());
    mapa_.orientacao(ori);
    mapa_.move_cordenada(false, false);
    mapa_.imprimir();
  }
  //Start
  else if (comando_manual() == 'S') {

    Serial.println("Comecei o mapa");
    //Cordenadas de inicio
    mapa_.save_cord();
    mapa_.recebe_passagens_cor(op.passagens, op.cor());
    mapa_.orientacao(ori);
    mapa_.imprimir();
  }
}


/*!<********** Declaração das Funções ***********/

/*! Recebe o comando pela porta Serial*/
char comando_manual() {

  char input;
  while (!Serial.available()) {
    // Wait for character input
  }

  input = Serial.read();

  return input;
}
