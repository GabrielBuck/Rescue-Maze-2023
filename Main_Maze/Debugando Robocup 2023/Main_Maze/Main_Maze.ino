
//   _____       __                _
//  |  __ \     / _|              | |
//  | |  | | ___| |_ ___ _ __   __| | ___ _ __ ___
//  | |  | |/ _ \  _/ _ \ '_ \ / _` |/ _ \ '__/ __|
//  | |__| |  __/ ||  __/ | | | (_| |  __/ |  \__ \
//  |_____/ \___|_| \___|_| |_|\__,_|\___|_|  |___/
//

/*!< Incluindo classes  */
#include "Motor.hpp"

Motor motores;

/*!<********************* Declaração de todas variaveis ************************/

#define CIRCUNFERENCIA_RODA 20
#define NUM_PASSOS 10

int
  quadrado_ant[2],
  passos = 0;

float
  a = 0;

bool
  ultimo_passo, //Encoder
  passo;

unsigned long
  b = 0;


/*!< ***************** Prototipos das Funcoes do Codigo main *******************/

/*!< ******************************************** Setup do Código ****************************************************/
void setup() {

  Serial.begin(1000000);

  /*!< Inicializacoes nescessarias >!*/
  motores.begin();
}



/*************************************************** INICIO DO LOOP **********************************************************************/
void loop() {

}
/*************************************************** FINAL DO LOOP ******************************************************************************/

