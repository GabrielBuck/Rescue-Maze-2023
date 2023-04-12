#ifndef Sensorcor_hpp
#define Sensorcor_hpp

#include<EEPROM.h>

class Sensorcor {

  private:

    int R, G, B, VminR, VminG, VminB, VmaxR, VmaxG, VmaxB, VminRa, VminGa, VminBa, VmaxRa, VmaxGa, VmaxBa, VminW, VmaxW, VminWa, VmaxWa, VminRc, VminGc,
        VminBc, VmaxRc, VmaxGc, VmaxBc, VminWc, VmaxWc;

    int CalibrarMinimo() {
      Serial.println("Digite - para calibrar o valor da cor preta");
      while (Serial.available() > 0) { //Enquanto nada for escrito o programa vai se manter esperando
        Serial.read();
      }
      while (Serial.available() == 0) {
        delay(10);
      }
      char leituraMenor = Serial.read();
      if (leituraMenor == '-') { //Se o caractér "-" for lido, a função será executada
        CalibrarMinR();          //Calibra todos os valores na cor preta
        CalibrarMinG();          //
        CalibrarMinB();          //
        CalibrarMinW();          //
      }
    }

    int CalibrarMinR() { //Calibra o o valor do LED Vermelho na cor preta
      int V  [10], soma = 0, i = 0;
      analogWrite(ledverm, 205); //Liga o LED Vermelho
      delay(10);
      for (i = 0; i < 5; i++) {
        V[i] = analogRead(porta); //Lê o fotodiodo e salva o valor no vetor
      }
      for (i = 0; i < 5; i++) {
        soma = soma + V[i]; //Soma todos os valores do vetor
      }
      analogWrite(ledverm, 0); //Desliga o LED Vermelho
      delay(10);
      VminR = soma / 5; //Faz a média de todos os valores do vetor
      delay(10);

      int V2 [10];
      soma = 0, i = 0;
      analogWrite(ledverm, 0); //Desliga o LED Vermelho
      delay(10);
      for (i = 0; i < 5; i++) {
        V2[i] = analogRead(porta); // Lê o fotodiodo e salva o valor no vetor
      }
      for (i = 0; i < 5; i++) {
        soma = soma + V2[i]; //Soma todos os valores do vetor
      }
      delay(10);
      VminRa = soma / 5; //Faz a média de todos os valores do vetor
      delay(10);

    }

    int CalibrarMinG() { //Calibra o o valor do LED Verde na cor preta
      int V  [10], soma = 0, i = 0;
      analogWrite(ledverd, 205); //Liga o LED Verde
      delay(10);
      for (i = 0; i < 5; i++) {
        V[i] = analogRead(porta); //Lê o fotodiodo e salva o valor no vetor
      }
      for (i = 0; i < 5; i++) {
        soma = soma + V[i]; //Soma todos os valores do vetor
      }
      analogWrite(ledverd, 0); //Desliga o LED Verde
      delay(10);
      VminG = soma / 5; //Faz a média de todos os valores do vetor
      delay(10);

      int V2  [10];
      soma = 0, i = 0;
      analogWrite(ledverd, 0); //Desliga o LED Verde
      delay(10);
      for (i = 0; i < 5; i++) {
        V2[i] = analogRead(porta); //Lê o fotodiodo e salva o valor no vetor
      }
      for (i = 0; i < 5; i++) {
        soma = soma + V2[i]; //Soma todos os valores do vetor
      }
      delay(10);
      VminGa = soma / 5; //Faz a média de todos os valores do vetor
      delay(10);

    }

    int CalibrarMinB() { //Calibra o o valor do LED Azul na cor preta
      int V  [10], soma = 0, i = 0;
      analogWrite(ledazul, 205); //Liga o LED Azul
      delay(10);
      for (i = 0; i < 5; i++) {
        V[i] = analogRead(porta); //Lê o fotodiodo e salva o valor no vetor
      }
      for (i = 0; i < 5; i++) {
        soma = soma + V[i]; //Soma todos os valores vetor
      }
      analogWrite(ledazul, 0); //Desliga o LED Azul
      delay(10);
      VminB = soma / 5; //Faz a média de todos os valores do vetor
      delay(10);

      int V2  [10];
      soma = 0, i = 0;
      analogWrite(ledazul, 0); //Desliga o LED Azul
      delay(10);
      for (i = 0; i < 5; i++) {
        V2[i] = analogRead(porta); //Lê o fotodiodo e salva o valor no vetor
      }
      for (i = 0; i < 5; i++) {
        soma = soma + V2[i]; //Soma todos os valores do vetor
      }
      delay(10);
      VminBa = soma / 5; //Faz a média de todos os valores do vetor
      delay(10);

    }

    int CalibrarMinW() { //Calibra o o valor do LEDs Vermelho, Verde e Azul simultâneamente na cor preta
      int V  [10], soma = 0, i = 0;
      analogWrite(ledverm, 255); //Liga todos os LEDs
      analogWrite(ledverd, 255); //
      analogWrite(ledazul, 255); //
      delay(10);
      for (i = 0; i < 5; i++) {
        V[i] = analogRead(porta); //Lê o fotodiodo e salva o valor no vetor
      }
      for (i = 0; i < 5; i++) {
        soma = soma + V[i]; //Soma todos os valores do vetor
      }
      analogWrite(ledverm, 0); //Desliga todos os LEDs
      analogWrite(ledverd, 0); //
      analogWrite(ledazul, 0); //
      delay(10);
      VminW = soma / 5; //Faz a média de todos os valores do vetor
      delay(10);

      int V2  [10];
      soma = 0, i = 0;
      analogWrite(ledverm, 0); //Desliga todos os LEDs
      analogWrite(ledverd, 0); //
      analogWrite(ledazul, 0); //
      delay(10);
      for (i = 0; i < 5; i++) {
        V2[i] = analogRead(porta); //Lê o fotodiodo e salva o valor no vetor
      }
      for (i = 0; i < 5; i++) {
        soma = soma + V2[i]; //Soma todos os valores do vetor
      }
      delay(10);
      VminWa = soma / 5; //Faz a média de todos os valores do vetor
      delay(10);

    }

    int CalibrarMaximo() {
      Serial.println("Digite + para calibrar o valor da cor branca");
      while (Serial.available() > 0) {
        Serial.read();
      }
      while (Serial.available() == 0) { //Enquanto nada for escrito o programa vai se manter esperando
        delay(10);
      }
      char leituraMaior = Serial.read();
      if (leituraMaior == '+') { //Se o caractér "+" for lido, a função
        CalibrarMaxR(); //Calibra todos os valores na cor branca
        CalibrarMaxG(); //
        CalibrarMaxB(); //
        CalibrarMaxW(); //
      }

    }

    int CalibrarMaxR() {  //Calibra o o valor do LED Vermelho na cor branca
      int V  [10], soma = 0, i = 0;
      analogWrite(ledverm, 205); //Liga o LED Vermelho
      delay(10);
      for (i = 0; i < 5; i++) {
        V[i] = analogRead(porta); //Lê o fotodiodo e salva o valor no vetor
      }
      for (i = 0; i < 5; i++) {
        soma = soma + V[i]; //Soma todos os valores do vetor
      }
      analogWrite(ledverm, 0); //Desliga o LED Vermelho
      delay(10);
      VmaxR = soma / 5; //Faz a média de todos os valores do vetor
      delay(10);

      int V2  [10];
      soma = 0, i = 0;
      analogWrite(ledverm, 0); //Desliga o LED Vermelho
      delay(10);
      for (i = 0; i < 5; i++) {
        V2[i] = analogRead(porta); //Lê o fotodiodo e salva o valor no vetor
      }
      for (i = 0; i < 5; i++) {
        soma = soma + V2[i]; //Soma todos os valores do vetor
      }
      delay(10);
      VmaxRa = soma / 5; //Faz a média de todos os valores do vetor
      delay(10);
    }

    int CalibrarMaxG() { //Calibra o o valor do LED Verde na cor branca
      int V  [10], soma = 0, i = 0;
      analogWrite(ledverd, 205); //Liga o LED Verde
      delay(10);
      for (i = 0; i < 5; i++) {
        V[i] = analogRead(porta); //Lê o fotodiodo e salva o valor no vetor
      }
      for (i = 0; i < 5; i++) {
        soma = soma + V[i]; //Soma todos os valores do vetor
      }
      analogWrite(ledverd, 0); //Desliga o LED Verde
      delay(10);
      VmaxG = soma / 5; //Faz a média de todos os valores do vetor
      delay(10);

      int V2  [10];
      soma = 0, i = 0;
      analogWrite(ledverd, 0); //Desliga o LED Verde
      delay(10);
      for (i = 0; i < 5; i++) {
        V2[i] = analogRead(porta); //Lê o fotodiodo e salva o valor no vetor
      }
      for (i = 0; i < 5; i++) {
        soma = soma + V2[i]; //Soma todos os valores do vetor
      }
      delay(10);
      VmaxGa = soma / 5; //Faz a média de todos os valores do vetor
      delay(10);
    }

    int CalibrarMaxB() { //Calibra o o valor do LED Azul na cor branca
      int V  [10], soma = 0, i = 0;
      analogWrite(ledazul, 205); //Liga o LED Azul
      delay(10);
      for (i = 0; i < 5; i++) {
        V[i] = analogRead(porta); //Lê o fotodiodo e salva o valor no vetor
      }
      for (i = 0; i < 5; i++) {
        soma = soma + V[i]; //Soma todos os valores do vetor
      }
      analogWrite(ledazul, 0); //Desliga o LED Azul
      delay(10);
      VmaxB = soma / 5; //Faz a média de todos os valores do vetor
      delay(10);

      int V2  [10];
      soma = 0, i = 0;
      analogWrite(ledazul, 0); //Desliga o LED Azul
      delay(10);
      for (i = 0; i < 5; i++) {
        V2[i] = analogRead(porta); //Lê o fotodiodo e salva o valor no vetor
      }
      for (i = 0; i < 5; i++) {
        soma = soma + V2[i]; //Soma todos os valores do vetor
      }
      delay(10);
      VmaxBa = soma / 5; //Faz a média de todos os valores no vetor
      delay(10);
    }

    int CalibrarMaxW() { //Calibra o o valor do LEDs Vermelho, Verde e Azul simultâneamente na cor branca
      int V  [10], soma = 0, i = 0;
      analogWrite(ledverm, 255); //Liga o LED Azul
      analogWrite(ledverd, 255); //
      analogWrite(ledazul, 255); //
      delay(10);
      for (i = 0; i < 5; i++) {
        V[i] = analogRead(porta); //Lê o fotodiodo e salva os valores no vetor
      }
      for (i = 0; i < 5; i++) {
        soma = soma + V[i]; //Soma todos os valores do vetor
      }
      analogWrite(ledverm, 0); //Desliga todos os LEDs
      analogWrite(ledverd, 0); //
      analogWrite(ledazul, 0); //
      delay(10);
      VmaxW = soma / 5; //Faz a média de todos os valores no vetor
      delay(10);

      int V2  [10];
      soma = 0, i = 0;
      analogWrite(ledverm, 205); // LIGA todos os LEDs
      analogWrite(ledverd, 205); //
      analogWrite(ledazul, 205); //
      delay(10);
      for (i = 0; i < 5; i++) {
        V2[i] = analogRead(porta); //Lê o fotodiodo e salva os valores no vetor
      }
      for (i = 0; i < 5; i++) {
        soma = soma + V2[i]; //Soma todos os valores do vetor
      }
      delay(10);
      VmaxWa = soma / 5; //Faz a média de todos os valores no vetor
      delay(10);

    }

  public:

    int porta, ledverm, ledverd, ledazul, Ra, Ga, Ba, Wa, Wapagado, Rapagado, Gapagado, Bapagado, MediaRGBMin, MediaRGBMax, MediaRGBa, cor = 0;

    bool range(int x, int min, int max) { //Checa se um valor está entre seu minímo e seu máximo
      if (x < min) { //Se o valor estiver a abaixo do mínimo a função retorna false
        //        Serial.println("RANGE ERRADO");
        return false;
      }
      else if (x > max) { //Se o valor estiver a acima do máximo a função retorna false
        //        Serial.println("RANGE ERRADO");
        return false;
      }
      else {
        //        Serial.println("RANGE CERTO");
        return true; //Se estiver entre seu mínimo e seu máximo a função retorna true
      }
    }

    Sensorcor(uint8_t p, uint8_t ledr, uint8_t ledg, uint8_t ledb) { //Construtor recebe a porta da leitura, e as portas dos LEDs R, G e B
      porta = p;
      pinMode(porta, INPUT); //Porta de leitura analógica do fotodiôdo
      ledverm = ledr;
      pinMode(ledverm, OUTPUT); //Porta do LED R
      ledverd = ledg;
      pinMode(ledverd, OUTPUT); //Porta do LED G
      ledazul = ledb;
      pinMode(ledazul, OUTPUT); //Porta do LED B
    }

    int calibrar() {

      analogWrite(ledverm, 0); //Liga todos os LEDS
      analogWrite(ledverd, 0); //
      analogWrite(ledazul, 0); //

      //      analogWrite(ledverm, 0); //Desliga todos os LEDS
      //      analogWrite(ledverd, 0); //
      //      analogWrite(ledazul, 0); //

      Serial.println("Digite qualquer coisa:");

      unsigned long inicioCalibracao = millis();

      while (millis() - inicioCalibracao <= 1300) { //O laço executará até 1,3 segundos de código executado
        Serial.read();
        if (Serial.available() == 0) { //Se nada for escrito
          delay(10);
        }
        else if (Serial.available() > 0) {
          CalibrarMinimo(); //Calibra a cor preta
          CalibrarMaximo(); //Calibra a cor prateada

          VminRc = abs(VminR - VminRa); //Os valores de calibração são definidos como o módulo do valor lido pelo fotodiodo com os LEDs aceso menos o
          VminGc = abs(VminG - VminGa); //valor lido pelo fotodiodo com os LEDs
          VminBc = abs(VminB - VminBa); //

          VmaxRc = abs(VmaxR - VmaxRa); //
          VmaxGc = abs(VmaxG - VmaxGa); //
          VmaxBc = abs(VmaxB - VmaxBa); //

          VminWc = abs(VminW - VminWa); //
          VmaxWc = abs(VmaxW - VmaxWa); //

          MediaRGBMin = (VminRc + VminGc + VminBc) / 3; //Calcula a média da leitura de todos os LEDs ligados na cor preta

          MediaRGBMax = (VmaxRc + VmaxGc + VmaxBc) / 3; //Calcula a média da leitura de todos os LEDs ligados na cor branca

          Serial.println(MediaRGBMin);
          Serial.println(MediaRGBMax);

          EEPROM.put(0, VminWc); //Guarda os valores de calibração na EEPROM
          EEPROM.put(sizeof(VminWc), VmaxWc); //
          EEPROM.put(sizeof(VminWc) + sizeof(VmaxWc), MediaRGBMin); //
          EEPROM.put(sizeof(VminWc) + sizeof(VmaxWc) + sizeof(MediaRGBMin), MediaRGBMax); //

          delay(10);

          break; //Dá um break após o if ser realizado
        }
      }
      delay(10);
      Serial.println("Execute o código normalmente :D");
    }

    int ler(void) { //Faz a leitura de qual cor o sensor está em cima

      analogWrite(ledverm, 0); //Desliga todos os LEDs
      analogWrite(ledverd, 0); //
      analogWrite(ledazul, 0); //

      analogWrite(ledverm, 0); //Certifica que o LED Vermelho está desligado
      delay(10);
      Rapagado = analogRead(porta); //Valor atual da reflexão enquanto apagado
      delay(10);

      analogWrite(ledverm, 205); //Liga o LED Vermelho
      delay(10);
      Ra = abs(analogRead(porta) - Rapagado); //Valor atual da reflexão do LED Vermelho
      delay(10);
      analogWrite(ledverm, 0); //Desliga o LED Vermelho

      analogWrite(ledverd, 0); //Certifica que o LED Verde está desligado
      delay(10);
      Gapagado = analogRead(porta); //Valor atual da reflexão enquanto apagado
      delay(10);

      analogWrite(ledverd, 205); //Liga o LED Verde
      delay(10);
      Ga = abs(analogRead(porta) - Gapagado); //Valor atual da reflexão do LED Vermelho
      delay(10);
      analogWrite(ledverd, 0); //Desliga o LED Verde

      analogWrite(ledazul, 0); //Certifica que o LED Azul está desligado
      delay(10);
      Bapagado = analogRead(porta); //Valor atual da reflexão enquanto apagado
      delay(10);

      analogWrite(ledazul, 205); //Liga o LED Azul
      delay(10);
      Ba = abs(analogRead(porta) - Bapagado); //Valor atual da reflexão do LED Azul
      delay(10);
      analogWrite(ledazul, 0); //Desliga o LED Azul

      Wapagado = analogRead(porta); //Valor atual da reflexão enquanto apagado
      analogWrite(ledverm, 255); // Liga todos os LEDs
      analogWrite(ledverd, 255); //
      analogWrite(ledazul, 255); //
      delay(10);
      Wa = abs(analogRead(porta) - Wapagado); //Valor atual da reflexão de todos os LEDs ligados
      delay(10);
      analogWrite(ledverm, 0); //Desliga todos os LEDs
      analogWrite(ledverd, 0); //
      analogWrite(ledazul, 0); //

      EEPROM.get(0, VminWc); //Recupera os valores de calibração na EEPROM
      EEPROM.get(sizeof(VminWc), VmaxWc); //
      EEPROM.get(sizeof(VminWc) + sizeof(VmaxWc), MediaRGBMin); //
      EEPROM.get(sizeof(VminWc) + sizeof(VmaxWc) + sizeof(MediaRGBMin), MediaRGBMax); //

      MediaRGBa = (Ra + Ga + Ba) / 3; //Faz a média dos valores atuais lidos com todos os LEDs ligados

      //            Serial.print("A média atual é: ");
      //            Serial.print(MediaRGBa);
      //            Serial.print(" MediaRGBMax: ");
      //            Serial.println(MediaRGBMax);
      //            Serial.print(" Wa: ");
      //            Serial.println(Wa);
      //            Serial.print("A média minima é:");
      //            Serial.println(VminWc);
      //            Serial.print("A média máxima é:");
      //            Serial.println(VmaxWc);
      //            delay(10);

      if (range(MediaRGBa, MediaRGBMax - 30, MediaRGBMax + 30) == true && range(Wa, VmaxWc - 170, VmaxWc + 170) == true)  { //Checa se os valores lidos estão entre seus máximos e mínimos
        cor = 2; //PRATA
      }
      else if (range(MediaRGBa, MediaRGBMin - 20, MediaRGBMin + 20) == true && range(Wa, VminWc - 170, VminWc + 170) == true) { //Checa se os valores lidos estão entre seus máximos e mínimos
        cor = 1; //PRETO
      }
      //      else if (range(MediaRGBa, 30, 80) == true && range(Wa, 85, 190) == true) { //Checa se os valores lidos estão entre seus máximos e mínimos
      //        cor = 3; //DESCIDA RAMPA
      //      }
      else {
        cor = 0; //BRANCO
      }
      //      Serial.print("COR ");Serial.println(cor); // DEBUG
    }
};

#endif
