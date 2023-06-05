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
  double kp;             // Proportional gain
  double ki;             // Integral gain
  double kd;             // Derivative gain
  double setpoint;       // Desired setpoint
  double output;         // Control output
  double integral;       // Integral term
  double previousError;  // Previous error

public:

  /*! Construtor que define as constantes e o set setpoint*/
  PID(double kp, double ki, double kd, double setpoint) {
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
    this->setpoint = setpoint;
    output = 0.0;
    integral = 0.0;
    previousError = 0.0;
  }

  /*!Calcula o PID*/
  int calcular(int input) {
    int error = setpoint - input;

    // Proportional term
    int proportional = kp * error;

    // Integral term
    integral += ki * error;

    // Anti-windup
    if (integral > 100) {
      integral = 100;
    } else if (integral < -100) {
      integral = -100;
    }

    // Derivative term
    int derivative = kd * (error - previousError);

    // Calculate the control output
    output = proportional + integral + derivative;

    // Update previous error
    previousError = error;

    return output;
  }
  /*!Zeramos todas referencias do PID chamado*/
  void zerar(){
    integral = 0;
    previousError = 0;
  }

  /*! Definir ou redefine setpoint*/
  void setSetpoint(double setpoint) {
    this->setpoint = setpoint;
  }

  /*! Redefine as constantes*/
  void setTunings(double kp, double ki, double kd) {
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
  }
};
#endif