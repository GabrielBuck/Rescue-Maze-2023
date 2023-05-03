
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
void setar_quadrado(int frente, int tras);
bool troca_quadrado(int f_atual, int t_atual); //Verifica a troca de quadrado
float pid_completo();//Encadeia todos PIDS
void contagemDeTempo();//Mede o intervalo de tempo entre as chamadas

/*!< ******************************************** Setup do Código ****************************************************/
void setup() {

  Serial.begin(2000000);

  /*!< Inicializacoes nescessarias >!*/
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
          Continua  */

/*************************************************** INICIO DO LOOP **********************************************************************/
void loop() {
  /*!< RECEBEMOS A DECISAO >!*/
  /*!< O CODIGO RODA DE ACORDO COM A MESMA >!*/

  /*!< Caso Vitima, buscamos por vitimas no quadrado da frente >!*/
  if(mapa.decisao() == 'V'){
    //MEXE SERVO MOTOR
    //OLHAMOS SERIAL
      //CASO ENCONTRADA A VITIMA
        //SERVO LIBERA KITS LADO DIREITO
        //SERVO LIBERA KITS LADO ESQUERDO
  }
  
  /*!< Caso de Giro >!*/
  if (mapa.decisao() == "E") {
    motores.girar("E");//Esquerda
  }
  else if (mapa.decisao() == "D") {
    motores.girar("D");//Direita
  }
  
  /*!< Caso de Movimentacao para frente >!*/
  else if (mapa.decisao() == "F") {

    setar.quadrado(dist[0], dist[3]); //Distancias para troca sao salvas

    //Enquanto nao houver troca se mantem na movimentacao
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
/*************************************************** FINAL DO LOOP ******************************************************************************/




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

//Funcao usada para salvar novos valores para a troca
void setar_quadrado(int frente, int tras){
  quadrado_ant[2] = {frente, tras};
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

