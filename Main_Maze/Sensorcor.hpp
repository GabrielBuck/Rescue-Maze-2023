#ifndef Sensorcor_hpp
#define Sensorcor_hpp

#include <EEPROM.h> /*!< Inclusão da biblioteca do Sensor de Cor */

/*!<Essa classe controla um sensor de cor que é capaz de identificar as cores preta, prata e branca.
   Ela fornece métodos para configurar o sensor, ler as cores detectadas e retornar  à cor detectada.
    A classe utiliza a biblioteca Adafruit_TCS34725 para se comunicar com o sensor de cor por meio do protocolo I2C. Essa biblioteca fornece funções para leitura dos valores de cor RGB e para conversão desses valores em uma cor específica.
    Para usar a classe, primeiro crie uma instância especificando a porta I2C a ser usada para se comunicar com o sensor de cor. Em seguida, configure o sensor usando os métodos correspondentes. Por fim, use o método readColor() para ler as cores detectadas e retornar uma enumeração correspondente à cor detectada.*/

class Sensorcor {

  private:

  public:

};
#endif