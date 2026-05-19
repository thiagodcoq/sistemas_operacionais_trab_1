#include "processos.h"
#include "auxiliar.h"
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX 10
#ifndef SIGRTMIN
#define SIGRTMIN 34
#endif
#define PAUSA                                                                  \
  for (int j = 0; j < 10000; j++) {                                            \
  }

/*
 * Como as chamadas de IO so existem para um unico dispositivo, essa informacao
 * sera omitida. Informacoes sobre o tipo de pedido ('R' e 'W') serao usadas por
 * didatismo, sem efeito pratico
 */
void sys_call(IOinfo *io_info, char c) {
  io_info->type = c;
  io_info->pid_requester = getpid();
  kill(getppid(), SIGUSR2); // Avisa o Kernel
}

void filho1(IOinfo *io_info) {

  for (int i = 0; i < MAX; i++) {
    print_time("[A1] PC=%d", i);
    if (i == 5 || i == 3) {
      sys_call(io_info, 'R');
    } else if (i == 7) {
      sys_call(io_info, 'W');
    }

    sleep(1);

    PAUSA
  }

  exit(0);
}

void filho2(IOinfo *io_info) {

  for (int i = 0; i < MAX; i++) {
    print_time("[A2] PC=%d", i);
    if (i == 8 || i == 9) {
      sys_call(io_info, 'R');
    }

    sleep(1);

    PAUSA
  }

  exit(0);
}

void filho3(IOinfo *io_info) {

  for (int i = 0; i < MAX; i++) {
    print_time("[A3] PC=%d", i);
    if (i == 2 || i == 1) {
      sys_call(io_info, 'R');
    } else if (i == 5) {
      sys_call(io_info, 'W');
    }

    sleep(1);

    PAUSA
  }

  exit(0);
}

void filho4(IOinfo *io_info) {

  for (int i = 0; i < MAX; i++) {
    print_time("[A4] PC=%d", i);
    if (i == 9) {
      sys_call(io_info, 'W');
    } else if (i == 2) {
      sys_call(io_info, 'R');
    }

    sleep(1);

    PAUSA
  }

  exit(0);
}

void filho5(IOinfo *io_info) {

  for (int i = 0; i < MAX; i++) {
    print_time("[A5] PC=%d", i);
    if (i == 6) {
      sys_call(io_info, 'W');
    }

    sleep(1);

    PAUSA
  }

  exit(0);
}

void filho6(IOinfo *io_info) {

  for (int i = 0; i < MAX; i++) {
    print_time("[A6] PC=%d", i);
    sleep(1);

    PAUSA
  }

  exit(0);
}
