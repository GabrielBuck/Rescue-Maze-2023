
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
Distancia dist;

/*!< Declaração de todas variaveis */
//Variaveis Inteiras
int
  quadrado_ant[2];

//Variaveis Float
float
  a = 0;

//Variaveis Booleanas
bool
  troca_quadrado = false;

//Variaveis longas
unsigned long
  a = 0;


/*!< Declaração de todas Funcoes do Codigo main */

//Funcao usada para salvar novos valores para a troca
void setar_quadrado(int frente, int tras){
  quadrado_ant[2] = {frente, tras};
}

//Funcao que verifica a troca de quadrado
bool troca_quadrado(int f_atual, int t_atual){
  if(f_atual <= quadrado_ant[1] - 300 || t_atual >= quadrado_ant[2] + 300){
    return true;
  }  
}

//Funcao faz a juncao de todos PIDs calculados separadamente
float pid_completo() {
  float saida = (dist.pid() + giro.pid_angulo()) / 2 return saida
}

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
void setup() {

  Serial.begin(2000000);

  //Iniciamos tudo
  motores.begin();
  sensores.begin_todos();
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
void loop() {

  //PROCURA VITIMA
  //RESGATE

  /*!< Apartir daqui se executa as movimentacoes indicadas pela decisao*/
  
  //Salvamos as distancias que serao usadas para encontar as trocas de quadrado
  setar.quadrado(dist[0], dist[3]);

  //Caso de Giro para Esquerda
  if (mapa.decisao() == "E") {
    motores.girar("E");
  }

  //Caso de Giro para Direita
  else if (mapa.decisao() == "D") {
    motores.girar("D");
  }

  //Caso de Andar para frente
  else if (mapa.decisao() == "F") {

    //Enquanto robo nao trocar de quadrado ele se mantem nesta movimentacao
    while (troca_quadrado() == false) {
      dist.leitura();  // Mede distancias

      if (cor.leitura() = "Preto") {  // Verifica se a cor em que estamos e preta
        motores.sair_preto();
        break;
      }

      motores.movimento(500, pid_completo())  // Executa a movimentacao
      //Algum jeito de verificar a troca de quadrados
    }
    // Apos terminar o movimento e realizada uma correcao com base no angulo
    motores.correcao(dist.angulo());
  }

  contagemDeTempo();
}
