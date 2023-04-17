
//   _____       __                _
//  |  __ \     / _|              | |
//  | |  | | ___| |_ ___ _ __   __| | ___ _ __ ___
//  | |  | |/ _ \  _/ _ \ '_ \ / _` |/ _ \ '__/ __|
//  | |__| |  __/ ||  __/ | | | (_| |  __/ |  \__ \
//  |_____/ \___|_| \___|_| |_|\__,_|\___|_|  |___/
//

/*!< Incluindo classes  */
#include "Cor.hpp"
#include "Sensores.hpp"
#include "Motor.hpp"  
#include "Distancia.hpp" 
#include "Mapeamento.hpp" 
#include "Vitimas.hpp" 

Cor cor;
Sensores sensores;
Vitimas vitima;
Mapeamento mapa;
Motor motores;
Distancia distancia;

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

//Variaveis longas
unsigned long
a = 0;


/*!< Declaração de todas Funcoes do Codigo main */

// A Funcao contagemDeTempo() mede o intervalo de tempo entre as chamadas
void contagemDeTempo() {
  
  static unsigned long tempoAnterior = 0;

  // Obtém o tempo atual
  unsigned long tempoAtual = millis();

  // Calcula o tempo decorrido desde a última passagem do loop
  unsigned long tempoDecorrido = tempoAtual - tempoAnterior;

  // Atualiza o tempo anterior para o tempo atual
  tempoAnterior = tempoAtual;

  // Imprime o tempo decorrido na porta serial
  Serial.print("Tempo decorrido: ");
  Serial.println(tempoDecorrido);
}







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

  contagemDeTempo();

}
