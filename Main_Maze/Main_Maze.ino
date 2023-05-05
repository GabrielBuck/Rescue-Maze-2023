
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

void setar_quadrado(int frente, int tras);      //Parametros para troca
bool troca_quadrado(int f_atual, int t_atual);  //Verifica a troca de quadrado, pelo encoder e distancias
float pid_completo();                           //Encadeia todos PIDS
void contagemDeTempo();                         //Mede o intervalo de tempo entre as chamadas
bool troca_encoder();                           //Verifica a troca atravas do encoder

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
  if (mapa.decisao() == 'V') {
    //MEXE SERVO MOTOR
    //OLHAMOS SERIAL
    //CASO ENCONTRADA A VITIMA
    //SERVO LIBERA KITS LADO DIREITO
    //SERVO LIBERA KITS LADO ESQUERDO
  }

  /*!< Caso de Giro >!*/
  if (mapa.decisao() == 'E') {
    motores.girar('E');  //Esquerda
  } else if (mapa.decisao() == 'D') {
    motores.girar('D');  //Direita
  }

  /*!< Caso de Movimentacao para frente >!*/
  else if (mapa.decisao() == 'F') {

    setar_quadrado(dist[0], dist[3]);  //Parametros para a troca sao salvos

    //Enquanto nao houver troca se mantem na movimentacao
    while (troca_quadrado() == false) {
      dist.leitura();  // Mede distancias

      if (cor.leitura() == "Preto") {  // Verifica se a cor em que estamos e preta
        motores.sair_preto();
        break;
      }
      else if (cor.leitura() == "Azul") {  // Verifica se a cor em que estamos e Azul
        motores.esperar_azul();
        break;
      }

      motores.movimento(500, pid_completo());  // Executa a movimentacao
      //Algum jeito de verificar a troca de quadrados
    }
    // Apos terminar o movimento e realizada uma correcao com base no angulo
    motores.correcao(dist.angulo());
  }

  //ENVIA INFORMACOES PARA O MAPA
  contagemDeTempo();
}
/*************************************************** FINAL DO LOOP ******************************************************************************/



/*************************************************** DECLARACAO DAS FUNCOES ******************************************************************************/
//Verifica a troca de quadrado
bool troca_quadrado(int f_atual, int t_atual) {
  if (f_atual <= quadrado_ant[0] - 300 || t_atual >= quadrado_ant[1] + 300) {  // Troca pelas distancias
    return true;
  } else if (troca_encoder() == true) {  // Troca pelo Encoder
    return true;
  } else {  // Nao houve troca
    return false;
  }
}

//Faz a juncao de todos PIDs calculados separadamente
float pid_completo() {
  float saida = (dist.pid() + giro.pid_angulo()) / 2;
  return saida;
}

//Usada para salvar novos valores para a troca,
//tanto de distancia tanto como os do encoder
void setar_quadrado(int frente, int tras) {
  quadrado_ant[2] = { frente, tras };
  zerar_encoder();
}

//Faz a leitura e retorna os passos
int encoder() {
  passo = digitalRead();
  if (ultimo_passo != passo) {
    passos++;
    passo = ultimo_passo;
  }
  return passos;
}

//Verifica a troca atravas do encoder
bool troca_encoder() {
  if (encoder() >= 15) {  //Checa se foram passos suficientes
    passos = 0;
    return true;
  } else if {
    return false;
  }
}

//Zeramos a contagem de passos do encoder 
bool zerar_encoder() {
  ultimo_passo = passo;
  passos = 0;
}

// Mede o intervalo de tempo entre as chamadas
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
