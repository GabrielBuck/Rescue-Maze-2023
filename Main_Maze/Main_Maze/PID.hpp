#ifndef PID_hpp
#define PID_hpp

/*!PID

   Esta classe implementa um algoritmo de controlador PID (Proportional-Integral-Derivative)
  para sistemas de controle. Ele calcula uma saída de controle com base no erro entre um
  ponto de ajuste desejado e o valor de entrada atual. A classe fornece flexibilidade para ajustar
  os ganhos do controlador e defina o ponto de ajuste desejado.

  Como usar:
  1. Crie uma instância da classe PID com os ganhos e setpoint desejados.
  2. Chame o método `calcular()` do controlador PID com o valor de entrada atual
     para calcular a saída de controle.
  3. Opcionalmente, ajuste os ganhos ou ponto de ajuste usando os métodos de configuração fornecidos para
     ajustando o desempenho do controlador.

  Nota: Recursos adicionais como limites de entrada/saída, prevenção de enrolamento integral,
  ou a filtragem pode precisar ser adicionada dependendo dos requisitos específicos. */

class PID {

private:
  float kp;                     // Proportional gain
  float ki;                     // Integral gain
  float kd;                     // Derivative gain
  float windup;
  float limite;
  float setpoint;               // Desired setpoint
  float output;                 // Control output
  float integral;               // Integral term
  float previousError;          // Previous error
  float ultima_passagem = 0.0;  //Usada na medicao do tempo
  float tempo_atual = 0.0;
  float tempo_decorrido = 0.0;


public:

  /*! Construtor que define as constantes e o set setpoint*/
  PID(float kp, float ki, float kd, float setpoint, float windup, float limite) {
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
    this->windup = windup;
    this->setpoint = setpoint;
    output = 0.0;
    integral = 0.0;
    previousError = 0.0;
  }

  float tempo() {
    tempo_atual = (float)micros() / 1000000.0;
    tempo_decorrido = tempo_atual - ultima_passagem;
    ultima_passagem = tempo_atual;
    return tempo_decorrido;
  }

  /*!Calcula o PID*/
  float calcular(float input) {
    float error = setpoint - input;

    float current_time = tempo();


    // Proportional term
    float proportional = kp * error;

    // Integral term
    if (error <= windup && error >= -windup) {
      integral += ki * error * current_time;
    }

    // Limitador
    if (integral > limite) {
      integral = limite;
    } else if (integral < -limite) {
      integral = -limite;
    }

    // Derivative term
    float derivative = (kd * (error - previousError)) / current_time;

    // Calculate the control output
    output = proportional + integral + derivative;

    // Update previous error
    previousError = error;

    return output;
  }
  /*!Zeramos todas referencias do PID chamado*/
  void zerar() {
    integral = 0;
    previousError = 0;
  }

  /*! Definir ou redefine setpoint*/
  void setSetpoint(float setpoint) {
    this->setpoint = setpoint;
  }

  /*! Redefine as constantes*/
  void setTunings(float kp, float ki, float kd) {
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
  }
};
#endif