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
  int kp;             // Proportional gain
  int ki;             // Integral gain
  int kd;             // Derivative gain
  int setpoint;       // Desired setpoint
  int output;         // Control output
  int integral;       // Integral term
  int previousError;  // Previous error

public:
};
#endif