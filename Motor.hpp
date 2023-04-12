#ifndef Motor_hpp
#define Motor_hpp
#include <Arduino.h>
#include <Wire.h>
#include <MPU6050.h>
#include <DynamixelMotor.h>

class Motor
{
  private:
    float timeStep = 0.01;
    float yaw = 0;
    int contador = 0;
    unsigned long timer = 0;
    const long unsigned int baudrate = 1000000;
    MPU6050* mpu;
    HardwareDynamixelInterface* interface;
    DynamixelMotor* motore1; //define a porta do motor esquerdo
    DynamixelMotor* motore2; //define a porta do motor esquerdo 2
    DynamixelMotor* motord1; //define a porta do motor direito
    DynamixelMotor* motord2; //define a porta do motor direito 2

    /*! A função que controla a movimentacao dos motores com a mesma potencia
      \param potencia Potencia a ser passada nos motores (para as funções de giro e de parar)
    */
    void mesmaPotencia(int potencia) {
      motore1->speed(potencia);
      motore2->speed(potencia);
      motord1->speed(potencia);
      motord2->speed(potencia);
    }



  public:
    Motor(void) {
      mpu = new MPU6050;
      interface = new HardwareDynamixelInterface(Serial1);
      motore1 = new DynamixelMotor(*interface, 11);
      motore2 = new DynamixelMotor(*interface, 5);
      motord1 = new DynamixelMotor(*interface, 1);
      motord2 = new DynamixelMotor(*interface, 6);
    }

    bool
      segundo_andar = false,
      descida = false,
      subida = false;
    int quadDescida = 0, inclinacao = 0, potenciaMaxima = 1000, inclinacao_descida = 0;
    int pitch = 0, diferencaFrontal = 0;

    /*! A função que faz o robo se movimentar
      \param potencia Parametro que define a potencia com que o robo se desloca no movimento
      \param diferencaLateral Parametro que define a diferenca de potencia entre os motores de cada lado (PID lateral)
      \param diferencaFrontal Parametro que define a diferenca de potencia entre os motores da frente e os de tras (rampa/escada)
    */
    void movimento(int potencia = 1000, int diferencaLateral = 0, int cor = 0) {
      Vector normAccel = mpu->readNormalizeAccel();
      pitch = 0 - (atan2(normAccel.XAxis, sqrt(normAccel.YAxis * normAccel.YAxis + normAccel.ZAxis * normAccel.ZAxis)) * 180.0) / M_PI;

      /*Debug*/
      //      Serial.print("pitch "); Serial.println(pitch);
      //      Serial.print("inclinacao "); Serial.print(inclinacao);
      //      Serial.print(" estado "); Serial.println(estado);

      /*Inclinação de subida*/
      if (pitch > 15) {
        inclinacao++;
        /*Inclinação de plana*/
      } else if (pitch >= 2 and pitch <= 15) {
        inclinacao = 0;
        inclinacao_descida = 0;
        /*Inclinação de Descida*/
      } else if (pitch < 2) {
        inclinacao_descida++;
      }

      /*Com base nas inclinaçoes, definimos quais casos que o robô se encontra*/
      /*Caso descida rampa*/
      if (inclinacao_descida >= 2) {
        descida = true;
        quadDescida = 0;
        //        Serial.println("DESCIDA RAMPA");
      }
      /*Caso Subida rampa*/
      else if (inclinacao >= 3) {
        subida = true;
        //        Serial.println("Rampa");
        //        Serial.println("SUBIDA RAMPA");
      }
      /*Caso Escada*/
      //      else if (inclinacao > 0 and inclinacao < 3) {
      //        diferencaFrontal = 200;
      //        //        Serial.println("ESCADA");
      //      }
      /*Caso segundo andar*/
      if (subida == true) {
        if ( inclinacao == 0 ) {
          segundo_andar = true;
          subida = false;
        }
      }


      /*Sabendo a situaçao atual do robo ajustamos as respectivas velocidades*/
      /*Velocidades primeiro andar*/
      if (segundo_andar == false and descida == false and subida == false) {
        potenciaMaxima = 800;
        //        Serial.println("Primeiro Andar");
      }
      /*Velocidades no segundo andar*/
      else if (segundo_andar == true) {
        potenciaMaxima = 300;
        //        Serial.println("Segundo andar");
      }
      /*Velocidades para descida*/
      else if (descida == true) {
        subida = false;
        motore1->speed(-200);
        motore2->speed(-100);
        motord1->speed(200);
        motord2->speed(100);
        //        Serial.println("Descida");
      }
      /*Velocidades volta para o primeiro andar*/
      if (segundo_andar == true and descida == true and inclinacao == 0) {
        potenciaMaxima = 800;
        //        Serial.println("Volta ao primeiro andar");
      }

      quadDescida = 0;
      diferencaFrontal = 0;

      //Ajustando diferença de Motores para velocidade
      int velo[4];

      velo[0] = potencia + diferencaLateral + diferencaFrontal; // motore1
      velo[1] = potencia + diferencaLateral - diferencaFrontal; // motore2
      velo[2] = -(potencia - diferencaLateral + diferencaFrontal); // motord1
      velo[3] = -(potencia - diferencaLateral - diferencaFrontal); // motord2

      for (int i = 0; i < 4; i++) {
        velo[i] = min(velo[i], potenciaMaxima);
        velo[i] = max(velo[i], -potenciaMaxima);
        //        Serial.print(velo[i]);
        //        Serial.print(", ");
      }
      //      Serial.println(" ");

      motore1->speed(velo[0]);
      motore2->speed(velo[1]);
      motord1->speed(velo[2]);
      motord2->speed(velo[3]);

    }



    /*! A função que faz o robo parar
      /
    */
    void parar(void) {
      mesmaPotencia(0);
    }

    /*! A função mede a inclinação do robo
      \return Retorna status um inteiro, 0 é descida; 1 é descida e 2 é sem rampa/escada
    */
    bool rampa(void) {
      Vector normAccel = mpu->readNormalizeAccel();
      pitch = 0 - (atan2(normAccel.XAxis, sqrt(normAccel.YAxis * normAccel.YAxis + normAccel.ZAxis * normAccel.ZAxis)) * 180.0) / M_PI;
      //      Serial.println(pitch);
      if (pitch >= 4 and pitch <= 15) { //normal
        return false;
      }
      return true;
    }

    void descer(int quad) {
      descida = true;
      quadDescida = quadDescida + quad;
      //      Serial.print("QUAD DESCIDA ");Serial.println(quadDescida);
    }

    /*! A função controla o giro do robo
      \param angulo Angulo do giro
    */
    void girar(int angulo, int potencia = 500) {
      timer = millis();
      yaw = 0;
      int tempodeleitura;
      unsigned long start = millis();

      while (abs(angulo - yaw) > 5 || millis() - start < 1.2) { // 3 || 1.2
        if (angulo < 0) {
          mesmaPotencia(potencia);
        }
        else {
          mesmaPotencia(-potencia);
        }
        Vector norm = mpu->readNormalizeGyro();
        //        if (norm.ZAxis > 1.5) {
        yaw = yaw + norm.ZAxis * timeStep; // define o valor de yaw
        //        } else{
        //          yaw = yaw + norm.ZAxis * timeStep * -10; // define o valor de yaw
        //        }
        tempodeleitura = (timeStep * 1000) - (millis() - timer);
        //        Serial.println(norm.ZAxis);
        //        Serial.println(tempodeleitura);
        if (tempodeleitura > 0) {
          delay(tempodeleitura); //tempo até a próxima leitura
        }
        //        Serial.println("WOOOOOOOOOOOOOOOOOOOOOOOW2");
        timer = millis();
      }
      parar();
    }

    /*Alinhar o robo lateralmente com angulo*/
    void alinhamento_secreto(int esquerdo_frente, int esquerdo_tras, int direito_frente, int direito_tras) {
      int dist[4] = {direito_frente, direito_tras, esquerdo_frente, esquerdo_tras};
      float angulo = 0,
            potencia = 200,
            tempodeleitura;
      yaw = 0;
      timer = millis();
      unsigned long start = millis();


      //Direito
      if (direito_frente - direito_tras < 250) {
        angulo = ((dist[0] - dist[1]) / 165.0);
        //Esquerdo
      } else if (esquerdo_tras - esquerdo_frente < 250) {
        angulo = ((dist[2] - dist[3]) / 165.0);
      }

      /*Debug*/
      //      Serial.print("Radianos:"); Serial.println(angulo);
      //      Serial.print("dists: "); Serial.print(dist[0]); Serial.print(" ");
      //      Serial.print(dist[1]); Serial.print(" "); Serial.print(dist[2]); Serial.print(" ");
      //      Serial.println(dist[3]);

      /*Radianos para Graus*/
      angulo = angulo * (180 / 3.1415);

      /*Debug*/
      //      Serial.print("Graus:"); Serial.println(angulo);
      //      Serial.print("Diferença Direita"); Serial.println(dist[0] - dist[1]);
      //      Serial.print("Diferença Esquerda"); Serial.println(dist[2] - dist[3]);
      //      Serial.print("---------------------------");

      /*Ajustes com angulo*/
      while ( abs(angulo - yaw) > 30 or millis() - start < 1.2 ) {
        if (angulo > 0) {
          mesmaPotencia(potencia);
        }
        else {
          mesmaPotencia(-potencia);
        }
        Vector norm = mpu->readNormalizeGyro();
        //        if (norm.ZAxis > 1.5) {
        yaw = yaw + norm.ZAxis * timeStep; // define o valor de yaw
        //        } else{
        //                        yaw = yaw + norm.ZAxis * timeStep * -10; // define o valor de yaw
        //        }
        tempodeleitura = (timeStep * 1000) - (millis() - timer);
        //        Serial.println(norm .ZAxis);
        //        Serial.println(tempodeleitura);
        if (tempodeleitura > 0) {
          delay(tempodeleitura); //tempo até a próxima leitura
        }
        //        Serial.println("WOOOOOOOOOOOOOOOOOOOOOOOW2");
        timer = millis();
        continue;
      }
      parar();
      delay(2000);
    }


    void setup(void) {
      //      Serial.begin(200000); //achei essa redefinição
      while (!mpu->begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)) {
        Serial.println("Não foi possível encontrar uma MPU6050 válida, dê uma olhada na ligação!");
        delay(500); //checa se a IMU esta conectada
      }
      interface->begin(baudrate);
      delay(100);
      uint8_t status = motore1->init();
      /*Debug*/
      //      Serial.println("Passei MPU");
      //      delay(100);
      if (status != DYN_STATUS_OK) //checa se o motor e1 esta funcionando
      {
        Serial.println("motore1 nao esta funcionando");
        delay(1000);
      }

      motore1->enableTorque();//torque ativado motor e1
      motore1->wheelMode();//modo roda ativado motor e1

      status = motore2->init();
      if (status != DYN_STATUS_OK) //checa se o motor e2 esta funcionando
      {
        Serial.println("motore2 nao esta funcionando");
        delay(1000);
      }

      motore2->enableTorque();//torque ativado motor e2
      motore2->wheelMode();//modo roda ativado motor e2

      status = motord1->init();
      if (status != DYN_STATUS_OK)//checa se o motor d1 esta funcionando
      {
        Serial.println("motord1 nao esta funcionando");
        delay(1000);
      }

      motord1->enableTorque();//torque ativado motor d1
      motord1->wheelMode();//modo roda ativado motor d1

      status = motord2->init();
      if (status != DYN_STATUS_OK)//checa se o motor d2 esta funcionando
      {
        Serial.println("motord2 nao esta funcionando");
        delay(1000);
      }

      motord2->enableTorque(); //torque ativado motor d2
      motord2->wheelMode(); //modo roda ativado motor d2

      pinMode(13, OUTPUT);
      parar();

    }

};
#endif
