
//   _____       __                _
//  |  __ \     / _|              | |
//  | |  | | ___| |_ ___ _ __   __| | ___ _ __ ___
//  | |  | |/ _ \  _/ _ \ '_ \ / _` |/ _ \ '__/ __|
//  | |__| |  __/ ||  __/ | | | (_| |  __/ |  \__ \
//  |_____/ \___|_| \___|_| |_|\__,_|\___|_|  |___/
//


/*! Ela é usada para debug e recebe informaçõe da serial e retorna.

  \param msn Isso é uma string que deve ser passado e ela é exibida na tela.
  \return Ela vai fazer a leitura da serial e retornar  como string.
  \sa Serial, String
*/

#define PAREDE 20
#define QUADRADO 300
#define ESPESSURA_PAREDE20
#define REF 100
#define REFL 150

String input(String msn) {
  Serial.println(msn);

  msn = "";
  while (Serial.available() == 0) {

  }
  while (Serial.available() > 0) {
    msn += (char)Serial.read();
    delay(2);
  }
  return msn;
}
#include "Sensorcor.hpp"
#include "Motor.hpp" /*!< Incluindo classe dos Motores */
#include "Distancia.hpp"

/*! Classe para controle da movimentação de motores onde utilizo a Serial nativa do Arduino para envio de dados para o motor. O giroscópio é utilizado para que haja precisão
   no ângulo de um giro. Cabe ao acelerômetro medir a inclinção relativa do robô, sendo usado para identificar se o robô está em uma rampa ou não.

  \param ID_FRONT_ESQ O ID do motor que está na frente no lado esquerdo.
  \param ID_BACK_ESQ O ID do motor que está atras no lado esquerdo.
  \param ID_FRONT_DIR O ID do motor que está na frente no lado direito.
  \param ID_BACK_DIR O ID do motor que está atras no lado direito.
  \sa Acelerometro, Giroscopio, Serial.

*/
Motor motores;

/*! Classe para identificação de cores, utilizando o nível de reflexão normal (com o LED apagado) e das cores vermelha, verde, azul e branco na superfície abaixo do robô.

  \param Porta analógica para leitura do fotodiodo.
  \param Porta digital do LED RGB no pino R.
  \param Porta digital do LED RGB no pino G.
  \param Porta digital do LED RGB no pino B.
  \sa Fotodiodo e LED RGB.

*/
Sensorcor leitor(A12, 10, 11, 12);

Distancia medir;

String comm;

unsigned long tempo = millis(),
              tempoPreto = millis(),
              startRescue = 0,
              tempoCam = millis(),
              tempo_anterior = millis();

float
Pf = 0,
If = 0,
Df = 0,
kAngF = 0,
kpf = 1.5,
kif = 0,
kdf = 0,
errof = 0,
ultimoErrof  = 0,
errol_anterior = 0,
angulo = 0,
peso = 0,
count = 0,

refAngF = 0.0,
kpAng = 0,
P_angl = 0,
erroAng = 0,
ultimoErrol = 0,
errol = 0,
errol_anteriores = 0,
Pl = 0,
Il = 0,
Dl = 0,
kpl = 1,
kil = 0,
dist_corrigida = 0,
kdl = 0;
int PID [2];
int ref = 0,
    TotalPIDf = 0,
    andou = 0,
    numQuad = 0,
    velAceleracao = 1000,
    diferencaFrontal = 0,
    giro = 0,
    rolls = 0,
    tempoVit = 0,
    lado = 0,
    tempo_integral = 0,
    t = 0,
    t2 = 0,
    reverso = 0,
    lado_anterior = 0;

bool
rampa = false,
quantidadeQuad = false,
distUlt = false;

/*Método de leitura antiga*/
//char sensores [1];
//sensores[0] = T;

unsigned int ultrassonic;

float Dl_Passada;
float Dl_Filtrada;
int quadradoOld[8];
int quadradoAtual[8];
int mr[8];
bool conditionDist = false,
     conditionGiro = false,
     conditionAnda = false,
     conditionVit = false,
     conditionMacaco = false,
     alinhou = false;

int checkpoint = 0;

void(* resetFunc) (void) = 0;

/*! Função de comunicação que recebe comandos do Rasp, inicializados com '#' e terminados com '\n'.
  \sa Comunicação Serial com o Rasp e string comm.
*/

void recebeRasp(void) { /*!< Quando algo é recebido na Serial Rasp o código é interrompido */
  String comm = "";

  while (Serial.available() > 0 && Serial.read() != '#') { /*!< Identifica se é um comando, iniciado com o caracter '#' */
    delay(2);
  }
  delay(2);
  while (Serial.available() > 0) { /*!< Escreve o comando na string comm */
    char letra = Serial.read();
    delay(2);
    if (letra != '\n') {
      comm += letra;
    }
  }

  /*Debug Teste*/
  //  comm[0] = sensores[count];
  //  count++;

  if (comm[0] == 'F') { /*!< Comando ir pra "Frente" */
    conditionAnda = true;

    ref = ESPESSURA_PAREDE + 70; /*!< Define a refencia como sendo parar logo antes da parede  */
    if (comm.substring(1).toInt() == 0) { /*!< Se o número mandado pelo rasp for 0, é para o robo ficar logo antes da parede  */
      quantidadeQuad = false; /*!< Não "ativa" a função de parar pelo quantidade de quadrados pedidos */
      numQuad = 0;
    }
    else {
      quantidadeQuad = true; /*!< "Ativa" a função de parar pelo quantidade de quadrados pedidos */
      numQuad = comm.substring(1).toInt(); /*!< Salva o número de quadrados pedidos para andar pelo rasp */
    }
    andou = 0; /*!< Variável que indica quantos quadrados foram percorridos desde o último comando recibido do rasp, zerado cada vez que recebe para ir para a frente */
    reverso = 0;
    tempo = millis();
    velAceleracao = 1000;

  }

  else if (comm[0] == 'E' || comm[0] == 'D' || comm[0] == 'T') { /*!< Comando virar à "Esq"uerda */
    conditionGiro = true;

    if (comm[0] == 'E') {
      giro = 90;
      rolls = 6;
    }

    else if (comm[0] == 'D') {
      giro = -90;
      rolls = 2;
    }

    else {
      giro = 180;
      rolls = 4;
    }

  }

  else if (comm[0] == 'S') { /*!< Comando enviar distância para o Rasp */
    conditionDist = true;

  }

  else if (comm[0] == 'U') {
    conditionDist = true;
    distUlt = true;
  }

  else if (comm[0] == 'V') { /*!< Comando para parar e esperar 6 segundos para o resgate das vítimas */
    conditionVit = true;

  }

  else if (comm[0] == 'K') { /*!< Comando para parar e desligar os motores */
    //KILL
    resetFunc();

  } else if (comm[0] == 'M') { /*!< Comando para parar e desligar os motores */
    conditionMacaco = true;
  }

}

/*Função que verifica troca de quadrado*/
bool mudancaQuad( ) {
  /* Distancias: frente, tras */
  int comp[4] = {0, 1, 4, 5};
  t = 0,  t2 = 0;
  //    Serial.print("mudanca quadrados ");

  for (int i = 0; i < 4; i++) {
    //    Serial.print(abs(quadradoOld[comp[i]] - quadradoAtual[comp[i]]));
    //    Serial.print("  ");

    /*Verifica se as leituras são validas*/
    if (quadradoOld[comp[i]] != -1 and quadradoAtual[comp[i]] != -1) {

      /*Debug*/
      //      Serial.print("Old - Atual:");
      //      Serial.println(abs(quadradoOld[comp[i]] - quadradoAtual[comp[i]]));

      //      if (abs(quadradoOld[comp[i]] - quadradoAtual[comp[i]]) > (dist - 20) and reverso == 0) {
      //        t++;
      //      }

      /*Caso a diferença, de distancias, da ultima troca seja suficiente, t++ (t = teste troca)  60*/
      if (abs(quadradoOld[comp[i]] - quadradoAtual[comp[i]]) > (QUADRADO - 40) and reverso == 0) {
        t++;
      }
    }
    /*t2 (teste2), quantas distancias são invalidas (-1)
      --> São nescessarias 2 para utilizar a mudança de quadrado por tempo*/
    else {
      t2++;
    }
  }


  //     Serial.println(t);
  /*se na verificacao mais de duas comparacoes entre  e quadradoAtual significa que mudou de quadrado*/
  if (t >= 2) {
    //    Serial.println("Mudanca pela Distancia");
    t = 0;
    t2 = 0;
    return true;
  }
  else if (reverso == 0 and (millis() - tempo > 2300 + tempoVit and t2 >= 2 and ref > 0) or (reverso == 3 and millis() - tempo > tempoPreto * 2)) {
    //    Serial.println("Mudanca pelo tempo");
    return true;
  }
  else {
    return false;
  }
  //  Serial.println("-------------------------------");
}

void roll(int num) {
  int valor[num];
  for (int i = 0; i < num; i++) {
    valor[i] = quadradoOld[i];
  }
  for (int i = 0; i < 8 - num; i++) {
    quadradoOld[i] = quadradoOld[i + num];
  }
  int v = 0;
  for (int i = 8 - num; i < 8; i++) {
    quadradoOld[i] = valor[v];
    v++;
  }
}

void zerarPID(void) {
  // valores usados no calculo do PID frontal
  PID[0];
  ref = 0;
  // valores usados no calculo do PID lateral
  PID[1];
  Il = 0;
  errol_anteriores = 0;
  lado = 0;
}

//**************************************
/*Filtro passa Baixa ( Derivada (PID) )*/
void filtro() {
  peso = 0.80;
  Dl_Filtrada = (peso * Dl) + ((1 - peso) * Dl_Passada);
  Dl_Passada = Dl_Filtrada;

}

//***************************************************
int PIDs(int x, int y) {

  /*Constantes*/
  int mrF = 0;
  kpf = 3.5;
  kil = 0.005; // Baixo: ()  Mediano: (0.005*)    Alto: (0.01)
  kdl = 2.8;   // Baixo: (1) Mediano: (2.8*)      Alto:
  kpl = 1.8;   // Baixo: (1) Mediano: (1.8*),(2)  Alto: (3)

  /*PID(0) = PID Frontal
    PID(1) = PID Lateral*/

  /*Define distancias como pontos materiais*/
  //  for (int i = 0; i < 8; i++) {
  //    mr[i] = (medir.dist(i) + 35) % QUADRADO;
  //  }
  //Sensor direito TORTO (10 graus)
  int(dist_corrigida) = int(medir.dist(2) - 20);// * sin(1.39));//( % QUADRADO);
  mr[2] = (dist_corrigida + 55) % QUADRADO;
  mr[7] = (medir.dist(7) + 55) % QUADRADO;


  //PID frontal
  /*Caso estejamos a uma distancia menor que 1 quadrado em relação a parede
    é realizado o calculo do PID Frontal*/
  if ((x < QUADRADO and x > 0) and (y < QUADRADO and y > 0) and (abs(x - y) < 70)) {
    //    Serial.print("X: "); Serial.println(x);
    //    Serial.print("Y: "); Serial.println(y);
    /*Média dos 2 erros frontais*/
    errof = (REF - medir.dist(0));
    errof = (REF - medir.dist(1) + errof) / 2;
    //    Serial.print("Errof: "); Serial.println(errof);
    Pf = errof * kpf;
    PID[0] = (Pf + If + Df) * -1;
    //    Serial.print("PID Frontal: "); Serial.println(PID[0]);
  } else {
    PID[0] = 400;
  }

  //PID Lateral

  //Filtro
  if ( (mr[2] + mr[7] < 330) and (mr[2] + mr[7] > 270)  or  ((medir.dist(7) + dist_corrigida > 650))) {

    //if (((dist_corrigida - medir.dist(7)) < -150) or medir.dist(7) == -1) {
    /*
      Referencias(Considerando angulação (45),(90)):
      Rampa(380
      0(29~30   and 170~175 ou 205~215 ou 250~255)
      1(329~330 and 470~475 ou 500~515 ou 640~643) + PAREDE
      2(629~630 and 770~775) + PAREDE

      /*Utiliza o lado mais próximo para se alinhar*/
    /*Lado Direito
      Verifica se os valores estão nos intevalos possiveis, considerando a Espessura das paredes*/
    if (((dist_corrigida - medir.dist(7) < -150) or medir.dist(7) == -1) and ((dist_corrigida > 28 and dist_corrigida < 216) or (dist_corrigida > (329 + PAREDE) and dist_corrigida < (475 + PAREDE)))) {
      errol = REFL - mr[2] ;
      /*Caso troquemos o lado, erros anteriores são desconsiderados*/
      lado_anterior = lado;
      lado = 1;
      //      Serial.println("DIREITO");
    }

    /*Lado Esquerdo*/
    /*Verifica se os valores estão nos intevalos possiveis*/
    else if ((medir.dist(7) > 28 and medir.dist(7) < 216) or (medir.dist(7) > (329 + PAREDE) and medir.dist(7) < (475 + PAREDE))) {
      /*Caso troquemos o lado, erros anteriores são desconsiderados*/
      lado_anterior = lado;
      errol = REFL - mr[7];
      lado = 2;
      //      Serial.println("ESQUERDO");

      /*Muito distante para se alinhar*/
    } else {
      errol = 0;
      errol_anterior = 0;
      errol_anteriores = 0;
      Dl_Passada = 0;
      Dl_Filtrada = 0;
      //      Serial.println("MUITO DISTANTE");
    }
    //Saida filtro
  } else {
    errol = 0;
    Dl_Passada = 0;
    Dl_Filtrada = 0;
    errol_anterior = 0;
    errol_anteriores = 0;
    //    Serial.println("Valores incoerentes");
  }
  /*Soma fatores PIDL*/

  /*Proporcional*/
  Pl = errol * kpl;

  /*Integral*/
  /*Caso referencia mude, erros antigos são resetados*/
  //  if (lado != lado_anterior) {
  //    errol_anteriores = 0;
  //  }
  /*Acumulo dos erros*/
  errol_anteriores = errol_anteriores + errol;
  Il = errol_anteriores * kil;
  /*Previne Integral de se acumular mais do que deveria (Anti Windup)*/
  if ((abs(Il) >= 10) or (abs(Il) >= 2 * abs(Pl))) {
    Il = 0;
    errol_anteriores = 0;
  }


  /*Derivada
    Verifica se nao é a primeira passagem, e se houve tempo suficiente para o calculo*/
  if (lado != 0 and (lado == lado_anterior)) { // and (lado == lado_anterior)) {
    Dl = ((errol - errol_anterior) / (((float)millis() / 1000.0) - tempo_anterior)) * kdl;
  } else {
    //  Dl = 0;
    //  Dl_Filtrada = 0;
    //  Dl_Passada = 0;
  }
  /*Filtro passa baixa para Derivada*/
  filtro();



  /*Soma PID*/
  /*Referencial Direito*/
  if (lado == 2) {
    PID[1] = (Pl + Il + Dl_Filtrada);
    /*Referencial Esquerdo*/
  } else {
    PID[1] = (Pl + Il + Dl_Filtrada) * -1;
  }
  tempo_anterior = ((float)millis() / 1000.0);
  errol_anterior = errol;

  /*Prints Debug*/
  //  Serial.print("LASER DIREITO 2: "); Serial.println( medir.dist(2));
  //  Serial.print("LASER DIREITO 3: "); Serial.println( medir.dist(3));
  //  Serial.print("LASER DIREITO (CORRIGIDO) 2: "); Serial.println(dist_corrigida);
  //  Serial.print("LASER ESQUERDO 6: "); Serial.println( medir.dist(6));
  //  Serial.print("LASER ESQUERDO 7: "); Serial.println( medir.dist(7));
  //  Serial.print("MR 2: "); Serial.println(mr[2]);
  //  Serial.print("MR 7: "); Serial.println(mr[7]);
  //  Serial.print("erroL: "); Serial.println(errol);
  //  Serial.print("erroL_Anteriores: "); Serial.println(errol_anteriores);
  //  Serial.print("Proporcional: "); Serial.println(Pl);
  //  Serial.print("Derivada: "); Serial.println(Dl);
  //  Serial.print("Derivada (FILTRADA): "); Serial.println(Dl_Filtrada);
  //  Serial.print("Integral: "); Serial.println(Il);
  //  Serial.print("PIDL: "); Serial.println(PID[1]);
  //  Serial.print("MR: ");

  //  for (int i = 0; i < 8; i++) {
  //    Serial.println(mr[i]); Serial.print(" ");
  //  }
  //  Serial.println("-------------------------");

}

//**************************************************
void setup() {

  Serial.begin(1000000);
  //  Serial.println("Comecei");
  //  delay(50);
  motores.setup();

  /*Debug*/
  //  Serial.println("Terminei Motores ");
  //  delay(100);
  medir.setup();

  //  Serial.println("Criei Objetos");
  //  delay(50);
  //  leitor.calibrar();
  medir.ler_sensor_laser();

  for (int i = 0; i < 8; i++) {
    quadradoOld[i] = medir.dist(i);
  }
  tempo_anterior = millis();
}

void loop(void) {

  /* Debug */
  //  Serial.println("Esperando Comando");
  //  delay(50);

  //COMECO LASER
  //  medir.ler_sensor_laser();

  if (Serial.available()) {
    recebeRasp();

    /*Debug*/
    //    Serial.println("Recebi Comando");
    //    delay(50);
  }

  //  leitor.ler();

  /*Debug*/
  //  Serial.println(leitor.cor);
  //    delay(200);
  //  motores.movimento(0,0, leitor.cor);

  /* Resgate (Fica parado) */
  if (conditionVit) {
    startRescue = millis();

    motores.parar();

    while (millis() - startRescue <= 5500) {
      continue;

    }

    tempoVit = 5500; //atualização do tempo considerando o tempo de resgate das vitimas

    conditionVit = false;
  }

  /* Enviar sensores */
  if (conditionDist) {
    medir.ler_sensor_laser();
    if (distUlt == false) {
      medir.rasp(rampa, motores.descida);
    }

    else {
      Serial.print("#U");
      Serial.println(medir.obstaculo());
    }

    distUlt = false;
    conditionDist = false;
  }

  /* Giro */
  if (conditionGiro) {
    motores.girar(giro);
    roll(rolls);
    Serial.println("#OK"); // envia para o rasp que terminou o comando (giro)
    tempo = millis();
    zerarPID();

    conditionGiro = false;

    //alinhamento();
  }

  /* Andando */
  if (conditionAnda) { // and millis() - tempoCam > 1200){

    /*Realiza a leitura dos sensores*/
    medir.ler_sensor_laser();
    leitor.ler();

    /*! A função a inclinação do robo
      /return Retorna status um inteiro, que vai 0 é subida ou 1 é descida e 2 é sem rampa
    */
    rampa = motores.rampa();

    //delay(10);
    for (int i = 0; i < 8; i++) {
      quadradoAtual[i] = medir.dist(i);
      //      Serial.print(quadradoAtual[i]);Serial.print(" ");
    }
    //    Serial.println("---------------------");

    if (ref > 0) {

      //      motores.alinhamento_secreto(medir.dist(7), medir.dist(6), medir.dist(2), medir.dist(3));

      /*Cálculo PIDs Frontal e Lateral*/
      PIDs(medir.dist(0), medir.dist(1));
      motores.movimento(
        PID[0], //0,  /*!< Correção frontal utilizado como potência dos motores */
        PID[1], //PID[1], // /*!< Correção lateral utilizado para o campo de diferençaLateral da  função */
        leitor.cor
      );
      //      Serial.println("Movimento");
    }

    /*!< Verfica se andou o suficiente para estar no centro do próximo quadrado (245)*/
    if (reverso == 0 and (mudancaQuad() == true or (max(quadradoAtual[0], quadradoAtual[1]) != -1 and max(quadradoAtual[0], quadradoAtual[1]) < ref + 10))) {
      //      Serial.println("Troquei quadrado");
      // Serial.println(ref);
      //      if (motores.inclinacao == 0 and motores.diferencaFrontal == 0) {
      motores.parar();
      //      mudanca_quad = true;

      for (int i = 0; i < 8; i++) {
        quadradoOld[i] = medir.dist(i);
      }

      /* Se o comando do rasp for para andar determinado número de quadrados:
        Soma um na variavel andou representando que já andou um quadrado
        Verifica se já andou o número de quadrado que foi pedido pelo rasp (salvo na variável numQuad)*/
      if (quantidadeQuad == true) {
        andou = andou + 1;
        if (andou == numQuad) {
          zerarPID();
          /*Correção com base nos LASER Frontal e Traseiro*/
          //        Serial.println("Andou");
          //        motores.alinhamento_secreto(medir.dist(0), medir.dist(1), medir.dist(5), medir.dist(4));
          motores.parar();
          conditionAnda = false;
          //          Serial.println("Terminou o comando");
        }
      }
    }

    // != 1
    //    if (medir.obstaculo() == 0 and rampa == false) {
    //      //Alinhamento final
    //      zerarPID();
    //      //      Serial.println("Andou 2");
    //      //      motores.alinhamento_secreto(medir.dist(0), medir.dist(1), medir.dist(5), medir.dist(4));
    //      motores.parar();
    //      conditionAnda = false;
    //      //         Serial.println("Parar");
    //    }

    /* Faz a leitura do sensor, salva internamente e retorna o resultado
      return Retornando um inteiro onde 1 = Preto, 2 = Prata, e 0 = Nada
    */

    if (leitor.cor == 1 and rampa == false and motores.pitch < 14) {
      //Enviar para rasp detecção de preto
      Serial.println("#C1");
      motores.parar();
      reverso++;
    }

    else if (leitor.cor == 2 and rampa == false) {
      //Enviar para rasp detecção de prata
      checkpoint++;
      //        Serial.println(checkpoint);

      //       ultrassonic = medir.distUlt();
      //        Serial.print("Ultrassonic: ");
      //        Serial.println(ultrassonic);
      //        delay(10);
      //        if (checkpoint % 2 != 0) {
      //          while (ultrassonic > 300) {
      //            ultrassonic = medir.distUlt();
      //            //            Serial.print("Ultrassonic: ");
      //            //            Serial.println(ultrassonic);
      //            //            Serial.println("ESPERANDO NO CHECKPOINT");
      //            delay(10);
      //          }
      //          while (ultrassonic < 300) {
      //            ultrassonic = medir.distUlt();
      //            //            Serial.print("Ultrassonic: ");
      //            //            Serial.println(ultrassonic);
      //            //            Serial.println("ESPERANDO O OUTRO ROBO PASSAR");
      //            delay(10);
      //          }
      //        }
      Serial.println("#C2");
      reverso = 0;
    }
    else {
      Serial.println("#C0");
      reverso = 0;
    }

    for (int i = 0; i < 8; i++) {
      quadradoAtual[i] = medir.dist(i);
    }

    // Voltar para o centro do quadrado anterior depois de identificar o ladrilho preto
    if (reverso >= 7) {
      //      Serial.println("HAHHAHHHAHAHHHAHHAHHAHHAHAH");
      zerarPID();
      tempoPreto = millis();
      while (millis() - tempoPreto <= 2000) {
        motores.movimento(-300, 0, 0); // anda para tras, sem PID lateral, na potencia 500
        /* ! A função registra os valores de distancia internamente de todos os lados*/
        //        Serial.println(millis() - tempoPreto);
        continue;
      }
      conditionAnda = false;
      reverso = 0;
      motores.parar();
    }

    //      medir.rasp(rampa); /*!< Distâncias são enviadas para o Rasp */
    tempo = millis();
    tempoVit = 0;
    tempoCam = millis();

    if (motores.descida == true) {
      motores.descer(1);
    }

  }

  /*Modo Macaco:
    Movimentação caótica que busca romper os loops*/
  else if (conditionMacaco == true) {

    motores.girar(45);
    motores.movimento(
      200,
      0,
      0
    );

    /*Caso  estejamos proximos o suficiente da parede
        É realizado uma (Reflexão) do robô*/
    if (medir.dist(0) < 60 or medir.dist(1) < 60) {

      //Direito
      if (medir.dist(2) + medir.dist(3) < medir.dist(7) + medir.dist(6)) {
        angulo = ((medir.dist(2) - medir.dist(3)) / 165.0);

        //Esquerdo
      } else {
        angulo = ( medir.dist(7) + medir.dist(6) / 165.0);
      }
      /*Radianos para Graus*/
      angulo = angulo * (180 / 3.1415); 

      /*Reflexão*/
      motores.girar(2 * angulo);
    }

  }

}
