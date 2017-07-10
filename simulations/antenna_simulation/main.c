#include <msp430.h>
#include <stdint.h>

#include "i2c.h"
#include "antenna.h"

int main(void) {

  uint8_t Data[2]; 
  uint8_t command;

  WDTCTL = WDTPW + WDTHOLD;                   // Stop WDT

  SetupI2C(Data);

  while(1) {     

    get_command(command);

    switch (command) {
      case ARMING:
        //verifica se ja esta aberta(switch control(pode ser um botao))
        //se sim n faz nada
        //se nao
        //atualiza uma variavel e liga um led para armar
        break;
      case DISARMING:
        //verifica a variavel "armado?", caso sim desarma(atualiza a variavel e desliga o led)
        //caso nao, nao faz nada
        break;
      case DEPLOY_ANT_1:
        //verifica se a antena esta aberta(switch control)
        //se estiver, nao faz nada
        //se nao estiver faz:
        //ativa o deploy (fica piscando um led bem rapido so esperando o switch control tirar do loop)
        //assim que sair do loop deixa o led ligado, o q indica que a antena esta aberta
        break;
      case DEPLOY_ANT_2:
        //deploy antena 2
        //mesmo da antena 1
        break;
      case DEPLOY_ANT_3:
        //deploy antena 3
        //mesmo da antena 1
        break;
      case DEPLOY_ANT_4:
        //deploy antena 4
        //mesmo da antena 1
        break;
      case DEPLOY_SEQUENCIAL:
        //faz uma condicao se pelo menos uma estiver fechada ele entra
        //ai faz o processo de deploy da antena 1, depois a 2...
        break;
      default:
        //espera
        break;
    }

    command = 0;
  }
}
