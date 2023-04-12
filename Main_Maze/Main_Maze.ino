
//   _____       __                _
//  |  __ \     / _|              | |
//  | |  | | ___| |_ ___ _ __   __| | ___ _ __ ___
//  | |  | |/ _ \  _/ _ \ '_ \ / _` |/ _ \ '__/ __|
//  | |__| |  __/ ||  __/ | | | (_| |  __/ |  \__ \
//  |_____/ \___|_| \___|_| |_|\__,_|\___|_|  |___/
//

/*!< Incluindo classes  */
#include "Sensorcor.hpp"
#include "Motor.hpp"  
#include "Distancia.hpp" 
#include "Mapeamento.hpp" 
#include "Vitimas.hpp" 

Vitimas vit;
Mapeamento mapa;
Motor motores;
Distancia medir;

/*!< Declaração de todas variaveis */
//Variaveis Inteiras
int 
a = 0;

//Variaveis Float
float
a = 0;

//Variaveis Booleanas
bool
a = 0;


/*!< Setup do Código */
void setup(){
  
   Serial.begin(2000000);
  
}

/*!< Inicio Loop Principal

  Ideia do loop:
    
    Procura Vítima
        Resgate
            
    Loop de movimentação:
      Mede distâncias 
        Verifica se Houve troca de quadrado
            Envia novas distancias para o mapeamento
            Recebe uma movimentação do mapeamento
                Giro
                    quebra
                Andar para Frente
                    Verifica a Cor
                        Prata
                        Branco
                        Preto
                            Volta para o quadrado anterior            
                        Envia cor para o mapeamento
                    Giroscopio
                        Rampa               
          Continua
            
      
*/
void loop(){

}
