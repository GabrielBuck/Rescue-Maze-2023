#ifndef Estrategia_hpp
#define Estrategia_hpp

/*! A classe Estrategia tem como funcao juntar e abstrair, ainda mais, o conteudo da classe "Operacional".
As funcoes dessa classe são as UNICAS que devem ser utilizadas no main*/

#include "Operacional.hpp"

Operacional op;

class Estrategia {

private:

public:

  /*! Realiza todas inicializações*/
  void iniciar(){
    op.begin();
  }

  /*! Movimentamos 1 quadrado para Frente */
  void frente(){
    
    op.ler_distancias();
    op.setar_quadrado(op.dist[0], op.dist[3]);//Parametros para troca
    while(true){
  
    }
  }

  /*! Giramos para Esq ou Dir */
  void giro(char com){
    op.girar(com);
  }

  /*! Busca vitimas com a camera*/
  void buscar_vit(){

  }


  
};
#endif